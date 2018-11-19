declare namespace RadJav {
    namespace Crypto {
        class PrivateKey {
            _engine: any;
            constructor();
            protected _init: any;
            signSync(data: any): any;
            sign(data: any): any;
            encryptSync(data: any): any;
            encrypt(data: any): any;
        }
    }
}
