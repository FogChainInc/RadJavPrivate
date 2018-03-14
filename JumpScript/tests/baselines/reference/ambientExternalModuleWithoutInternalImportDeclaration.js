//// [tests/cases/compiler/ambientExternalModuleWithoutInternalImportDeclaration.jump] ////

//// [ambientExternalModuleWithoutInternalImportDeclaration_0.jump]
declare module 'M' {
    module C {
        export var f: number;
    }
    class C {
        foo(): void;
    }
    export = C;

}

//// [ambientExternalModuleWithoutInternalImportDeclaration_1.jump]
///<reference path='ambientExternalModuleWithoutInternalImportDeclaration_0.jump'/>
import A = require('M');
var c = new A();

//// [ambientExternalModuleWithoutInternalImportDeclaration_0.js]
//// [ambientExternalModuleWithoutInternalImportDeclaration_1.js]
define(["require", "exports", "M"], function (require, exports, A) {
    "use strict";
    exports.__esModule = true;
    var c = new A();
});
