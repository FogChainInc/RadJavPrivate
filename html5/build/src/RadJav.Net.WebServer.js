var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var WebServer = (function () {
            function WebServer() {
                this.port = 80;
                this._serverType = RadJav.Net.WebServerTypes.HTTP;
            }
            return WebServer;
        }());
        Net.WebServer = WebServer;
        var WebServerTypes;
        (function (WebServerTypes) {
            WebServerTypes[WebServerTypes["HTTP"] = 1] = "HTTP";
            WebServerTypes[WebServerTypes["HTTPS"] = 2] = "HTTPS";
        })(WebServerTypes = Net.WebServerTypes || (Net.WebServerTypes = {}));
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
