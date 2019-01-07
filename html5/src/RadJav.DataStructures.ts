/*
	MIT-LICENSE
	Copyright (c) 2019 FogChain, Corp

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
	and associated documentation files (the "Software"), to deal in the Software without restriction, 
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial 
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

namespace RadJav
{
	/// Data structures.
	export namespace DataStructures
	{
		/// A linked list.
		export class LinkedList
		{
			/// The root of this linked list.
			public root: Node;
			/// The end of this linked list.
			public end: Node;

			constructor ()
			{
				this.root = null;
				this.end = null;
			}
		}

		/// A node.
		export class Node
		{
			/// Data associated with this node.
			public data: any;
			/// The next node.
			public next: Node;
			/// The previous node.
			public prev: Node;

			constructor ()
			{
				this.data = null;
				this.next = null;
				this.prev = null;
			}
		}

		/// A blockchain
		export class Blockchain
		{
			/// The genesis block.
			public genesisBlock: Block;
			/// The block at the top.
			public topBlock: Block;
		}

		/// A block that's part of a blockchain.
		export class Block
		{
			/// The height of this block.
			public height: number;
			/// The data stored in this block.
			public data: { [name: string]: any };
		}
	}
}

