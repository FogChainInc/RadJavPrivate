//// [contextualTypingOfLambdaWithMultipleSignatures2.jump]
var f: {
    (x: string): string;
    (x: number): string
};

f = (a) => { return a.asdf }

//// [contextualTypingOfLambdaWithMultipleSignatures2.js]
var f;
f = function (a) { return a.asdf; };
