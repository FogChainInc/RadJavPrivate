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
    /** 
     * A web socket client.
     * Available on platforms: Windows,Linux,OSX,HTML5
     */
    export class WebSocketClient {
      constructor(obj) {
        if (obj == null) {
          obj = {};
        }

        if (typeof obj == "string") {
          var tempobj = obj;
          obj = {};
          obj.url = tempobj;
        }
        this.url = RadJav.setDefaultValue(obj.url, "");
        this._socket = RadJav.setDefaultValue(obj._socket, null);
        this._events = RadJav.setDefaultValue(obj._events, {});

        if ((<any>this)._init != null)
          (<any>this)._init(this.url);
      }

      /** 
       * The URL to open.
       */
      url: string;
      /** 
       * The web socket that will be used for the connection.
       */
      _socket: any;
      /** 
       * The events to execute when triggered.
       * Possible events:
       * * connected
       * * error
       * * receive
       * * close
       */
      _events: Object;

      /** 
       * Connect to the URL.
       * @return {Promise} The promise to execute when the connection has completed.
       */
      connect(): Promise<any>
      {
        var promise = new Promise(
          RadJav.keepContext(function (resolve, reject) {
            if (WebSocket == null) {
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

      /** 
       * Send a message to the server.
       * @param {String/Object} message The message to send.
       */
      send(message: string | object): void {
        this._socket.send(message);
      }

      /** 
       * Call a function when an event is executed.
       * @param {String} eventName The name of the event.
       * @param {Function} func The function to execute when the event has been executed.
       */
      on(eventName: string, func: Function) {
        this._events[eventName] = func;
      }
    }
  }
}
