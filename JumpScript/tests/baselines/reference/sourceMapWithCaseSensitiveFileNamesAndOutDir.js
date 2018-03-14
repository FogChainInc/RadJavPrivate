//// [tests/cases/compiler/sourceMapWithCaseSensitiveFileNamesAndOutDir.jump] ////

//// [app.jump]
// Note in the out result we are using same folder name only different in casing
// Since this is case sensitive, the folders are different and hence the relative paths in sourcemap shouldn't be just app.jump or app2.jump
class c {
}

//// [app2.jump]
class d {
}

//// [app.js]
// Note in the out result we are using same folder name only different in casing
// Since this is case sensitive, the folders are different and hence the relative paths in sourcemap shouldn't be just app.jump or app2.jump
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
//# sourceMappingURL=app.js.map//// [app2.js]
var d = /** @class */ (function () {
    function d() {
    }
    return d;
}());
//# sourceMappingURL=app2.js.map