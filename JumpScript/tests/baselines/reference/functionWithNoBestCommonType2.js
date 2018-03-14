//// [functionWithNoBestCommonType2.jump]
var v = function () {
   return true;
   return bar();
};

function bar(): void {
}

//// [functionWithNoBestCommonType2.js]
var v = function () {
    return true;
    return bar();
};
function bar() {
}
