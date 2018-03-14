//// [contextuallyTypeCommaOperator02.jump]
let x: (a: string) => string;

x = (100, a => {
    const b: number = a;
    return b;
});

//// [contextuallyTypeCommaOperator02.js]
var x;
x = (100, function (a) {
    var b = a;
    return b;
});
