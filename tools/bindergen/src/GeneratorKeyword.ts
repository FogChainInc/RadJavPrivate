/// A keyword.
export class GeneratorKeyword
{
	/// The keyword.
	public keyword: string;
	/// The pass associated with this keyword.
	public pass: string;
	/// The value.
	public value: string;

	constructor ()
	{
		this.keyword = "";
		this.pass = "";
		this.value = "";
	}
}