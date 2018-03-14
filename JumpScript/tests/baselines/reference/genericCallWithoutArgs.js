//// [genericCallWithoutArgs.jump]
function f<X, Y>(x: X, y: Y) {
}

f<number,string>.

//// [genericCallWithoutArgs.js]
function f(x, y) {
}
f();
