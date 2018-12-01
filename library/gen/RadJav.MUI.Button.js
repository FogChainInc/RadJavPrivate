(async function ()
{
	let embed = Bindergen.useGenerator ("RadJav");
	embed.usePasses (["V8", "JSC"]);
	embed.appendToCustomFile ("cmakeV8Includes", "${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIButton.h");
	embed.outputFilename ("RadJav%PASS%GUIButton");
	let RadJavGUI = embed.createNamespace ("RadJav.GUI");
	let obj = await RadJavGUI.createClass ("Button", {
			name: "RadJav.GUI.Button", 
			extends: "RadJav.GUI.GObject", 
			functions: [
					{
						name: "create", 
						code: `
						CPP::GUI::Button *appObject = RJNEW CPP::GUI::Button(%ENGINE_CALL_ARGS%);
						appObject->create();
								`, 
						map: [{
							native: "appObject", 
							external: "_appObj"
						}], 
						returns: "%_guiFinishedCreatingObject%"
					}, 
					{
						name: "setText", 
						code: `
						String str = %parseValue%(%arg0%);
						UITYPE *appObject = (UITYPE *)%ENGINE%->%getExternal%(%thisObj%, "_appObj");

						if (appObject != NULL)
							appObject->setText(str);
								`
					}, 
					{
						name: "getText", 
						code: `
						String text = %ENGINE%->%getString%(%thisObj%, "_text");
						UITYPE *appObject = (UITYPE *)%ENGINE%->%getExternal%(%thisObj%, "_appObj");

						if (appObject != NULL)
							text = appObject->getText();

						%EngineString% = text->%toEngineString()%;
								`, 
						returns: "%EngineString%"
					}
				]
		});
	//embed.generate ();

	let AndroidJNI = Bindergen.useGenerator ("AndroidJNI");
	AndroidJNI.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUIButton.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUIButtonFrame.cpp"
		]);
	let jni = AndroidJNI.createNamespace ("jni");
	let button = await jni.createClass ("button", "android.widget.Button");
	let jniHeaders = AndroidJNI.getOutput ("jniHeaders");
	let jniCreate = AndroidJNI.getOutput ("jniCreate");

	AndroidJNI.createKeyword ("jniType", "Button");
	AndroidJNI.generate ();
}())