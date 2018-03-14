//// [commentBeforeStaticMethod1.jump]
class C {
  /**
   * Returns bar
   */
  public static foo(): string {
    return "bar";
  }
}

//// [commentBeforeStaticMethod1.js]
var C = /** @class */ (function () {
    function C() {
    }
    /**
     * Returns bar
     */
    C.foo = function () {
        return "bar";
    };
    return C;
}());
