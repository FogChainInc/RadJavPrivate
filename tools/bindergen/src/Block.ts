export class Block
{
	public start: number;
	public end: number;
	public keywordStart: number;
	public contents: string;

	constructor (start: number, end: number)
	{
		this.start = start;
		this.end = end;
		this.keywordStart = this.start + `/*^:`.length;
		this.contents = "";
	}

	static findBlock (content: string): Block
	{
		let start: number = content.indexOf (`/*^:`);
		let end: number = content.indexOf ("*/", start);
		let block: Block = null;
	
		if (start > -1)
		{
			block = new Block (start, end);
			block.contents = content.substring (block.keywordStart, (end - 1));
		}
	
		return (block);
	}
}