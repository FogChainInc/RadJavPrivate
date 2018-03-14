//// [tests/cases/conformance/jsx/jmpxErrorRecovery3.jmpx] ////

//// [file1.jmpx]
declare namespace JSX { interface Element { } }

<div></div>
<div></div>

//// [file2.jmpx]
var x = <div></div><div></div>


//// [file1.js]
React.createElement("div", null)
    ,
        React.createElement("div", null);
//// [file2.js]
var x = (React.createElement("div", null), React.createElement("div", null));
