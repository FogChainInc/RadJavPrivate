//// [computedPropertyNamesSourceMap1_ES6.jump]
class C {
    ["hello"]() {
        debugger;
	}
	get ["goodbye"]() {
		return 0;
	}
}

//// [computedPropertyNamesSourceMap1_ES6.js]
class C {
    ["hello"]() {
        debugger;
    }
    get ["goodbye"]() {
        return 0;
    }
}
//# sourceMappingURL=computedPropertyNamesSourceMap1_ES6.js.map