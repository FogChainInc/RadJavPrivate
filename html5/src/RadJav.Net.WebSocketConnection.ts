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
		/// Web server
		export class WebSocketConnection
		{
			/** @property {string} [_url=""]
			* The URL.
			*/
			private _url: string;

			/** @property {Mixed} [_appObj=null]
			* The native websocket object.
			*/
			private _appObj: any;

			constructor (httpConnection?: RadJav.Net.HttpConnection, request?: RadJav.Net.HttpIncomingMessage)
			{
                this._url = "";
				this._appObj = null;
				
				if(this["_init"] != null)
					this["_init"].apply(this, arguments);
			}

			connect (url: string): void
			{
				this._url = url;

				if(this["_connect"] != null)
				{
					this["_connect"].apply(this, arguments);
				}
			}

			send (data: string | ArrayBuffer): void
			{
				if (this["_send"] != null)
				{
					this["_send"].apply(this, arguments);
				}
			}

			close (): void
			{
				if (this["_close"] != null)
				{
					this["_close"].apply(this, arguments);
				}
			}

            on (event: string, func: Function): void
			{
				if (this["_on"] != null)
				{
					this["_on"].apply(this, arguments);
				}
			}
        }

        export function handleUpgrade(connection: RadJav.Net.HttpConnection, request: RadJav.Net.HttpIncomingMessage) : WebSocketConnection
        {
            var webSocketConnection = new WebSocketConnection(connection, request);
            
            return webSocketConnection;
        }
    }
}
