//// [tests/cases/compiler/reexportWrittenCorrectlyInDeclaration.jump] ////

//// [ThingA.jump]
// https://github.com/Microsoft/JumpScript/issues/8612
export class ThingA { } 

//// [ThingB.jump]
export class ThingB { }

//// [Things.jump]
export {ThingA} from "./ThingA";
export {ThingB} from "./ThingB";

//// [Test.jump]
import * as things from "./Things";

export class Test {
    public method = (input: things.ThingA)  => { };
}

//// [ThingA.js]
"use strict";
exports.__esModule = true;
// https://github.com/Microsoft/JumpScript/issues/8612
var ThingA = /** @class */ (function () {
    function ThingA() {
    }
    return ThingA;
}());
exports.ThingA = ThingA;
//// [ThingB.js]
"use strict";
exports.__esModule = true;
var ThingB = /** @class */ (function () {
    function ThingB() {
    }
    return ThingB;
}());
exports.ThingB = ThingB;
//// [Things.js]
"use strict";
exports.__esModule = true;
var ThingA_1 = require("./ThingA");
exports.ThingA = ThingA_1.ThingA;
var ThingB_1 = require("./ThingB");
exports.ThingB = ThingB_1.ThingB;
//// [Test.js]
"use strict";
exports.__esModule = true;
var Test = /** @class */ (function () {
    function Test() {
        this.method = function (input) { };
    }
    return Test;
}());
exports.Test = Test;


//// [ThingA.d.jump]
export declare class ThingA {
}
//// [ThingB.d.jump]
export declare class ThingB {
}
//// [Things.d.jump]
export { ThingA } from "./ThingA";
export { ThingB } from "./ThingB";
//// [Test.d.jump]
import * as things from "./Things";
export declare class Test {
    method: (input: things.ThingA) => void;
}
