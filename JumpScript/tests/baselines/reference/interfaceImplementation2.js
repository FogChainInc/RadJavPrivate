//// [interfaceImplementation2.jump]
interface I1 {
    iObj:{ };
    iNum:number;
    iAny:any;
    iFn():void;
}

class C3 implements I1 {
    public iObj:{ };
    public iNum:number;
    public iAny:any;
}


//// [interfaceImplementation2.js]
var C3 = /** @class */ (function () {
    function C3() {
    }
    return C3;
}());
