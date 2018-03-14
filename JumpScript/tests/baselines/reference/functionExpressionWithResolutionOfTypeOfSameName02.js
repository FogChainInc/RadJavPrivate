//// [functionExpressionWithResolutionOfTypeOfSameName02.jump]
interface Foo {
}

var x = function Foo() {
    var x: Foo;
}

//// [functionExpressionWithResolutionOfTypeOfSameName02.js]
var x = function Foo() {
    var x;
};
