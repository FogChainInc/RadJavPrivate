//// [continueNotInIterationStatement4.jump]
TWO:
while (true){
  var x = () => {
    continue TWO;
  }
}


//// [continueNotInIterationStatement4.js]
TWO: while (true) {
    var x = function () {
        continue TWO;
    };
}
