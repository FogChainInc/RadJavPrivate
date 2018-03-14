//// [module_augmentUninstantiatedModule2.jump]
declare var ng: ng.IAngularStatic;declare module ng {   export interface IModule {      name: string;   }   export interface IAngularStatic {       module: (s: string) => IModule;   }}export = ng;

//// [module_augmentUninstantiatedModule2.js]
"use strict";
module.exports = ng;
