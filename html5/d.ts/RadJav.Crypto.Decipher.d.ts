declare namespace RadJav {
    namespace Crypto {
        class Decipher {
            _engine: any;
            constructor();
            protected _init: any;
            getCapabilities(): any;
            decipherSync(data: any): any;
            decipher(data: any): any;
        }
    }
}
