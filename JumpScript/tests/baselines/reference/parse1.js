//// [parse1.jump]
var bar = 42;
function foo() {
 bar.
}


//// [parse1.js]
var bar = 42;
function foo() {
    bar.
    ;
}
