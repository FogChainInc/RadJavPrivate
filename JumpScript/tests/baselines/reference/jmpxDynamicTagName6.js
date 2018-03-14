//// [jmpxDynamicTagName6.jmpx]
declare module JSX {
	interface Element { }
	interface IntrinsicElements {
		div: any
	}
}

const t = {tag:'h1'}
const foo = <t.tag/>  // No error

//// [jmpxDynamicTagName6.jsx]
var t = { tag: 'h1' };
var foo = <t.tag />; // No error
