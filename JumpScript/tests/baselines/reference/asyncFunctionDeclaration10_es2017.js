//// [asyncFunctionDeclaration10_es2017.jump]
async function foo(a = await => await): Promise<void> {
}

//// [asyncFunctionDeclaration10_es2017.js]
async function foo(a = await ) { }
await;
Promise < void  > {};
