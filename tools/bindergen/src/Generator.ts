import { GeneratorClass } from "./GeneratorClass";
import { CustomFile } from "./CustomFile";
import { Block } from "./Block";
import * as fs from "fs";

/// A generator.
export class Generator
{
	/// The name of the generator.
	public name: string;
	/// The custom files to write to.
	protected customFiles: { [name: string]: CustomFile };
	/// The filename to use when outputting all files.
	protected filename: string;
	/// The files to parse during generation.
	protected filesToParse: string[];
	/// The classes that will generate bindings or parse.
	protected classes: GeneratorClass[];

	constructor ()
	{
		this.name = "";
		this.customFiles = {};
		this.filename = "";
		this.filesToParse = [];
		this.classes = [];
	}

	/// Append data to a custom file.
	createCustomFile (file: CustomFile): void
	{
		this.customFiles[file.id] = file;
	}

	/// Append data to a custom file.
	appendToCustomFile (customFileID: string, data: string): void
	{
		this.customFiles[customFileID].contents += data;
	}

	/// When generating all files, this will determine the output name used.
	outputFilename (filename: string)
	{
		this.filename = filename;
	}

	/// The list of files to parse during generation.
	parseFiles (files: string[]): void
	{
		this.filesToParse = files;
	}

	/// Generate bindings and parse files.
	generate (): void
	{
		for (let iIdx = 0; iIdx < this.filesToParse.length; iIdx++)
		{
			let file: string = this.filesToParse[iIdx];
			let content: string = fs.readFileSync (file).toString ();
			let block: Block = Block.findBlock (content);

			eval (block.contents);
		}

		for (let iIdx = 0; iIdx < this.classes.length; iIdx++)
		{
			let genClass: GeneratorClass = this.classes[iIdx];

			genClass.generate ();
		}
	}

	/// Create a class for either parsing or generating.
	createClass (className: string | GeneratorClass): GeneratorClass
	{
		let genClass: GeneratorClass = new GeneratorClass ();

		this.classes.push (genClass);

		return (genClass);
	}
}