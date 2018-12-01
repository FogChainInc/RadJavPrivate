import { GeneratorFunction } from "./GeneratorFunction";
import { GeneratorReference } from "./GeneratorReference";
import * as utils from "./utils";

/// A class to be used for generating bindings or parsing.
export class GeneratorClass
{
	/// The name of the class. Can include the namespace as well.
	/// Example: RadJav.GUI.Button
	public $name: string;
	/// The code-friendly name of the class. Does not include the namespace.
	/// Example: Button
	public $typeName: string;
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
			this.$typeName = "";
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

			// For the devs who don't want to use the $ in front of the properties.
			if (tempObj.name != null)
				this.$name = tempObj.name;

			if (tempObj.typeName != null)
				this.$typeName = tempObj.typeName;

			if (tempObj.extends != null)
				this.$extends = tempObj.extends;

			if (tempObj.functions != null)
				this.$functions = tempObj.functions;

			if (tempObj.parseEvents != null)
				this.$parseEvents = tempObj.parseEvents;

			if (tempObj.methods != null)
				this.$methods = tempObj.methods;

			if (tempObj.properties != null)
				this.$properties = tempObj.properties;

			if (tempObj.generator != null)
				this.$generator = tempObj.generator;

			if (tempObj.$name != null)
				this.$name = tempObj.$name;

			if (tempObj.$typeName != null)
				this.$typeName = tempObj.$typeName;

			if (tempObj.$extends != null)
				this.$extends = tempObj.$extends;

			if (tempObj.$functions != null)
				this.$functions = tempObj.$functions;

			if (tempObj.$parseEvents != null)
				this.$parseEvents = tempObj.$parseEvents;

			if (tempObj.$methods != null)
				this.$methods = tempObj.$methods;

			if (tempObj.$properties != null)
				this.$properties = tempObj.$properties;

			if (tempObj.$generator != null)
				this.$generator = tempObj.$generator;
		}

		if ((this.$name.indexOf (".") > -1) || (this.$name.indexOf ("/") > -1))
		{
			let classes: string[] = GeneratorClass.getClassFromString (this.$name);
			this.$typeName = classes[classes.length - 1];
		}
	}

	/// Get this class's name. Include the namespace.
	getName (): string
	{
		return (this.$name);
	}

	/// Get this class's typename. Does not include the namespace.
	getTypeName (): string
	{
		return (this.$typeName);
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

	/// Get the full class name using a different separators.
	getClassUsingSeparator (separator: string): string
	{
		let classes: string[] = GeneratorClass.getClassFromString (this.getName());
		let output: string = "";

		for (let iIdx = 0; iIdx < classes.length; iIdx++)
		{
			if (iIdx == (classes.length - 1))
				output += classes[iIdx];
			else
				output += classes[iIdx] + separator;
		}

		return (output);
	}

	/// Add a function.
	addFunction (func: GeneratorFunction): void
	{
		this[func.name] = utils.keepContext (function (...args): void
			{
				this.$generator.functionCalled.apply (this.$generator, [func, args]);
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