//// [unusedFunctionsinNamespaces2.jump]
namespace Validation {
    var function1 = function() {
    }
}

//// [unusedFunctionsinNamespaces2.js]
var Validation;
(function (Validation) {
    var function1 = function () {
    };
})(Validation || (Validation = {}));
