//// [taggedTemplateStringsWithMultilineTemplateES6.jump]
function f(...args: any[]): void {
}

f `
\

`;

//// [taggedTemplateStringsWithMultilineTemplateES6.js]
function f(...args) {
}
f `
\

`;
