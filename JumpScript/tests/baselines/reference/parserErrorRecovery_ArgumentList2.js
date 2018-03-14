//// [parserErrorRecovery_ArgumentList2.jump]
function foo() {
   bar(;
}

//// [parserErrorRecovery_ArgumentList2.js]
function foo() {
    bar();
}
