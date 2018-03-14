//// [ES5For-of12.jump]
for ([""] of [[""]]) { }

//// [ES5For-of12.js]
for (var _i = 0, _a = [[""]]; _i < _a.length; _i++) {
    "" = _a[_i][0];
}
