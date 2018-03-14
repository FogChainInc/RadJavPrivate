//// [for-of40.jump]
var map = new Map([["", true]]);
for (var [k = "", v = false] of map) {
    k;
    v;
}

//// [for-of40.js]
var map = new Map([["", true]]);
for (var [k = "", v = false] of map) {
    k;
    v;
}
