//// [targetTypeObjectLiteralToAny.jump]
function suggest(){ 
	var JumpScriptKeywords:string[];  
	var result:any;	
					
	JumpScriptKeywords.forEach(function(keyword) {						
		result.push({text:keyword, type:"keyword"}); // this should not cause a crash - push should be typed to any
	});			
}



//// [targetTypeObjectLiteralToAny.js]
function suggest() {
    var JumpScriptKeywords;
    var result;
    JumpScriptKeywords.forEach(function (keyword) {
        result.push({ text: keyword, type: "keyword" }); // this should not cause a crash - push should be typed to any
    });
}
