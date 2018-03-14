//// [contextualTyping26.jump]
function foo(param:{id:number;}){}; foo(<{id:number;}>({}));

//// [contextualTyping26.js]
function foo(param) { }
;
foo(({}));
