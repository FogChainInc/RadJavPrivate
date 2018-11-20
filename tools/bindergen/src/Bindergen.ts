import { Generator } from "./Generator";
import * as utils from "./utils";
import * as fs from "fs";
import * as vm from "vm";
import * as path from "path";

/// Main class for generating and parsing files.
export class Bindergen
{
	/// The generators.
	protected static generators: { [name: string]: Generator } = {};

	/// Add a generator.
	static addGenerator (generator: Generator): void
	{
		Bindergen.generators[generator.name] = generator;
	}

	/// Use a generator.
	static useGenerator (name: string): Generator
	{
		return (Bindergen.generators[name]);
	}

	/// Start generating.
	static generate (config: any): void
	{
		for (let iIdx = 0; iIdx < config.files.length; iIdx++)
		{
			let file: string = path.normalize (config.parentDir + "/" + config.files[iIdx]);

			try
			{
				let content: string = fs.readFileSync (file).toString ();
				const sandbox = { Bindergen: Bindergen };

				vm.createContext (sandbox);
				vm.runInContext (content, sandbox);
			}
			catch (ex)
			{
				utils.showError (ex.message);
			}
		}
	}
}