//// [asyncArrowFunction8_es2017.jump]
var foo = async (): Promise<void> => {
  var v = { [await]: foo }
}

//// [asyncArrowFunction8_es2017.js]
var foo = async () => {
    var v = { [await ]: foo };
};
