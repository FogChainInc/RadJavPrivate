declare namespace RadJav {
    namespace Crypto {
        class Hash {
            _engine: any;
            constructor();
            protected _init: any;
            getCapabilities(): any;
            digestSync(data: any): any;
            digest(data: any): any;
        }
    }
}
