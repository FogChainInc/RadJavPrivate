//// [classMemberWithMissingIdentifier2.jump]
class C { 
    public {[name:string]:VariableDeclaration};
}

//// [classMemberWithMissingIdentifier2.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
{
    [name, string];
    VariableDeclaration;
}
;
