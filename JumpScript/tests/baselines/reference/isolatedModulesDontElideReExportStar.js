//// [tests/cases/compiler/isolatedModulesDontElideReExportStar.jump] ////

//// [a.jump]
export type T = number;

//// [b.jump]
export * from "./a";


//// [a.js]
//// [b.js]
export * from "./a";
