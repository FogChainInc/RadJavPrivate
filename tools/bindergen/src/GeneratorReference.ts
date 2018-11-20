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
	public reference: ReferPathType;

	/// Events to be called.
	public events: { [name: string]: Function};

	constructor ()
	{
		this.name = "";
		this.path = "";
		this.reference = ReferPathType.FILE;
		this.events = {};
	}

	/// Call when an event has occurred.
	on (event: string, func: Function): void
	{
		this.events[event] = func;
	}
}