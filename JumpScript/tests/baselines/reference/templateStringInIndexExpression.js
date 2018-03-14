//// [templateStringInIndexExpression.jump]
`abc${0}abc`[`0`];

//// [templateStringInIndexExpression.js]
("abc" + 0 + "abc")["0"];
