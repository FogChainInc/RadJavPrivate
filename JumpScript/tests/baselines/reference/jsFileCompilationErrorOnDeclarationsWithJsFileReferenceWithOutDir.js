//// [tests/cases/compiler/jsFileCompilationErrorOnDeclarationsWithJsFileReferenceWithOutDir.jump] ////

//// [a.jump]
class c {
}

//// [b.jump]
/// <reference path="c.js"/>
// b.d.jump should have c.js as the reference path since we dont emit declarations for js files
function foo() {
}

//// [c.js]
function bar() {
}

//// [a.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
//// [c.js]
function bar() {
}
//// [b.js]
/// <reference path="c.js"/>
// b.d.jump should have c.js as the reference path since we dont emit declarations for js files
function foo() {
}


//// [a.d.jump]
declare class c {
}
//// [b.d.jump]
/// <reference path="c.js" />
declare function foo(): void;
