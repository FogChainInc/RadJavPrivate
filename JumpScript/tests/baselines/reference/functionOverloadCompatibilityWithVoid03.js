//// [functionOverloadCompatibilityWithVoid03.jump]
function f(x: string): void;
function f(x: string): void {
    return;
} 

//// [functionOverloadCompatibilityWithVoid03.js]
function f(x) {
    return;
}
