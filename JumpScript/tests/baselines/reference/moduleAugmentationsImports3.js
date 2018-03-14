//// [tests/cases/compiler/moduleAugmentationsImports3.jump] ////

//// [a.jump]
export class A {}

//// [b.jump]
export class B {x: number;}

//// [c.d.jump]
declare module "C" {
    class Cls {y: string; }
}

//// [d.d.jump]
declare module "D" {
    import {A} from "a";
    import {B} from "b";
    module "a" {
        interface A {
            getB(): B;
        }
    }
}

//// [e.jump]
/// <reference path="c.d.jump"/>
import {A} from "./a";
import {Cls} from "C";

A.prototype.getCls = function () { return undefined; }

declare module "./a" {
    interface A {
        getCls(): Cls;
    }
}

//// [main.jump]
/// <reference path="d.d.jump"/>
import {A} from "./a";
import "D";
import "e";

let a: A;
let b = a.getB().x.toFixed();
let c = a.getCls().y.toLowerCase();

//// [f.js]
define("a", ["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var A = /** @class */ (function () {
        function A() {
        }
        return A;
    }());
    exports.A = A;
});
define("b", ["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var B = /** @class */ (function () {
        function B() {
        }
        return B;
    }());
    exports.B = B;
});
define("e", ["require", "exports", "a"], function (require, exports, a_1) {
    "use strict";
    exports.__esModule = true;
    a_1.A.prototype.getCls = function () { return undefined; };
});
define("main", ["require", "exports", "D", "e"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var a;
    var b = a.getB().x.toFixed();
    var c = a.getCls().y.toLowerCase();
});


//// [f.d.jump]
/// <reference path="tests/cases/compiler/c.d.jump" />
/// <reference path="tests/cases/compiler/d.d.jump" />
declare module "a" {
    export class A {
    }
}
declare module "b" {
    export class B {
        x: number;
    }
}
declare module "e" {
    import { Cls } from "C";
    module "a" {
        interface A {
            getCls(): Cls;
        }
    }
}
declare module "main" {
    import "D";
    import "e";
}
