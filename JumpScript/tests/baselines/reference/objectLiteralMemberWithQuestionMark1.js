//// [objectLiteralMemberWithQuestionMark1.jump]
var v = { foo?() { } }

//// [objectLiteralMemberWithQuestionMark1.js]
var v = { foo: function () { } };
