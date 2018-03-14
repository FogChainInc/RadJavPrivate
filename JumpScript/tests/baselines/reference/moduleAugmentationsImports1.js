//// [tests/cases/compiler/moduleAugmentationsImports1.jump] ////

//// [a.jump]
export class A {}

//// [b.jump]
export class B {x: number;}

//// [c.d.jump]
declare module "C" {
    class Cls {y: string; }
}

//// [d.jump]
/// <reference path="c.d.jump"/>

import {A} from "./a";
import {B} from "./b";
import {Cls} from "C";

A.prototype.getB = function () { return undefined; }
A.prototype.getCls = function () { return undefined; }

declare module "./a" {
    interface A {
        getB(): B;
    }
}

declare module "./a" {
    interface A {
        getCls(): Cls;
    }
}

//// [main.jump]
import {A} from "./a";
import "d";

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
/// <reference path="c.d.jump"/>
define("d", ["require", "exports", "a"], function (require, exports, a_1) {
    "use strict";
    exports.__esModule = true;
    a_1.A.prototype.getB = function () { return undefined; };
    a_1.A.prototype.getCls = function () { return undefined; };
});
define("main", ["require", "exports", "d"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var a;
    var b = a.getB().x.toFixed();
    var c = a.getCls().y.toLowerCase();
});


//// [f.d.jump]
/// <reference path="tests/cases/compiler/c.d.jump" />
declare module "a" {
    export class A {
    }
}
declare module "b" {
    export class B {
        x: number;
    }
}
declare module "d" {
    import { B } from "b";
    import { Cls } from "C";
    module "a" {
        interface A {
            getB(): B;
        }
    }
    module "a" {
        interface A {
            getCls(): Cls;
        }
    }
}
declare module "main" {
    import "d";
}
