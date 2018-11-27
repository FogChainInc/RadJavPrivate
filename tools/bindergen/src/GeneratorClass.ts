import { GeneratorFunction } from "./GeneratorFunction";
import { GeneratorReference } from "./GeneratorReference";
import * as utils from "./utils";

/// A class to be used for generating bindings or parsing.
export class GeneratorClass
{
	/// The name of the class. Can include the namespace as well.
	/// Example: RadJav.GUI.Button
	public $name: string;
	/// The object this one extends. Can include the namespace.
	public $extends: string;
	/// The functions this generator will generate.
	public $functions: GeneratorFunction[];
	/// The functions this generator will generate.
	public $parseEvents: { [name: string]: Function };
	/// Info associated with each method in this class.
	public $methods: { [name: string]: GeneratorFunction };
	/// The properties associated with this class.
	public $properties: object;
	/// The generator associated with this class.
	public $generator: Generator;

	constructor (obj: string | GeneratorClass)
	{
		if (typeof obj == "string")
		{
			this.$name = (<string>obj);
			this.$extends = "";
			this.$functions = [];
			this.$parseEvents = {};
			this.$methods = {};
			this.$properties = {};
			this.$generator = null;
		}
		else
		{
			let tempObj: GeneratorClass = (<GeneratorClass>obj);

			this.$name = tempObj.$name;
			this.$extends = tempObj.$extends;
			this.$functions = tempObj.$functions;
			this.$parseEvents = tempObj.$parseEvents;
			this.$methods = tempObj.$methods;
			this.$properties = tempObj.$properties;
			this.$generator = tempObj.$generator;
		}
	}

	/// Get this class's name.
	getName (): string
	{
		return (this.$name);
	}

	/// Get a class and its namespace(s) from a string.
	static getClassFromString (className: string): string[]
	{
		let strs: string[] = [];

		if (className.indexOf (".") > -1)
			strs = className.split (".");

		if (className.indexOf ("/") > -1)
			strs = className.split ("/");

		return (strs);
	}

	/// Add a function.
	addFunction (func: GeneratorFunction): void
	{
		this[func.name] = utils.keepContext (function (...args): void
			{
				this.$generator.functionCalled (func, args[0]);
			}, this, [func]);
		this.$methods[func.name] = func;
	}

	/// Create a parse event.
	parse (event: string, func: Function): void
	{
		this.$parseEvents[event] = func;
	}

	/// Generate bindings and parse files.
	generate ():  void
	{
	}
}