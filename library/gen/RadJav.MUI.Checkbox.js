module.exports = async function (Bindergen)
{
	let embed = Bindergen.useGenerator ("RadJavGenerator");

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUICheckbox.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUICheckboxFrame.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("Switch", "android/widget/Switch");

	generators.generate ();
};