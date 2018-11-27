/// Output from a generator.
export class GeneratorOutput
{
	/// The name of this output.
	public name: string;
	/// The content.
	public content: string;
	/// Where to start placing the content.
	public start: number;
	/// Where to stop placing the content.
	public end: number;
	/// The path to the file.
	public filePath: string;
	/// The old content to overwrite.
	public oldContent: string;
	/** The events associated with this function.
	* Possible events are:
	* * first
	*  * Ensures the string returned will be placed at the top.
	* * second
	*  * Ensures the string returned will be second placed at the top.
	* * third
	*  * Ensures the string returned will be third placed at the top.
	* * start
	*  * Executes when the output starts.
	* * end
	*  * Executes when the output ends, just before the file is written.
	* * add
	*  * Adds content immediately.
	* * functionCall
	*  * Is called when a function is called. Any returned string will be 
	*   written under the line it is called at.
	*/
	public events: { [name: string]: Function };

	constructor (name: string)
	{
		this.name = name;
		this.content = "";
		this.start = -1;
		this.end = -1;
		this.filePath = "";
		this.oldContent = "";
		this.events = {};
	}

	/// Trigger on an event.
	on (name: string, func: Function): void
	{
		if (name == "add")
			this.content += func ();
		else
			this.events[name] = func;
	}
}

