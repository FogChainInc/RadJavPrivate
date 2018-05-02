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
#ifndef _RADJAV_V8_JAVASCRIPT_ENGINE_H_
	#define _RADJAV_V8_JAVASCRIPT_ENGINE_H_

	#include "RadJavPreprocessor.h"

#ifdef USE_V8
	#include <v8.h>
	#include <v8-inspector.h>

	#include <libplatform/libplatform.h>

	#include <atomic>
	#include <mutex>

	#include "RadJavJavascriptEngine.h"
	#include "cpp/RadJavCPPNetWebSocketServer.h"

	// The USE(x, ...) template is used to silence C++ compiler warnings
	// issued for (yet) unused variables (typically parameters).
	// The arguments are guaranteed to be evaluated from left to right.
	struct Use {
		template <typename T>
		Use(T&&) {}  // NOLINT(runtime/explicit)
	};
	#define USE(...)                                         \
	  do {                                                   \
		::Use unused_tmp_array_for_use_macro[]{__VA_ARGS__}; \
		(void)unused_tmp_array_for_use_macro;                \
	  } while (false)

	#define V8_CALLBACK(context, functionName, function) context->Set(String(functionName).toV8String(isolate), \
												v8::Function::New(isolate, function, String(functionName).toV8String(isolate)));
	#define V8_JAVASCRIPT_ENGINE static_cast <V8JavascriptEngine *> (RadJav::javascriptEngine)
	#define V8_RADJAV V8_JAVASCRIPT_ENGINE->radJav->Get (V8_JAVASCRIPT_ENGINE->isolate)

	enum {
		kModuleEmbedderDataIndex,
		kInspectorClientIndex
	};

	namespace RadJAV
	{
		class V8JavascriptEngine;

		/// Create an asynchronous function call.
		class RADJAV_EXPORT AsyncFunctionCall
		{
			public:
				AsyncFunctionCall(v8::Persistent<v8::Function> *newfunc, 
					v8::Persistent<v8::Array> *newargs = NULL, RJBOOL newDeleteOnComplete = true);
				~AsyncFunctionCall();

				/// Check to see if an asynchronous function call has a result. Be sure to set deleteOnComplete = true;
				inline RJBOOL checkForResult()
				{
					if (result != NULL)
						return (true);

					return (false);
				}

				/// Get the result from the async function call. Be sure to set deleteOnComplete = true;
				v8::Local<v8::Value> getResult(V8JavascriptEngine *engine);

				v8::Persistent<v8::Function> *func;
				v8::Persistent<v8::Array> *args;
				RJBOOL deleteOnComplete;

				v8::Persistent<v8::Value> *result;
		};

		/// The array buffer allocator used for V8.
		/*class RADJAV_EXPORT V8ArrayBufferAllocator: public v8::ArrayBuffer::Allocator
		{
			public:
				void *Allocate(size_t length);
				void *AllocateUninitialized(size_t length);
				void Free(void *data, size_t length);
		};*/

		/// V8 Channel for use in V8 Inspector.
		class RADJAV_EXPORT V8JSChannel final : public v8_inspector::V8Inspector::Channel {
		public:
			explicit V8JSChannel(v8::Local<v8::Context> context);
			virtual ~V8JSChannel() = default;

		private:
			void sendResponse(
				int callId,
				std::unique_ptr<v8_inspector::StringBuffer> message) override {
				Send(message->string());
			}
			void sendNotification(
				std::unique_ptr<v8_inspector::StringBuffer> message) override {
				Send(message->string());
			}
			void flushProtocolNotifications() override {}

			void Send(const v8_inspector::StringView& string) {
				v8::Isolate::AllowJavascriptExecutionScope allow_script(isolate_);
				int length = static_cast<int>(string.length());
				//DCHECK(length < v8::String::kMaxLength);
				v8::Local<v8::String> message =
					(string.is8Bit()
						? v8::String::NewFromOneByte(
							isolate_,
							reinterpret_cast<const uint8_t*>(string.characters8()),
							v8::NewStringType::kNormal, length)
						: v8::String::NewFromTwoByte(
							isolate_,
							reinterpret_cast<const uint16_t*>(string.characters16()),
							v8::NewStringType::kNormal, length))
					.ToLocalChecked();
				v8::Local<v8::String> callback_name =
					v8::String::NewFromUtf8(isolate_, "receive", v8::NewStringType::kNormal)
					.ToLocalChecked();
				v8::Local<v8::Context> context = context_.Get(isolate_);
				v8::Local<v8::Value> callback =
					context->Global()->Get(context, callback_name).ToLocalChecked();
				if (callback->IsFunction()) {
					v8::TryCatch try_catch(isolate_);
					v8::Local<v8::Value> args[] = { message };
					USE(v8::Local<v8::Function>::Cast(callback)->Call(context, Undefined(isolate_), 1,
						args));

#ifdef DEBUG
					if (try_catch.HasCaught()) {
						v8::Local<v8::Object> exception = v8::Local<v8::Object>::Cast(try_catch.Exception());
						v8::Local<String> key = v8::String::NewFromUtf8(isolate_, "message",
							v8::NewStringType::kNormal)
							.ToLocalChecked();
						v8::Local<String> expected =
							v8::String::NewFromUtf8(isolate_,
								"Maximum call stack size exceeded",
								v8::NewStringType::kNormal)
							.ToLocalChecked();
						v8::Local<Value> value = exception->Get(context, key).ToLocalChecked();
						//DCHECK(value->StrictEquals(expected));
					}
#endif

				}
			}

			v8::Isolate* isolate_;
			v8::Global<v8::Context> context_;
		};

		/// V8 Inspector for debugging javascript applications.
		class RADJAV_EXPORT V8JSInspectorClient: public v8_inspector::V8InspectorClient
		{
			public:
				V8JSInspectorClient(v8::Local<v8::Context> context, bool connect);
				~V8JSInspectorClient()
				{
					//if ( (null != server_) && (server_->isAlive) )
					//{
					//	server_->close();
					//}

					//DELETEOBJ(server_);
				};

		private:
			static v8_inspector::V8InspectorSession* GetSession(v8::Local<v8::Context> context) {
				V8JSInspectorClient* inspector_client = static_cast<V8JSInspectorClient*>(
					context->GetAlignedPointerFromEmbedderData(kInspectorClientIndex));
				return inspector_client->session_.get();
			}

			v8::Local<v8::Context> ensureDefaultContextInGroup(int group_id) override {
				//DCHECK(isolate_);
				//DCHECK_EQ(kContextGroupId, group_id);
				return context_.Get(isolate_);
			}

			static void SendInspectorMessage(
				const v8::FunctionCallbackInfo<v8::Value>& args) {
				v8::Isolate* isolate = args.GetIsolate();
				v8::HandleScope handle_scope(isolate);
				v8::Local<v8::Context> context = isolate->GetCurrentContext();
				args.GetReturnValue().Set(Undefined(isolate));
				v8::Local<v8::String> message = args[0]->ToString(context).ToLocalChecked();
				v8_inspector::V8InspectorSession* session =
					V8JSInspectorClient::GetSession(context);
				int length = message->Length();
				std::unique_ptr<uint16_t[]> buffer(new uint16_t[length]);
				message->Write(buffer.get(), 0, length);
				v8_inspector::StringView message_view(buffer.get(), length);
				session->dispatchProtocolMessage(message_view);
				args.GetReturnValue().Set(True(isolate));
			}

			static const int kContextGroupId = 1;

			std::unique_ptr<v8_inspector::V8Inspector> inspector_;
			std::unique_ptr<v8_inspector::V8InspectorSession> session_;
			std::unique_ptr<v8_inspector::V8Inspector::Channel> channel_;
			RadJAV::CPP::Net::WebSocketServer* server_;
			v8::Global<v8::Context> context_;
			v8::Isolate* isolate_;
		};

		/// The V8 javascript engine.
		class RADJAV_EXPORT V8JavascriptEngine: public JavascriptEngine
		{
			public:
				V8JavascriptEngine();
				~V8JavascriptEngine();

				/// Start the inspector.
				void startInspector(v8::Local<v8::Context> context);

				/// Run an application.
				void runApplication(String applicationSource, String fileName);
				/// Run an application from a javascript file.
				void runApplicationFromFile(String file);
				/// Execute Javascript code.
				void executeScript(String code, String fileName);
				/// Execute Javascript code.
				void executeScript(String code, String fileName, v8::Local<v8::Object> context);
				/// Execute unbounded Javascript code.
				void unboundedExecuteScript(String code, String fileName, v8::Local<v8::Object> context = v8::Local<v8::Object>());
				/// Execute javascript on the next tick.
				void executeScriptNextTick(String code, String fileName, v8::Local<v8::Object> context = v8::Local<v8::Object>());
				/// Call a function on the next tick. Any args passed MUST be an array.
				void callFunctionOnNextTick(AsyncFunctionCall *call);
				/// Connect the native library to the Javascript library.
				void loadNativeCode();

				/// Destroy the JavaScript objects.
				void destroyJSObjects();

				/// Collect the garbage. This will only work if the engine is started with exposeGC = true.
				void collectGarbage();

				#ifdef C3D_USE_OGRE
					/// Start the 3d engine.
					void start3DEngine();
				#endif

				/// Add a timeout, process it.
				void addTimeout(v8::Persistent<v8::Function> *func, RJINT time);

				/// A blockchain event has occurred, process it.
				void blockchainEvent(String event, String dataType = "null", void *data = NULL);
				void blockchainEvent(String event, RJINT numargs, v8::Local<v8::Value> *args, RJBOOL alreadyEnteredCritialSection = false);

				/// Add a thread to be handled by the engine.
				void addThread(Thread *thread);
				/// Remove a thread.
				void removeThread(Thread *thread);
				/// Throw a Javascript exception.
				void throwException(String message);

				/// Shutdown the application entirely.
				void exit(RJINT exitCode);

				template<typename P>
				static void weakCallback(const v8::WeakCallbackInfo<P> &data);

				/// Get a V8 function.
				v8::Handle<v8::Function> v8GetFunction(v8::Local<v8::Object> context, String functionName);
				/// Get a V8 value.
				v8::Handle<v8::Value> v8GetValue(v8::Local<v8::Object> context, String functionName);
				/// Set a string.
				void v8SetString(v8::Local<v8::Object> context, String functionName, String str);
				/// Get a string from a V8 variable name. If the value is null or has an empty handle, an empty string will return.
				String v8GetString(v8::Local<v8::Object> context, String functionName);
				/// Set a number.
				void v8SetNumber(v8::Local<v8::Object> context, String functionName, RDECIMAL number);
				/// Get a V8 int. If the value is null or has an empty handle, 0 will be returned.
				RJINT v8GetInt(v8::Local<v8::Object> context, String functionName);
				/// Get a V8 decimal. If the value is null or has an empty handle, 0 will be returned.
				RDECIMAL v8GetDecimal(v8::Local<v8::Object> context, String functionName);
				/// Set a bool from a V8 boolean value.
				void v8SetBool(v8::Local<v8::Object> context, String functionName, bool value);
				/// Get a V8 bool. If the value is null or has an empty handle, false will be returned.
				RJBOOL v8GetBool(v8::Local<v8::Object> context, String functionName);
				/// Set a V8 object.
				void v8SetObject(v8::Local<v8::Object> context, String functionName, v8::Handle<v8::Object> obj);
				/// Get a V8 object.
				v8::Handle<v8::Object> v8GetObject(v8::Local<v8::Object> context, String functionName);
				/// Call a V8 function.
				v8::Local<v8::Value> v8CallFunction(
					v8::Local<v8::Object> context, String functionName, RJINT numArgs, v8::Local<v8::Value> *args);
				/// Call a V8 function as a constructor.
				v8::Local<v8::Object> v8CallAsConstructor(v8::Local<v8::Object> function, RJINT numArgs, v8::Local<v8::Value> *args);
				/// Get a V8 native object.
				void *v8GetExternal(v8::Local<v8::Object> context, String functionName);
				/// Set a V8 native object.
				void v8SetExternal(v8::Local<v8::Object> context, String functionName, void *obj);
				/// Set internal field.
				template<typename P>
				void v8SetInternalField(v8::Local<v8::Object> context, String functionName, P *obj)
				{
					/*v8::Local<v8::Object> objInst = internalObjectTemplate->NewInstance(context->CreationContext()).ToLocalChecked();

					v8::Local<v8::External> val = v8::External::New(isolate, obj);
					objInst->SetInternalField(0, val);
					context->Set(functionName.toV8String(isolate), objInst);
					v8::Persistent<v8::Object> *pval = RJNEW v8::Persistent<v8::Object>();
					pval->Reset(context->GetIsolate(), objInst);
					pval->SetWeak<P>(obj, V8JavascriptEngine::weakCallback<P>, v8::WeakCallbackType::kParameter);
					pval->MarkIndependent();*/
				}
				/// Get an internal field.
				void *v8GetInternalField(v8::Local<v8::Object> context, String functionName);
				/// Get a V8 argument.
				v8::Handle<v8::Value> v8GetArgument(const v8::FunctionCallbackInfo<v8::Value> &args, RJUINT index);
				/// Set a V8 Value.
				void v8SetValue(v8::Local<v8::Object> context, String functionName, v8::Handle<v8::Value> obj);

				/// Checks if a V8 value is undefined, null, or if the handle is empty. Returns true if it is.
				bool v8IsNull(v8::Local<v8::Value> val);
				/// Get a bool value from a V8 value.
				RJBOOL v8ParseBool(v8::Local<v8::Value> val);
				/// Get an integer value from a V8 value.
				RJINT v8ParseInt(v8::Local<v8::Value> val);
				/// Get a decimal value from a V8 value.
				RDECIMAL v8ParseDecimal(v8::Local<v8::Value> val);

				// Create a promise.
				v8::Local<v8::Object> createPromise(v8::Local<v8::Function> function);
				// Create a promise.
				v8::Local<v8::Object> createPromise(
					v8::Local<v8::Object> context, v8::Local<v8::Function> function, v8::Local<v8::Array> args = v8::Local<v8::Array> ());

				/// Load the native templates into the javascript library.
				static void loadTemplates(const v8::FunctionCallbackInfo<v8::Value> &args);
				/// Run the event loop.
				static void runEventLoopSingleStep(const v8::FunctionCallbackInfo<v8::Value> &args);
				/// Run a script.
				static void runScript(const v8::FunctionCallbackInfo<v8::Value> &args);
				/// Destroy an object from v8.
				//static void destroyObject(const v8::WeakCallbackData<v8::Value, JSValue> &data);

				/// The persistent data being held.
				Array<v8::Persistent<v8::Value, v8::CopyablePersistentTraits<v8::Value> > *> persistentData;
				/// The V8 isolate.
				v8::Isolate *isolate;

				v8::Platform *platform;
				v8::Local<v8::Context> globalContext;
				v8::Local<v8::ObjectTemplate> internalObjectTemplate;
				v8::Persistent<v8::Object> *radJav;

				#ifdef GUI_USE_WXWIDGETS
					wxCriticalSection *criticalSection;
				#endif

			protected:
				Array<String> jsToExecuteNextCode;
				Array<String> jsToExecuteNextFilename;
				Array< v8::Local<v8::Object> > jsToExecuteNextContext;

				Array<AsyncFunctionCall *> funcs;

				Array<v8::Persistent<v8::Function> *> timeoutFuncs;
				Array<RJINT> timeouts;

				RJBOOL useInspector;
				//not used
				std::unique_ptr<V8JSInspectorClient> inspector;
		};
	}
#endif
#endif
