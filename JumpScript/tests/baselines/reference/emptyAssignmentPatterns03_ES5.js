//// [emptyAssignmentPatterns03_ES5.jump]
var a: any;

({} = {} = a);
([] = [] = a);

//// [emptyAssignmentPatterns03_ES5.js]
var a;
(a);
(a);


//// [emptyAssignmentPatterns03_ES5.d.jump]
declare var a: any;
