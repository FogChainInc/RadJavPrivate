import { GeneratorFunction } from "./GeneratorFunction";
import { Generator } from "./Generator";
import { GeneratorNamespace } from "./GeneratorNamespace";
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
	/// The namespaces used.
	public $namespaces: GeneratorNamespace[];

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
			this.$namespaces = [];
		}
		else
		{
			let tempObj: GeneratorClass = (<GeneratorClass>obj);

			// For the devs who don't want to use the $ in front of the properties.
			// @ts-ignore
			if (tempObj.name != null)
			// @ts-ignore
				this.$name = tempObj.name;

			// @ts-ignore
			if (tempObj.typeName != null)
				// @ts-ignore
				this.$typeName = tempObj.typeName;

			// @ts-ignore
			if (tempObj.extends != null)
				// @ts-ignore
				this.$extends = tempObj.extends;

			// @ts-ignore
			if (tempObj.functions != null)
				// @ts-ignore
				this.$functions = tempObj.functions;

			// @ts-ignore
			if (tempObj.parseEvents != null)
				// @ts-ignore
				this.$parseEvents = tempObj.parseEvents;

			// @ts-ignore
			if (tempObj.methods != null)
				// @ts-ignore
				this.$methods = tempObj.methods;

			// @ts-ignore
			if (tempObj.properties != null)
				// @ts-ignore
				this.$properties = tempObj.properties;

			// @ts-ignore
			if (tempObj.generator != null)
				// @ts-ignore
				this.$generator = tempObj.generator;

			// @ts-ignore
			if (tempObj.namespaces != null)
				// @ts-ignore
				this.$namespaces = tempObj.namespaces;

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

			if (tempObj.$namespaces != null)
				this.$namespaces = tempObj.$namespaces;
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
		this[func.name] = utils.keepContext (function (...args): string
			{
				let genClass: GeneratorClass = this;	// Why is this an error?
				let result: string = genClass.$generator.functionCalled.apply (genClass.$generator, [func, args]);

				return (result);
			}, this, [func]);
		this.$methods[func.name] = func;
	}

	/// Create a parse event.
	parse (event: string, func: Function): void
	{
		this.$parseEvents[event] = func;
	}
}