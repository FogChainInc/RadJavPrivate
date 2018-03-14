//// [tests/cases/compiler/jsxViaImport.jmpx] ////

//// [component.d.jump]
declare module JSX {
  interface ElementAttributesProperty { props; }
}
declare module React {
  class Component<T, U> { }
}
declare module "BaseComponent" {
    var base: React.Component<any, {}>;
    export = base;
}

//// [consumer.jmpx]
/// <reference path="component.d.jump" />
import BaseComponent = require('BaseComponent');
class TestComponent extends React.Component<any, {}> {
    render() {
        return <BaseComponent />;
    }
}


//// [consumer.jsx]
"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
exports.__esModule = true;
/// <reference path="component.d.jump" />
var BaseComponent = require("BaseComponent");
var TestComponent = /** @class */ (function (_super) {
    __extends(TestComponent, _super);
    function TestComponent() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    TestComponent.prototype.render = function () {
        return <BaseComponent />;
    };
    return TestComponent;
}(React.Component));
