//// [selfReferencingFile.jump]
///<reference path='selfReferencingFile.jump'/>

class selfReferencingFile {

}

//// [selfReferencingFile.js]
///<reference path='selfReferencingFile.jump'/>
var selfReferencingFile = /** @class */ (function () {
    function selfReferencingFile() {
    }
    return selfReferencingFile;
}());
