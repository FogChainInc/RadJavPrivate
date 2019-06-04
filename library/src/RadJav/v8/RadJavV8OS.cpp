/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
	and associated documentation files (the "Software"), to deal in the Software without restriction, 
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial 
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "v8/RadJavV8OS.h"

#include "RadJav.h"
#include "RadJavString.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPOS.h"


namespace RadJAV
{
	namespace V8B
	{
		v8::Persistent<v8::Value> *OS::onReadyFunction = NULL;

		void OS::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "onReady", OS::onReady);
			V8_CALLBACK(object, "openWebBrowserURL", OS::openWebBrowserURL);
			V8_CALLBACK(object, "exec", OS::exec);
			V8_CALLBACK(object, "getDocumentsPath", OS::getDocumentsPath);
			V8_CALLBACK(object, "getTempPath", OS::getTempPath);
			V8_CALLBACK(object, "getUserDataPath", OS::getUserDataPath);
			V8_CALLBACK(object, "getApplicationPath", OS::getApplicationPath);
			V8_CALLBACK(object, "getCurrentWorkingPath", OS::getCurrentWorkingPath);
			V8_CALLBACK(object, "setCurrentWorkingPath", OS::setCurrentWorkingPath);
			V8_CALLBACK(object, "saveFileAs", OS::saveFileAs);
			V8_CALLBACK(object, "openFileAs", OS::openFileAs);
		}

		void OS::destroy ()
		{
			DELETEOBJ (onReadyFunction);
		}

		void OS::onReady(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();

			persistent->Reset(V8_JAVASCRIPT_ENGINE->isolate, func);

			DELETEOBJ(onReadyFunction);

			onReadyFunction = persistent;
		}

		void OS::openWebBrowserURL(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String url = parseV8Value(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			CPP::OS::openWebBrowserURL(url);
		}

		void OS::exec(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			if (!args.Length() ||
				!args[0]->IsString())
			{
				V8_JAVASCRIPT_ENGINE->throwException("Executable parameter is required");
				return;
			}
			
			String command = parseV8Value(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			RJINT exitCode = CPP::OS::exec(command);

			args.GetReturnValue().Set(v8::Number::New(V8_JAVASCRIPT_ENGINE->isolate, exitCode));
		}

		void OS::getDocumentsPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String str = CPP::OS::getDocumentsPath ();

			args.GetReturnValue().Set(str.toV8String(args.GetIsolate()));
		}

		void OS::getTempPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String str = CPP::OS::getTempPath ();

			args.GetReturnValue().Set(str.toV8String(args.GetIsolate()));
		}

		void OS::getUserDataPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String str = CPP::OS::getUserDataPath ();

			args.GetReturnValue().Set(str.toV8String(args.GetIsolate()));
		}

		void OS::getApplicationPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String str = CPP::OS::getApplicationPath ();

			args.GetReturnValue().Set(str.toV8String(args.GetIsolate()));
		}

		void OS::getCurrentWorkingPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String str = CPP::OS::getCurrentWorkingPath ();

			args.GetReturnValue().Set(str.toV8String(args.GetIsolate()));
		}

		void OS::setCurrentWorkingPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String str = parseV8Value(args[0]);

			CPP::OS::setCurrentWorkingPath(str);
		}

		void OS::saveFileAs(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String message = "";
			String defaultDir = "";
			String defaultFile = "";
			String wildcard = "";
			RJBOOL overwritePrompt = true;

			if (args.Length() > 0)
			{
				if (args[0]->IsString() == true)
					message = parseV8Value(args[0]);

				if (args[0]->IsObject() == true)
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);

					message = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "message");
					defaultDir = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "defaultDir");
					defaultFile = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "defaultFile");
					wildcard = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "wildcard");
					overwritePrompt = V8_JAVASCRIPT_ENGINE->v8GetBool(obj, "overwritePrompt");
				}
			}

			String path = CPP::OS::saveFileAs(message, defaultDir, defaultFile, wildcard, overwritePrompt);

			args.GetReturnValue().Set(path.toV8String(args.GetIsolate()));
		}

		void OS::openFileAs(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			String message = "";
			String defaultDir = "";
			String defaultFile = "";
			String wildcard = "";
			RJBOOL fileMustExist = true;

			if (args.Length() > 0)
			{
				if (args[0]->IsString() == true)
					message = parseV8Value(args[0]);

				if (args[0]->IsObject() == true)
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);

					message = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "message");
					defaultDir = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "defaultDir");
					defaultFile = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "defaultFile");
					wildcard = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "wildcard");
					fileMustExist = V8_JAVASCRIPT_ENGINE->v8GetBool(obj, "fileMustExist");
				}
			}

			String path = CPP::OS::openFileAs(message, defaultDir, defaultFile, wildcard, fileMustExist);

			args.GetReturnValue().Set(path.toV8String(args.GetIsolate()));
		}

		void OS::SystemProcess::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "_init", OS::SystemProcess::_init);
			V8_CALLBACK(object, "execute", OS::SystemProcess::execute);
			V8_CALLBACK(object, "kill", OS::SystemProcess::kill);
			V8_CALLBACK(object, "on", OS::SystemProcess::on);
		}

		void OS::SystemProcess::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			std::shared_ptr<CPP::OS::SystemProcess> appObject(RJNEW CPP::OS::SystemProcess(V8_JAVASCRIPT_ENGINE, args),
															  [](CPP::OS::SystemProcess* p) {
																  DELETEOBJ(p);
															  });
			
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
		}

		void OS::SystemProcess::execute(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			std::shared_ptr<CPP::OS::SystemProcess> appObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<CPP::OS::SystemProcess>(args.This(), "_appObj");

			if (!appObject)
			{
				V8_JAVASCRIPT_ENGINE->throwException("SystemProcess not initialized");
				return;
			}

			appObject->execute();
		}

		void OS::SystemProcess::kill(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			std::shared_ptr<CPP::OS::SystemProcess> appObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<CPP::OS::SystemProcess>(args.This(), "_appObj");

			if (!appObject)
			{
				V8_JAVASCRIPT_ENGINE->throwException("SystemProcess not initialized");
				return;
			}

			appObject->kill();
		}
		
		void OS::SystemProcess::on(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			std::shared_ptr<CPP::OS::SystemProcess> appObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<CPP::OS::SystemProcess>(args.This(), "_appObj");
			
			if (!appObject)
			{
				V8_JAVASCRIPT_ENGINE->throwException("SystemProcess not initialized");
				return;
			}
			
			if( args.Length() < 2 ||
			   !args[0]->IsString() ||
			   !args[1]->IsFunction())
			{
				V8_JAVASCRIPT_ENGINE->throwException("Event name and function is required");
				return;
			}
			
			appObject->on(parseV8Value(args[0]), v8::Local<v8::Function>::Cast(args[1]));
		}
	}
}
