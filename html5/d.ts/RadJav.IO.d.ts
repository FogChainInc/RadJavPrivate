/// <reference path="RadJav.d.ts" />
interface DOMElement extends HTMLElement {
}
declare namespace RadJav {
    class IO {
        static isDir(path: string): boolean;
        static isFile(path: string): boolean;
        static isSymLink(path: string): boolean;
        static currentPath(): string;
        static changePath(path: string): void;
        static exists(path: string): boolean;
        static createDir(path: string): void;
        static copyDir(src: string, dest: string, recursive?: boolean): void;
        static renameDir(src: string, dest: string): void;
        static deleteDir(path: string): void;
        static isEmpty(path: string): boolean;
        static createSymLink(path: string, link: string): void;
        static copySymLink(src: string, dest: string): void;
        static renameSymLink(src: string, dest: string): void;
        static deleteSymLink(path: string): void;
        static copyFile(src: string, dest: string): void;
        static renameFile(src: string, dest: string): void;
        static deleteFile(path: string): void;
        static listFiles(path: string, recursive?: boolean): string[];
        static listFilesAsync(asyncCallback: (newFileOrDir: string) => boolean, path: string, recursive?: boolean): string[];
        static normalizePath(path: string, basePath?: string): string;
        static normalizeAndVerifyPath(path: string, basePath?: string): string;
        static normalizeCurrentPath(path: string): string;
    }
    namespace IO {
        class SerialComm {
            getPort(): string;
            getBaud(): Number;
            getByteSize(): Number;
            getStopBits(): Number;
            getParity(): Number;
            open(): boolean;
            isOpen(): boolean;
            read(bufferSize: Number): string;
            write(buffer: Number, bufferSize?: Number): Number;
            close(): void;
        }
        class TextFile {
            static writeFile(path: string, content: string): void;
            static writeFileAsync(path: string, content: string): void;
            static readFile(path: string): string;
            static readFileAsync(path: string): string;
            static read: Number;
            static write: Number;
            static append: Number;
        }
        class StreamFile {
            static writeStream(path: string, content: string): void;
            static writeStreamAsync(path: string, content: string): void;
            static readStream(path: string): string;
            static readStreamAsync(path: string): string;
            static read: Number;
            static write: Number;
            static append: Number;
        }
    }
    namespace XML {
        class XMLFile {
            _parser: DOMParser;
            _root: XMLTag;
            _loadedFile: XMLParser;
            constructor();
            parseXMLFile(filePath: string): Promise<string>;
            parseXML(xmlString: string): void;
            getRoot(): XMLTag;
            static loadFile(filePath: string): Promise<XMLFile>;
            static loadString(xmlString: string): XMLFile;
        }
        class XMLParser {
            xmlFile: any;
            constructor(xmlFile?: any);
        }
        class XMLTag {
            _tag: string;
            _attributes: {
                [name: string]: XMLAttribute;
            };
            _value: string;
            _children: XMLTag[];
            _loadedFile: XMLParser;
            constructor(tag: string | XMLParser);
            getChildren(): XMLTag[];
            getAttributes(): {
                [name: string]: XMLAttribute;
            };
            setTag(name: string): void;
            getTag(): string;
            setValue(value: string): void;
            getValue(): string;
            getTags(tag: string): XMLTag[];
            setAttribute(attribute: string, value: string): void;
            hasAttribute(attribute: string): boolean;
            getAttribute(attribute: string): XMLAttribute;
            getAttributeString(attribute: string, defaultValue?: string): string;
            getAttributeInt(attribute: string, defaultValue?: number): number;
            getAttributeFloat(attribute: string, defaultValue?: number): number;
            getAttributeBoolean(attribute: string, defaultValue?: boolean): boolean;
            toString(): string;
        }
        class XMLAttribute {
            _name: string;
            _value: string;
            constructor(name: string, value: string);
            setValue(value: string): void;
            getValue(): string;
            toInt(): number;
            toFloat(): number;
            toBoolean(): boolean;
            toString(): string;
        }
    }
}
