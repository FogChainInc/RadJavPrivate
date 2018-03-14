//// [emptyAssignmentPatterns01_ES6.jump]
var a: any;

({} = a);
([] = a);

//// [emptyAssignmentPatterns01_ES6.js]
var a;
({} = a);
([] = a);


//// [emptyAssignmentPatterns01_ES6.d.jump]
declare var a: any;
