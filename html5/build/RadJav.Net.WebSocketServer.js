var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var WebSocketServer = (function () {
            function WebSocketServer() {
                this.port = "9229";
                this._webSocket = null;
                if (typeof this["_init"] == "function")
                    this["_init"]();
            }
            WebSocketServer.prototype.onAccept = function (id) {
                alert(this.id);
            };
            WebSocketServer.prototype.onReceive = function (id, msg) {
                alert(this.msg);
            };
            return WebSocketServer;
        }());
        Net.WebSocketServer = WebSocketServer;
        var WebSocketClient = (function () {
            function WebSocketClient() {
                this.port = 0;
                this.url = "";
                this._webSocket = null;
            }
            return WebSocketClient;
        }());
        Net.WebSocketClient = WebSocketClient;
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
