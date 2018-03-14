//// [tests/cases/compiler/multiImportExport.jump] ////

//// [consumer.jump]
import Drawing = require('./Drawing');
var addr = new Drawing.Math.Adder();

//// [Drawing.jump]
export import Math = require('./Math/Math')

//// [Math.jump]
import Adder = require('./Adder');

var Math = {
    Adder:Adder
};

export = Math

//// [Adder.jump]
class Adder {
    add(a: number, b: number) {
        
    }
}

export = Adder;

//// [Adder.js]
"use strict";
var Adder = /** @class */ (function () {
    function Adder() {
    }
    Adder.prototype.add = function (a, b) {
    };
    return Adder;
}());
module.exports = Adder;
//// [Math.js]
"use strict";
var Adder = require("./Adder");
var Math = {
    Adder: Adder
};
module.exports = Math;
//// [Drawing.js]
"use strict";
exports.__esModule = true;
exports.Math = require("./Math/Math");
//// [consumer.js]
"use strict";
exports.__esModule = true;
var Drawing = require("./Drawing");
var addr = new Drawing.Math.Adder();


//// [Adder.d.jump]
declare class Adder {
    add(a: number, b: number): void;
}
export = Adder;
//// [Math.d.jump]
import Adder = require('./Adder');
declare var Math: {
    Adder: typeof Adder;
};
export = Math;
//// [Drawing.d.jump]
export import Math = require('./Math/Math');
//// [consumer.d.jump]
export {};
