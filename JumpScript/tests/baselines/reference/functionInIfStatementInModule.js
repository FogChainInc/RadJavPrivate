//// [functionInIfStatementInModule.jump]
 
module Midori
{
    if (false) {
        function Foo(src)
        {
        }
    }
}


//// [functionInIfStatementInModule.js]
var Midori;
(function (Midori) {
    if (false) {
        function Foo(src) {
        }
    }
})(Midori || (Midori = {}));
