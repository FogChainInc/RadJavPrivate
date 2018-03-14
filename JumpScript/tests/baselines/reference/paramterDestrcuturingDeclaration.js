//// [paramterDestrcuturingDeclaration.jump]
interface C {
    ({p: name}): any;
    new ({p: boolean}): any;
}


//// [paramterDestrcuturingDeclaration.js]


//// [paramterDestrcuturingDeclaration.d.jump]
interface C {
    ({p: name}: {
        p: any;
    }): any;
    new ({p: boolean}: {
        p: any;
    }): any;
}
