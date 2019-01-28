/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace Net {
        class WebSocketServer {
            port: string;
            _webSocket: any;
            constructor();
            onAccept(id: string): void;
            onReceive(id: string, msg: string): void;
        }
        class WebSocketClient {
            /** @property {Number} [port=0]
            * The port.
            */
            port: Number;
            /** @property {String} [url=""]
            * The URL.
            */
            url: String;
            /** @property {Mixed} [_webSocket=null]
            * The port.
            */
            _webSocket: any;
            constructor();
        }
    }
}
