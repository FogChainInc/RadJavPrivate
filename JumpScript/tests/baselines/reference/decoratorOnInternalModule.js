//// [decoratorOnInternalModule.jump]
declare function dec<T>(target: T): T;

@dec
module M {
    
}

//// [decoratorOnInternalModule.js]
