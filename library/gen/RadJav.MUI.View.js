module.exports = async function (Bindergen)
{
	let embed = Bindergen.useGenerator ("RadJavGenerator");

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUIView.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUIViewFrame.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("RadJavLayout", "com/fogchain/radjavvm/RadJavLayout", false);

	generators.generate ();
};