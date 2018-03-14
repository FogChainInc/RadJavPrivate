//// [tests/cases/compiler/es6ImportWithoutFromClauseInEs5.jump] ////

//// [es6ImportWithoutFromClauseInEs5_0.jump]
export var a = 10;

//// [es6ImportWithoutFromClauseInEs5_1.jump]
import "es6ImportWithoutFromClauseInEs5_0";

//// [es6ImportWithoutFromClauseInEs5_0.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.a = 10;
//// [es6ImportWithoutFromClauseInEs5_1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
require("es6ImportWithoutFromClauseInEs5_0");


//// [es6ImportWithoutFromClauseInEs5_0.d.jump]
export declare var a: number;
//// [es6ImportWithoutFromClauseInEs5_1.d.jump]
import "es6ImportWithoutFromClauseInEs5_0";
