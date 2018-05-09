/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    namespace Net {
        class WebServer {
            port: Number;
            _serverType: RadJav.Net.WebServerTypes;
            _webServer: any;
            constructor();
        }
        enum WebServerTypes {
            HTTP = 1,
            HTTPS = 2,
        }
    }
}
