/// A text block.
export class Block
{
	/// The file where this block is at.
	public file: string;
	/// The starting position.
	public start: number;
	/// The end position.
	public end: number;
	/// The keyword start position.
	public keywordStart: number;
	/// The contents found from the block.
	public contents: string;
	/// The full file content.
	public fileContent: string;
	/// The full line.
	public fullLine: string;

	constructor (start: number, end: number)
	{
		this.file = "";
		this.start = start;
		this.end = end;
		this.keywordStart = this.start + `/*^:`.length;
		this.contents = "";
		this.fileContent = "";
		this.fullLine = "";
	}

	/// Find a block in some content.
	static findBlock (content: string, startPos: number = 0): Block
	{
		let start: number = content.indexOf (`/*^:`, startPos);
		let end: number = content.indexOf ("*/", start);
		let block: Block = null;
	
		if (start > -1)
		{
			block = new Block (start, end);
			block.fileContent = content;
			block.contents = content.substring (block.keywordStart, end);
			block.fullLine = content.substring (start, end + 2);
		}
	
		return (block);
	}

	/// Find all blocks in content.
	static findBlocks (content: string): Block[]
	{
		let blocks: Block[] = [];
		let pos: number = 0;
	
		while (true)
		{
			let block: Block = Block.findBlock (content, pos);

			if (block != null)
			{
				blocks.push (block);
				pos = block.end + 1;
			}
			else
				break;
		}
	
		return (blocks);
	}
}