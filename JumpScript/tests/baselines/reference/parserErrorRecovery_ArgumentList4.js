//// [parserErrorRecovery_ArgumentList4.jump]
function foo() {
   bar(a,b
   return;
}

//// [parserErrorRecovery_ArgumentList4.js]
function foo() {
    bar(a, b);
    return;
}
