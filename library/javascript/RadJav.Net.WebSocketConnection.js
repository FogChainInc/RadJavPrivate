var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var WebSocketConnection = (function () {
            function WebSocketConnection(httpConnection, request) {
                this._url = "";
                this._appObj = null;
                if (this["_init"] != null)
                    this["_init"].apply(this, arguments);
            }
            WebSocketConnection.prototype.connect = function (url) {
                this._url = url;
                if (this["_connect"] != null) {
                    this["_connect"].apply(this, arguments);
                }
            };
            WebSocketConnection.prototype.send = function (data) {
                if (this["_send"] != null) {
                    this["_send"].apply(this, arguments);
                }
            };
            WebSocketConnection.prototype.close = function () {
                if (this["_close"] != null) {
                    this["_close"].apply(this, arguments);
                }
            };
            WebSocketConnection.prototype.on = function (event, func) {
                if (this["_on"] != null) {
                    this["_on"].apply(this, arguments);
                }
            };
            return WebSocketConnection;
        }());
        Net.WebSocketConnection = WebSocketConnection;
        function handleUpgrade(connection, request) {
            var webSocketConnection = new WebSocketConnection(connection, request);
            return webSocketConnection;
        }
        Net.handleUpgrade = handleUpgrade;
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.Net.WebSocketConnection.js.map