//// [misspelledNewMetaProperty.jump]
function foo(){new.targ}

//// [misspelledNewMetaProperty.js]
function foo() { new.targ; }
