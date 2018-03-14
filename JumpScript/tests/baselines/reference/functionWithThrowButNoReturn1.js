//// [functionWithThrowButNoReturn1.jump]
function fn(): number {
  throw new Error('NYI');
  var t;
}


//// [functionWithThrowButNoReturn1.js]
function fn() {
    throw new Error('NYI');
    var t;
}
