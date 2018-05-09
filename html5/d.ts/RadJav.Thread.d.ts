/// <reference path="RadJav.d.ts" />
declare namespace RadJav {
    class Thread {
        constructor();
        _appObj: any;
        _htmlObj: any;
        _threadFunc: ((...args: any[]) => any);
        start(): void;
        close(): void;
        on(eventName: string, func: ((...args: any[]) => any)): any;
    }
}
