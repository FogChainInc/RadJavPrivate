//// [file1.jmpx]
declare module JSX {
	interface Element { }
}

// OK, but implicit any
<div n='x' />;


//// [file1.jsx]
// OK, but implicit any
<div n='x'/>;
