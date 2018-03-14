//// [functionOverloads17.jump]
function foo():{a:number;}
function foo():{a:string;} { return {a:""} }


//// [functionOverloads17.js]
function foo() { return { a: "" }; }
