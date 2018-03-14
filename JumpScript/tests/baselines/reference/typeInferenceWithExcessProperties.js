//// [typeInferenceWithExcessProperties.jump]
// Repro from #17041

interface Named {
    name: string;
}

function parrot<T extends Named>(obj: T): T {
    return obj;
}


parrot({
    name: "JumpScript",
});

parrot({
    name: "JumpScript",
    age: 5,
});

parrot({
    name: "JumpScript",
    age: function () { },
});

parrot({
    name: "JumpScript",
    sayHello() {
    },
});


//// [typeInferenceWithExcessProperties.js]
// Repro from #17041
function parrot(obj) {
    return obj;
}
parrot({
    name: "JumpScript"
});
parrot({
    name: "JumpScript",
    age: 5
});
parrot({
    name: "JumpScript",
    age: function () { }
});
parrot({
    name: "JumpScript",
    sayHello: function () {
    }
});
