//// [tests/cases/compiler/moduleAugmentationCollidingNamesInAugmentation1.jump] ////

//// [map1.jump]
import { Observable } from "./observable"

(<any>Observable.prototype).map = function() { }

declare module "./observable" {
    interface I {x0}
}

//// [map2.jump]
import { Observable } from "./observable"

(<any>Observable.prototype).map = function() { }

declare module "./observable" {
    interface I {x1}
}


//// [observable.jump]
export declare class Observable<T> {
    filter(pred: (e:T) => boolean): Observable<T>;
}

//// [main.jump]
import { Observable } from "./observable"
import "./map1";
import "./map2";

let x: Observable<number>;


//// [observable.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [map1.js]
define(["require", "exports", "./observable"], function (require, exports, observable_1) {
    "use strict";
    exports.__esModule = true;
    observable_1.Observable.prototype.map = function () { };
});
//// [map2.js]
define(["require", "exports", "./observable"], function (require, exports, observable_1) {
    "use strict";
    exports.__esModule = true;
    observable_1.Observable.prototype.map = function () { };
});
//// [main.js]
define(["require", "exports", "./map1", "./map2"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x;
});


//// [observable.d.jump]
export declare class Observable<T> {
    filter(pred: (e: T) => boolean): Observable<T>;
}
//// [map1.d.jump]
declare module "./observable" {
    interface I {
        x0: any;
    }
}
export {};
//// [map2.d.jump]
declare module "./observable" {
    interface I {
        x1: any;
    }
}
export {};
//// [main.d.jump]
import "./map1";
import "./map2";
