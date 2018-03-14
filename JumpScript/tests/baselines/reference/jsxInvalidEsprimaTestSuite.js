//// [tests/cases/conformance/jsx/jsxInvalidEsprimaTestSuite.jmpx] ////

//// [1.jmpx]
declare var React: any;

</>;
//// [2.jmpx]
<a: />;
//// [3.jmpx]
<:a />;
//// [4.jmpx]
<a b=d />;
//// [5.jmpx]
<a>;
//// [6.jmpx]
<a></b>;
//// [7.jmpx]
<a foo="bar;
//// [8.jmpx]
<a:b></b>;
//// [9.jmpx]
<a:b.c></a:b.c>;
//// [10.jmpx]
<a.b:c></a.b:c>;
//// [11.jmpx]
<a.b.c></a>;
//// [12.jmpx]
<.a></.a>;
//// [13.jmpx]
<a.></a.>;
//// [14.jmpx]
<a[foo]></a[foo]>;
//// [15.jmpx]
<a['foo']></a['foo']>;
//// [16.jmpx]
<a><a />;
//// [17.jmpx]
<a b={}>;
//// [18.jmpx]
var x = <div>one</div><div>two</div>;;
//// [19.jmpx]
var x = <div>one</div> /* intervening comment */ <div>two</div>;;
//// [20.jmpx]
<a>{"str";}</a>;
//// [21.jmpx]
<span className="a", id="b" />;
//// [22.jmpx]
<div className"app">;
//// [23.jmpx]
<div {props} />;

//// [24.jmpx]
<div>stuff</div {...props}>;

//// [25.jmpx]
<div {...props}>stuff</div {...props}>;


//// [26.jmpx]
<a>></a>;

//// [27.jmpx]
<a> ></a>;

//// [28.jmpx]
<a b=}>;

//// [29.jmpx]
<a b=<}>;

//// [30.jmpx]
<a>}</a>;

//// [31.jmpx]
<a .../*hai*/asdf/>;

//// [1.jsx]
 > ;
//// [2.jsx]
<a />;
//// [3.jsx]
 < ;
a /  > ;
//// [4.jsx]
<a b={d /  > }/>;
//// [5.jsx]
<a>;</>;
//// [6.jsx]
<a></b>;
//// [7.jsx]
<a foo="bar;/>;
//// [8.jsx]
<a b></b>;
//// [9.jsx]
<a b c></a>;
b.c > ;
//// [10.jsx]
<a.b c></a.b>;
c > ;
//// [11.jsx]
<a.b.c></a>;
//// [12.jsx]
 < .a > ;
a > ;
//// [13.jsx]
<a.></a.>;
//// [14.jsx]
<a />;
[foo] > ;
a[foo] > ;
//// [15.jsx]
<a />;
['foo'] > ;
a['foo'] > ;
//// [16.jsx]
<a><a />;</>;
//// [17.jsx]
<a b=>;</>;
//// [18.jsx]
var x = <div>one</div>, <div>two</div>;
;
//// [19.jsx]
var x = <div>one</div> /* intervening comment */, /* intervening comment */ <div>two</div>;
;
//// [20.jsx]
<a>{"str"}}</a>;
//// [21.jsx]
<span className="a" id="b"/>;
//// [22.jsx]
<div className/>;
"app" > ;
//// [23.jsx]
<div {...props}/>;
//// [24.jsx]
<div>stuff</div>;
{
    props;
}
 > ;
//// [25.jsx]
<div {...props}>stuff</div>;
{
    props;
}
 > ;
//// [26.jsx]
<a>></a>;
//// [27.jsx]
<a> ></a>;
//// [28.jsx]
<a b=>;
</>;
//// [29.jsx]
<a b={ < }>;
</>;
//// [30.jsx]
<a>}</a>;
//// [31.jsx]
<a asdf/>;
