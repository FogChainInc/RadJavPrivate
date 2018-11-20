/// Aides in helping map values from native to an external resource.
export class GeneratorMap
{
	/// The native C++ object to map to a binding object.
	public native: string;
	/// The binding object to bind the native object to.
	public external: string;

	constructor ()
	{
		this.native = "";
		this.external = "";
	}
}