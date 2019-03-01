module.exports = async function (Bindergen)
{
	if (Bindergen.hasGenerator ("V8") == true)
	{
		let embed = Bindergen.useGenerators (["RadJavGenerator", "V8"]);
		//embed.appendToCustomFile ("cmakeV8Includes", "${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIButton.h");
		//embed.outputFilename ("RadJav%PASS%GUIButton");
		let RadJavGUI = embed.useNamespace ("RadJav.GUI");
		let GObject = RadJavGUI.getClass ("GObject");

		class Button extends GObject
		{
			constructor ()
			{
			}

			create ()
			{
				let func = this.createFunction ("create");
				let engineArgs = this.$generator.getFunction ("RadJav.getEngineArgs");

				func.code = `
				CPP::GUI::Button *appObject = RJNEW CPP::GUI::Button(${engineArgs ()});
				appObject->create();
				${this._appObj.set ("appObject")};
						`;
				func.returns = this.$generator.callFunction ("RadJav.createPromise");

				return (func);
			}

			setText (text)
			{
				let func = this.createFunction ("setText");

				func.code = `
				String str = ${text.toEngineString()};
				UITYPE *appObject = (UITYPE *)${this.getExternal ("_appObj")};

				if (appObject != NULL)
					appObject->setText(str);
						`;

				return (func);
			}

			getText ()
			{
				let func = this.createFunction ("getText");
				let toEngineString = this.$generator.getFunction ("RadJav.toEngineString");

				func.code = `
				String text = ${this.getString ("_text")});
				UITYPE *appObject = (UITYPE *)${this.getExternal ("_appObj")};

				if (appObject != NULL)
					text = appObject->getText();
						`;
				func.returns = `${toEngineString ("text")}`;

				return (func);
			}
		}
		//embed.generate ();
	}

	let generators = Bindergen.useGenerators (["AndroidJNI", "RadJavGenerator"]);
	generators.parseFiles ([
			"library/include/RadJav/cpp/RadJavCPPMUIButton.h", 
			"library/src/RadJav/cpp/android/RadJavCPPMUIButtonFrame.cpp"
		]);
	let jni = generators.useNamespace ("jni");
	await jni.createClass ("Button", "android.widget.Button");

	generators.generate ();
};