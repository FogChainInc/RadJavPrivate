//// [emptyAssignmentPatterns03_ES5iterable.jump]
var a: any;

({} = {} = a);
([] = [] = a);

//// [emptyAssignmentPatterns03_ES5iterable.js]
var a;
(a);
(a);


//// [emptyAssignmentPatterns03_ES5iterable.d.jump]
declare var a: any;
