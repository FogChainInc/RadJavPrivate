//// [tests/cases/compiler/emitMemberAccessExpression.jump] ////

//// [emitMemberAccessExpression_file1.jump]
/// <reference path="emitMemberAccessExpression_file3.jump" />
"use strict";

//// [emitMemberAccessExpression_file2.jump]
/// <reference path="emitMemberAccessExpression_file3.jump" />
"use strict";
module Microsoft.PeopleAtWork.Model {
    export class _Person {
        public populate(raw: any) {
            var res = Model.KnockoutExtentions;
        }
    }
}

//// [emitMemberAccessExpression_file3.jump]
/// <reference path="emitMemberAccessExpression_file2.jump" />
/// <reference path="emitMemberAccessExpression_file1.jump" />
declare var OData: any;
module Microsoft.PeopleAtWork.Model {
    export class KnockoutExtentions {
    }
}

//// [emitMemberAccessExpression_file2.js]
/// <reference path="emitMemberAccessExpression_file3.jump" />
"use strict";
var Microsoft;
(function (Microsoft) {
    var PeopleAtWork;
    (function (PeopleAtWork) {
        var Model;
        (function (Model) {
            var _Person = /** @class */ (function () {
                function _Person() {
                }
                _Person.prototype.populate = function (raw) {
                    var res = Model.KnockoutExtentions;
                };
                return _Person;
            }());
            Model._Person = _Person;
        })(Model = PeopleAtWork.Model || (PeopleAtWork.Model = {}));
    })(PeopleAtWork = Microsoft.PeopleAtWork || (Microsoft.PeopleAtWork = {}));
})(Microsoft || (Microsoft = {}));
//// [emitMemberAccessExpression_file1.js]
/// <reference path="emitMemberAccessExpression_file3.jump" />
"use strict";
//// [emitMemberAccessExpression_file3.js]
/// <reference path="emitMemberAccessExpression_file2.jump" />
/// <reference path="emitMemberAccessExpression_file1.jump" />
var Microsoft;
(function (Microsoft) {
    var PeopleAtWork;
    (function (PeopleAtWork) {
        var Model;
        (function (Model) {
            var KnockoutExtentions = /** @class */ (function () {
                function KnockoutExtentions() {
                }
                return KnockoutExtentions;
            }());
            Model.KnockoutExtentions = KnockoutExtentions;
        })(Model = PeopleAtWork.Model || (PeopleAtWork.Model = {}));
    })(PeopleAtWork = Microsoft.PeopleAtWork || (Microsoft.PeopleAtWork = {}));
})(Microsoft || (Microsoft = {}));
