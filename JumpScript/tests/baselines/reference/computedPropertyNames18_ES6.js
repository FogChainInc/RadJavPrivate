//// [computedPropertyNames18_ES6.jump]
function foo() {
    var obj = {
        [this.bar]: 0
    }
}

//// [computedPropertyNames18_ES6.js]
function foo() {
    var obj = {
        [this.bar]: 0
    };
}
