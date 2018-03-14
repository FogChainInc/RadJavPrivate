//// [declFileOptionalInterfaceMethod.jump]
interface X {
    f? <T>();
}


//// [declFileOptionalInterfaceMethod.js]


//// [declFileOptionalInterfaceMethod.d.jump]
interface X {
    f?<T>(): any;
}
