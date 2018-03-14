//// [parserUnterminatedGeneric2.jump]
declare module ng {
    interfaceICompiledExpression {
        (context: any, locals?: any): any;
        assign(context: any, value: any): any;
    }

    interface IQService {
        all(promises: IPromise < any > []): IPromise<

//// [parserUnterminatedGeneric2.js]
