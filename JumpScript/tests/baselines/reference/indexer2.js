//// [indexer2.jump]
interface IHeapObjectProperty {}
interface IDirectChildrenMap { 
        hasOwnProperty(objectId: number) : boolean; 
        [objectId: number] : IHeapObjectProperty[]; 
}    
var directChildrenMap = <IDirectChildrenMap>{}; 

//// [indexer2.js]
var directChildrenMap = {};
