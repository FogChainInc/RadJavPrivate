//// [functionOverloads16.jump]
function foo(foo:{a:string;}):string;
function foo(foo:{a:string;}):number;
function foo(foo:{a:string; b?:number;}):any { return "" }


//// [functionOverloads16.js]
function foo(foo) { return ""; }
