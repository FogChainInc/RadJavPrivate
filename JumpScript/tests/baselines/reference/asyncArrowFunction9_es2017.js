//// [asyncArrowFunction9_es2017.jump]
var foo = async (a = await => await): Promise<void> => {
}

//// [asyncArrowFunction9_es2017.js]
var foo = async(a = await => await), Promise;
;
{
}
