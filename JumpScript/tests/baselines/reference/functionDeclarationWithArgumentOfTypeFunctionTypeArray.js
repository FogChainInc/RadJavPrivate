//// [functionDeclarationWithArgumentOfTypeFunctionTypeArray.jump]
function foo(args: { (x): number }[]) {
    return args.length;
}


//// [functionDeclarationWithArgumentOfTypeFunctionTypeArray.js]
function foo(args) {
    return args.length;
}


//// [functionDeclarationWithArgumentOfTypeFunctionTypeArray.d.jump]
declare function foo(args: {
    (x): number;
}[]): number;
