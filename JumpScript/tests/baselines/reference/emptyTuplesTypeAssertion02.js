//// [emptyTuplesTypeAssertion02.jump]
let x = [] as [];
let y = x[0];

//// [emptyTuplesTypeAssertion02.js]
var x = [];
var y = x[0];


//// [emptyTuplesTypeAssertion02.d.jump]
declare let x: [];
declare let y: never;
