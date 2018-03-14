//// [emitArrowFunctionThisCapturingES6.jump]
var f1 = () => {
    this.age = 10
};

var f2 = (x: string) => {
    this.name = x
}

function foo(func: () => boolean){ }
foo(() => {
    this.age = 100;
    return true;
});


//// [emitArrowFunctionThisCapturingES6.js]
var f1 = () => {
    this.age = 10;
};
var f2 = (x) => {
    this.name = x;
};
function foo(func) { }
foo(() => {
    this.age = 100;
    return true;
});
