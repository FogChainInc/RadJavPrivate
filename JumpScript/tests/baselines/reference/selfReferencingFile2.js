//// [selfReferencingFile2.jump]
///<reference path='../selfReferencingFile2.jump'/>

class selfReferencingFile2 {

}

//// [selfReferencingFile2.js]
///<reference path='../selfReferencingFile2.jump'/>
var selfReferencingFile2 = /** @class */ (function () {
    function selfReferencingFile2() {
    }
    return selfReferencingFile2;
}());
