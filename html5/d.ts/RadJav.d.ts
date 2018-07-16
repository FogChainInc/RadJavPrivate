declare let define: any;
declare namespace RadJav {
    let defaults: any;
    let useEval: boolean;
    let MIN_VERSION: number;
    let VERSION: number;
    let baseUrl: string;
    let themeUrl: string;
    let selectedLanguage: string;
    let themes: Theme;
    let _isInitialized: boolean;
    let _included: any[];
    let _lang: {
        [key: string]: any;
    };
    let _screenWidth: number;
    let _screenHeight: number;
    let themeUtils: any;
    let useAjax: boolean;
    let isMinified: boolean;
    function quit(exitCode?: number): void;
    function exit(exitCode: number): void;
    function include(path: string): Promise<any>;
    function initialize(libraries: {
        [key: string]: any;
    }[]): Promise<void>;
    function getStandardLibrary(): {
        file: string;
        themeFile: boolean;
        loadFirst?: boolean;
    }[];
    function getGUILibrary(): {
        file: string;
        themeFile: boolean;
        loadFirst?: boolean;
    }[];
    function getC3DLibrary(): {
        file: string;
        themeFile: boolean;
        loadFirst?: boolean;
    }[];
    function getNetLibrary(): {
        file: string;
        themeFile: boolean;
    }[];
    function includeLibraries(libraries: any): Promise<void>;
    function _loadLanguages(): Promise<any>;
    function _loadTheme(themeURL: string): Promise<any>;
    function runApplication(file: string | Function): Promise<any>;
    function runApplicationFromFile(file: string): Promise<void>;
    function loadObjects(objs: any[]): Promise<{
        [key: string]: any;
    }>;
    function _isUsingInternetExplorerTheWorstWebBrowserEver(): boolean;
    function _getSyncResponse(addr: string): string;
    function _getResponse(addr: string): Promise<string>;
    function getWidth(): number;
    function getHeight(): number;
    function clone(...obj: any[]): any[];
    function cloneObject(obj: {
        [key: string]: any;
    }): {
        [key: string]: any;
    };
    function cloneArray(obj: any[]): any[];
    function copyProperties(obj1: object, obj2: object, overwriteExisting: boolean): object;
    function setDefaultValue(param: any, defaultValue: any, onValue?: Function): any;
    function keepContext(func: Function, context: object, val?: any): any;
    function getLangString(keyword: any, ...other: any[]): string;
    function combineString(primaryString: string, ...otherStrings: string[]): string;
    class Theme {
        obj: {
            [key: string]: any;
        };
        name: string;
        version: string;
        author: string;
        lastUpdated: Date;
        description: string;
        url: string;
        initFile: string;
        cssFiles: string[];
        fonts: object[];
        constructor(obj?: {});
        static loadInitializationFile(): Promise<any>;
        static loadJavascriptFiles(): Promise<any>;
        static event(file: string, event: string, ...other: any[]): Promise<any>;
        static eventSync(file: string, event: string, ...other: any[]): any;
        static exports(): any;
        static themeObjects(): any;
        static loadTheme(url: string, data: any): any;
    }
    namespace GUI {
        function initObj(type: string | object | any, name: any, text: string, parent: object): object;
        function create(type: string, name: string, text: string, parent: object): any;
        function createObjects(objects: any, parent: object, beforeCreated?: Function): Promise<any>;
    }
    namespace C3D {
    }
    namespace Net {
        function httpRequest(req: string | object): Promise<any>;
    }
    class Console {
        print(message: string): void;
        println(message: string): void;
    }
    namespace OS {
        let type: "windows" | "linux" | "macosx" | "html5";
        let numBits: number;
        function onReady(func: any): Promise<any>;
        function openWebBrowserURL(url: string): void;
        namespace HTML5 {
            let absolutePositioning: boolean;
            function showElement(elm: any, show: boolean): void;
            function getOS(): String;
            function getUrlParamObj(): Object;
            function getUrlParam(name: string): any;
            function useAbsolutePositioning(absolutePositioning: boolean): void;
            function downloadTextAsFile(text: string, fileName: string, mimeType: string): void;
            function reloadPage(forceNewPage: boolean): void;
            function getParentHTML(obj: any): any;
            function getHTMLDOM(str: string): Object;
            function appendHTML(obj: HTMLElement, html: any): object;
            function selectDOM(obj: HTMLElement, selector: any): Object;
            function ready(obj: any): Promise<void>;
            function interfaceConnector(connectorName: string | object | any, methodName: string): Object;
        }
    }
}
declare var _eval: typeof eval;
declare var _Function: FunctionConstructor;
declare var __extends: any;
