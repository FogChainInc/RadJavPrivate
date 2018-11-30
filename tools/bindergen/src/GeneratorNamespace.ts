import { Generator } from "./Generator";
import { GeneratorClass } from "./GeneratorClass";
import { GeneratorReference } from "./GeneratorReference";

/// A namespace.
export class GeneratorNamespace
{
	/// The name of this namespace.
	public $name: string;
	/// The parent generator.
	public $generator: Generator;
	/// The classes that will generate bindings or parse.
	protected $classes: { [name: string]: GeneratorClass };

	constructor (generator: Generator, name: string)
	{
		this.$name = name;
		this.$generator = generator;
		this.$classes = {};
	}

	/// Create a class for either parsing or generating.
	async createClass (variableName: string, className: string | GeneratorClass): Promise<GeneratorClass>
	{
		let genClass: GeneratorClass = new GeneratorClass (className);
		genClass.$generator = this.$generator;

		for (let iIdx = 0; iIdx < this.$generator.references.length; iIdx++)
		{
			let reference: GeneratorReference = this.$generator.references[iIdx];

			if (reference.events["classReference"] != null)
				await reference.events["classReference"].call (reference, genClass);
		}

		/// @fixme Throw an exception for any existing classes 
		/// with the same property name as one in this class.
		this[variableName] = genClass;
		this.$classes[variableName] = genClass;

		this.$generator.classCreated (genClass, []);

		return (genClass);
	}
}