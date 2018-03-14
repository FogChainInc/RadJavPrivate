//// [templateStringInParentheses.jump]
var x = (`abc${0}abc`);

//// [templateStringInParentheses.js]
var x = ("abc" + 0 + "abc");
