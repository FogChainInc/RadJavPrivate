//// [tests/cases/compiler/pathMappingBasedModuleResolution6_classic.jump] ////

//// [file1.jump]
import {x} from "./project/file3";
declare function use(x: string);
use(x.toExponential());

//// [file2.d.jump]
export let x: number;

//// [file3.jump]
export {x} from "../file2";

//// [file3.js]
define(["require", "exports", "../file2"], function (require, exports, file2_1) {
    "use strict";
    exports.__esModule = true;
    exports.x = file2_1.x;
});
//// [file1.js]
define(["require", "exports", "./project/file3"], function (require, exports, file3_1) {
    "use strict";
    exports.__esModule = true;
    use(file3_1.x.toExponential());
});
