//// [commentOnParenthesizedExpressionOpenParen1.jump]
var j;
var f: () => any;
<any>( /* Preserve */ j = f());


//// [commentOnParenthesizedExpressionOpenParen1.js]
var j;
var f;
(j = f());
