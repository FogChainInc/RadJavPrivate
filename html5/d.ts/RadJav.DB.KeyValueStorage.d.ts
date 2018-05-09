declare namespace RadJav {
    namespace DB {
        class KeyValueStorage {
            _storage: any;
            constructor();
            protected _init: any;
            open(path: string): boolean;
            write(key: string, value: string): Promise<void>;
            read(key: string): Promise<string>;
            close(): void;
        }
    }
}
