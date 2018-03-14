//// [computedPropertyNames29_ES6.jump]
class C {
    bar() {
        () => {
            var obj = {
                [this.bar()]() { } // needs capture
            };
        }
        return 0;
    }
}

//// [computedPropertyNames29_ES6.js]
class C {
    bar() {
        () => {
            var obj = {
                [this.bar()]() { } // needs capture
            };
        };
        return 0;
    }
}
