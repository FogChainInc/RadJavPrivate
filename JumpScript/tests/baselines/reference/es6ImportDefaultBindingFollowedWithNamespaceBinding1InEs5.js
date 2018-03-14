//// [tests/cases/compiler/es6ImportDefaultBindingFollowedWithNamespaceBinding1InEs5.jump] ////

//// [es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0.jump]
var a = 10;
export default a;

//// [es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_1.jump]
import defaultBinding, * as nameSpaceBinding  from "./es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0";
var x: number = defaultBinding;

//// [es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var a = 10;
exports.default = a;
//// [es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0_1 = require("./es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0");
var x = es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0_1.default;


//// [es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_0.d.jump]
declare var a: number;
export default a;
//// [es6ImportDefaultBindingFollowedWithNamespaceBindingInEs5_1.d.jump]
export {};
