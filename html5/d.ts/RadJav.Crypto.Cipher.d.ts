declare namespace RadJav {
    namespace Crypto {
        class Cipher {
            _engine: any;
            constructor();
            protected _init: any;
            getCapabilities(): any;
            cipherSync(data: any): any;
            cipher(data: any): any;
        }
    }
}
