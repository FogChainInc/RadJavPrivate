// @ts-ignore
import { Generator } from "./Generator";
// @ts-ignore
import { GeneratorClass } from "./GeneratorClass";
// @ts-ignore
import { GeneratorNamespace } from "./GeneratorNamespace";
// @ts-ignore
import { GeneratorFunction } from "./GeneratorFunction";
// @ts-ignore
import { Block } from "./Block";

var radJavExport = async function ()
{
	let RadJavGenerator: Generator = new Generator ("RadJavGenerator");
	RadJavGenerator.createCustomFile ({
			id: "cmakeV8Includes",
			pass: "V8", 
			path: "library/include/includes.cmake", 
			contents: ``
		});
	RadJavGenerator.createKeyword ("ENGINE_CALL_ARGS", "V8", "V8_JAVASCRIPT_ENGINE, args");
	RadJavGenerator.createKeyword ("_guiFinishedCreatingObject", "V8", `
	v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
	v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

	args.GetReturnValue().Set(promise);
	`);

	String.prototype["toWrappedJNIString"] = function (getImmediately: boolean = true): string
		{
			let output: string = `wrap_local(env, ${this}.toJNIString())`;

			if (getImmediately == true)
				output += ".get ()";

			return (output);
		};

	RadJavGenerator.addJSObject ("String", String);
	let radjavN: GeneratorNamespace = RadJavGenerator.createNamespace ("RadJav");
	let Android: GeneratorClass = await radjavN.createClass ("Android", "RadJav.Android");
	let runOnUiThreadAsync: GeneratorFunction = new GeneratorFunction (Android);
	runOnUiThreadAsync.name = "runOnUiThreadAsync";
	runOnUiThreadAsync.exec (function (block: Block, genFunc: GeneratorFunction, args: any[]): string
		{
			let captures: string[] = args[0];
			let func: Function = args[1];
			let result: string = func.apply (this, captures);
			let output: string = `RadJav::runOnUiThreadAsync ([${captures.toString ()}](JNIEnv *env, void *data){
				${result}
			});`;

			return (output);
		});
	Android.addFunction (runOnUiThreadAsync);

	return (RadJavGenerator);
};

module.exports = radJavExport;