import { Generator } from "./Generator";
import * as utils from "./utils";
import * as fs from "fs";
import * as vm from "vm";
import * as path from "path";

/// Main class for generating and parsing files.
export class Bindergen
{
	/// The config.
	public static config: any = {
					generators: [], 
					root: ""
				};
	/// The generators.
	protected static generators: { [name: string]: any } = {};
	/// The command line arguments.
	protected static args: { [name: string]: string } = {};

	/// Set a command line argument.
	static setArg (arg: string): void
	{
		let keyword: string = utils.getCommandLineArgKeyword (arg);
		let value: string = utils.getCommandLineArgValue (arg);
		this.args[keyword] = value;
	}

	/// Get a command line argument.
	static getArg (arg: string): string
	{
		return (this.args["--" + arg]);
	}

	/// Add a generator.
	static addGenerator (generator: Generator): void
	{
		Bindergen.generators[generator.name] = generator;
	}

	/// Use a generator.
	static useGenerator (name: string): Generator
	{
		let gen: Generator = Bindergen.generators[name];

		return (gen);
	}

	/// Start generating.
	static generate (): void
	{
		for (let iIdx = 0; iIdx < Bindergen.config.files.length; iIdx++)
		{
			let file: string = path.normalize (Bindergen.config.root + "/" + Bindergen.config.files[iIdx]);

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