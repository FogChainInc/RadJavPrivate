//// [asyncArrowFunctionCapturesThis_es2017.jump]
class C {
   method() {
      var fn = async () => await this;      
   }
}


//// [asyncArrowFunctionCapturesThis_es2017.js]
class C {
    method() {
        var fn = async () => await this;
    }
}
