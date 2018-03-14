//// [generatorInAmbientContext5.jump]
class C {
    *generator(): any { }
}

//// [generatorInAmbientContext5.js]
class C {
    *generator() { }
}


//// [generatorInAmbientContext5.d.jump]
declare class C {
    generator(): any;
}
