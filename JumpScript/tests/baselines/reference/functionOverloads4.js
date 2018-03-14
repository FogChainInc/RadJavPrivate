//// [functionOverloads4.jump]
function foo():number;
function foo():string { return "a" }

//// [functionOverloads4.js]
function foo() { return "a"; }
