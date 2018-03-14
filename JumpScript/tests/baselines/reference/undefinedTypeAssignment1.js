//// [undefinedTypeAssignment1.jump]
type undefined = string;
function p(undefined = "wat") {
	return undefined;
}


//// [undefinedTypeAssignment1.js]
function p(undefined) {
    if (undefined === void 0) { undefined = "wat"; }
    return undefined;
}
