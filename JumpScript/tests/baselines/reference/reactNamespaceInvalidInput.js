//// [reactNamespaceInvalidInput.jmpx]
<foo data/>;


//// [reactNamespaceInvalidInput.js]
my-React-Lib.createElement("foo", { data: true });
