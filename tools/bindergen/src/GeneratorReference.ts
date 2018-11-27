import * as fs from "fs";
import * as request from "request";

/// The type of path.
export enum ReferPathType
{
	URL, 
	FILE
}

/// References used.
export class GeneratorReference
{
	/// The name of this reference.
	public name: string;
	/// The path to the reference.
	public path: string;
	/// The type of reference.
	public referenceType: ReferPathType;

	/** Events to be called.
	 * Event types:
	 * * classReference
	 *  * Whenever a class reference is created, the function will be called.
	*/
	public events: { [name: string]: Function};

	constructor ()
	{
		this.name = "";
		this.path = "";
		this.referenceType = ReferPathType.FILE;
		this.events = {};
	}

	/// Call when an event has occurred.
	on (event: string, func: Function): void
	{
		this.events[event] = func;
	}

	/// Get the contents of the data.
	async lookup (file: string): Promise<string>
	{
		let contents: string = "";

		if (this.referenceType == ReferPathType.FILE)
			contents = fs.readFileSync (this.path + "/" + file).toString ();

		if (this.referenceType == ReferPathType.URL)
		{
			contents = await (async function (path: string): Promise<string>
				{
					let newData: string = "";

					request (path + "/" + file, function (error, response, body)
						{
							if (error != null)
								throw new Error (error);

							newData = body;
						});

					return (newData);
				}(this.path));
		}

		return (contents);
	}

	/// Get the data associated with this reference.
	getData (input: string): string
	{
		let data: string = "";

		if (this.events["reference"] != null)
			data = this.events["reference"] ();

		return (data);
	}
}