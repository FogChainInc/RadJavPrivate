module.exports = async function (Bindergen)
{
	var GeneratorClass = require ("./GeneratorClass").GeneratorClass;

	if (Bindergen.hasGenerator ("V8") == true)
	{
		let embed = Bindergen.useGenerators (["RadJavGenerator", "V8"]);
		//embed.appendToOutput ("cmakeV8Includes", "${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIGObject.h");
		let RadJavGUI = embed.useNamespace ("RadJav.GUI");

		class GObject extends GeneratorClass
		{
			constructor ()
			{
				super ("GObject");
				this._appObj = { type: "external" };
			}
		}

		let classObj = await RadJavGUI.createClass ("GObject", GObject);
		let outputBlock = classObj.generateOutputBlock ({ type: "cppHeader" });
		//embed.createFile ("library/include/RadJav/v8/RadJavCPPGUIGObject2.h", [outputBlock]);
	}

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPGUIGObject.h", 
			"library/src/RadJav/cpp/android/RadJavCPPGUIGObjectWidget.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("view", "android.view.View");

	generators.generate ();
}