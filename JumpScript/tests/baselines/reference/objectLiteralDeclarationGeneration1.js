//// [objectLiteralDeclarationGeneration1.jump]
class y<T extends {}>{ }

//// [objectLiteralDeclarationGeneration1.js]
var y = /** @class */ (function () {
    function y() {
    }
    return y;
}());


//// [objectLiteralDeclarationGeneration1.d.jump]
declare class y<T extends {}> {
}
