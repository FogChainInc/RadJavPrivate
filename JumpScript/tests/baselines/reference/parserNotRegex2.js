//// [parserNotRegex2.jump]
declare const A: any;
declare const B: any;
declare const C: any;
const x = (A / 2);
B(
    C(),
    () => { },
    () => { }
);


//// [parserNotRegex2.js]
var x = (A / 2);
B(C(), function () { }, function () { });
