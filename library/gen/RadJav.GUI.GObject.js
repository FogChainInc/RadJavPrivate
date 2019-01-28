module.exports = async function (Bindergen)
{
	var GeneratorClass = require ("./GeneratorClass").GeneratorClass;

	let embed = Bindergen.useGenerator ("RadJavGenerator");
	let RadJavGUI = embed.useNamespace ("RadJav.GUI");

	class GObject extends GeneratorClass
	{
		constructor ()
		{
			this._appObj = { type: "external" };
		}
	}

	await RadJavGUI.createClass ("GObject", GObject);

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPGUIGObject.h", 
			"library/src/RadJav/cpp/android/RadJavCPPGUIGObjectWidget.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("view", "android.view.View");

	generators.generate ();
}