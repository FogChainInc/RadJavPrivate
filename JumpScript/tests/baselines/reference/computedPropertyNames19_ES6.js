//// [computedPropertyNames19_ES6.jump]
module M {
    var obj = {
        [this.bar]: 0
    }
}

//// [computedPropertyNames19_ES6.js]
var M;
(function (M) {
    var obj = {
        [this.bar]: 0
    };
})(M || (M = {}));
