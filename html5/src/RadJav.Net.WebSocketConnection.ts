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
/// <reference path="RadJav.Net.HttpConnection.ts" />

namespace RadJav
{
	export namespace Net
	{
		/// Web socket connection for server or client.
		export class WebSocketConnection
		{
			/// The URL.
			private _url: string;
			/// The native websocket object.
			private _appObj: any;
			/// The web socket that will be used for the connection.
			_socket: WebSocket;
			/** 
			 * The events to execute when triggered.
			 * Possible events:
			 * * connected
			 * * error
			 * * receive
			 * * close
			 */
			_events: Object;

			constructor (httpConnection?: RadJav.Net.HttpConnection | WebSocketConnection, request?: RadJav.Net.HttpIncomingMessage)
			{
				this._url = RadJav.setDefaultValue(httpConnection["_url"], "");
				this._socket = RadJav.setDefaultValue(httpConnection["_socket"], null);
				this._events = RadJav.setDefaultValue(httpConnection["_events"], {});

				if(this["_init"] != null)
					this["_init"].apply(this, arguments);
			}

			connect (url: string = ""): Promise<void>
			{
				if (url !== "")
					this._url = url;

				if(this["_connect"] != null)
				{
					return (this["_connect"].apply(this, arguments));
				}
				else
				{
					var promise: Promise<void> = new Promise(
					RadJav.keepContext(function (resolve, reject)
					{
						if (WebSocket == null)
						{
							reject(RadJav._lang.websocketsNotSupported);
							return;
						}
			
						this._socket = new WebSocket(this.url);
			
						this._socket.onopen = RadJav.keepContext(function () {
						resolve();
			
						if (this._events["connected"] != null) {
							this._events["connected"]();
						}
						}, this);
			
						this._socket.onerror = RadJav.keepContext(function (error) {
						reject(error);
			
						if (this._events["error"] != null) {
							this._events["error"](error);
						}
						}, this);
			
						this._socket.onmessage = RadJav.keepContext(function (message) {
						if (this._events["receive"] != null) {
							this._events["receive"](message.data);
						}
						}, this);
			
						this._socket.onclose = RadJav.keepContext(function (message) {
						if (this._events["close"] != null) {
							this._events["close"]();
						}
						}, this);
					}, this)
					);

					return promise;
				}
			}

			send (data: string | ArrayBuffer): void
			{
				if (this["_send"] != null)
				{
					this["_send"].apply(this, arguments);
				}
				else
				{
					if (this._socket !== null)
						this._socket.send(data);
				}
			}

			close (): void
			{
				if (this["_close"] != null)
				{
					this["_close"].apply(this, arguments);
				}
				else
				{
					this._socket.close ();
				}
			}

            on (event: string, func: Function): void
			{
				if (this["_on"] != null)
				{
					this["_on"].apply(this, arguments);
				}
				else
				{
					this._events[event] = func;
				}
			}
		}

		/// Alias for WebSocketConnection.
		export let WebSocketClient = WebSocketConnection;

        export function handleUpgrade(connection: RadJav.Net.HttpConnection, request: RadJav.Net.HttpIncomingMessage) : WebSocketConnection
        {
            var webSocketConnection = new WebSocketConnection(connection, request);
            
            return webSocketConnection;
        }
    }
}
