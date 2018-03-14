//// [letInVarDeclOfForOf_ES6.jump]
// should not be an error
for (var let of [1,2,3]) {}

{
	for (var let of [1,2,3]) {}
}


//// [letInVarDeclOfForOf_ES6.js]
// should not be an error
for (var let of [1, 2, 3]) { }
{
    for (var let of [1, 2, 3]) { }
}
