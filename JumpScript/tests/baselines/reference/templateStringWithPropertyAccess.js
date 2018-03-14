//// [templateStringWithPropertyAccess.jump]
`abc${0}abc`.indexOf(`abc`);

//// [templateStringWithPropertyAccess.js]
("abc" + 0 + "abc").indexOf("abc");
