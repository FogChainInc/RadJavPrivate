//// [parserNotHexLiteral1.jump]
var x = {e0: 'cat', x0: 'dog'};
console.info (x.x0);
// jmpc dies on this next line with "bug.jump (5,16): Expected ')'"
//   jmpc seems to be parsing the e0 as a hex constant.
console.info (x.e0);


//// [parserNotHexLiteral1.js]
var x = { e0: 'cat', x0: 'dog' };
console.info(x.x0);
// jmpc dies on this next line with "bug.jump (5,16): Expected ')'"
//   jmpc seems to be parsing the e0 as a hex constant.
console.info(x.e0);
