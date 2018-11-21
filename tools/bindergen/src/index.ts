import * as fs from "fs";
import * as path from "path";
import * as utils from "./utils";
import { Bindergen } from "./Bindergen";
import { Generator } from "./Generator";

var config = null;

function build (configPath): void
{
	let parentDir: string = path.dirname (configPath);
	let configStr: string = fs.readFileSync (configPath).toString ();
	config = JSON.parse (configStr);
	config.parentDir = parentDir;

	for (let iIdx = 0; iIdx < config.generators.length; iIdx++)
	{
		let generatorPath: string = config.generators[iIdx];
		
		try
		{
			let foundGenPath: string = "";

			if (fs.existsSync (`${__dirname}/gen/${generatorPath}`) == true)
				foundGenPath = `${__dirname}/gen/${generatorPath}`;

			if (fs.existsSync (`${__dirname}/build/gen/${generatorPath}`) == true)
				foundGenPath = `${__dirname}/build/gen/${generatorPath}`;

			if (fs.existsSync (`${parentDir}/${generatorPath}`) == true)
				foundGenPath = `${parentDir}/${generatorPath}`;

			if (fs.existsSync (`${parentDir}/gen/${generatorPath}`) == true)
				foundGenPath = `${parentDir}/gen/${generatorPath}`;

			if (foundGenPath != "")
			{
				let content: string = fs.readFileSync (foundGenPath).toString ();

				if (content != "")
				{
					let generator: Generator = eval (content);

					Bindergen.addGenerator (generator);
				}
			}
		}
		catch (ex)
		{
			utils.showError (ex.message);
		}
	}

	Bindergen.generate (config);
}

const commands = [
	{
		cmd: ["build", "b"], 
		desc: "Generate the bindings.", 
		help: "", 
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
	}
];

utils.commandLine (commands);

