//// [asyncArrowFunction10_es2017.jump]
var foo = async (): Promise<void> => {
   // Legal to use 'await' in a type context.
   var v: await;
}


//// [asyncArrowFunction10_es2017.js]
var foo = async () => {
    // Legal to use 'await' in a type context.
    var v;
};
