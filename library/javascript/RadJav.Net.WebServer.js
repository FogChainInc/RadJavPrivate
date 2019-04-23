var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var WebServer = (function () {
            function WebServer(type) {
                if (type === void 0) { type = RadJav.Net.WebServerTypes.HTTP; }
                this._type = type;
                this._appObj = null;
                if (this["_init"] != null)
                    this["_init"].apply(this, arguments);
            }
            WebServer.prototype.start = function (address, port) {
                if (address === void 0) { address = "0.0.0.0"; }
                if (port === void 0) { port = 80; }
                this._address = address;
                this._port = port;
                if (this["_start"] != null) {
                    this["_start"].apply(this, arguments);
                }
            };
            WebServer.prototype.stop = function () {
                if (this["_stop"] != null) {
                    this["_stop"].apply(this, arguments);
                }
            };
            WebServer.prototype.on = function (event, func) {
                if (this["_on"] != null) {
                    this["_on"].apply(this, arguments);
                }
            };
            return WebServer;
        }());
        Net.WebServer = WebServer;
        var WebServerTypes;
        (function (WebServerTypes) {
            WebServerTypes[WebServerTypes["HTTP"] = 1] = "HTTP";
            WebServerTypes[WebServerTypes["HTTPS"] = 2] = "HTTPS";
        })(WebServerTypes = Net.WebServerTypes || (Net.WebServerTypes = {}));
        var WebServerResponse = (function () {
            function WebServerResponse() {
                this.httpVersionMajor = 1;
                this.httpVersionMinor = 0;
                this.statusCode = 0;
                this.statusMessage = "";
                this.headers = {};
                this.payload = "";
            }
            WebServerResponse.prototype.setHttpVersion = function (major, minor) {
                this.httpVersionMajor = major;
                this.httpVersionMinor = minor;
            };
            WebServerResponse.prototype.setStatus = function (status, statusMessage) {
                this.statusCode = status;
                if (statusMessage != null)
                    this.statusMessage = statusMessage;
            };
            WebServerResponse.prototype.setHeader = function (headerName, headerValue) {
                this.headers[headerName] = headerValue;
            };
            WebServerResponse.prototype.setHeaders = function (headers) {
                for (var property in headers) {
                    if (headers.hasOwnProperty(property)) {
                        this.headers[property] = headers[property];
                    }
                }
            };
            WebServerResponse.prototype.setPayload = function (data) {
                this.payload = data;
            };
            return WebServerResponse;
        }());
        Net.WebServerResponse = WebServerResponse;
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.Net.WebServer.js.map