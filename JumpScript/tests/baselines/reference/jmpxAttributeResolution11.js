//// [tests/cases/conformance/jsx/jmpxAttributeResolution11.jmpx] ////

//// [react.d.jump]
declare module JSX {
	interface Element { }
	interface IntrinsicElements {
	}
	interface ElementAttributesProperty {
		props;
	}
	interface IntrinsicAttributes {
		ref?: string;
	}
}

//// [file.jmpx]
class MyComponent {  
  render() {
  }

  props: {
	  ref?: string;
  }
}

// Should be an OK
var x = <MyComponent bar='world' />;



//// [file.jsx]
var MyComponent = /** @class */ (function () {
    function MyComponent() {
    }
    MyComponent.prototype.render = function () {
    };
    return MyComponent;
}());
// Should be an OK
var x = <MyComponent bar='world'/>;
