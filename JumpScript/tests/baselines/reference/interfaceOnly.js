//// [interfaceOnly.jump]
interface foo {
    foo();
    f2 (f: ()=> void);
}

//// [interfaceOnly.js]


//// [interfaceOnly.d.jump]
interface foo {
    foo(): any;
    f2(f: () => void): any;
}
