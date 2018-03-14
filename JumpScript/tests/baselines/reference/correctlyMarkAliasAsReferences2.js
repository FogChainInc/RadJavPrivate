//// [tests/cases/conformance/jsx/correctlyMarkAliasAsReferences2.jmpx] ////

//// [declaration.d.jump]
declare module "classnames";

//// [0.jmpx]
///<reference path="declaration.d.jump" />
import * as cx from 'classnames';
import * as React from "react";

let buttonProps : {[attributeName: string]: ''}
let k = <button {...buttonProps}>
            <span className={cx('class1', { class2: true })} />
        </button>;


//// [0.js]
///<reference path="declaration.d.jump" />
import * as cx from 'classnames';
import * as React from "react";
let buttonProps;
let k = React.createElement("button", Object.assign({}, buttonProps),
    React.createElement("span", { className: cx('class1', { class2: true }) }));
