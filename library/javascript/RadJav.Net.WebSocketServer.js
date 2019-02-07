var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var WebSocketServer = (function () {
            function WebSocketServer(port) {
                this.port = port;
                this._webSocket = null;
                if (typeof this["_init"] == "function")
                    this["_init"]();
            }
            WebSocketServer.prototype.send = function (id, msg) {
            };
            WebSocketServer.prototype.sendToAll = function (msg) {
            };
            WebSocketServer.prototype.receive = function () {
                return ("");
            };
            WebSocketServer.prototype.listen = function (port) {
            };
            WebSocketServer.prototype.on = function (eventName, func) {
            };
            WebSocketServer.prototype.close = function () {
            };
            WebSocketServer.prototype.onAccept = function (func) {
            };
            WebSocketServer.prototype.onReceive = function (func) {
            };
            return WebSocketServer;
        }());
        Net.WebSocketServer = WebSocketServer;
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
