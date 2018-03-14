//// [tests/cases/compiler/moduleAugmentationGlobal4.jump] ////

//// [f1.jump]
declare global {
    interface Something {x}
}
export {};
//// [f2.jump]
declare global {
    interface Something {y}
}
export {};
//// [f3.jump]
import "./f1";
import "./f2";



//// [f1.js]
"use strict";
exports.__esModule = true;
//// [f2.js]
"use strict";
exports.__esModule = true;
//// [f3.js]
"use strict";
exports.__esModule = true;
require("./f1");
require("./f2");


//// [f1.d.jump]
declare global  {
    interface Something {
        x: any;
    }
}
export {  };
//// [f2.d.jump]
declare global  {
    interface Something {
        y: any;
    }
}
export {  };
//// [f3.d.jump]
import "./f1";
import "./f2";
