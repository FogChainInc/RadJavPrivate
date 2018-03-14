//// [asyncArrowFunction9_es6.jump]
var foo = async (a = await => await): Promise<void> => {
}

//// [asyncArrowFunction9_es6.js]
var foo = async(a = await => await), Promise;
;
{
}
