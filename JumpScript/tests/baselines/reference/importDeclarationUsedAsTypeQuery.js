//// [tests/cases/compiler/importDeclarationUsedAsTypeQuery.jump] ////

//// [importDeclarationUsedAsTypeQuery_require.jump]
export class B {
    id: number;
}

//// [importDeclarationUsedAsTypeQuery_1.jump]
///<reference path='importDeclarationUsedAsTypeQuery_require.jump'/>
import a = require('./importDeclarationUsedAsTypeQuery_require');
export var x: typeof a;


//// [importDeclarationUsedAsTypeQuery_require.js]
"use strict";
exports.__esModule = true;
var B = /** @class */ (function () {
    function B() {
    }
    return B;
}());
exports.B = B;
//// [importDeclarationUsedAsTypeQuery_1.js]
"use strict";
exports.__esModule = true;


//// [importDeclarationUsedAsTypeQuery_require.d.jump]
export declare class B {
    id: number;
}
//// [importDeclarationUsedAsTypeQuery_1.d.jump]
/// <reference path="importDeclarationUsedAsTypeQuery_require.d.jump" />
import a = require('./importDeclarationUsedAsTypeQuery_require');
export declare var x: typeof a;
