/*
    MIT-LICENSE
    Copyright (c) 2017-2018 Higher Edge Software, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
    LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
var RadJav;
(function (RadJav) {
    var Crypto;
    (function (Crypto) {
        /// PrivateKey
        var PrivateKey = /** @class */ (function () {
            function PrivateKey() {
                //this._engine = null;
                if (this._init != null)
		    this._init.apply(this, arguments);
            }

            /// Sign
            PrivateKey.prototype.sign = function (data) {
            };

            PrivateKey.prototype.signSync = function (data) {
            };
	    
            /// Encrypt
            PrivateKey.prototype.encrypt = function (data) {
            };

            PrivateKey.prototype.encryptSync = function (data) {
            };

	    /// 
            PrivateKey.prototype.getPublicKey = function () {
            };
	    
	    /// 
            PrivateKey.prototype.savePemSync = function (path, password) {
            };
	    
            return PrivateKey;
        }());
        Crypto.PrivateKey = PrivateKey;
    })(Crypto = RadJav.Crypto || (RadJav.Crypto = {}));
})(RadJav || (RadJav = {}));
