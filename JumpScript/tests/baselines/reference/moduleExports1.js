//// [moduleExports1.jump]
export module JumpScript.Strasse.Street {
	export class Rue {
		public address:string;
	}	
}

var rue = new JumpScript.Strasse.Street.Rue();

rue.address = "1 Main Street";

void 0;

if (!module.exports) module.exports = "";

//// [moduleExports1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var JumpScript;
    (function (JumpScript) {
        var Strasse;
        (function (Strasse) {
            var Street;
            (function (Street) {
                var Rue = /** @class */ (function () {
                    function Rue() {
                    }
                    return Rue;
                }());
                Street.Rue = Rue;
            })(Street = Strasse.Street || (Strasse.Street = {}));
        })(Strasse = JumpScript.Strasse || (JumpScript.Strasse = {}));
    })(JumpScript = exports.JumpScript || (exports.JumpScript = {}));
    var rue = new JumpScript.Strasse.Street.Rue();
    rue.address = "1 Main Street";
    void 0;
    if (!module.exports)
        module.exports = "";
});
