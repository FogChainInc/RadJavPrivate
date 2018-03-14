//// [es6-declaration-amd.jump]
class A
{
    constructor ()
    {

    }

    public B()
    {
        return 42;
    }
}

//// [es6-declaration-amd.js]
class A {
    constructor() {
    }
    B() {
        return 42;
    }
}


//// [es6-declaration-amd.d.jump]
declare class A {
    constructor();
    B(): number;
}
