import * as fs from "fs";
import * as path from "path";
import * as utils from "./utils";
import { Bindergen } from "./Bindergen";
import { Generator } from "./Generator";
import * as vm from "vm";
import * as mod from "module";

function build (configPath): void
{
	let parentDir: string = path.dirname (configPath);
	let configStr: string = fs.readFileSync (configPath).toString ();

	if (Bindergen.config.root != "")
		parentDir = Bindergen.config.root;

	Bindergen.config = JSON.parse (configStr);

	if ((Bindergen.config.root == undefined) || (Bindergen.config.root == ""))
		Bindergen.config.root = parentDir;

	for (let iIdx = 0; iIdx < Bindergen.config.generators.length; iIdx++)
	{
		let generatorPath: string = Bindergen.config.generators[iIdx];
		
		try
		{
			let foundGenPath: string = "";

			if (fs.existsSync (`${__dirname}/gen/${generatorPath}`) == true)
				foundGenPath = `${__dirname}/gen/${generatorPath}`;

			if (fs.existsSync (`${__dirname}/build/gen/${generatorPath}`) == true)
				foundGenPath = `${__dirname}/build/gen/${generatorPath}`;

			/*if (fs.existsSync (`${parentDir}/${generatorPath}`) == true)
				foundGenPath = `${parentDir}/${generatorPath}`;

			if (fs.existsSync (`${parentDir}/gen/${generatorPath}`) == true)
				foundGenPath = `${parentDir}/gen/${generatorPath}`;*/

			if (foundGenPath != "")
			{
				let content: string = fs.readFileSync (foundGenPath).toString ();

				if (content != "")
				{
					let wrappedContent: string = mod.wrap (content);
					let genMod: Function = vm.runInThisContext (wrappedContent);
					genMod (exports, require, module, __filename, __dirname);
					let generator: Generator = module.exports;

					Bindergen.addGenerator (generator);
				}
			}
		}
		catch (ex)
		{
			utils.showError (ex.message);
		}
	}

	Bindergen.generate ();
}

const commands = [
	{
		cmd: ["build", "b"], 
		desc: "Generate the bindings.", 
		help: "", 
		executeLast: true, 
		evt: function ()
			{
				let configPath: string = path.normalize (
						__dirname + "/../../../library/gen/config.json");
				build (configPath);
			}
	}, 
	{
		cmd: ["watch", "w"], 
		desc: "Watch for files changes and generate.", 
		help: "", 
		evt: function (args)
			{
			}
	}, 
	{
		cmd: ["root", "r"], 
		desc: "Set the root directory.", 
		help: "", 
		evt: function (args)
			{
				Bindergen.config.root = args;
			}
	}
];

process.argv.forEach (function (val, index)
			{
				Bindergen.setArg (val);
			});

utils.commandLine (commands);

