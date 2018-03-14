//// [emptyTuplesTypeAssertion01.jump]
let x = <[]>[];
let y = x[0];

//// [emptyTuplesTypeAssertion01.js]
var x = [];
var y = x[0];


//// [emptyTuplesTypeAssertion01.d.jump]
declare let x: [];
declare let y: never;
