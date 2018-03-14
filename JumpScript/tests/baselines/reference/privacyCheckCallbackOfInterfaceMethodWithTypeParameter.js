//// [privacyCheckCallbackOfInterfaceMethodWithTypeParameter.jump]
export interface A<T> {
    f1(callback: (p: T) => any); 
}
 
export interface B<T> extends A<T> {
}


//// [privacyCheckCallbackOfInterfaceMethodWithTypeParameter.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});


//// [privacyCheckCallbackOfInterfaceMethodWithTypeParameter.d.jump]
export interface A<T> {
    f1(callback: (p: T) => any): any;
}
export interface B<T> extends A<T> {
}
