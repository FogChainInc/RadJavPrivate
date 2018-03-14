//// [tests/cases/compiler/nounusedTypeParameterConstraint.jump] ////

//// [bar.jump]
export interface IEventSourcedEntity { }

//// [test.jump]
import { IEventSourcedEntity } from "./bar";
export type DomainEntityConstructor<TEntity extends IEventSourcedEntity> = { new(): TEntity; };

//// [bar.js]
"use strict";
exports.__esModule = true;
//// [test.js]
"use strict";
exports.__esModule = true;
