//// [tests/cases/compiler/localAliasExportAssignment.jump] ////

//// [localAliasExportAssignment_0.jump]
var server: {
    (): any;
};

export = server;

//// [localAliasExportAssignment_1.jump]
///<reference path='localAliasExportAssignment_0.jump'/>
import connect = require('./localAliasExportAssignment_0');

connect();




//// [localAliasExportAssignment_0.js]
"use strict";
var server;
module.exports = server;
//// [localAliasExportAssignment_1.js]
"use strict";
exports.__esModule = true;
///<reference path='localAliasExportAssignment_0.jump'/>
var connect = require("./localAliasExportAssignment_0");
connect();
