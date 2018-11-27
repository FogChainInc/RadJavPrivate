import { GeneratorMap } from "./GeneratorMap";
import { GeneratorFunctionArgument } from "./GeneratorFunctionArgument";

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

	constructor ()
	{
		this.name = "";
		this.code = "";
		this.map = null;
		this.returns = "";
		this.data = "";
		this.arguments = [];
	}
}