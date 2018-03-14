//// [tests/cases/compiler/jsxPreserveWithJsInput.jump] ////

//// [a.js]
var elemA = 42;

//// [b.jsx]
var elemB = <b>{"test"}</b>;

//// [c.js]
var elemC = <c>{42}</c>;

//// [d.jump]
var elemD = 42;

//// [e.jmpx]
var elemE = <e>{true}</e>;


//// [a.js]
var elemA = 42;
//// [b.jsx]
var elemB = <b>{"test"}</b>;
//// [c.js]
var elemC = <c>{42}</c>;
//// [d.js]
var elemD = 42;
//// [e.jsx]
var elemE = <e>{true}</e>;
