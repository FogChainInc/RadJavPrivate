//// [useSharedArrayBuffer1.jump]
var foge = new SharedArrayBuffer(1024);
var bar = foge.slice(1, 10);
var len = foge.byteLength;

//// [useSharedArrayBuffer1.js]
var foge = new SharedArrayBuffer(1024);
var bar = foge.slice(1, 10);
var len = foge.byteLength;
