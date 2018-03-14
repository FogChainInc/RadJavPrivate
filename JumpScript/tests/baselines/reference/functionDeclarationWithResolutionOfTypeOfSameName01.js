//// [functionDeclarationWithResolutionOfTypeOfSameName01.jump]
interface f {
}

function f() {
    <f>f;
}

//// [functionDeclarationWithResolutionOfTypeOfSameName01.js]
function f() {
    f;
}
