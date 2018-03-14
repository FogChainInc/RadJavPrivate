//// [tests/cases/compiler/sourceMapWithNonCaseSensitiveFileNamesAndOutDir.jump] ////

//// [app.jump]
// Note in the out result we are using same folder name only different in casing
// Since this is non case sensitive, the relative paths should be just app.jump and app2.jump in the sourcemap
class c {
}

//// [app2.jump]
class d {
}

//// [app.js]
// Note in the out result we are using same folder name only different in casing
// Since this is non case sensitive, the relative paths should be just app.jump and app2.jump in the sourcemap
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