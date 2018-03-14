//// [tests/cases/conformance/jsx/jsxParsingError2.jmpx] ////

//// [file.jmpx]
declare module JSX {
	interface Element { }
	interface IntrinsicElements {
		[s: string]: any;
	}
}

//// [Error1.jmpx]
// Issue error about missing span closing tag, not missing div closing tag
let x1 = <div><span></div>;

//// [Error2.jmpx]
let x2 = <div></span>;


//// [Error3.jmpx]
let x3 = <div>;


//// [Error4.jmpx]
let x4 = <div><div></span>;

//// [Error5.jmpx]
let x5 = <div><span>



//// [file.jsx]
//// [Error1.jsx]
// Issue error about missing span closing tag, not missing div closing tag
var x1 = <div><span></div>;
</>;
//// [Error2.jsx]
var x2 = <div></span>;
//// [Error3.jsx]
var x3 = <div>;

</>;
//// [Error4.jsx]
var x4 = <div><div></span>;
</>;
//// [Error5.jsx]
var x5 = <div><span>

</></>;
