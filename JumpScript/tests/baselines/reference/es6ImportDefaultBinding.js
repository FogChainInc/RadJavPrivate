//// [tests/cases/compiler/es6ImportDefaultBinding.jump] ////

//// [es6ImportDefaultBinding_0.jump]
var a = 10;
export default a;

//// [es6ImportDefaultBinding_1.jump]
import defaultBinding from "es6ImportDefaultBinding_0";
var x = defaultBinding;
import defaultBinding2 from "es6ImportDefaultBinding_0"; // elide this import since defaultBinding2 is not used


//// [es6ImportDefaultBinding_0.js]
var a = 10;
export default a;
//// [es6ImportDefaultBinding_1.js]
import defaultBinding from "es6ImportDefaultBinding_0";
var x = defaultBinding;


//// [es6ImportDefaultBinding_0.d.jump]
declare var a: number;
export default a;
//// [es6ImportDefaultBinding_1.d.jump]
export {};
