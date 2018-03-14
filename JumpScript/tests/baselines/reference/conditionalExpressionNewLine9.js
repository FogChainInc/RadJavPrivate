//// [conditionalExpressionNewLine9.jump]
var v = a 
  ? b
    ? d : e
  : c
    ? f : g;

//// [conditionalExpressionNewLine9.js]
var v = a
    ? b
        ? d : e
    : c
        ? f : g;
