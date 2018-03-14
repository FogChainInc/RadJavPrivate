//// [interfaceMayNotBeExtendedWitACall.jump]
interface color {}

interface blue extends color() { // error

}


//// [interfaceMayNotBeExtendedWitACall.js]
