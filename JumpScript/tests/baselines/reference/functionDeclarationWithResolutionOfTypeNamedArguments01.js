//// [functionDeclarationWithResolutionOfTypeNamedArguments01.jump]
interface arguments {
}

function f() {
    <arguments>arguments;
}

//// [functionDeclarationWithResolutionOfTypeNamedArguments01.js]
function f() {
    arguments;
}
