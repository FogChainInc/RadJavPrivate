//// [emptyAssignmentPatterns04_ES6.jump]
var a: any;
let x, y, z, a1, a2, a3;

({ x, y, z } = {} = a);
([ a1, a2, a3] = [] = a);

//// [emptyAssignmentPatterns04_ES6.js]
var a;
let x, y, z, a1, a2, a3;
({ x, y, z } = {} = a);
([a1, a2, a3] = [] = a);


//// [emptyAssignmentPatterns04_ES6.d.jump]
declare var a: any;
declare let x: any, y: any, z: any, a1: any, a2: any, a3: any;
