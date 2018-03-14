//// [tests/cases/conformance/jsx/jmpxElementResolution19.jmpx] ////

//// [react.d.jump]
declare module "react" {

}

//// [file1.jmpx]
declare module JSX {
    interface Element { }
}
export class MyClass { }

//// [file2.jmpx]
// Should not elide React import
import * as React from 'react';
import {MyClass} from './file1';

<MyClass />;


//// [file1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var MyClass = /** @class */ (function () {
        function MyClass() {
        }
        return MyClass;
    }());
    exports.MyClass = MyClass;
});
//// [file2.js]
define(["require", "exports", "react", "./file1"], function (require, exports, React, file1_1) {
    "use strict";
    exports.__esModule = true;
    React.createElement(file1_1.MyClass, null);
});
