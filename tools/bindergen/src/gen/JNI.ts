import { GeneratorClass } from "../GeneratorClass";

/// The JNI class to use for parsing.
export class JNI extends GeneratorClass
{
	public name: string;

	constructor ()
	{
		super ();

		this.name = "JNI";
	}
 
	
}