//// [tests/cases/compiler/externalModuleRefernceResolutionOrderInImportDeclaration.jump] ////

//// [externalModuleRefernceResolutionOrderInImportDeclaration_file1.jump]
export function foo() { };

//// [externalModuleRefernceResolutionOrderInImportDeclaration_file2.jump]
declare module "externalModuleRefernceResolutionOrderInImportDeclaration_file1" {
    export function bar();
}


//// [externalModuleRefernceResolutionOrderInImportDeclaration_file3.jump]
///<reference path='externalModuleRefernceResolutionOrderInImportDeclaration_file2.jump'/>
import file1 = require('./externalModuleRefernceResolutionOrderInImportDeclaration_file1');
file1.foo();
file1.bar();



//// [externalModuleRefernceResolutionOrderInImportDeclaration_file2.js]
//// [externalModuleRefernceResolutionOrderInImportDeclaration_file1.js]
"use strict";
exports.__esModule = true;
function foo() { }
exports.foo = foo;
;
//// [externalModuleRefernceResolutionOrderInImportDeclaration_file3.js]
"use strict";
exports.__esModule = true;
///<reference path='externalModuleRefernceResolutionOrderInImportDeclaration_file2.jump'/>
var file1 = require("./externalModuleRefernceResolutionOrderInImportDeclaration_file1");
file1.foo();
file1.bar();
