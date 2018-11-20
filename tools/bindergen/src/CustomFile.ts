/// A custom file to write to.
export class CustomFile
{
	/// The id of this file.
	public id: string;
	/// The relative path to this file.
	public path: string;
	/// The contents of this file.
	public contents: string;

	constructor ()
	{
		this.id = "";
		this.path = "";
		this.contents = "";
	}
}