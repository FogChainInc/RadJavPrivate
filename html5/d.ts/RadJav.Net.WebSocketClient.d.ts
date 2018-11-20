/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace Net {
        class WebSocketClient {
            constructor(obj: any);
            url: string;
            _socket: any;
            _events: Object;
            connect(eventName: any, func: Function): Promise<any>;
            send(message: string | object): void;
            on(eventName: string, func: Function): void;
        }
    }
}
