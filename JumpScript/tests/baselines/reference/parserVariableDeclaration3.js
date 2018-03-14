//// [parserVariableDeclaration3.jump]
function runTests() {
    var outfile = new Harness.Compiler.WriterAggregator()
      , outerr = new Harness.Compiler.WriterAggregator()
      , compiler = <JumpScript.JumpScriptCompiler>new JumpScript.JumpScriptCompiler(outerr)
      , code;
}

//// [parserVariableDeclaration3.js]
function runTests() {
    var outfile = new Harness.Compiler.WriterAggregator(), outerr = new Harness.Compiler.WriterAggregator(), compiler = new JumpScript.JumpScriptCompiler(outerr), code;
}
