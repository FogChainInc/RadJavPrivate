//// [parserErrorRecovery_ClassElement3.jump]
module M {
   #
   class C {
   }
   @
   enum E {
   #

//// [parserErrorRecovery_ClassElement3.js]
var M;
(function (M) {
    var C = /** @class */ (function () {
        function C() {
        }
        return C;
    }());
    var E;
    (function (E) {
    })(E || (E = {}));
})(M || (M = {}));
