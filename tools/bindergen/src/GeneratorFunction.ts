import { GeneratorMap } from "./GeneratorMap";
import { GeneratorFunctionArgument } from "./GeneratorFunctionArgument";
import { GeneratorClass } from "./GeneratorClass";

/// A function 
export class GeneratorFunction
{
	/// The name of this function or method.
	public name: string;
	/// The code to be executed once this function is generated.
	public code: string;
	/// Map the native object created in code to the binder.
	public map: GeneratorMap;
	/// What this returns as.
	public returns: string;
	/// Extra data associated with this function.
	public data: string;
	/// Arguments for this function.
	public arguments: GeneratorFunctionArgument[];
	/// The parent class storing this function.
	public parentClass: GeneratorClass;
	/** Events. Types can be:
	 * * generate
	 *  * Executes when a string is being generated.
	 */
	public events: { [name: string]: Function };
	/// The function to execute when this function is called.
	public execFunc: Function;

	constructor (parentClass: GeneratorClass = null)
	{
		this.name = "";
		this.code = "";
		this.map = null;
		this.returns = "";
		this.data = "";
		this.arguments = [];
		this.parentClass = parentClass;
		this.events = {};
		this.execFunc = null;
	}

	/// Execute a function when this function is called.
	exec (func: Function)
	{
		this.execFunc = func;
	}

	/// Trigger on an event.
	on (event: string, func: Function): void
	{
		this.events[event] = func;
	}

	/// Generate a string from this function.
	generate (): string
	{
		let str: string = "";

		if (this.events["generate"] != null)
			str = this.events["generate"].apply (this, arguments);

		return (str);
	}
}