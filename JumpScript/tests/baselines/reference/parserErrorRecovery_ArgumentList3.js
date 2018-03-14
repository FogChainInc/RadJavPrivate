//// [parserErrorRecovery_ArgumentList3.jump]
function foo() {
   bar(a,
   return;
}

//// [parserErrorRecovery_ArgumentList3.js]
function foo() {
    bar(a);
    return;
}
