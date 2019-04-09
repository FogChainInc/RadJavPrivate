var path = require ("path");
var express = require ("express");
var typescript = require ("typescript");
var fs = require ("fs");
var rcopy = require ("recursive-copy");
var execSync = require ("child_process").execSync;
var ws = require ("ws");
var app = express ();
var crypto = require ("crypto");
var archiver = require ("archiver");
var OS = require('os');

var tsLibs = ["lib.es5.d.ts", "lib.es2015.promise.d.ts", "lib.dom.d.ts", "lib.scripthost.d.ts"];
var tsTypes = [];

var httpOptions = {
		location: path.normalize (path.dirname(__filename) + "/../"),
		index: "index.htm",
		port: 80,
		wsPort: 8585,
		wsServer: null,
		listeningAddr: "127.0.0.1",
		locationChanged: false,
		watchFilesAtLocations: [],
		url: ""
	};
var helpHeader = "RadJav JavaScript Builder\n";
helpHeader += "Copyright(c) 2018, Higher Edge Software, LLC\n";
helpHeader += "Under the MIT License\n\n";
var canRebuild = true;
var canRecopy = true;

function getCommand (cmdList, cmdName)
{
	let foundCmd = null;

	cmdList.forEach (function (val, index)
					{
						let cmd = val.cmd[0];

						if (cmd == cmdName)
							foundCmd = val;
					});

	return (foundCmd);
}

function getTypeScriptFiles (dirPath)
{
	let tsfiles = [];

	if (dirPath == null)
		dirPath = path.normalize (__dirname + "/src/");

	let files = fs.readdirSync (dirPath);
	files.forEach (function (file)
		{
			if (file.indexOf (".ts") > -1)
				tsfiles.push (path.normalize (dirPath + "/" + file));
		});

	return (tsfiles);
}

/// Get the SHA256 checksum of a file.
function sha256FileHash (filePath)
{
	let hash = crypto.createHash ("sha256");
	hash.update (fs.readFileSync (filePath));
	let result = hash.digest ("hex");

	return (result);
}

function keepContext (func, context, val)
{
	var objReturn = function ()
		{
			var aryArgs = Array.prototype.slice.call (arguments);

			if (val != undefined)
				aryArgs.push (val);

			if (context == null)
				return (func.apply (this, aryArgs));
			else
				return (func.apply (context, aryArgs));
		};

	return (objReturn);
}

// Compile function taken from https://github.com/Microsoft/TypeScript/wiki/Using-the-Compiler-API
function compile (fileNames, options)
{
	let program = typescript.createProgram(fileNames, options);
	let emitResult = program.emit();

	let allDiagnostics = typescript.getPreEmitDiagnostics(program).concat(emitResult.diagnostics);

	allDiagnostics.forEach(function (diagnostic)
		{
			if (diagnostic.file)
			{
				let { line, character } = diagnostic.file.getLineAndCharacterOfPosition(diagnostic.start);
				let message = typescript.flattenDiagnosticMessageText(diagnostic.messageText, '\n');
				console.log(`${diagnostic.file.fileName} (${line + 1},${character + 1}): ${message}`);
			}
			else
				console.log(`${typescript.flattenDiagnosticMessageText(diagnostic.messageText, '\n')}`);
		});
}

function watchFiles (watchLocations, broadCastMessage)
{
	let locations = "";

	for (let iIdx = 0; iIdx < watchLocations.length; iIdx++)
	{
		let wl = watchLocations[iIdx];

		if (typeof wl == "string")
			locations += "\t" + wl + "\n";

		if (typeof wl == "object")
		{
			if (wl.rebuild != null)
				locations += "\t" + wl.path + " -- Rebuild JS when file changes: " + wl.rebuild.toString () + "\n";

			if (wl.recopyOnChanges != null)
				locations += "\t" + wl.path + " -- Recopy files when file changes: " + wl.recopyOnChanges.toString () + "\n";
		}
	}

	console.log ("\nWatching files located at: \n" + locations);

	// Watching files recursively in NodeJS is only available on Windows and Mac.
	for (let iIdx = 0; iIdx < watchLocations.length; iIdx++)
	{
		let wl = watchLocations[iIdx];
		let loc = wl;

		if (typeof wl == "object")
			loc = wl.path;

		fs.watch (loc, {
					persistent: true,
					recursive: true
				}, keepContext (function (eventType, filename, wl2)
					{
						let wl3 = wl2[0];
						let loc2 = wl3;
						let rebuild = false;
						let recopyOnChanges = false;

						if (typeof wl3 == "object")
						{
							loc2 = wl3.path;
							rebuild = wl3.rebuild;
							recopyOnChanges = wl3.recopyOnChanges;
						}

						let changedFilePath = path.normalize (loc2 + "/" + filename);

						if ((rebuild == true) && (canRebuild == true))
						{
							let tsfiles = getTypeScriptFiles ();

							console.log ("Rebuilding...");
							compile (tsfiles, {
									noImplicitUseStrict: true, removeComments: true, importHelpers: true,
									target: typescript.ScriptTarget.ES3, module: typescript.ModuleKind.None,
									lib: tsLibs, sourceMap: true, outDir: __dirname + "/build"
								});
							console.log ("Finishing rebuilding.");
							canRebuild = false;
							setTimeout (function ()
									{
										canRebuild = true;
									}, 200);
						}

						/// @fixme Fix this to actually copy the files from wl.path to the build directory...
						if ((recopyOnChanges == true) && (canRecopy == true))
						{
							try
							{
								fs.mkdirSync (__dirname + "/build/themes/");
								fs.mkdirSync (__dirname + "/build/languages/");
							}
							catch (ex)
							{
							}

							let promises = [];
							promises.push (rcopy (__dirname + "/themes/", __dirname + "/build/themes/", { overwrite: true }));
							promises.push (rcopy (__dirname + "/languages/", __dirname + "/build/languages/", { overwrite: true }));

							canRecopy = false;
							broadCastMessage = false;
							Promise.all (promises).then (function ()
									{
										canRecopy = true;
										broadCastMessage = true;
										httpOptions.wsServer.broadcast ("refresh");
									});
						}

						if (broadCastMessage == true)
							httpOptions.wsServer.broadcast ("refresh");
					}, this, [wl]));
	}
}

function startHTTP ()
{
	httpOptions.watchFilesAtLocations.push (path.normalize (__dirname + "/../examples"));
	httpOptions.watchFilesAtLocations.push (path.normalize (__dirname + "/build"));
	httpOptions.watchFilesAtLocations.push ({ path: path.normalize (__dirname + "/themes"), recopyOnChanges: true });
	httpOptions.watchFilesAtLocations.push ({ path: path.normalize (__dirname + "/src"), rebuild: true });

	app.use (express.static (httpOptions.location));
	app.get ("/", function (req, res)
		{
			if (httpOptions.locationChanged == false)
				res.redirect ("examples/exampleBrowser.htm?devtools=1");
			else
				res.sendFile (path.normalize (httpOptions.location + "/" + httpOptions.index));
		});

	if (httpOptions.locationChanged == false)
	{
		app.get ("/examples", function (req, res)
			{
				res.sendFile (path.normalize (httpOptions.location + "/examples/exampleBrowser.htm"));
			});
	}

	httpOptions.wsServer = new ws.Server ({
			port: httpOptions.wsPort
		});
	httpOptions.wsServer.on ("connection", function (connection)
		{
			connection.on ("message", function (message)
				{

				});
		});
	httpOptions.wsServer.broadcast = function (data)
		{
			httpOptions.wsServer.clients.forEach (function (client)
				{
					if (client.readyState == ws.OPEN)
						client.send (data);
				});
		};

	app.listen (httpOptions.port, httpOptions.listeningAddr, null, function ()
		{
			httpOptions.url = "http://" + httpOptions.listeningAddr + ":" + httpOptions.port + "/";

			console.log ("HTTP server listening on port " + httpOptions.port);
			console.log ("WebSocket server listening on port " + httpOptions.wsPort);
			console.log ("URL: " + httpOptions.url);
			console.log ("HTTP server serving files located at: " + httpOptions.location);

			let canWatch = false;

			if (process.platform == "darwin")
				canWatch = true;

			if (process.platform == "win32")
				canWatch = true;

			if (canWatch == true)
				watchFiles (httpOptions.watchFilesAtLocations, true);
		});
}

var commands = [
		{
			cmd: ["http", "h"],
			desc: "Start HTTP server",
			help: "",
			evt: function (args)
				{
					if (args.length > 0)
					{
						httpOptions.location = args[0];
						httpOptions.location = path.normalize (httpOptions.location);
						httpOptions.watchFilesAtLocations.push (httpOptions.location);
						httpOptions.locationChanged = true;
					}

					if (args.length > 1)
						httpOptions.index = args[1];

					if (args.length > 2)
						httpOptions.port = parseInt (args[2]);

					if (args.length > 3)
						httpOptions.listeningAddr = args[3];

					startHTTP ();
				}
		},
		{
			cmd: ["buildIPA", "ipa"],
			desc: "Build an iOS IPA from a selected folder.",
			help: "",
			evt: function (args)
				{
					let appFolder = path.normalize (args[0]);
					let dirName = path.normalize (appFolder);
					let customFileName = "app.xrj";

					if (args.length > 1)
						customFileName = args[1];

					if (fs.existsSync (`${dirName}/${customFileName}`) == false)
					{
						console.error (`${customFileName} not found in ${dirName}`);

						return;
					}

					let file = fs.createWriteStream (__dirname + "/app.ipa");
					let zip = archiver ("zip", { zlib: { level: 9 } });

					zip.pipe (file);
					zip.directory ("../sdk/prebuilt/ios/Payload/", "Payload");

					zip.file (`${dirName}/${customFileName}`, { name: "Payload/RadJavVM.app/app.xrj" });
					zip.directory (dirName, "Payload/RadJavVM.app/");

					zip.finalize ();
				}
		},
		{
			cmd: ["buildAPK", "apk"],
			desc: "Build an Android APK from a selected folder.",
			help: "",
			evt: function (args)
				{
					let appFolder = path.normalize (args[0]);
					let dirName = path.normalize (appFolder);
					let androidsdk = "";
					let customFileName = "app.xrj";
					let jarSignerPath = "";

					if (args.length > 1)
						customFileName = args[1];

					if (args.length > 2)
						androidsdk = args[2];
					else
					{
						if (process.platform == "win32")
						{
							androidsdk = `${OS.homedir ()}\\AppData\\Local\\Android\\Sdk`;

							if (fs.existsSync (`C:/Program Files/Android/Android Studio/jre/bin/jarsigner.exe`) == true)
								jarSignerPath = `C:/Program Files/Android/Android Studio/jre/bin/jarsigner.exe`;

							if (fs.existsSync (`C:/Program Files (x86)/Java/jdk/bin/jarsigner.exe`) == true)
								jarSignerPath = `C:/Program Files (x86)/Java/jdk/bin/jarsigner.exe`;
						}

						if (process.platform == "linux")
						{
							androidsdk = `${OS.homedir ()}/Android\\Sdk`;
							jarSignerPath = "jarsigner";
						}

						if (process.platform == "darwin")
						{
							androidsdk = `${OS.homedir ()}/Library/Android\\sdk`;
							jarSignerPath = "jarsigner";
						}
					}

					if (fs.existsSync (`${dirName}/${customFileName}`) == false)
					{
						console.error (`${customFileName} not found in ${dirName}`);

						return;
					}

					let dirs = fs.readdirSync (`${androidsdk}/build-tools/`);
					let buildToolsDir = path.normalize (`${androidsdk}/build-tools/${dirs[(dirs.length - 1)]}`);
					dirs = fs.readdirSync (`${androidsdk}/platforms/`);
					let platformDir = dirs[(dirs.length - 1)];
					let sdkPath = path.normalize (`${__dirname}/../sdk/prebuilt/android/`);
					let androidI = path.normalize (`${androidsdk}/platforms/${platformDir}/android.jar`);
					let sdkAPK = path.normalize (`${sdkPath}/app.apk`);

					/*execSync (`${buildToolsDir}/aapt package -f -m -J gen -M ${sdkPath}/AndroidManifest.xml -S ${sdkPath}/res -I ${androidI}`);
					execSync (`${buildToolsDir}/aapt package -f -M ${sdkPath}/AndroidManifest.xml -S ${sdkPath}/res -I ${androidI} -F ${sdkAPK}.unaligned`);
					execSync (`${buildToolsDir}/aapt add ${sdkAPK} ${sdkPath}/classes.dex`);*/

					let file = fs.createWriteStream (__dirname + "/app.apk");
					let zip = archiver ("zip");

					file.on ("close", function ()
						{
							execSync (`"${buildToolsDir}/apksigner" sign --ks "${OS.homedir ()}/.android/debug.keystore" --ks-key-alias androiddebugkey --ks-pass pass:"android" "${__dirname}/app.apk"`);

							//execSync (`"${jarSignerPath}" -keystore "${OS.homedir ()}/.android/debug.keystore" -storepass "android" "${__dirname}/app.apk" androiddebugkey`);
							//execSync (`"${buildToolsDir}/zipalign" -f 4 "${__dirname}/app.apk" "${__dirname}/app-debug.apk"`);
							//fs.unlinkSync (`${__dirname}/app.apk`);
						}.bind (this));

					zip.pipe (file);
					zip.directory ("../sdk/prebuilt/android/", false);

					zip.file (`${dirName}/${customFileName}`, { name: "assets/app.xrj" });
					zip.directory (dirName, "assets/");

					zip.finalize ();
				}
		},
		{
			cmd: ["convertFormDesignerToJSON", "c"],
			desc: "Convert Visual Studio's form designer output to RadJav's GUI JSON. Can either be a .cs or .vb file.",
			help: "",
			evt: function (args)
				{
					let file = "";
					let outputFile = "";

					if (args[0] != null)
						file = args[0];

					if (args[1] != null)
						outputFile = args[1];

					let fileType = path.extname (file);
					fileType = fileType.toLowerCase ();
					let fileContent = fs.readFileSync (file).toString ();
					let thisType = "this";
					let newType = "new";

					if (fileType == ".vb")
					{
						thisType = "Me";
						newType = "New";
					}

					let convertibleTypes = [
							{ dotNetType: "System.Windows.Forms.Button", radjavType: "RadJav.GUI.Button"},
							{ dotNetType: "System.Windows.Forms.Label", radjavType: "RadJav.GUI.Label"},
							{ dotNetType: "System.Windows.Forms.TextBox", radjavType: "RadJav.GUI.Textbox"},
							{ dotNetType: "System.Windows.Forms.CheckBox", radjavType: "RadJav.GUI.Checkbox"},
							{ dotNetType: "System.Windows.Forms.ComboBox", radjavType: "RadJav.GUI.Combobox"},
							{ dotNetType: "System.Windows.Forms.RadioButton", radjavType: "RadJav.GUI.Radio"},
							{ dotNetType: "System.Windows.Forms.PictureBox", radjavType: "RadJav.GUI.Image"},
							{ dotNetType: "System.Windows.Forms.ListView", radjavType: "RadJav.GUI.List"},
							{ dotNetType: "System.Windows.Forms.GroupBox", radjavType: "RadJav.GUI.Container"}
						];
					let guiJSON = [];
					let parents = {};

					for (let iIdx = 0; iIdx < convertibleTypes.length; iIdx++)
					{
						let convType = convertibleTypes[iIdx];
						let findStr = `${newType} ${convType.dotNetType}\\(\\)`;
						findStr = findStr.replace (new RegExp("\\.", "g"), "\\.");
						let findReg = new RegExp (`(?<=${thisType}\\.)(.*)(?= \= ${findStr})`, "g");
						let foundNames = fileContent.match (findReg);

						for (let iJdx = 0; iJdx < foundNames.length; iJdx++)
						{
							let foundName = foundNames[iJdx];
							let obj = { type: convType.radjavType, name: foundName };
							findReg = new RegExp (`(?<=${thisType}\\.${foundName}\\.Location = New System\\.Drawing\\.Point\\()(.*)(?=\\))`);
							obj.position = fileContent.match (findReg);
							findReg = new RegExp (`(?<=${thisType}\\.${foundName}\\.Size = New System\\.Drawing\\.Size\\()(.*)(?=\\))`);
							obj.size = fileContent.match (findReg);
							findReg = new RegExp (`(?<=${thisType}\\.${foundName}\\.Text = \\")(.*)(?=\\")`);
							obj.text = fileContent.match (findReg);
							findReg = new RegExp (`(?<=${thisType}\\.${foundName}\\.Visible = )(.*)`);
							obj.visibility = fileContent.match (findReg);

							if (obj.position != null)
							{
								if (obj.position.length > 0)
									obj.position = obj.position[0];
							}
							else
								delete obj.position;

							if (obj.size != null)
							{
								if (obj.size.length > 0)
									obj.size = obj.size[0];
							}
							else
								delete obj.size;

							if (obj.text != null)
							{
								if (obj.text.length > 0)
									obj.text = obj.text[0];
							}
							else
								delete obj.text;

							if (obj.visibility != null)
							{
								if (obj.visibility.length > 0)
								{
									let visible = obj.visibility[0].toLowerCase ();

									if (visible == "true")
										obj.visibility = true;

									if (visible == "false")
										obj.visibility = false;
								}
							}
							else
								delete obj.visibility;

							guiJSON.push (obj);

							findReg = new RegExp (`(?<=${thisType}\\.${foundName}\\.Controls\\.Add\\(${thisType}\\.)(.*)(?=\\))`, "g");
							let howCuteItsAParent = fileContent.match (findReg);

							if (howCuteItsAParent != null)
							{
								if (howCuteItsAParent.length > 0)
								{
									parents[foundName] = obj;

									for (let iKdx = 0; iKdx < howCuteItsAParent.length; iKdx++)
									{
										let child = howCuteItsAParent[iKdx];

										for (let iUdx = 0; iUdx < guiJSON.length; iUdx++)
										{
											let guiObj = guiJSON[iUdx];

											if (guiObj.name == child)
											{
												if (parents[foundName].children == null)
													parents[foundName].children = [];

												parents[foundName].children.push (guiObj);
												guiJSON.splice (iUdx, 1);

												break;
											}
										}
									}
								}
							}
						}
					}

					fs.writeFileSync (outputFile, JSON.stringify (guiJSON, null, 4));
				}
		},
		{
			cmd: ["build", "b"],
			desc: "Build JavaScript",
			help: [
				{
					cmd: ["doNotCopyHTML5Files"],
					desc: "The build is not an HTML5 build, so do not copy theme files and other HTML5 related files."
				},
				{
					cmd: ["doNotCopyFilesToLibrary"],
					desc: "Do not copy the generated JS to library/javascript."
				},
				{
					cmd: ["doNotGenerateSourceMaps"],
					desc: "Do not generate the TS-to-JS source maps."
				},
				{
					cmd: ["doNotMinify"],
					desc: "Do not minify the generated JS."
				},
				{
					cmd: ["clearCache"],
					desc: "Clear the cache."
				},
				{
					cmd: ["watch"],
					desc: "Watch locations and rebuild when files are updated."
				},
				{
					cmd: ["generateSourceMap"],
					desc: "Generate source maps for each generated JS file."
				}],
			evt: function (args)
				{
					let copyHTML5Files = true;
					let copyFilesToLibrary = true;
					let minify = true;
					let watch = false;
					let mapFiles = true;
					let clearCache = false;
					let sourceRoot = "";
					let tsfiles = getTypeScriptFiles ();
					let generateSourceMap = false;

					for (let iIdx = 0; iIdx < args.length; iIdx++)
					{
						if (args[iIdx] == "doNotCopyHTML5Files")
							copyHTML5Files = false;

						if (args[iIdx] == "doNotCopyFilesToLibrary")
							copyFilesToLibrary = false;

						if (args[iIdx] == "doNotMinify")
							minify = false;

						if (args[iIdx] == "doNotGenerateSourceMaps")
							mapFiles = false;

						if (args[iIdx] == "clearCache")
							clearCache = true;

						if (args[iIdx] == "sourceRoot")
						{
							let sourceRootPath = args[iIdx + 1];
							sourceRoot = sourceRootPath;
						}

						if (args[iIdx] == "watch")
							watch = true;

						if (args[iIdx] == "help")
						{
							let str = helpHeader;
							let buildCmd = getCommand (commands, "build");

							buildCmd.help.forEach (function (val, index)
								{
									let line = "  ";
									let spaces = "";
									let maxSpaces = 6;

									for (let iIdx = 0; iIdx < val.cmd.length; iIdx++)
									{
										let cmd = val.cmd[iIdx];

										if (iIdx == 0)
											line += cmd;

										if (iIdx == 1)
											line += ", " + cmd;
									}

									if (index == 1)
										maxSpaces -= 4;

									if (index == 2)
										maxSpaces += 8;

									for (let iIdx = 0; iIdx < maxSpaces; iIdx++)
										spaces += " ";

									line += spaces + val.desc;
									str += line + "\n";
								});

							console.log (str);

							return;
						}
					}

					console.log ("Building JavaScript from TypeScript...");

					if (clearCache == true)
					{
						if (fs.existsSync ("./cache.json"))
							fs.unlinkSync ("./cache.json");
					}

					let temptsFiles = [];
					let fileDiffs = {};

					if (fs.existsSync ("./cache.json"))
					{
						let fileDiffsStr = fs.readFileSync ("./cache.json").toString ();

						fileDiffs = JSON.parse (fileDiffsStr);

						for (let key in fileDiffs)
						{
							let checkHash = fileDiffs[key];
							let hash = sha256FileHash (key);

							if (checkHash != hash)
								temptsFiles.push (key);
						}
					}
					else
						temptsFiles = tsfiles;

					if (temptsFiles.length > 0)
					{
						compile (temptsFiles, {
								noImplicitUseStrict: true, removeComments: true, importHelpers: true,
								target: typescript.ScriptTarget.ES3, module: typescript.ModuleKind.None,
								sourceMap: generateSourceMap, lib: tsLibs, sourceMap: mapFiles,
								sourceRoot: sourceRoot, outDir: "./build"
							});

						for (let iIdx = 0; iIdx < temptsFiles.length; iIdx++)
						{
							let tempFile = temptsFiles[iIdx];

							fileDiffs[tempFile] = sha256FileHash (tempFile);
						}

						fs.writeFileSync ("./cache.json", JSON.stringify (fileDiffs));
					}

					console.log ("Done.");

					if (copyHTML5Files == true)
					{
						console.log ("Copying HTML5 files to build directory...");

						try
						{
							fs.mkdirSync ("./build/themes/");
							fs.mkdirSync ("./build/languages/");
						}
						catch (ex)
						{
						}

						rcopy ("./themes/", "./build/themes/", { overwrite: true });
						rcopy ("./languages/", "./build/languages/", { overwrite: true });

						console.log ("Done.");
					}

					if (copyFilesToLibrary == true)
					{
						console.log ("Copying JavaScript files to library/javascript directory...");

						rcopy ("./build/", "../library/javascript/", { filter: ["*.js"], overwrite: true });

						console.log ("Done.");
					}

					if (minify == true)
					{
						console.log ("Minifying JavaScript...");

						let localFiles = fs.readdirSync ("./");
						let compilerType = 0;
						let compiler = "";

						localFiles.forEach (function (file)
							{
								if (file.indexOf (".jar") > -1)
								{
									if (file.indexOf ("closure") > -1)
									{
										compiler = file;
										compilerType = 1;
									}
								}
							});

						localFiles = fs.readdirSync ("./build");
						let list = "";

						localFiles.forEach (function (file)
							{
								if (file.indexOf (".js") > -1)
								{
									if (! (file.indexOf (".js.map") > -1))
										list += "./build/" + file + " ";
								}
							});

						var output = "";

						try
						{
							if (compilerType == 1)
							{
								if (fs.existsSync ("./build/RadJav.min.js"))
									fs.unlinkSync ("./build/RadJav.min.js");

								output = execSync ("java -jar " + compiler + " --compilation_level WHITESPACE_ONLY " +
								" --js_output_file=./build/RadJav.min.js " + list + __dirname + "/src/RadJavMinify.js").toString ();
							}
						}
						catch (ex)
						{
							console.log (ex.stack);
						}

						if (output != "")
							console.log (output);

						if (compilerType == 0)
							console.log ("No compiler found.");

						console.log ("Done.");
					}

					if (watch == true)
						watchFiles ([{ path: __dirname + "/src/", rebuild: true } ], false);
				}
		},
		{
			cmd: ["defs", "d"],
			desc: "Build definitions",
			evt: function (args)
				{
					console.log ("Building TypeScript declaration files...");

					let tsfiles = getTypeScriptFiles ();

					compile (tsfiles, {
							noImplicitUseStrict: true, removeComments: false, importHelpers: true,
							target: typescript.ScriptTarget.ES3, module: typescript.ModuleKind.None,
							lib: tsLibs, declaration: true, emitDeclarationOnly: true,
							declarationDir: "./d.ts"
						});

					console.log ("Done.");
				}
		},
		{
			cmd: ["help", "h"],
			desc: "Help",
			evt: function (args)
				{
					let str = helpHeader;

					commands.forEach (function (val, index)
						{
							let line = "  ";
							let spaces = "";
							let maxSpaces = 4;

							for (let iIdx = 0; iIdx < val.cmd.length; iIdx++)
							{
								let cmd = val.cmd[iIdx];

								if (iIdx == 0)
									line += "--" + cmd;

								if (iIdx == 1)
									line += ", -" + cmd;
							}

							if (index == 2)
								maxSpaces--;

							for (let iIdx = 0; iIdx < maxSpaces; iIdx++)
								spaces += " ";

							line += spaces + val.desc;
							str += line + "\n";
						});

					console.log (str);
				}
		}
	];

var execFunc = null;
let args = [];

process.argv.forEach (function (val, index)
		{
			if (index == 2)
			{
				commands.forEach (function (val2, index2)
					{
						for (let iIdx = 0; iIdx < val2.cmd.length; iIdx++)
						{
							let cmd = val2.cmd[iIdx];

							if (iIdx == 0)
								cmd = "--" + cmd;

							if (val == cmd)
								execFunc = val2.evt;
						}
					});
			}

			if (index > 2)
				args.push (val);
		});

if (execFunc != null)
	execFunc (args);
else
{
	let helpCmd = getCommand (commands, "help");
	helpCmd.evt ();
}
