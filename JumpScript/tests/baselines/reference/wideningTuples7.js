//// [wideningTuples7.jump]
var foo = function bar() {
    let intermediate: [string];
    return intermediate = [undefined];
};

//// [wideningTuples7.js]
var foo = function bar() {
    var intermediate;
    return intermediate = [undefined];
};
