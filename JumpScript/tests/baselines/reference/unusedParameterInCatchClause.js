//// [unusedParameterInCatchClause.jump]
function f1() {
    try {} catch(ex){}
}

//// [unusedParameterInCatchClause.js]
function f1() {
    try { }
    catch (ex) { }
}
