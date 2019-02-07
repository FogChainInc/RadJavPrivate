/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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

/// <reference path="RadJav.ts" />

namespace RadJav
{
	export namespace Net
	{
		/// A WebSocket Server.
		export class WebSocketServer
		{
			/// The port to listen on.
			port: number;
			/// The websocket to use.
			_webSocket: any;	// This will only be used on desktop machines and will be instantiated in _init().
			
			constructor (port: number)
			{
				this.port = port;
				this._webSocket = null;
				
				// Only call _init() if this is running on desktop.
				if (typeof this["_init"] == "function")
					this["_init"] ();
			}

			/// Send a message to someone.
			send (id: string, msg: string | string[])
			{
			}

			/// Send a message to everyone.
			sendToAll (msg: string)
			{
			}

			/// Receive a message.
			receive (): string
			{
				return ("");
			}

			/// Start the server and listen on the port.
			listen (port: number)
			{
			}

			/// Executes on an event.
			on (eventName: string, func: Function): any
			{
			}

			/// Stop the server and close.
			close ()
			{
			}

			/// The on accept event.
			onAccept (func: ((id: string) => void)): void
			{
			}

			// The on receive event.
			onReceive (func: ((id: string, msg: string) => void)): void
			{
			}
		}
	}
}

