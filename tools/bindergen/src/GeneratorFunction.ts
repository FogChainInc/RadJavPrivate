import { GeneratorMap } from "./GeneratorMap";

/// A generator 
export class GeneratorFunction
{
	/// The name of this function or method.
	public name: string;
	/// The code to be executed once this function is generated.
	public code: string;
	/// Map the native object created in code to the 
	public map: GeneratorMap;

	constructor ()
	{
		this.name = "";
		this.code = "";
		this.map = null;
	}
}