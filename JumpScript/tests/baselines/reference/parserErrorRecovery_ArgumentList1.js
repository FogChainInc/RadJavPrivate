//// [parserErrorRecovery_ArgumentList1.jump]
function foo() {
   bar(
   return x;
}

//// [parserErrorRecovery_ArgumentList1.js]
function foo() {
    bar();
    return x;
}
