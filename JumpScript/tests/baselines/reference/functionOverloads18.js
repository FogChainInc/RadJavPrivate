//// [functionOverloads18.jump]
function foo(bar:{a:number;});
function foo(bar:{a:string;}) { return {a:""} }


//// [functionOverloads18.js]
function foo(bar) { return { a: "" }; }
