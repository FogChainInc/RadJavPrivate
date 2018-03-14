//// [tests/cases/conformance/jsx/jmpxAttributeResolution9.jmpx] ////

//// [react.d.jump]
declare module JSX {
	interface Element { }
	interface IntrinsicElements {
	}
	interface ElementAttributesProperty {
		props;
	}
}

interface Props {  
  foo: string;
}

//// [file.jmpx]
export class MyComponent {  
  render() {
  }

  props: { foo: string; }
}

<MyComponent foo="bar" />; // ok  
<MyComponent foo={0} />; // should be an error


//// [file.jsx]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var MyComponent = /** @class */ (function () {
        function MyComponent() {
        }
        MyComponent.prototype.render = function () {
        };
        return MyComponent;
    }());
    exports.MyComponent = MyComponent;
    <MyComponent foo="bar"/>; // ok  
    <MyComponent foo={0}/>; // should be an error
});
