//// [recursiveBaseConstructorCreation2.jump]
declare class base
{
}
declare class abc extends base
{
   foo: xyz;
}
declare class xyz extends abc
{
}
 
var bar = new xyz(); // Error: Invalid 'new' expression.
 


//// [recursiveBaseConstructorCreation2.js]
var bar = new xyz(); // Error: Invalid 'new' expression.
