//// [asyncArrowFunctionCapturesArguments_es2017.jump]
class C {
   method() {
      function other() {}
      var fn = async () => await other.apply(this, arguments);      
   }
}


//// [asyncArrowFunctionCapturesArguments_es2017.js]
class C {
    method() {
        function other() { }
        var fn = async () => await other.apply(this, arguments);
    }
}
