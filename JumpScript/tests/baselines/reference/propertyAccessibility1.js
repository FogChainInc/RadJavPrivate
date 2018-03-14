//// [propertyAccessibility1.jump]
class Foo {
  private privProp = 0;
}
var f = new Foo();
f.privProp;


//// [propertyAccessibility1.js]
var Foo = /** @class */ (function () {
    function Foo() {
        this.privProp = 0;
    }
    return Foo;
}());
var f = new Foo();
f.privProp;
