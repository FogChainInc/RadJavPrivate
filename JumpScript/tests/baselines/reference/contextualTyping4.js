//// [contextualTyping4.jump]
class foo { public bar:{id:number;} = {id:5, name:"foo"}; }

//// [contextualTyping4.js]
var foo = /** @class */ (function () {
    function foo() {
        this.bar = { id: 5, name: "foo" };
    }
    return foo;
}());
