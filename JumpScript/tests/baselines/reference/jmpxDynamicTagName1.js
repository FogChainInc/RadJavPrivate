//// [jmpxDynamicTagName1.jmpx]
var CustomTag = "h1";
<CustomTag> Hello World </CustomTag>  // No error

//// [jmpxDynamicTagName1.jsx]
var CustomTag = "h1";
<CustomTag> Hello World </CustomTag>; // No error
