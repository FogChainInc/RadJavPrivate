import { GeneratorFunction } from "./GeneratorFunction";
import { GeneratorReference } from "./GeneratorReference";

/// A class to be used for generating bindings or parsing.
export class GeneratorClass
{
	/// The name of the class. Can include the namespace as well.
	/// Example: RadJav.GUI.Button
	public name: string;
	/// The object this one extends. Can include the namespace.
	public extends: string;
	/// The functions this generator will generate.
	public functions: GeneratorFunction[];
	/// The functions this generator will generate.
	public parseEvents: { [name: string]: Function };

	constructor ()
	{
		this.name = "";
		this.extends = "";
		this.functions = [];
		this.parseEvents = {};
	}

	/// Create a parse event.
	parse (event: string, func: Function): void
	{
		this.parseEvents[event] = func;
	}

	/// Generate bindings and parse files.
	generate ():  void
	{
	}
}