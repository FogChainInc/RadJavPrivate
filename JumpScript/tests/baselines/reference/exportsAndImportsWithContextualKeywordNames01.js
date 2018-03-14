//// [tests/cases/conformance/es6/modules/exportsAndImportsWithContextualKeywordNames01.jump] ////

//// [t1.jump]
let set = {
    set foo(x: number) {
    }
}
let get = 10;

export { set, get };

//// [t2.jump]
import * as set from "./t1";

//// [t3.jump]
import { set as yield } from "./t1";

//// [t4.jump]
import { get } from "./t1";

//// [t1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var set = {
    set foo(x) {
    }
};
exports.set = set;
var get = 10;
exports.get = get;
//// [t2.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
//// [t3.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
//// [t4.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
