import { Generator } from "./Generator";
import { CustomFile } from "./CustomFile";

var RadJav = new Generator ("RadJav");
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

module.exports = RadJav;