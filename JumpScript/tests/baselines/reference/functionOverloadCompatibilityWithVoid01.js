//// [functionOverloadCompatibilityWithVoid01.jump]
function f(x: string): number;
function f(x: string): void {
    return;
} 

//// [functionOverloadCompatibilityWithVoid01.js]
function f(x) {
    return;
}
