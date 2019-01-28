module.exports = async function (Bindergen)
{
	let embed = Bindergen.useGenerator ("RadJavGenerator");

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUITextbox.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUITextboxFrame.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("EditText", "android/widget/EditText");

	generators.generate ();
};