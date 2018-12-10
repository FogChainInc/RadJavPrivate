/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace Net {
        /** @class RadJav.Net.WebSocketClient
         * A web socket.
         * Available on platforms: Windows,Linux,OSX,HTML5
         */
        class WebSocketClient {
            constructor(obj: any);
            /** @property {String} [url=""]
             * @protected
             * The URL to open.
             */
            url: string;
            /** @property {Object} [_socket=null]
             * @protected
             * The web socket that will be used for the connection.
             */
            _socket: any;
            /** @property {Object} [_events={}]
             * @protected
             * The events to execute when triggered.
             */
            _events: Object;
            /** @method connect
             * Connect to the URL.
             * @return {Promise} The promise to execute when the connection has completed.
             */
            connect(eventName: any, func: Function): Promise<any>;
            /** @method send
             * Send a message to the server.
             * @param {String/Object} message The message to send.
             */
            send(message: string | object): void;
            /** @method on
             * Call a function when an event is executed.
             * @param {String} eventName The name of the event.
             * @param {Function} func The function to execute when the event has been executed.
             */
            on(eventName: string, func: Function): void;
        }
    }
}
