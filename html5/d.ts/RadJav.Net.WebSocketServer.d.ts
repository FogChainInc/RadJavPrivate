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
            port: Number;
            url: String;
            _webSocket: any;
            constructor();
        }
    }
}
