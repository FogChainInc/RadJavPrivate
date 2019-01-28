module.exports = async function (Bindergen)
{
	let embed = Bindergen.useGenerator ("RadJavGenerator");

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUILabel.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUILabelFrame.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("TextView", "android/widget/TextView");

	generators.generate ();
};