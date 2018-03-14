//// [stringLiteralTypesWithTemplateStrings01.jump]
let ABC: "ABC" = `ABC`;
let DE_NEWLINE_F: "DE\nF" = `DE
F`;
let G_QUOTE_HI: 'G"HI';
let JK_BACKTICK_L: "JK`L" = `JK\`L`;

//// [stringLiteralTypesWithTemplateStrings01.js]
var ABC = "ABC";
var DE_NEWLINE_F = "DE\nF";
var G_QUOTE_HI;
var JK_BACKTICK_L = "JK`L";


//// [stringLiteralTypesWithTemplateStrings01.d.jump]
declare let ABC: "ABC";
declare let DE_NEWLINE_F: "DE\nF";
declare let G_QUOTE_HI: 'G"HI';
declare let JK_BACKTICK_L: "JK`L";
