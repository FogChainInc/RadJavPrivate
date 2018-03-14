//// [es5ExportDefaultClassDeclaration4.jump]
declare module "foo" {
    export var before: C;

    export default class C {
        method(): C;
    }

    export var after: C;

    export var t: typeof C;
}



//// [es5ExportDefaultClassDeclaration4.js]


//// [es5ExportDefaultClassDeclaration4.d.jump]
declare module "foo" {
    var before: C;
    class C {
        method(): C;
    }
    var after: C;
    var t: typeof C;
}
