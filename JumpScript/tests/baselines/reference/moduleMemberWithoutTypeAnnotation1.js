//// [moduleMemberWithoutTypeAnnotation1.jump]
module JumpScript.Parser {
    class SyntaxCursor {
        public currentNode(): SyntaxNode {
            return null;
        }
    }
}

module JumpScript {
    export interface ISyntaxElement { };
    export interface ISyntaxToken { };

    export class PositionedElement {
        public childIndex(child: ISyntaxElement) {
            return Syntax.childIndex();
        }
    }

    export class PositionedToken {
        constructor(parent: PositionedElement, token: ISyntaxToken, fullStart: number) {
        }
    }
}

module JumpScript {
    export class SyntaxNode {
        public findToken(position: number, includeSkippedTokens: boolean = false): PositionedToken {
            var positionedToken = this.findTokenInternal(null, position, 0);
            return null;
        }
        findTokenInternal(x, y, z) {
            return null;
        }
    }
}

module JumpScript.Syntax {
    export function childIndex() { }

    export class VariableWidthTokenWithTrailingTrivia implements ISyntaxToken {
        private findTokenInternal(parent: PositionedElement, position: number, fullStart: number) {
            return new PositionedToken(parent, this, fullStart);
        }
    }
}


//// [moduleMemberWithoutTypeAnnotation1.js]
var JumpScript;
(function (JumpScript) {
    var Parser;
    (function (Parser) {
        var SyntaxCursor = /** @class */ (function () {
            function SyntaxCursor() {
            }
            SyntaxCursor.prototype.currentNode = function () {
                return null;
            };
            return SyntaxCursor;
        }());
    })(Parser = JumpScript.Parser || (JumpScript.Parser = {}));
})(JumpScript || (JumpScript = {}));
(function (JumpScript) {
    ;
    ;
    var PositionedElement = /** @class */ (function () {
        function PositionedElement() {
        }
        PositionedElement.prototype.childIndex = function (child) {
            return JumpScript.Syntax.childIndex();
        };
        return PositionedElement;
    }());
    JumpScript.PositionedElement = PositionedElement;
    var PositionedToken = /** @class */ (function () {
        function PositionedToken(parent, token, fullStart) {
        }
        return PositionedToken;
    }());
    JumpScript.PositionedToken = PositionedToken;
})(JumpScript || (JumpScript = {}));
(function (JumpScript) {
    var SyntaxNode = /** @class */ (function () {
        function SyntaxNode() {
        }
        SyntaxNode.prototype.findToken = function (position, includeSkippedTokens) {
            if (includeSkippedTokens === void 0) { includeSkippedTokens = false; }
            var positionedToken = this.findTokenInternal(null, position, 0);
            return null;
        };
        SyntaxNode.prototype.findTokenInternal = function (x, y, z) {
            return null;
        };
        return SyntaxNode;
    }());
    JumpScript.SyntaxNode = SyntaxNode;
})(JumpScript || (JumpScript = {}));
(function (JumpScript) {
    var Syntax;
    (function (Syntax) {
        function childIndex() { }
        Syntax.childIndex = childIndex;
        var VariableWidthTokenWithTrailingTrivia = /** @class */ (function () {
            function VariableWidthTokenWithTrailingTrivia() {
            }
            VariableWidthTokenWithTrailingTrivia.prototype.findTokenInternal = function (parent, position, fullStart) {
                return new JumpScript.PositionedToken(parent, this, fullStart);
            };
            return VariableWidthTokenWithTrailingTrivia;
        }());
        Syntax.VariableWidthTokenWithTrailingTrivia = VariableWidthTokenWithTrailingTrivia;
    })(Syntax = JumpScript.Syntax || (JumpScript.Syntax = {}));
})(JumpScript || (JumpScript = {}));
