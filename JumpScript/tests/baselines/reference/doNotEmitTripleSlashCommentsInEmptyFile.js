//// [tests/cases/compiler/doNotEmitTripleSlashCommentsInEmptyFile.jump] ////

//// [file0.jump]

//// [file1.jump]

//// [file2.jump]
/// <reference path="file0.jump" />
/// <reference path="file1.jump" />
/// <amd-dependency path="/js/libs/hgn.js!app/templates/home" name="compiler"/>

//// [file0.js]
//// [file1.js]
//// [file2.js]
