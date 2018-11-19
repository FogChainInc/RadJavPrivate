declare let define: any;
declare namespace RadJav {
    let defaults: any;
    let useEval: boolean;
    let MIN_VERSION: number;
    let VERSION: number;
    let baseUrl: string;
    let themeUrl: string;
    let selectedLanguage: string;
    let currentTheme: Theme;
    let _isInitialized: boolean;
    let _included: any[];
    let _lang: {
        [key: string]: any;
    };
    let themeUtils: any;
    let useAjax: boolean;
    let isMinified: boolean;
    let _animations: RadJav.Animation[];
    let animationFrameRate: number;
    let prevTime: number;
    let screens: RadJav.OS.ScreenInfo[];
    function quit(exitCode?: number): void;
    function exit(exitCode: number): void;
    function include(path: string): Promise<any>;
    function initialize(libraries?: {
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
    function getMUILibrary(): {
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
    function getPrimaryScreen(): RadJav.OS.ScreenInfo;
    function addScreen(screen: RadJav.OS.ScreenInfo): void;
    function setupScreens(): void;
    function connectDevTools(server?: string, showMessage?: boolean): void;
    function includeLibraries(libraries: any): Promise<void>;
    function _loadLanguages(): Promise<any>;
    function _loadTheme(themeURL: string): Promise<any>;
    function runApplication(file: string | Function): Promise<any>;
    function runApplicationFromFile(file: string): Promise<void>;
    function loadObjects(objs: any[]): Promise<{
        [key: string]: any;
    }>;
    function loadXML(xml: string): Promise<any>;
    function loadAppXML(parent: RadJav.GUI.GObject, objs: RadJav.XML.XMLTag[]): RadJav.GUI.GObject[];
    function _isUsingInternetExplorerTheWorstWebBrowserEver(): boolean;
    function _getSyncResponse(addr: string): string;
    function _getResponse(addr: string): Promise<string>;
    function clone(...obj: any[]): any[];
    function cloneObject(obj: any): any;
    function cloneArray(obj: any[]): any[];
    function copyProperties(obj1: object, obj2: object, overwriteExisting: boolean): object;
    function setDefaultValue(param: any, defaultValue: any, onValue?: Function): any;
    function loadModule(moduleData: string): any;
    function keepContext(func: Function, context: object, val?: any): any;
    function getLangString(keyword: any, ...other: any[]): string;
    function combineString(primaryString: string, ...otherStrings: string[]): string;
    function getTime(): number;
    function addAnimation(anim: RadJav.Animation): void;
    function animationUpdate(): void;
    class TagType {
        tag: string;
        type: string;
        constructor();
    }
    class Theme {
        name: string;
        version: string;
        author: string;
        lastUpdated: Date;
        description: string;
        url: string;
        initFile: string;
        cssFiles: string[];
        fonts: object[];
        exports: any;
        themeObjects: any[];
        constructor(obj?: Theme);
        loadInitializationFile(): Promise<any>;
        loadJavascriptFiles(): Promise<any>;
        event(file: string, event: string, ...other: any[]): Promise<any>;
        eventSync(file: string, event: string, ...other: any[]): any;
        static loadTheme(url: string, data: any): Theme;
    }
    namespace GUI {
        function initObj(type: string | object | any, name: any, text: string, parent: object): object;
        function create(type: string, name: string, text: string, parent: object): any;
        function createObjects(objects: any, parent: object, beforeCreated?: Function): Promise<any>;
    }
    namespace MUI {
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
        static print(message: string): void;
        static println(message: string): void;
    }
    namespace OS {
        let type: string;
        let numBits: number;
        let args: string[];
        class ScreenInfo {
            width: number;
            height: number;
            scale: number;
            constructor(width?: number, height?: number, scale?: number);
            getWidth(): number;
            getHeight(): number;
            getScale(): number;
            static getNumberOfScreens(): number;
            static getScreenInfo(screenIndex: number): ScreenInfo;
        }
        function onReady(func: any): Promise<any>;
        function openWebBrowserURL(url: string): void;
        namespace HTML5 {
            let absolutePositioning: boolean;
            function showElement(elm: any, show: boolean): void;
            function getOS(): String;
            function getUrlParamObj(): any;
            function getUrlParam(name: string): any;
            function useAbsolutePositioning(absolutePositioning: boolean): void;
            function downloadTextAsFile(text: string, fileName: string, mimeType: string): void;
            function reloadPage(forceNewPage: boolean): void;
            function getParentHTML(obj: any): any;
            function getHTMLDOM(str: string): Object;
            function appendHTML(obj: HTMLElement, html: any): any;
            function selectDOM(obj: HTMLElement, selector: any): any;
            function ready(obj: any): Promise<void>;
            function interfaceConnector(connectorName: string | object | any, methodName: string): any;
        }
    }
}
declare function parseBoolean(str: string): boolean;
declare var _eval: typeof eval;
declare var _Function: FunctionConstructor;
