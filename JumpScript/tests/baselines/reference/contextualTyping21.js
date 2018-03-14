//// [contextualTyping21.jump]
var foo:{id:number;}[] = [{id:1}]; foo = [{id:1}, 1];

//// [contextualTyping21.js]
var foo = [{ id: 1 }];
foo = [{ id: 1 }, 1];
