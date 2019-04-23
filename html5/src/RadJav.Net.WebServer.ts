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
		export class WebServer
		{
			/** @property {string} [_address=0.0.0.0]
			* The address.
			*/
			private _address: string;

			/** @property {number} [_port=80]
			* The port.
			*/
			private _port: number;

			/** @property {number} [_type=RadJav.Net.WebServerTypes.HTTP]
			* The server type.
			*/
			private _type: RadJav.Net.WebServerTypes;
			
			/** @property {Mixed} [_appObj=null]
			* The native web server.
			*/
			private _appObj: any;

			constructor (type: RadJav.Net.WebServerTypes = RadJav.Net.WebServerTypes.HTTP)
			{
				this._type = type;
				this._appObj = null;
				
				if(this["_init"] != null)
					this["_init"].apply(this, arguments);
			}

			start (address: string = "0.0.0.0", port: number = 80): void
			{
				this._address = address;
				this._port = port;

				if(this["_start"] != null)
				{
					this["_start"].apply(this, arguments);
				}
			}

			stop (): void
			{
				if (this["_stop"] != null)
				{
					this["_stop"].apply(this, arguments);
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

		export enum WebServerTypes
		{
			HTTP = 1, 
			HTTPS = 2
		}

		export class WebServerResponse
		{
            httpVersionMajor: number;

            httpVersionMinor: number;

			statusCode: number;

            statusMessage: string;

			headers: object;

            payload: ArrayBuffer | string;

			setHttpVersion (major: number, minor: number)
			{
				this.httpVersionMajor = major;
				this.httpVersionMinor = minor;
			}

			setStatus (status: number, statusMessage?: string)
			{
				this.statusCode = status;

				if (statusMessage != null)
					this.statusMessage = statusMessage;
			}

			setHeader (headerName: string, headerValue: string | Array<string>)
			{
				this.headers[headerName] = headerValue;
			}

			setHeaders (headers: Object)
			{
				for (let property in headers)
				{
					if (headers.hasOwnProperty(property))
					{
						this.headers[property] = headers[property];
					}
				}
			}

			setPayload (data: string | ArrayBuffer)
			{
				this.payload = data;
			}

            constructor()
            {
				this.httpVersionMajor = 1;
				this.httpVersionMinor = 0;
                this.statusCode = 0;
                this.statusMessage = "";
                this.headers = {};
				this.payload = "";
            }
		}
	}
}

