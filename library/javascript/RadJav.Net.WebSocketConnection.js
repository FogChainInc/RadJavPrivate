var RadJav;
(function (RadJav) {
    var Net;
    (function (Net) {
        var WebSocketConnection = (function () {
            function WebSocketConnection(httpConnection, request) {
                this._url = "";
                this._appObj = null;
                this._socket = null;
                this._events = {};
                if (httpConnection !== null) {
                    this._url = RadJav.setDefaultValue(httpConnection["_url"], "");
                    this._socket = RadJav.setDefaultValue(httpConnection["_socket"], null);
                    this._events = RadJav.setDefaultValue(httpConnection["_events"], {});
                }
                if (this["_init"] != null) {
                    this["_init"].apply(this, arguments);
                    if (httpConnection != undefined &&
                        request != undefined) {
                        this["_on"]("open", RadJav.keepContext(function () {
                            if (this._events["open"] != null) {
                                this._events["open"]();
                            }
                        }, this));
                        this["_on"]("message", RadJav.keepContext(function (data) {
                            if (this._events["message"] != null) {
                                this._events["message"](data);
                            }
                        }, this));
                        this["_on"]("error", RadJav.keepContext(function (code, description) {
                            if (this._events["error"] != null) {
                                this._events["error"](code, description);
                            }
                        }, this));
                        this["_on"]("close", RadJav.keepContext(function () {
                            if (this._events["close"] != null) {
                                this._events["close"]();
                            }
                        }, this));
                    }
                }
            }
            WebSocketConnection.prototype.connect = function (url) {
                if (url === void 0) { url = ""; }
                if (url !== "")
                    this._url = url;
                var promise = new Promise(RadJav.keepContext(function (resolve, reject) {
                    if (this["_connect"] != null) {
                        this["_on"]("open", RadJav.keepContext(function () {
                            if (this._events["open"] != null) {
                                this._events["open"]();
                            }
                            resolve();
                        }, this));
                        this["_on"]("message", RadJav.keepContext(function (data) {
                            if (this._events["message"] != null) {
                                this._events["message"](data);
                            }
                        }, this));
                        this["_on"]("error", RadJav.keepContext(function (code, description) {
                            reject();
                            if (this._events["error"] != null) {
                                this._events["error"](code, description);
                            }
                        }, this));
                        this["_on"]("close", RadJav.keepContext(function () {
                            if (this._events["close"] != null) {
                                this._events["close"]();
                            }
                        }, this));
                        this["_connect"](this._url);
                    }
                    else {
                        if (WebSocket == null) {
                            reject(RadJav._lang.websocketsNotSupported);
                            return;
                        }
                        this._socket = new WebSocket(this._url);
                        this._socket.onopen = RadJav.keepContext(function () {
                            resolve();
                            if (this._events["connected"] != null) {
                                this._events["connected"]();
                            }
                        }, this);
                        this._socket.onerror = RadJav.keepContext(function (error) {
                            reject(error);
                            if (this._events["error"] != null) {
                                this._events["error"](error);
                            }
                        }, this);
                        this._socket.onmessage = RadJav.keepContext(function (message) {
                            if (this._events["receive"] != null) {
                                this._events["receive"](message.data);
                            }
                        }, this);
                        this._socket.onclose = RadJav.keepContext(function (message) {
                            if (this._events["close"] != null) {
                                this._events["close"]();
                            }
                        }, this);
                    }
                }, this));
                return promise;
            };
            WebSocketConnection.prototype.send = function (data) {
                if (this["_send"] != null) {
                    this["_send"].apply(this, arguments);
                }
                else {
                    if (this._socket !== null)
                        this._socket.send(data);
                }
            };
            WebSocketConnection.prototype.close = function () {
                if (this["_close"] != null) {
                    this["_close"].apply(this, arguments);
                }
                else {
                    this._socket.close();
                }
            };
            WebSocketConnection.prototype.on = function (event, func) {
                this._events[event] = func;
            };
            return WebSocketConnection;
        }());
        Net.WebSocketConnection = WebSocketConnection;
        Net.WebSocketClient = WebSocketConnection;
        function handleUpgrade(connection, request) {
            var webSocketConnection = new WebSocketConnection(connection, request);
            return webSocketConnection;
        }
        Net.handleUpgrade = handleUpgrade;
    })(Net = RadJav.Net || (RadJav.Net = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.Net.WebSocketConnection.js.map