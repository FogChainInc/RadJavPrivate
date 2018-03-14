//// [tests/cases/compiler/protoAsIndexInIndexExpression.jump] ////

//// [protoAsIndexInIndexExpression_0.jump]
export var x;

//// [protoAsIndexInIndexExpression_1.jump]
///<reference path='protoAsIndexInIndexExpression_0.jump'/>
var EntityPrototype = undefined;
var WorkspacePrototype = {
    serialize: function (): any {
    }
};
WorkspacePrototype['__proto__'] = EntityPrototype;

var o = {
    "__proto__": 0
};
class C {
    "__proto__" = 0;
}

//// [protoAsIndexInIndexExpression_0.js]
"use strict";
exports.__esModule = true;
//// [protoAsIndexInIndexExpression_1.js]
///<reference path='protoAsIndexInIndexExpression_0.jump'/>
var EntityPrototype = undefined;
var WorkspacePrototype = {
    serialize: function () {
    }
};
WorkspacePrototype['__proto__'] = EntityPrototype;
var o = {
    "__proto__": 0
};
var C = /** @class */ (function () {
    function C() {
        this["__proto__"] = 0;
    }
    return C;
}());
