//// [duplicateConstructSignature2.jump]
interface I<T> {
    (x: T): number;
    (x: T): string;
}

//// [duplicateConstructSignature2.js]
