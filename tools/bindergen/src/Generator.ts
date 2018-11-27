import { GeneratorNamespace } from "./GeneratorNamespace";
import { CustomFile } from "./CustomFile";
import { Block } from "./Block";
import * as fs from "fs";
import * as path from "path";
import * as vm from "vm";
import { GeneratorKeyword } from "./GeneratorKeyword";
import { GeneratorReference } from "./GeneratorReference";
import { Bindergen } from "./Bindergen";
import * as utils from "./utils";
import { GeneratorOutput } from "./GeneratorOutput";

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
	/// Keywords to use.
	protected keywords: { [name: string]: GeneratorKeyword };
	/// The passes to use.
	protected passes: string[];
	/// The object this one extends. Can include namespaces.
	public references: GeneratorReference[];
	/// The temporary block used when generating passes.
	public tempBlock: Block;
	/// The namespaces used.
	public namespaces: GeneratorNamespace[];
	/// Events associated with this generator.
	protected events: { [name: string]: Function };
	/// Outputs to write to the file.
	public outputs: { [name: string]: GeneratorOutput };

	constructor (name: string)
	{
		this.name = name;
		this.customFiles = {};
		this.filename = "";
		this.filesToParse = [];
		this.keywords = {};
		this.passes = [];
		this.references = [];
		this.tempBlock = null;
		this.namespaces = [];
		this.events = {};
		this.outputs = {};
	}

	/// During generation, refer to a reference for something.
	referFrom (reference: GeneratorReference): void
	{
		if (reference.name == "")
			reference.name = this.name;

		this.references.push (reference);
	}

	/// Get all reference data.
	getReferences (input: string): string[]
	{
		let data: string[] = [];

		for (let iIdx = 0; iIdx < this.references.length; iIdx++)
		{
			let ref: GeneratorReference = this.references[iIdx];

			data.push (ref.getData (input));
		}

		return (data);
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

	/// Create an output.
	createOutput (name: string): GeneratorOutput
	{
		if (this.outputs[name] == null)
			this.outputs[name] = new GeneratorOutput (name);

		return (this.outputs[name]);
	}

	/// Get an output.
	getOutput (name: string): GeneratorOutput
	{
		if (this.outputs[name] == null)
			this.outputs[name] = new GeneratorOutput (name);

		return (this.outputs[name]);
	}

	/// Call a function.
	functionCalled (func: GeneratorFunction, args: any[]): void
	{
		for (let key in this.outputs)
		{
			if (this.outputs[key].events["functionCall"] != null)
			{
				this.outputs[key].content += 
					this.replaceKeywords (this.outputs[key].events["functionCall"] (func, args));
			}
		}
	}

	/// Add content to an output.
	addToOutput (outputName: string, content: string): void
	{
		this.createOutput (outputName);

		this.outputs[outputName].content += content;
	}

	/// Add content to the beginning of an output.
	prependToOutput (outputName: string, content: string): void
	{
		this.createOutput (outputName);

		this.outputs[outputName].content = 
			utils.insertString (this.outputs[outputName].content, content, 0);
	}

	/// The output to start generating.
	outputStart (type: string)
	{
		this.createOutput (type);

		this.outputs[type].start = (this.tempBlock.end + 2);
		this.outputs[type].filePath = this.tempBlock.file;
		this.outputs[type].oldContent = this.tempBlock.fileContent;

		if (this.outputs[type].events["start"] != null)
		{
			this.outputs[type].content += 
				this.replaceKeywords (this.outputs[type].events["start"].call (this, this.outputs[type]));
		}
	}

	/// The output to stop generating.
	outputEnd (type: string)
	{
		if (this.outputs[type] == null)
			throw new Error (`Output ${type} does not exist!`);

		this.outputs[type].end = this.tempBlock.start;
		let content: string = this.outputs[type].oldContent;

		if (this.outputs[type].events["end"] != null)
		{
			this.outputs[type].content += 
				this.replaceKeywords (this.outputs[type].events["end"].call (this, this.outputs[type]));
		}

		if (this.outputs[type].events["first"] != null)
		{
			this.prependToOutput (type, 
				this.replaceKeywords (this.outputs[type].events["first"].call (this, this.outputs[type])));
		}

		if (this.outputs[type].events["second"] != null)
		{
			this.prependToOutput (type, 
				this.replaceKeywords (this.outputs[type].events["second"].call (this, this.outputs[type])));
		}

		if (this.outputs[type].events["third"] != null)
		{
			this.prependToOutput (type, 
				this.replaceKeywords (this.outputs[type].events["third"].call (this, this.outputs[type])));
		}

		content = utils.removeStringBetween (content, this.outputs[type].start, this.outputs[type].end);
		content = utils.insertString (content, this.outputs[type].content, this.outputs[type].start);

		fs.writeFileSync (this.outputs[type].filePath, content);
	}

	/// Create a keyword to be used in generating.
	createKeyword (keyword: string | GeneratorKeyword, value: string = ""): void
	{
		if (typeof (keyword) == "string")
		{
			this.keywords[keyword] = new GeneratorKeyword ();
			this.keywords[keyword].keyword = keyword;
			this.keywords[keyword].value = value;
		}
		else
			this.keywords[keyword.keyword] = keyword;
	}

	/// The list of files to parse during generation.
	parseFiles (files: string[]): void
	{
		this.filesToParse = files;
	}

	/// Look for a keyword and replace it with it's value.
	replaceKeywords (content: string): string
	{
		for (let key in this.keywords)
		{
			let keyword: GeneratorKeyword = this.keywords[key];

			content.replace (new RegExp (`(s|${keyword.keyword})`, "g"), keyword.value);
		}

		return (content);
	}

	/// Trigger on an event.
	on (event: string, func: Function): void
	{
		this.events[event] = func;
	}

	/// Generate bindings and parse files.
	generate (): void
	{
		for (let iIdx = 0; iIdx < this.filesToParse.length; iIdx++)
		{
			let file: string = path.normalize(Bindergen.config.root + "/" + this.filesToParse[iIdx]);
			let content: string = fs.readFileSync (file).toString ();
			let blocks: Block[] = Block.findBlocks (content);

			for (let iJdx = 0; iJdx < blocks.length; iJdx++)
			{
				let block: Block = blocks[iJdx];
				let sandbox = {};
	
				block.file = file;
				this.tempBlock = block;
				sandbox[this.name] = this;

				for (let iKdx = 0; iKdx < this.namespaces.length; iKdx++)
				{
					let nspace: GeneratorNamespace = this.namespaces[iKdx];

					sandbox[nspace.$name] = nspace;
				}

				vm.createContext (sandbox);
				vm.runInContext (block.contents, sandbox);
			}
		}
	}

	/// Create a namespace to store classes.
	createNamespace (name: string): GeneratorNamespace
	{
		let namespace: GeneratorNamespace = new GeneratorNamespace (this, name);

		/// @fixme Throw an exception for any existing namespaces 
		/// with the same property name as one in this class.
		this[name] = namespace;
		this.namespaces.push (namespace);

		return (namespace);
	}
}