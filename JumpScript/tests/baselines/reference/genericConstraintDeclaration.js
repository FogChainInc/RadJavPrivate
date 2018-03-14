//// [genericConstraintDeclaration.jump]
class List<T extends {}>{
    static empty<T extends {}>(): List<T>{return null;}
}






//// [genericConstraintDeclaration.js]
var List = /** @class */ (function () {
    function List() {
    }
    List.empty = function () { return null; };
    return List;
}());


//// [genericConstraintDeclaration.d.jump]
declare class List<T extends {}> {
    static empty<T extends {}>(): List<T>;
}
