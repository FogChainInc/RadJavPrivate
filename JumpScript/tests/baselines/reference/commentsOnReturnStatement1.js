//// [commentsOnReturnStatement1.jump]
class DebugClass {
    public static debugFunc() {
        // Start Debugger Test Code
        var i = 0;

        // End Debugger Test Code
        return true;
    }
}

//// [commentsOnReturnStatement1.js]
var DebugClass = /** @class */ (function () {
    function DebugClass() {
    }
    DebugClass.debugFunc = function () {
        // Start Debugger Test Code
        var i = 0;
        // End Debugger Test Code
        return true;
    };
    return DebugClass;
}());
