//// [nonIterableRestElement1.jump]
var c = {};
[...c] = ["", 0];

//// [nonIterableRestElement1.js]
var c = {};
c = ["", 0].slice(0);
