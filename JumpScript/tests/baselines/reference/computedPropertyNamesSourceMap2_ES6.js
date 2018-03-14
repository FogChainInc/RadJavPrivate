//// [computedPropertyNamesSourceMap2_ES6.jump]
var v = {
    ["hello"]() {
        debugger;
	},
	get ["goodbye"]() {
		return 0;
	}
}

//// [computedPropertyNamesSourceMap2_ES6.js]
var v = {
    ["hello"]() {
        debugger;
    },
    get ["goodbye"]() {
        return 0;
    }
};
//# sourceMappingURL=computedPropertyNamesSourceMap2_ES6.js.map