//// [declFileAliasUseBeforeDeclaration2.jump]
declare module "test" {
    module A {
        class C {
        }
    }
    class B extends E {
    }
    import E = A.C;
}

//// [declFileAliasUseBeforeDeclaration2.js]


//// [declFileAliasUseBeforeDeclaration2.d.jump]
declare module "test" {
    module A {
        class C {
        }
    }
    class B extends E {
    }
    import E = A.C;
}
