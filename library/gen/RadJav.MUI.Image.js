module.exports = async function (Bindergen)
{
	let embed = Bindergen.useGenerator ("RadJavGenerator");

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUIImage.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUIImageFrame.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("ImageView", "android/widget/ImageView");

	generators.generate ();
};