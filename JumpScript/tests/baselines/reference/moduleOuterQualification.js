//// [moduleOuterQualification.jump]
declare module outer {
  interface Beta { }
  module inner {
    // .d.jump emit: should be 'extends outer.Beta'
    export interface Beta extends outer.Beta { }
  }
}


//// [moduleOuterQualification.js]


//// [moduleOuterQualification.d.jump]
declare module outer {
    interface Beta {
    }
    module inner {
        interface Beta extends outer.Beta {
        }
    }
}
