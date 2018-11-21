import { GeneratorClass } from "./GeneratorClass";
import { CustomFile } from "./CustomFile";
import { Block } from "./Block";
import * as fs from "fs";
import * as path from "path";
import { GeneratorKeyword } from "./GeneratorKeyword";
import { GeneratorReference } from "./GeneratorReference";
import { Bindergen } from "./Bindergen";

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
	/// Keywords to use.
	protected keywords: { [name: string]: GeneratorKeyword };
	/// The passes to use.
	protected passes: string[];
	/// The object this one extends. Can include namespaces.
	public references: GeneratorReference[];

	constructor (name: string)
	{
		this.name = name;
		this.customFiles = {};
		this.filename = "";
		this.filesToParse = [];
		this.classes = [];
		this.keywords = {};
		this.passes = [];
		this.references = [];
	}

	/// During generation, refer to a reference for something.
	referFrom (reference: GeneratorReference): void
	{
		this.references.push (reference);
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

	/// The passes to use when generating.
	usePasses (passes: string | string[]): void
	{
		if (typeof passes == "string")
			this.passes = [(<string>passes)];
		else
			this.passes = (<string[]>passes);
	}

	/// When generating all files, this will determine the output name used.
	outputFilename (filename: string)
	{
		this.filename = filename;
	}

	/// Create a keyword to be used in generating.
	createKeyword (keyword: GeneratorKeyword): void
	{
		this.keywords[keyword.keyword] = keyword;
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
			let file: string = path.normalize(Bindergen.config.root + "/" + this.filesToParse[iIdx]);
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