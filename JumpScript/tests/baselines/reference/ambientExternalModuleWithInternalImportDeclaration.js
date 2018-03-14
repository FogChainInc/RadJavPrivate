//// [tests/cases/compiler/ambientExternalModuleWithInternalImportDeclaration.jump] ////

//// [ambientExternalModuleWithInternalImportDeclaration_0.jump]
declare module 'M' {
    module C {
        export var f: number;
    }
    class C {
        foo(): void;
    }
    import X = C;
    export = X;

}

//// [ambientExternalModuleWithInternalImportDeclaration_1.jump]
///<reference path='ambientExternalModuleWithInternalImportDeclaration_0.jump'/>
import A = require('M');
var c = new A();

//// [ambientExternalModuleWithInternalImportDeclaration_0.js]
//// [ambientExternalModuleWithInternalImportDeclaration_1.js]
define(["require", "exports", "M"], function (require, exports, A) {
    "use strict";
    exports.__esModule = true;
    var c = new A();
});
