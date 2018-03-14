//// [nonIterableRestElement3.jump]
var c = { bogus: 0 };
[...c] = ["", 0];

//// [nonIterableRestElement3.js]
var c = { bogus: 0 };
c = ["", 0].slice(0);
