//// [parser509669.jump]
function foo():any {
 return ():void {};
}

//// [parser509669.js]
function foo() {
    return function () { };
}
