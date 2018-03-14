//// [tests/cases/conformance/jsx/jmpxElementResolution17.jmpx] ////

//// [file.jmpx]
declare module JSX {
	interface Element { }
	interface IntrinsicElements { }
}

declare module 'elements1' {
	class MyElement {

	}
}

declare module 'elements2' {
	class MyElement {

	}
}

//// [consumer.jmpx]
///<reference path="file.jmpx" />
// Should keep s1 and elide s2
import s1 = require('elements1');
import s2 = require('elements2');
<s1.MyElement />;


//// [file.jsx]
//// [consumer.jsx]
define(["require", "exports", "elements1"], function (require, exports, s1) {
    "use strict";
    exports.__esModule = true;
    <s1.MyElement />;
});
