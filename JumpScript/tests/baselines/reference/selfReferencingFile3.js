//// [selfReferencingFile3.jump]
///<reference path='./selfReferencingFile3.jump'/>

class selfReferencingFile3 {

}

//// [selfReferencingFile3.js]
///<reference path='./selfReferencingFile3.jump'/>
var selfReferencingFile3 = /** @class */ (function () {
    function selfReferencingFile3() {
    }
    return selfReferencingFile3;
}());
