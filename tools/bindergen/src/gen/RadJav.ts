import { Generator } from "./Generator";
import { CustomFile } from "./CustomFile";
import { GeneratorClass } from "./GeneratorClass";
import { GeneratorNamespace } from "./GeneratorNamespace";
import { GeneratorFunction } from "./GeneratorFunction";

var exportRadJav = (async function ()
{
	let RadJav: Generator = new Generator ("RadJav");
	RadJav.createCustomFile ({
			id: "cmakeV8Includes",
			pass: "V8", 
			path: "library/include/includes.cmake", 
			contents: ``
		});
	RadJav.createKeyword ("ENGINE_CALL_ARGS", "V8", "V8_JAVASCRIPT_ENGINE, args");
	RadJav.createKeyword ("_guiFinishedCreatingObject", "V8", `
	v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
	v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

	args.GetReturnValue().Set(promise);
	`);

	let radjavN: GeneratorNamespace = RadJav.createNamespace ("RadJav");
	let Android: GeneratorClass = await radjavN.createClass ("Android", "RadJav.Android");
	let runOnUiThreadAsync: GeneratorFunction = new GeneratorFunction (Android);
	runOnUiThreadAsync.exec (function (params: string[], func: Function): string
		{
			return ("TEST");
		});
	Android.addFunction (runOnUiThreadAsync);

	return (RadJav);
}())

module.exports = exportRadJav;