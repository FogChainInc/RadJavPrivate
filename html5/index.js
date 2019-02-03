var path = require ("path");
var express = require ("express");
var typescript = require ("typescript");
var fs = require ("fs");
var rcopy = require ("recursive-copy");
var execSync = require ("child_process").execSync;
var ws = require ("ws");
var app = express ();

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
			cmd: ["build", "b"], 
			desc: "Build Javascript from TypeScript files.", 
			help: "", 
			evt: function (args)
				{
					let buildPath = path.normalize (args[0]);
					let tsfiles = getTypeScriptFiles (buildPath);
					let outputPath = path.normalize (args[1]);
					let tempTsLibs = tsLibs;
					let tsDecFiles = getTypeScriptFiles (path.normalize (__dirname + "/d.ts/"));
					let generateSourceMap = false;

					for (let iIdx = 0; iIdx < tsDecFiles.length; iIdx++)
					{
						let decFile = tsDecFiles[iIdx];

						tempTsLibs.push (decFile);
					}

					console.log ("Building JavaScript from TypeScript...");

					compile (tsfiles, {
							noImplicitUseStrict: true, removeComments: true, importHelpers: true, 
							target: typescript.ScriptTarget.ES3, module: typescript.ModuleKind.None, 
							sourceMap: generateSourceMap, lib: tempTsLibs, outDir: outputPath
						});

					console.log ("Done.");
				}
		}, 
		{
			cmd: ["build-radjav", "r"], 
			desc: "Build RadJav's JavaScript", 
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
					cmd: ["doNotMinify"],
					desc: "Do not minify the generated JS."
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

						if (args[iIdx] == "generateSourceMap")
							generateSourceMap = true;

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

					compile (tsfiles, {
							noImplicitUseStrict: true, removeComments: true, importHelpers: true, 
							target: typescript.ScriptTarget.ES3, module: typescript.ModuleKind.None, 
							sourceMap: generateSourceMap, lib: tsLibs, outDir: "./build"
						});

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
								output = execSync ("java -jar " + compiler + " --compilation_level WHITESPACE_ONLY " + 
								" --js_output_file=./build/RadJav.min.js " + list + __dirname + "/src/RadJavMinify.js").toString ();
							}
						}
						catch (ex)
						{
							console.log (ex.message);
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
