import { GeneratorMap } from "./GeneratorMap";

/// An argument for a function
export class GeneratorFunctionArgument
{
	/// The name of this argument.
	public name: string;
	/// The code to be executed once this function is generated.
	public code: string;
	/// The data type.
	public dataType: string;

	constructor (name: string = "", dataType: string = "")
	{
		this.name = name;
		this.code = "";
		this.dataType = dataType;
	}
}