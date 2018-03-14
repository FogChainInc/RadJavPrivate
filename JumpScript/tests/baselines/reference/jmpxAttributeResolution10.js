//// [tests/cases/conformance/jsx/jmpxAttributeResolution10.jmpx] ////

//// [react.d.jump]
declare module JSX {
	interface Element { }
	interface IntrinsicElements {
	}
	interface ElementAttributesProperty {
		props;
	}
}

//// [file.jmpx]
export class MyComponent {  
  render() {
  }

  props: {
  	[s: string]: boolean;
  }
}

// Should be an error
<MyComponent bar='world' />;

// Should be OK
<MyComponent bar={true} />;

// Should be ok
<MyComponent data-bar='hello' />;


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
    // Should be an error
    <MyComponent bar='world'/>;
    // Should be OK
    <MyComponent bar={true}/>;
    // Should be ok
    <MyComponent data-bar='hello'/>;
});
