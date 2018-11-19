declare namespace RadJav {
    namespace Crypto {
        class PublicKey {
            _engine: any;
            constructor();
            protected _init: any;
            verifySync(data: any, signature: any): any;
            verify(data: any, signature: any): any;
            decryptSync(data: any): any;
            decrypt(data: any): any;
        }
    }
}
