var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var HttpConnection = (function () {
            function HttpConnection() {
                this._appObj = null;
                if (this["_init"] != null)
                    this["_init"].apply(this, arguments);
            }
            return HttpConnection;
        }());
        Net.HttpConnection = HttpConnection;
        var HttpIncomingMessage = (function () {
            function HttpIncomingMessage() {
                this.method = "";
                this.url = "";
                this.httpVersion = "";
                this.httpVersionMajor = 0;
                this.httpVersionMinor = 0;
                this.headers = {};
                this.payload = new Uint8Array(0);
                this._appObj = null;
            }
            return HttpIncomingMessage;
        }());
        Net.HttpIncomingMessage = HttpIncomingMessage;
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.Net.HttpConnection.js.map