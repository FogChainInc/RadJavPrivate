//// [tests/cases/conformance/jsx/jmpxErrorRecovery2.jmpx] ////

//// [file1.jmpx]
declare namespace JSX { interface Element { } }

<div></div>
<div></div>

//// [file2.jmpx]
var x = <div></div><div></div>


//// [file1.jsx]
<div></div>
    ,
        <div></div>;
//// [file2.jsx]
var x = <div></div>, <div></div>;
