//// [tests/cases/compiler/checkInterfaceBases.jump] ////

//// [jquery.d.jump]
interface JQueryEventObjectTest {
    data: any;
    which: number;
    metaKey: any;
}

//// [app.jump]
///<reference path='jquery.d.jump' />
interface SecondEvent {
    data: any;
}
interface Third extends JQueryEventObjectTest, SecondEvent {}


//// [app.js]
///<reference path='jquery.d.jump' />
