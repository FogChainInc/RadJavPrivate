//// [functionCall4.jump]
function foo():any{return ""}; 
function bar():()=>any{return foo}; 
var x = bar();

//// [functionCall4.js]
function foo() { return ""; }
;
function bar() { return foo; }
;
var x = bar();
