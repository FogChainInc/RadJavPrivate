var ts = require ("typescript");
var fs = require ("fs");

var options = {
		target: ts.ScriptTarget.ES3, 
		module: ts.ModuleKind.None, 
		noResolve: false
	};

let files = fs.readdirSync ("./src/");
let compileFiles = [];

for (let iIdx = 0; iIdx < files.length; iIdx++)
{
	//if (files[iIdx].indexOf (".ts") > -1)
		if (files[iIdx].indexOf ("RadJav.ts") > -1)
		compileFiles.push (files[iIdx]);
}

console.log ("Transpiling...");

/*for (let iIdx = 0; iIdx < compileFiles.length; iIdx++)
{
	let inFileName = "./src/" + compileFiles[iIdx];
	let outFileName = "./build/" + compileFiles[iIdx] + ".js";
	options.fileName = inFileName;
	options.outFile = outFileName;
	console.log("Transpiling file: " + inFileName);
	let content = fs.readFileSync (inFileName, "utf8");
	let result = ts.transpileModule(content, options);

	if (result.diagnostics.length > 0)
	{
		for (let iJdx = 0; iJdx < result.diagnostics.length; iJdx++)
			console.log(JSON.stringify(result.diagnostics[iJdx]));
	}

	//console.log(JSON.stringify(result));
}*/

for (let iIdx = 0; iIdx < compileFiles.length; iIdx++)
{
	let inFileName = "./src/" + compileFiles[iIdx];
	let outFileName = "./build/" + compileFiles[iIdx];
	var sourceMapText;
	var outputText;

	let program = ts.createProgram ([inFileName], options);
	let result = program.emit (undefined, function (name, text) {
                if (ts.fileExtensionIs(name, ".map")) {
                    ts.Debug.assertEqual(sourceMapText, undefined, "Unexpected multiple source map outputs, file:", name);
                    sourceMapText = text;
                }
                else {
                    //ts.Debug.assertEqual(outputText, undefined, "Unexpected multiple outputs, file:", name);
                    outputText = text;
                }
            });

	let diagnostics = ts.getPreEmitDiagnostics (program).concat (result.diagnostics);

	for (let iIdx = 0; iIdx < diagnostics.length; iIdx++)
	{
		let diagnostic = diagnostics[iIdx];

		if (diagnostic.file)
		{
			let { line, character } = diagnostic.file.getLineAndCharacterOfPosition(diagnostic.start);
				let message = ts.flattenDiagnosticMessageText(diagnostic.messageText, '\n');
				console.log(`${diagnostic.file.fileName} (${line + 1},${character + 1}): ${message}`);
				
				process.exit();
		}
		else
		{
			console.log(`${ts.flattenDiagnosticMessageText(diagnostic.messageText, '\n')}`);
		}
	}

	outFileName = outFileName.substr (0, (outFileName.length - 3));
	outFileName += ".js";

	fs.writeFileSync (outFileName, outputText);
}

