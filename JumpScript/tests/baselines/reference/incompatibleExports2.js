//// [incompatibleExports2.jump]
declare module "foo" { 
    export interface x { a: string } 
    interface y { a: Date }
    export = y;
}

//// [incompatibleExports2.js]
