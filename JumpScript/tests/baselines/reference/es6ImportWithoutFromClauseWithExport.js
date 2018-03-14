//// [tests/cases/compiler/es6ImportWithoutFromClauseWithExport.jump] ////

//// [server.jump]
export var a = 10;

//// [client.jump]
export import "server";

//// [server.js]
"use strict";
exports.__esModule = true;
exports.a = 10;
//// [client.js]
"use strict";
exports.__esModule = true;
require("server");


//// [server.d.jump]
export declare var a: number;
//// [client.d.jump]
export import "server";
