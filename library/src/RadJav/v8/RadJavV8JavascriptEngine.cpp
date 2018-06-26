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
#include "v8/RadJavV8JavascriptEngine.h"

#include "RadJav.h"
#include "RadJavException.h"

#include "RadJavJSFile.h"
#include "RadJavJavascriptCode.h"

#ifdef GUI_USE_WXWIDGETS
	#include <wx/stdpaths.h>
	#include <wx/filename.h>
#endif

#ifdef USE_V8
	#include "v8/RadJavV8Global.h"
	#include "v8/RadJavV8OS.h"

	#include "v8/RadJavV8Console.h"
	#include "v8/RadJavV8IO.h"
	#include "v8/RadJavV8Net.h"
	#include "v8/RadJavV8Thread.h"

	#include "v8/RadJavV8BlockchainV1.h"

	// Database
	#include "v8/RadJavV8DBKeyValueStorage.h"

	// Net
	#include "v8/RadJavV8NetWebSocket.h"
	#include "v8/RadJavV8NetWebServer.h"

	// GUI
	#include "v8/RadJavV8GUIGObject.h"
	#include "v8/RadJavV8GUIWindow.h"
	#include "v8/RadJavV8GUIButton.h"
	#include "v8/RadJavV8GUILabel.h"
	#include "v8/RadJavV8GUIImage.h"
	#include "v8/RadJavV8GUIContainer.h"
	#include "v8/RadJavV8GUICombobox.h"
	#include "v8/RadJavV8GUITextbox.h"
	#include "v8/RadJavV8GUITextarea.h"
	#include "v8/RadJavV8GUICheckbox.h"
	#include "v8/RadJavV8GUIRadio.h"
	#include "v8/RadJavV8GUIList.h"
	#include "v8/RadJavV8GUIMenuBar.h"
	#include "v8/RadJavV8GUIMenuItem.h"
	#include "v8/RadJavV8GUIWebView.h"
	#include "v8/RadJavV8GUICanvas3D.h"

	// C3D
	#include "v8/RadJavV8C3DObject3D.h"
	#include "v8/RadJavV8C3DEntity.h"
	#include "v8/RadJavV8C3DWorld.h"

	// Crypto
	#include "v8/RadJavV8CryptoCipher.h"
	#include "v8/RadJavV8CryptoDecipher.h"
	#include "v8/RadJavV8CryptoCipherMultipart.h"
	#include "v8/RadJavV8CryptoDecipherMultipart.h"
	#include "v8/RadJavV8CryptoKeyGenerator.h"
	#include "v8/RadJavV8CryptoPrivateKey.h"
	#include "v8/RadJavV8CryptoPublicKey.h"
	#include "v8/RadJavV8CryptoHash.h"
	#include "v8/RadJavV8CryptoHashMultipart.h"

#endif

#include "cpp/RadJavCPPIO.h"
#include "cpp/RadJavCPPChainedPtr.h"

#include <cstring>

namespace RadJAV
{
	#ifdef USE_V8
		AsyncFunctionCall::AsyncFunctionCall (v8::Persistent<v8::Function> *newfunc, 
			v8::Persistent<v8::Array> *newargs, RJBOOL newDeleteOnComplete)
		{
			func = newfunc;
			args = newargs;
			deleteOnComplete = newDeleteOnComplete;
			result = NULL;
		}

		AsyncFunctionCall::~AsyncFunctionCall()
		{
			DELETEOBJ(func);
			DELETEOBJ(args);
			DELETEOBJ(result);
		}

		v8::Local<v8::Value> AsyncFunctionCall::getResult(V8JavascriptEngine *engine)
		{
			v8::Local<v8::Value> asyncResult = result->Get(engine->isolate);

			return (asyncResult);
		}

		/*void *V8ArrayBufferAllocator::Allocate(size_t length)
		{
			void *data = AllocateUninitialized(length);

			if (data != NULL)
				data = std::memset(data, 0, length);

			return (data);
		}

		void *V8ArrayBufferAllocator::AllocateUninitialized(size_t length)
		{
			return (malloc(length));
		}

		void V8ArrayBufferAllocator::Free(void *data, size_t length)
		{
			free(data);
		}*/

		V8JSInspectorClient::V8JSInspectorClient(v8::Local<v8::Context> context, bool connect)
		{
			if (!connect) return;

			//server_ = RJNEW RadJAV::CPP::Net::WebSocketServer();
			//server_->listen();

			isolate_ = context->GetIsolate();
			channel_.reset(new V8JSChannel(context));
			inspector_ = v8_inspector::V8Inspector::create(isolate_, this);
			session_ =
				inspector_->connect(1, channel_.get(), v8_inspector::StringView());
			context->SetAlignedPointerInEmbedderData(kInspectorClientIndex, this);
			inspector_->contextCreated(v8_inspector::V8ContextInfo(
				context, kContextGroupId, v8_inspector::StringView()));

			v8::Local<v8::Value> function =
				v8::FunctionTemplate::New(isolate_, SendInspectorMessage)
				->GetFunction(context)
				.ToLocalChecked();
			v8::Local<v8::String> function_name =
				v8::String::NewFromUtf8(isolate_, "send", v8::NewStringType::kNormal)
				.ToLocalChecked();
			//CHECK(
			context->Global()->Set(context, function_name, function).FromJust();
			//);

			//enabled by default, not exported to v8 lib
			//v8::debug::SetLiveEditEnabled(isolate_, true);

			context_.Reset(isolate_, context);
		}

		/**
		 * Class to wrap native C++ objects with V8 one.
		 */
		class ExternalFieldWrapper
		{
		public:
			/**
			 * A constructor.
			 * @param handle is a context to which we will add new V8 object.
			 * @param objectId is a unique ID of external object.
			 * @param data a pointer to ChainedPtr derived class which will be
			 *  exposed to V8
			 */
			ExternalFieldWrapper (const v8::Local<v8::Object> &handle, uint32_t objectId, CPP::ChainedPtr *data)
			{
				objectUniqueId = objectId;
				
				// Hook on raw pointer to be notified when it is destroyed
				objectHook = RJNEW CPP::ChainedPtrHook(data, [&]
				{
					objectDestroyed();
				});
				
				// Create V8 objects to wrap raw pointer
				wrap(handle);
			}
			
			/**
			 * A destructor.
			 */
			virtual ~ExternalFieldWrapper ()
			{
				// Notify user when wrapper goes out of scope
				if(aboutDelete)
					aboutDelete(this);

				// Remove hooked raw pointer if not removed
				// by GC callback or CPP side
				delete objectHook;
				
				if (!persistent.IsEmpty())
				{
					// Clear GC callback
					persistent.ClearWeak();

					// Clear V8 Persistent object
					persistent.Reset();
				}
			}
			
			/**
			 * Get newly created V8 object for embedding into JavaScript.
			 * @return v8::Local<v8::Object> newly created V8 object.
			 */
			v8::Local<v8::Object> objectTemplateInstance () const
			{
				return objectInstance;
			}
			
			/**
			 * Get unique Id of external object.
			 * @return uint64_t external object ID.
			 */
			uint64_t objectId() const
			{
				return objectUniqueId;
			}
			
			/**
			 * Get underlying external object.
			 * @return CPP::ChainedPtr a pointer to external object.
			 */
			CPP::ChainedPtr* objectPtr()
			{
				if(objectHook)
				{
					return objectHook->object();
				}
				
				return nullptr;
			}
			/**
			 * Action to execute when wrapper is going out of scope.
			 * Can be used to remove wrapper from global list of
			 * wrapped objects.
			 * @param aboutToDelete a pointer to callback function.
			 * @return void.
			 */
			void onDelete(std::function<void (ExternalFieldWrapper*)> aboutToDelete)
			{
				aboutDelete = aboutToDelete;
			}
			
			ExternalFieldWrapper () = delete;
			ExternalFieldWrapper (const ExternalFieldWrapper& other) = delete;
			
		protected:
			/**
			 * Create V8 object to be exposed to JavaScript.
			 * @param handle is a context to which we will add new V8 object.
			 * @return void.
			 */
			virtual void wrap (const v8::Local<v8::Object> &handle)
			{
				//Variant 1 using External
				objectTemplate = v8::ObjectTemplate::New(handle->GetIsolate());
				objectTemplate->SetInternalFieldCount(1);
				
				objectInstance = objectTemplate->NewInstance (handle->CreationContext()).ToLocalChecked();
				
				v8::Local<v8::Integer> val = v8::Uint32::NewFromUnsigned(handle->GetIsolate(), objectUniqueId);
				objectInstance->SetInternalField(0, val);
				
				// Associate Persistent with object
				persistent.Reset(handle->GetIsolate(), objectInstance);
				
				// Mark Persistent as weak to receive callback from garbage collector
				persistent.SetWeak(this, callback, v8::WeakCallbackType::kParameter);
				persistent.MarkIndependent();
			}
			
			/**
			 * Notification on raw pointer destruction.
			 * @return void.
			 */
			void objectDestroyed()
			{
				objectHook = nullptr;
				
				if (!persistent.IsEmpty())
				{
					persistent.ClearWeak();
					persistent.Reset();
				}
			}
			
		protected:
			/**
			 * A static weak callback function which will be called by V8 garbage collector.
			 * @param data reference to ExternalFieldWrapper class which hold the
			 *  native raw pointer.
			 * @return void.
			 */
			/// Callback called by garbage collector to free memory and reset Persistent
			static void callback (const v8::WeakCallbackInfo<ExternalFieldWrapper> &data)
			{
				ExternalFieldWrapper *wrapper = data.GetParameter();
				DELETEOBJ(wrapper);
			}
			
		protected:
			v8::Persistent<v8::Object> persistent;
			v8::Local<v8::ObjectTemplate> objectTemplate;
			
			CPP::ChainedPtrHook* objectHook;
			uint32_t objectUniqueId;
			v8::Local<v8::Object> objectInstance;
			std::function<void (ExternalFieldWrapper*)> aboutDelete;
		};

		class ExternalsManager
		{
		public:
			ExternalsManager() {}
			~ExternalsManager()
			{
				while (!externals.empty())
				{
					// Remove objects which were not cleared by V8 garbage collector
					auto item = --externals.end();
					ExternalFieldWrapper* wrapper = item->second;
					externals.erase(item);
					DELETEOBJ(wrapper);
				}
			}
			
			ExternalsManager(const ExternalsManager& ) = delete;
			ExternalsManager& operator = (const ExternalsManager&) = delete;
			
			void set(const v8::Local<v8::Object>& handle, const String& functionName, CPP::ChainedPtr* object)
			{
				uint32_t objectId = nextId();
				ExternalFieldWrapper* wrapper = RJNEW ExternalFieldWrapper(handle, objectId, object);
				wrapper->onDelete( [&](ExternalFieldWrapper* wrapper)
								  {
									  auto pos = externals.find( wrapper->objectId());
									  if(pos != externals.end())
									  {
										  // Remove wrapped object from the list of external objects
										  externals.erase(pos);
									  }
								  });

				// Put wrapped object into the list of external objects
				externals[objectId] = wrapper;
				
				// Assotiate JavaScript variable with wrapped external object
				handle->Set( functionName.toV8String( handle->GetIsolate()), wrapper->objectTemplateInstance());
			}
			
			CPP::ChainedPtr* get(const v8::Local<v8::Object>& handle, const String& functionName)
			{
				v8::Isolate* isolate = handle->GetIsolate();
				v8::Handle<v8::Value> value = handle->Get(functionName.toV8String(isolate));
				
				if (V8_JAVASCRIPT_ENGINE->v8IsNull(value) == true)
					return nullptr;
				
				v8::Handle<v8::Object> object = value->ToObject(isolate);
				
				v8::Local<v8::Value> objectIdValue = object->GetInternalField(0);
				
				v8::Handle<v8::Uint32> external = v8::Handle<v8::Uint32>::Cast(objectIdValue);
				
				uint32_t objectId = external->Value();
				
				auto pos = externals.find( objectId);
				
				if(pos == externals.end())
					return nullptr;
				
				return pos->second->objectPtr();
			}
			
			void clear(const v8::Local<v8::Object>& handle, const String& functionName)
			{
				v8::Isolate* isolate = handle->GetIsolate();
				v8::Handle<v8::Value> value = handle->Get(functionName.toV8String(isolate));
				
				if (V8_JAVASCRIPT_ENGINE->v8IsNull(value) == true)
					return;
				
				v8::Handle<v8::Object> object = value->ToObject(isolate);
				
				v8::Local<v8::Integer> val = v8::Uint32::New (isolate, 0);
				object->SetInternalField(0, val);
			}
			
		private:
			uint32_t nextId()
			{
				static uint32_t objectId = 0;
				
				return ++objectId;
			}
			
		private:
			std::map<uint32_t, ExternalFieldWrapper*> externals;
		};
	
		V8JavascriptEngine::V8JavascriptEngine()
			: JavascriptEngine(),
			  arrayBufferAllocator(nullptr)
		{
			externalsManager = RJNEW ExternalsManager();
			
			String execPath = "";

			#ifdef GUI_USE_WXWIDGETS
				execPath = parsewxString (wxStandardPaths::Get ().GetExecutablePath());
			#endif

			v8::V8::InitializeICUDefaultLocation(execPath.c_str ());
			String flags = "";
			exposeGC = false;
			useInspector = false;
			radJav = NULL;
			exceptionsDisplayMessageBox = false;
			shutdownOnException = false;

			if (RadJav::arguments.size() > 0)
			{
				for (size_t iIdx = 0; iIdx < RadJav::arguments.size(); iIdx++)
				{
					String endSpace = " ";

					if (iIdx == (RadJav::arguments.size() - 1))
						endSpace = "";

					String arg = RadJav::arguments.at(iIdx);

					if (arg == "--expose_gc")
						exposeGC = true;

					if (arg == "--inspect")
					{
						useInspector = true;

						//consume non-v8 flag
						continue;
					}
						
					flags += arg + endSpace;
				}
			}

			#ifdef GUI_USE_WXWIDGETS
				execPath = parsewxString (wxStandardPaths::Get ().GetExecutablePath());
			#endif

			v8::V8::SetFlagsFromString(flags.c_str(), flags.size());
			v8::V8::InitializeExternalStartupData(execPath.c_str ());
			platform = v8::platform::CreateDefaultPlatform();
			v8::V8::InitializePlatform(platform);
			v8::V8::Initialize();

			v8::Isolate::CreateParams createParams;
			//V8ArrayBufferAllocator allocator;
			//createParams.array_buffer_allocator = &allocator;
			
			// ArrayBuffer allocator needs to be freed by us
			arrayBufferAllocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator ();
			createParams.array_buffer_allocator = arrayBufferAllocator;

			isolate = v8::Isolate::New(createParams);

			#ifdef GUI_USE_WXWIDGETS
				criticalSection = RJNEW wxCriticalSection ();
			#endif
		}

		V8JavascriptEngine::~V8JavascriptEngine()
		{
			DELETEOBJ(externalsManager);

			DELETEOBJ(radJav);
			destroyJSObjects();

			//v8::Unlocker unlocker(isolate);
			isolate->Dispose();
			v8::V8::Dispose();
			v8::V8::ShutdownPlatform();

			#ifdef GUI_USE_WXWIDGETS
				DELETEOBJ(criticalSection);
			#endif

			DELETEOBJ(platform);

			delete arrayBufferAllocator;
			arrayBufferAllocator = nullptr;
		}

		void V8JavascriptEngine::startInspector(v8::Local<v8::Context> context)
		{
			// create a v8 inspector client
			v8::Context::Scope cscope(context);
			inspector.reset(new V8JSInspectorClient(context, true));
			//inspector.
			//V8JSInspector inspector_client(context, true);

		}

		void V8JavascriptEngine::runApplication(String applicationSource, String fileName)
		{
			String parentDir = fileName;

			#ifdef GUI_USE_WXWIDGETS
				wxFileName file(parentDir.towxString());
				file.MakeAbsolute();
				wxString tempStr = file.GetPath();
				parentDir = parsewxString(tempStr);
				wxSetWorkingDirectory(parentDir);
			#endif

			//v8::Locker locker(isolate);
			v8::Isolate::Scope scope(isolate);
			v8::HandleScope handleScope(isolate);

			v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
			globalContext = v8::Context::New(isolate, NULL, global);
			v8::Context::Scope contextScope(globalContext);

			loadJavascriptLibrary();

			// Insert the javascript libraries to be used.
			for (RJUINT iIdx = 0; iIdx < javascriptFiles.size(); iIdx++)
			{
				JSFile jsfile = javascriptFiles.at(iIdx);
				String contentStr = jsfile.getContent();

				executeScript(contentStr, jsfile.filename);
			}

			loadNativeCode();

			v8::Local<v8::Object> obj = v8GetObject(globalContext->Global(), "RadJav");
			radJav = RJNEW v8::Persistent<v8::Object>();
			radJav->Reset(isolate, obj);

			//if (useInspector) {
			//	startInspector(globalContext);
			//}
			V8JSInspectorClient* client = RJNEW V8JSInspectorClient(globalContext, useInspector);

			try
			{
				// Check the application source to see if its an XRJ file.
				if (applicationSource != "")
				{
					RJBOOL executeContent = true;

					if (fileName.find(".xrj") != String::npos)
					{
						Array<String> filesToExecute;

						// If the file begins with a {, assume its JSON and parse it.
						if (applicationSource.at(0) == '{')
						{
							executeContent = false;
							v8::MaybeLocal<v8::Value> parsedObj;

							try
							{
								parsedObj = v8::JSON::Parse(isolate, applicationSource.toV8String(isolate));
							}
							catch (Exception ex)
							{
								RadJav::showError(ex.getMessage(), true);
							}

							v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(parsedObj.ToLocalChecked());

							String name = parseV8Value(obj->Get(String("name").toV8String(isolate)));
							String developer = parseV8Value(obj->Get(String("developer").toV8String(isolate)));
							String license = parseV8Value(obj->Get(String("license").toV8String(isolate)));
							v8::Local<v8::Array> executeFiles = v8::Local<v8::Array>::Cast(obj->Get(String("execute_files").toV8String(isolate)));
							v8::Local<v8::Array> dependencies = v8::Local<v8::Array>::Cast(obj->Get(String("dependencies").toV8String(isolate)));
							RJUINT length = executeFiles->Length();

							// Get the list of JavaScript files to execute.
							for (RJUINT iIdx = 0; iIdx < length; iIdx++)
							{
								v8::Local<v8::Value> v8file = v8::Local<v8::Value>::Cast(executeFiles->Get(iIdx));

								if (v8file->IsString() == true)
								{
									String filePath = parseV8Value(v8::Local<v8::String>::Cast(v8file));
									filesToExecute.push_back(filePath);
								}

								if (v8file->IsObject() == true)
								{
									v8::Local<v8::Object> fileObj = v8::Local<v8::Object>::Cast(v8file);
									String javascriptFilePath = parseV8Value(fileObj->Get(String("javascript").toV8String(isolate)));
									filesToExecute.push_back(javascriptFilePath);
								}
							}

							// Once the list has been collected, execute each file.
							for (RJUINT iIdx = 0; iIdx < filesToExecute.size(); iIdx++)
							{
								String executeFile = filesToExecute.at(iIdx);
								String fileContents = CPP::IO::TextFile::getFileContents(executeFile);

								executeScript(fileContents, executeFile);
							}
						}
					}

					if (executeContent == true)
						executeScript(applicationSource, fileName);
				}
			}
			catch (Exception ex)
			{
				RadJav::printToOutputWindow(ex.getMessage ());

				if (exceptionsDisplayMessageBox == true)
					RadJav::showMessageBox(ex.getMessage(), "Error");

				return;
			}

			RJBOOL firstRun = true;
			RJBOOL startedBlockchainV1 = false;

			#ifdef GUI_USE_WXWIDGETS
				wxLongLong currentTime = 0;
				wxLongLong prevTime = 0;
				RJLONG diffTime = 0;
			#endif

			while (true)
			{
				auto execCodeBegin = jsToExecuteNextCode.begin();
				auto execFilenameBegin = jsToExecuteNextFilename.begin();
				auto execContextBegin = jsToExecuteNextContext.begin();
				auto execCodeEnd = jsToExecuteNextCode.end();

				try
				{
					#ifdef GUI_USE_WXWIDGETS
						currentTime = wxGetLocalTimeMillis();
						diffTime = (currentTime - prevTime).GetValue();
						prevTime = currentTime;
					#endif

					v8::platform::PumpMessageLoop(platform, isolate);
					RadJav::runEventLoopSingleStep();

					#ifdef C3D_USE_OGRE
						if (mRoot != NULL)
						{
							if (mRoot->isInitialised() == true)
								mRoot->renderOneFrame();
						}
					#endif

					// Handle the on ready function.
					if (firstRun == true)
					{
						if (V8B::OS::onReadyFunction != NULL)
						{
							v8::Local<v8::Function> val = v8::Local<v8::Function>::Cast(V8B::OS::onReadyFunction->Get(isolate));

							if (v8IsNull(val) == false)
								val->Call(globalContext->Global(), 0, NULL);
						}

						firstRun = false;
					}

					#ifdef USE_BLOCKCHAIN_V1
					if (V8B::BlockchainV1::hasBlockchainStarted == true)
					{
						if (startedBlockchainV1 == false)
						{
							V8B::BlockchainV1::startBlockchain();
							startedBlockchainV1 = true;
						}
					}
					#endif

					auto timeoutBegin = timeoutFuncs.begin();
					auto timeoutsBegin = timeouts.begin();
					auto timeoutEnd = timeoutFuncs.end();

					while (timeoutBegin != timeoutEnd)
					{
						v8::Persistent<v8::Function> *funcp = *timeoutBegin;
						RJINT time = *timeoutsBegin;

						if (time <= 0)
						{
							timeoutFuncs.erase(timeoutBegin);
							timeouts.erase(timeoutsBegin);

							v8::Local<v8::Function> func = funcp->Get(isolate);

							if (func->IsNullOrUndefined() == false)
								func->Call(globalContext->Global(), 0, NULL);

							DELETEOBJ(funcp);

							timeoutBegin = timeoutFuncs.begin();
							timeoutsBegin = timeouts.begin();
							timeoutEnd = timeoutFuncs.end();

							continue;
						}

						#ifdef GUI_USE_WXWIDGETS
							(*timeoutsBegin) = time - (RJINT)diffTime;
						#endif

						timeoutBegin++;
						timeoutsBegin++;
					}

					for (RJUINT iIdx = 0; iIdx < removeThreads.size(); iIdx++)
					{
						auto tbegin = threads.find (removeThreads.at (iIdx));
						auto tend = threads.end();

						if (tbegin == tend)
							return;

						/// @bug tbegin->second should be deleted, or does wxWidgets delete it automatically?
                        /// @note wxWidgets delete wxThread automatically, we need to update removeThreads only
						//DELETEOBJ (tbegin->second);
						threads.erase(tbegin);
					}

					removeThreads.clear();

					// Handle any threads.
					auto tbegin = threads.begin ();
					auto tend = threads.end ();

					while (tbegin != tend)
					{
						Thread *thread = tbegin->second;

						#ifdef GUI_USE_WXWIDGETS
							wxThread *wthread = (wxThread *)thread;

							if (thread->hasThreadStarted () == false)
							{
								wthread->Create();
								wthread->Run();
								thread->setAsStarted(true);
							}
						#endif

						tbegin++;
					}

					// Handle any scripts that need to be executed.
					while (execCodeBegin != execCodeEnd)
					{
						String code = *execCodeBegin;
						executeScript(code, *execFilenameBegin, *execContextBegin);

						execCodeBegin++;
						execFilenameBegin++;
						execContextBegin++;
					}

					jsToExecuteNextCode.clear();
					jsToExecuteNextFilename.clear();
					jsToExecuteNextContext.clear();

					#ifdef GUI_USE_WXWIDGETS
					if (criticalSection->TryEnter() == true)
					{
					#endif

					// Handle any functions that need to be executed from a thread.
					auto funcBegin = funcs.begin();
					auto funcEnd = funcs.end();

					// Handle any functions that need to be executed.
					//if (funcBegin != funcEnd)
					while (funcBegin != funcEnd)
					{
						AsyncFunctionCall *asyncCall = *funcBegin;
						v8::Persistent<v8::Function> *funcp = asyncCall->func;
						v8::Persistent<v8::Array> *args = asyncCall->args;
						RJBOOL deleteOnComplete = asyncCall->deleteOnComplete;
						v8::Local<v8::Array> args2;
						RJINT numArgs = 0;

						if (args != NULL)
						{
							args2 = args->Get(isolate);
							numArgs = args2->Length();
						}

						v8::Local<v8::Value> *args3 = NULL;

						if (numArgs > 0)
							args3 = RJNEW v8::Local<v8::Value>[numArgs];

						for (RJINT iIdx = 0; iIdx < numArgs; iIdx++)
							args3[iIdx] = args2->Get(iIdx);

						v8::Local<v8::Function> func = funcp->Get(isolate);

						if (func->IsNullOrUndefined() == false)
						{
							v8::Local<v8::Value> result = func->Call(globalContext->Global(), numArgs, args3);
							v8::Persistent<v8::Value> *presult = RJNEW v8::Persistent<v8::Value>();

							presult->Reset(isolate, result);

							asyncCall->result = presult;
						}

						if (numArgs > 0)
							DELETEARRAY(args3);

						if (deleteOnComplete == true)
							DELETEOBJ(asyncCall);

						//funcNext.erase(funcBegin);
						//funcNextArgs.erase (funcArgsBegin);
						//funcDelete.erase(funcDeleteBegin);
						funcBegin++;
					}

					funcs.clear();

					#ifdef GUI_USE_WXWIDGETS
						criticalSection->Leave();
						}
					#endif

					if (shutdown == true)
						break;
				}
				catch (Exception ex)
				{
					RadJav::printToOutputWindow(ex.getMessage ());

					if (exceptionsDisplayMessageBox == true)
						RadJav::showMessageBox(ex.getMessage(), "Error");

					jsToExecuteNextCode.erase(execCodeBegin);
					jsToExecuteNextFilename.erase(execFilenameBegin);
					jsToExecuteNextContext.erase(execContextBegin);

					if (shutdownOnException == true)
						break;
				}
			}
			DELETEOBJ( client );
			isolate->ContextDisposedNotification();
			isolate->LowMemoryNotification();
		}

		void V8JavascriptEngine::runApplicationFromFile(String file)
		{
			std::fstream appFile;

			if (file == "")
				throw RadJAV::Exception("Application file name is empty!");

			appFile.open(file, std::ios_base::in);

			if (appFile.is_open() == false)
				throw RadJAV::Exception("Unable to open file: " + file);

			String content = "";

			while (appFile.good() == true)
			{
				char cChar = appFile.get();

				if (appFile.good() == false)
					break;

				content += cChar;
			}

			if (content != "")
				runApplication(content, file);
		}

		void V8JavascriptEngine::executeScript(Array<String> code, String fileName)
		{
			v8::Local<v8::String> script = v8::String::NewFromUtf8(isolate, "");

			for (RJINT iIdx = 0; iIdx < code.size(); iIdx++)
				script = v8::String::Concat(script, code.at(iIdx).toV8String(isolate));

			executeScript(script, fileName.toV8String(isolate), globalContext->Global());
		}

		void V8JavascriptEngine::executeScript(String code, String fileName)
		{
			executeScript(code, fileName, globalContext->Global());
		}

		void V8JavascriptEngine::executeScript(String code, String fileName, v8::Local<v8::Object> context)
		{
			executeScript(code.toV8String(isolate), fileName.toV8String(isolate), context);
		}

		void V8JavascriptEngine::executeScript(v8::Local<v8::String> code, v8::Local<v8::String> fileName, v8::Local<v8::Object> context)
		{
			v8::Local<v8::Context> scriptContext;
			v8::EscapableHandleScope scope(isolate);

			if (context.IsEmpty() == true)
				scriptContext = globalContext;
			else
				scriptContext = context->CreationContext ();

			v8::TryCatch tryCatch(isolate);
			v8::ScriptOrigin origin(fileName);
			v8::MaybeLocal<v8::Script> scriptTemp = v8::Script::Compile(scriptContext, code, &origin);

			if (scriptTemp.IsEmpty() == true)
			{
				v8::Local<v8::Value> exception = tryCatch.Exception();
				v8::Local<v8::Value> stackTrace = tryCatch.StackTrace();
				String exceptionStr = parseV8Value(exception);
				String stackTraceStr = parseV8Value(stackTrace);

				throw RadJAV::Exception("Compiling Javascript error: " + exceptionStr + "\n" + stackTraceStr);
			}

			v8::Local<v8::Script> script = scriptTemp.ToLocalChecked();
			v8::MaybeLocal<v8::Value> result = script->Run(scriptContext);

			if (result.IsEmpty() == true)
			{
				v8::Local<v8::Value> exception = tryCatch.Exception();
				v8::Local<v8::Value> stackTrace = tryCatch.StackTrace();
				String exceptionStr = parseV8Value(exception);
				String stackTraceStr = parseV8Value(stackTrace);

				throw RadJAV::Exception("Executing Javascript error: " + exceptionStr + "\n" + stackTraceStr);
			}

			scope.Escape(result.ToLocalChecked());
		}

		void V8JavascriptEngine::unboundedExecuteScript(String code, String fileName, v8::Local<v8::Object> context)
		{
			v8::Local<v8::Context> scriptContext;

			if (context.IsEmpty() == true)
				scriptContext = globalContext;
			else
				scriptContext = context->CreationContext();

			v8::TryCatch tryCatch(isolate);
			v8::ScriptOrigin origin(fileName.toV8String(isolate));
			v8::MaybeLocal<v8::Script> scriptTemp = v8::Script::Compile(scriptContext, code.toV8String(isolate), &origin);

			if (scriptTemp.IsEmpty() == true)
			{
				v8::Local<v8::Value> exception = tryCatch.Exception();
				String exceptionStr = parseV8Value(exception);

				throw RadJAV::Exception("Compiling Javascript error: " + exceptionStr);
			}

			v8::Local<v8::Script> script = scriptTemp.ToLocalChecked();
			v8::Local<v8::UnboundScript> unboundScript = script->GetUnboundScript();
			v8::Local<v8::Script> script2 = unboundScript->BindToCurrentContext();
			v8::MaybeLocal<v8::Value>result = script2->Run(scriptContext);

			if (result.IsEmpty() == true)
			{
				v8::Local<v8::Value> exception = tryCatch.Exception();
				String exceptionStr = parseV8Value(exception);

				throw RadJAV::Exception("Executing Javascript error: " + exceptionStr);
			}
			/*v8::Local<v8::Context> scriptContext;

			if (context == NULL)
			scriptContext = globalContext;
			else
			{
			if (context->resultType == "Context")
			scriptContext = *(v8::Local<v8::Context> *)context->result;

			if (context->resultType == "Function")
			{
			v8::Local<v8::Function> func = *(v8::Local<v8::Function> *)context->result;
			scriptContext = func->CreationContext();
			}

			if (context->resultType == "Object")
			{
			v8::Local<v8::Object> obj = *(v8::Local<v8::Object> *)context->result;
			scriptContext = obj->CreationContext();
			}
			}

			v8::TryCatch tryCatch(isolate);
			v8::ScriptOrigin origin(fileName.toV8String(isolate));
			v8::ScriptCompiler::Source source(code.toV8String(isolate), origin);
			v8::ScriptCompiler::CompileOptions options = v8::ScriptCompiler::kNoCompileOptions;
			v8::MaybeLocal<v8::UnboundScript> unboundScript = v8::ScriptCompiler::CompileUnboundScript (isolate, &source, options);

			if (unboundScript.IsEmpty() == true)
			{
			v8::Local<v8::Value> exception = tryCatch.Exception();
			String exceptionStr = parseV8Value(exception);

			throw RadJAV::Exception("Compiling Javascript error: " + exceptionStr);
			}

			v8::Local<v8::UnboundScript> unboundScriptLocalChecked = unboundScript.ToLocalChecked();
			v8::Local<v8::Script> script = unboundScriptLocalChecked->BindToCurrentContext ();

			v8::MaybeLocal<v8::Value>result = script->Run(scriptContext);

			if (result.IsEmpty() == true)
			{
			v8::Local<v8::Value> exception = tryCatch.Exception();
			String exceptionStr = parseV8Value(exception);

			throw RadJAV::Exception("Executing Javascript error: " + exceptionStr);
			}*/
		}

		void V8JavascriptEngine::executeScriptNextTick(String code, String fileName, v8::Local<v8::Object> context)
		{
			jsToExecuteNextCode.push_back(code);
			jsToExecuteNextFilename.push_back(fileName);
			jsToExecuteNextContext.push_back(context);
		}

		void V8JavascriptEngine::callFunctionOnNextTick(AsyncFunctionCall *call)
		{
			funcs.push_back(call);
		}

		void V8JavascriptEngine::collectGarbage()
		{
			if (exposeGC == false)
			{
				throw RadJAV::Exception("Unable to manually collect garbage, --expose_gc command line argument not set. Its best not to manually collect garbage anyway.");

				return;
			}

			isolate->RequestGarbageCollectionForTesting(v8::Isolate::GarbageCollectionType::kFullGarbageCollection);
		}

		#ifdef C3D_USE_OGRE
		void V8JavascriptEngine::start3DEngine()
		{
            mRoot = Ogre::Root::getSingletonPtr();
            
            //If we have Root then we assume that we already has been initialized
            if (mRoot != NULL)
                return;

			#ifdef GUI_USE_WXWIDGETS
				String userConfigDir = parsewxString(wxStandardPaths::Get().GetUserConfigDir());
				String tempDir = userConfigDir + "/RadJav/";

				if (wxDirExists(tempDir.towxString ()) == false)
				{
					wxMkDir(tempDir.towxString(), wxS_DIR_DEFAULT);
					tempDir = userConfigDir + "/RadJav/3D Engine/";
					wxMkDir(tempDir.towxString(), wxS_DIR_DEFAULT);
				}

				userConfigDir += "/RadJav/3D Engine";
			#endif

			Ogre::String mPluginCfg = "";
			Ogre::String mConfig = "";
			Ogre::String mLog = "";

			#ifdef _DEBUG
				mPluginCfg = userConfigDir + "/plugins_d.cfg";
				mConfig = userConfigDir + "/ogre_d.cfg";
				mLog = userConfigDir + "/ogre_log_d.cfg";
			#else
				mPluginCfg = userConfigDir + "/plugins.cfg";
				mConfig = userConfigDir + "/ogre.cfg";
				mLog = userConfigDir + "/ogre_log.cfg";
			#endif

            mRoot = RJNEW Ogre::Root(mPluginCfg, mConfig, mLog);

            //Check if we have successfull runs before
            if(!mRoot->restoreConfig())
            {
                //Get available renderers loaded from plugins.cfg file
                Ogre::RenderSystemList renderers = mRoot->getAvailableRenderers();
                
                //Set default settings for available renderers
                for(Ogre::RenderSystemList::const_iterator it = renderers.begin(); it != renderers.end(); it++)
                {
                    Ogre::RenderSystem* rs = *it;
                    
                    rs->setConfigOption("Video Mode", "1024 x 768");
                    rs->setConfigOption("Colour Depth", "32");
                    rs->setConfigOption("FSAA", "0");
                    rs->setConfigOption("Full Screen", "No");
                    rs->setConfigOption("RTT Preferred Mode", "FBO");
                    rs->setConfigOption("sRGB Gamma Conversion", "No");
                    rs->setConfigOption("Content Scaling Factor", "1.0");
                    #ifdef __APPLE__
                        #ifdef __LP64__
                            rs->setConfigOption("macAPI", "cocoa");
                        #else
                            rs->setConfigOption("macAPI", "carbon");
                        #endif
                    #endif
                }
                
                //Set default render system
                #ifdef WIN32
                    Ogre::RenderSystem* rs3D11 = mRoot->getRenderSystemByName("Direct3D11 Rendering Subsystem");
                    Ogre::RenderSystem* rs3D9 = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
                
                    mRoot->setRenderSystem( rs3D11 ? rs3D11 : rs3D9);
                #else
                    Ogre::RenderSystem* rsGL = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
                    mRoot->setRenderSystem(rsGL);
                #endif
                
                //Saving config file with selected render system and parameters for it
                mRoot->saveConfig();
            }

			/*Ogre::ConfigFile configFile;
			configFile.load(userConfigDir + "/resources.cfg");

			Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();

			Ogre::String secName, typeName, archName;

			while (seci.hasMoreElements())
			{
				secName = seci.peekNextKey();
				Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
				Ogre::ConfigFile::SettingsMultiMap::iterator i;

				for (i = settings->begin(); i != settings->end(); ++i)
				{
					typeName = i->first;
					archName = i->second;

					Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
						archName, typeName, secName);

				}
			}*/

			//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
            
            //Engine initialization
            //Here we specify that we don't need window from Ogre3D Engine
            //We will supply it from other GUI library
            mRoot->initialise(false);
		}
		#endif

		void V8JavascriptEngine::addTimeout (v8::Persistent<v8::Function> *func, RJINT time)
		{
			timeoutFuncs.push_back(func);
			timeouts.push_back(time);
		}

		void V8JavascriptEngine::blockchainEvent(String event, String dataType, void *data)
		{
			RJINT numArgs = 0;
			v8::Local<v8::Value> *args = NULL;

			if (data != NULL)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (criticalSection->TryEnter() == false)
					{
						/// @bug Stupid hack for now. This will skip any function that needs to execute if a lock can't be placed.
						//if (deleteOnComplete == true)
						//DELETE_ARRAY(args);

						return;
					}
				#endif

				numArgs = 1;
				args = RJNEW v8::Local<v8::Value> [numArgs];

				if (dataType == "int")
					args[0] = v8::Integer::New(isolate, *(RJINT *)data);

				if (dataType == "number")
					args[0] = v8::Number::New(isolate, *(RJNUMBER *)data);

				if (dataType == "bool")
					args[0] = v8::Boolean::New(isolate, *(RJBOOL *)data);

				if (dataType == "string")
					args[0] = ((String *)data)->toV8String (isolate);
			}

			blockchainEvent(event, numArgs, args, true);
		}

		void V8JavascriptEngine::blockchainEvent(String event, RJINT numargs, v8::Local<v8::Value> *args, RJBOOL alreadyEnteredCritialSection)
		{
			#ifdef GUI_USE_WXWIDGETS
				if (alreadyEnteredCritialSection == false)
				{
					if (criticalSection->TryEnter() == false)
					{
						/// @bug Stupid hack for now. This will skip any function that needs to execute if a lock can't be placed.
						//if (deleteOnComplete == true)
						DELETE_ARRAY(args);

						return;
					}
				}
			#endif

			#ifdef USE_BLOCKCHAIN_V1
			if (event == "ready")
			{
				if (V8B::BlockchainV1::onReadyFunction != NULL)
					callFunctionOnNextTick(V8B::BlockchainV1::onReadyFunction, NULL, false);
			}

			if (event == "connectBlock")
			{
				if (V8B::BlockchainV1::connectBlockFunction != NULL)
				{
					v8::Persistent<v8::Array> *results = RJNEW v8::Persistent<v8::Array>();
					v8::Local<v8::Array> ary = v8::Array::New(isolate);

					for (RJINT iIdx = 0; iIdx < numargs; iIdx++)
						ary->Set(iIdx, args[iIdx]);

					results->Reset(V8_JAVASCRIPT_ENGINE->isolate, ary);

					callFunctionOnNextTick(V8B::BlockchainV1::connectBlockFunction, results, true);
				}
			}

			if (event == "proofOfWorkFound")
			{
				if (V8B::BlockchainV1::proofOfWorkFoundFunction != NULL)
					callFunctionOnNextTick(V8B::BlockchainV1::proofOfWorkFoundFunction, NULL, false);
			}

			if (event == "passphraseRequired")
			{
				if (V8B::BlockchainV1::passphraseRequiredFunction != NULL)
					callFunctionOnNextTick(V8B::BlockchainV1::passphraseRequiredFunction, NULL, false);
			}

			if (event == "error")
			{
				if (V8B::BlockchainV1::onErrorFunction != NULL)
				{
					v8::Persistent<v8::Array> *results = RJNEW v8::Persistent<v8::Array>();
					v8::Local<v8::Array> ary = v8::Array::New(isolate);

					for (RJINT iIdx = 0; iIdx < numargs; iIdx++)
						ary->Set(iIdx, args[iIdx]);

					results->Reset(V8_JAVASCRIPT_ENGINE->isolate, ary);

					callFunctionOnNextTick(V8B::BlockchainV1::onErrorFunction, results, true);
				}
			}

			DELETE_ARRAY(args);
			#endif

			#ifdef GUI_USE_WXWIDGETS
				criticalSection->Leave();
			#endif
		}

		void V8JavascriptEngine::addThread(Thread *thread)
		{
			threads.insert (HashMapPair<RJULONG, Thread *> ((RJULONG)thread, thread));
		}

		void V8JavascriptEngine::removeThread(Thread *thread)
		{
			removeThreads.push_back((RJULONG)thread);
		}

		void V8JavascriptEngine::throwException(String message)
		{
			v8::Local<v8::Object> err = v8::Object::New(isolate);
			err->Set(String ("message").toV8String (isolate), message.toV8String(isolate));

			isolate->ThrowException (err);

			RadJav::printToOutputWindow(message);

			if (exceptionsDisplayMessageBox == true)
				RadJav::showMessageBox(message, "Error");

			if (shutdownOnException == true)
				shutdown = true;
		}

		void V8JavascriptEngine::exit(RJINT exitCode)
		{
			shutdown = true;
		}

		void V8JavascriptEngine::destroyJSObjects()
		{
			// OS
			{
				V8B::OS::destroy();
			}
		}

		void V8JavascriptEngine::loadNativeCode()
		{
			// Globals
			{
				V8B::Global::createV8Callbacks(isolate, globalContext->Global());

				if (exposeGC == true)
				{
					V8_CALLBACK(globalContext->Global(), "collectGarbage", V8B::Global::collectGarbage);
				}
			}

			// RadJav
			{
				v8::Handle<v8::Function> radJavFunc = v8GetFunction(globalContext->Global(), "RadJav");

				V8B::Console::createV8Callbacks(isolate, radJavFunc);
				V8B::Thread::createV8Callbacks(isolate, radJavFunc);

				V8_CALLBACK(radJavFunc, "exit", V8B::Global::exit);
				V8_CALLBACK(radJavFunc, "quit", V8B::Global::exit);

				V8_CALLBACK(radJavFunc, "quit", V8B::Global::exit);

				// RadJav.OS
				{
					v8::Handle<v8::Function> osFunc = v8GetFunction(radJavFunc, "OS");

					V8B::OS::createV8Callbacks(isolate, osFunc);
				}

				// RadJav.Console
				{
					v8::Handle<v8::Function> consoleFunc = v8GetFunction(radJavFunc, "Console");

					V8B::Console::createV8Callbacks(isolate, consoleFunc);
				}

				// RadJav.DB
				#ifdef USE_DATABASE
					{
						v8::Handle<v8::Function> dbFunc = v8GetFunction(radJavFunc, "DB");

						// RadJav.DB.KeyValueStorage
						{
							v8::Handle<v8::Function> keyValueStorageFunc = v8GetFunction(dbFunc, "KeyValueStorage");
							v8::Handle<v8::Object> keyValueStoragePrototype = v8GetObject(keyValueStorageFunc, "prototype");

							V8B::Database::KeyValueStorage::createV8Callbacks(isolate, keyValueStoragePrototype);
						}
					}
				#endif

				// RadJav.IO
				{
					v8::Handle<v8::Function> ioFunc = v8GetFunction(radJavFunc, "IO");

					V8B::IO::createV8Callbacks(isolate, ioFunc);

					// RadJav.IO.SerialComm
					{
						v8::Handle<v8::Function> serialCommFunc = v8GetFunction(ioFunc, "SerialComm");
						v8::Handle<v8::Object> serialPrototype = v8GetObject(serialCommFunc, "prototype");

						V8B::IO::SerialComm::createV8Callbacks(isolate, serialPrototype);
					}

					// RadJav.IO.TextFile
					{
						v8::Handle<v8::Function> textFileFunc = v8GetFunction(ioFunc, "TextFile");

						V8B::IO::TextFile::createV8Callbacks(isolate, textFileFunc);
					}
				}

				// RadJav.Net
				{
					v8::Handle<v8::Function> netFunc = v8GetFunction(radJavFunc, "Net");

					V8B::Net::NetCallbacks::createV8Callbacks(isolate, netFunc);

					// WebServer
					{
						v8::Handle<v8::Function> webServerFunc = v8GetFunction(netFunc, "WebServer");
						v8::Handle<v8::Object> webServerPrototype = v8GetObject(webServerFunc, "prototype");

						V8B::Net::WebServer::createV8Callbacks(isolate, webServerPrototype);
					}

					// WebSocketServer
					{
						v8::Handle<v8::Function> webSocketServerFunc = v8GetFunction(netFunc, "WebSocketServer");
						v8::Handle<v8::Object> webSocketServerPrototype = v8GetObject(webSocketServerFunc, "prototype");

						V8B::Net::WebSocketServer::createV8Callbacks(isolate, webSocketServerPrototype);
					}

					// WebSocketClient
					{
						v8::Handle<v8::Function> webSocketClientFunc = v8GetFunction(netFunc, "WebSocketClient");
						v8::Handle<v8::Object> webSocketClientPrototype = v8GetObject(webSocketClientFunc, "prototype");

						V8B::Net::WebSocketClient::createV8Callbacks(isolate, webSocketClientPrototype);
					}
				}

				#ifdef USE_BLOCKCHAIN_V1
				// RadJav.BlockchainV1
				{
					v8::Handle<v8::Function> blockchainFunc = v8GetFunction(radJavFunc, "BlockchainV1");

					V8B::BlockchainV1::createV8Callbacks(isolate, blockchainFunc);
				}
				#endif

				// RadJav.GUI
				{
					v8::Handle<v8::Function> guiFunc = v8GetFunction(radJavFunc, "GUI");

					// RadJav.GUI.GObject
					{
						v8::Handle<v8::Function> gobjectFunc = v8GetFunction(guiFunc, "GObject");
						v8::Handle<v8::Object> gobjectPrototype = v8GetObject(gobjectFunc, "prototype");

						V8B::GUI::GObject::createV8Callbacks(isolate, gobjectPrototype);
					}

					// RadJav.GUI.Window
					{
						v8::Handle<v8::Function> windowFunc = v8GetFunction(guiFunc, "Window");
						v8::Handle<v8::Object> windowPrototype = v8GetObject(windowFunc, "prototype");

						V8B::GUI::Window::createV8Callbacks(isolate, windowPrototype);
					}

					// RadJav.GUI.Button
					{
						v8::Handle<v8::Function> buttonFunc = v8GetFunction(guiFunc, "Button");
						v8::Handle<v8::Object> buttonPrototype = v8GetObject(buttonFunc, "prototype");

						V8B::GUI::Button::createV8Callbacks(isolate, buttonPrototype);
					}

					// RadJav.GUI.Label
					{
						v8::Handle<v8::Function> labelFunc = v8GetFunction(guiFunc, "Label");
						v8::Handle<v8::Object> labelPrototype = v8GetObject(labelFunc, "prototype");

						V8B::GUI::Label::createV8Callbacks(isolate, labelPrototype);
					}

					// RadJav.GUI.Image
					{
						v8::Handle<v8::Function> imageFunc = v8GetFunction(guiFunc, "Image");
						v8::Handle<v8::Object> imagePrototype = v8GetObject(imageFunc, "prototype");

						V8B::GUI::Image::createV8Callbacks(isolate, imagePrototype);
					}

					// RadJav.GUI.Container
					{
						v8::Handle<v8::Function> containerFunc = v8GetFunction(guiFunc, "Container");
						v8::Handle<v8::Object> containerPrototype = v8GetObject(containerFunc, "prototype");

						V8B::GUI::Container::createV8Callbacks(isolate, containerPrototype);
					}

					// RadJav.GUI.Combobox
					{
						v8::Handle<v8::Function> comboboxFunc = v8GetFunction(guiFunc, "Combobox");
						v8::Handle<v8::Object> comboboxPrototype = v8GetObject(comboboxFunc, "prototype");

						V8B::GUI::Combobox::createV8Callbacks(isolate, comboboxPrototype);
					}

					// RadJav.GUI.Textbox
					{
						v8::Handle<v8::Function> textboxFunc = v8GetFunction(guiFunc, "Textbox");
						v8::Handle<v8::Object> textboxPrototype = v8GetObject(textboxFunc, "prototype");

						V8B::GUI::Textbox::createV8Callbacks(isolate, textboxPrototype);
					}

					// RadJav.GUI.Textarea
					{
						v8::Handle<v8::Function> textareaFunc = v8GetFunction(guiFunc, "Textarea");
						v8::Handle<v8::Object> textareaPrototype = v8GetObject(textareaFunc, "prototype");

						V8B::GUI::Textarea::createV8Callbacks(isolate, textareaPrototype);
					}

					// RadJav.GUI.Checkbox
					{
						v8::Handle<v8::Function> checkboxFunc = v8GetFunction(guiFunc, "Checkbox");
						v8::Handle<v8::Object> checkboxPrototype = v8GetObject(checkboxFunc, "prototype");

						V8B::GUI::Checkbox::createV8Callbacks(isolate, checkboxPrototype);
					}

					// RadJav.GUI.Radio
					{
						v8::Handle<v8::Function> radioFunc = v8GetFunction(guiFunc, "Radio");
						v8::Handle<v8::Object> radioPrototype = v8GetObject(radioFunc, "prototype");

						V8B::GUI::Radio::createV8Callbacks(isolate, radioPrototype);
					}

					// RadJav.GUI.List
					{
						v8::Handle<v8::Function> listFunc = v8GetFunction(guiFunc, "List");
						v8::Handle<v8::Object> listPrototype = v8GetObject(listFunc, "prototype");

						V8B::GUI::List::createV8Callbacks(isolate, listPrototype);
					}

					// RadJav.GUI.MenuBar
					{
						v8::Handle<v8::Function> menuBarFunc = v8GetFunction(guiFunc, "MenuBar");
						v8::Handle<v8::Object> menuBarPrototype = v8GetObject(menuBarFunc, "prototype");

						V8B::GUI::MenuBar::createV8Callbacks(isolate, menuBarPrototype);
					}

					// RadJav.GUI.MenuItem
					{
						v8::Handle<v8::Function> menuItemFunc = v8GetFunction(guiFunc, "MenuItem");
						v8::Handle<v8::Object> menuItemPrototype = v8GetObject(menuItemFunc, "prototype");

						V8B::GUI::MenuItem::createV8Callbacks(isolate, menuItemPrototype);
					}

					// RadJav.GUI.WebView
					{
						v8::Handle<v8::Function> webViewFunc = v8GetFunction(guiFunc, "WebView");
						v8::Handle<v8::Object> webViewPrototype = v8GetObject(webViewFunc, "prototype");

						V8B::GUI::WebView::createV8Callbacks(isolate, webViewPrototype);
					}

					#ifdef C3D_USE_OGRE
						// RadJav.GUI.Canvas3D
						{
							v8::Handle<v8::Function> canvas3DFunc = v8GetFunction(guiFunc, "Canvas3D");
							v8::Handle<v8::Object> canvas3DFuncPrototype = v8GetObject(canvas3DFunc, "prototype");

							V8B::GUI::Canvas3D::createV8Callbacks(isolate, canvas3DFuncPrototype);
						}
					#endif
				}

				#ifdef C3D_USE_OGRE
				// RadJav.C3D
				{
					v8::Handle<v8::Function> c3dFunc = v8GetFunction(radJavFunc, "C3D");

					// RadJav.C3D.Object3D
					{
						v8::Handle<v8::Function> object3DFunc = v8GetFunction(c3dFunc, "Object3D");
						v8::Handle<v8::Object> object3DPrototype = v8GetObject(object3DFunc, "prototype");

						V8B::C3D::Object3D::createV8Callbacks(isolate, object3DPrototype);
					}

					// RadJav.C3D.World
					{
						v8::Handle<v8::Function> worldFunc = v8GetFunction(c3dFunc, "World");
						v8::Handle<v8::Object> worldPrototype = v8GetObject(worldFunc, "prototype");

						V8B::C3D::World::createV8Callbacks(isolate, worldPrototype);
					}

					// RadJav.C3D.Entity
					{
						v8::Handle<v8::Function> entityFunc = v8GetFunction(c3dFunc, "Entity");
						v8::Handle<v8::Object> entityPrototype = v8GetObject(entityFunc, "prototype");

						V8B::C3D::Entity::createV8Callbacks(isolate, entityPrototype);
					}

					// RadJav.C3D.Camera
					{
                        /*
						v8::Handle<v8::Function> cameraFunc = v8GetFunction(c3dFunc, "Camera");
						v8::Handle<v8::Object> cameraPrototype = v8GetObject(cameraFunc, "prototype");

						V8B::C3D::Camera::createV8Callbacks(isolate, cameraPrototype);
                        */
					}
				}
				#endif
				#ifdef USE_CRYPTOGRAPHY
				// RadJav.Crypto.Hash
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {

				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "Hash");
				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::Hash::getCapabilities);
				    //std::cout << "Obj FieldCount: " << func -> InternalFieldCount() << std::endl << std::flush;
				    //std::cout << "Obj ExtFieldCount: " << func -> GetIndexedPropertiesExternalArrayDataLength() << std::endl << std::flush;
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    //std::cout <<  "Len: " << str -> Length() << std::endl << std::flush;
				    //std::cout << "Obj FieldCount: " << prototype -> InternalFieldCount() << std::endl << std::flush;
				    

				    V8B::Crypto::Hash::createV8Callbacks(isolate, prototype);
				  }

				}


				// RadJav.Crypto.HashMultipart
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "HashMultipart");
				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::HashMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    V8B::Crypto::HashMultipart::createV8Callbacks(isolate, prototype);
				  }

				}
				// RadJav.Crypto.Cipher
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "Cipher");
				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::Cipher::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    V8B::Crypto::Cipher::createV8Callbacks(isolate, prototype);
				  }
				}
				// RadJav.Crypto.Decipher
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "Decipher");
				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::Decipher::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    V8B::Crypto::Decipher::createV8Callbacks(isolate, prototype);
				  }
				}
				// RadJav.Crypto.CipherMultipart
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "CipherMultipart");
				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::CipherMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    V8B::Crypto::CipherMultipart::createV8Callbacks(isolate, prototype);
				  }
				}


				// RadJav.Crypto.DecipherMultipart
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "DecipherMultipart");
				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::DecipherMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    V8B::Crypto::DecipherMultipart::createV8Callbacks(isolate, prototype);

				  }
				}

				// RadJav.Crypto.KeyGenerator
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "KeyGenerator");
				    //				    V8_CALLBACK(func, "getCapabilities", V8B::Crypto::DecipherMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    //				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    V8B::Crypto::KeyGenerator::createV8Callbacks(isolate, prototype);
				  }
				}

				// RadJav.Crypto.PrivateKey
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "PrivateKey");
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");

				    V8B::Crypto::PrivateKey::createV8Callbacks(isolate, prototype);
				    V8B::Crypto::PrivateKey::setConstructor(isolate, func);

				    v8::Handle<v8::Object> init = v8GetObject(prototype, "_init");
				    
				  }
				}

				// RadJav.Crypto.PublicKey
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "PublicKey");
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");

				    V8B::Crypto::PublicKey::createV8Callbacks(isolate, prototype);
				    V8B::Crypto::PublicKey::setConstructor(isolate, func);

				  }
				}

				#endif
				
			}
		}

		void V8JavascriptEngine::loadTemplates(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			static_cast <V8JavascriptEngine *> (RadJav::javascriptEngine)->loadNativeCode();
		}

		void V8JavascriptEngine::runEventLoopSingleStep(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			V8JavascriptEngine *jsEngine = static_cast <V8JavascriptEngine *> (RadJav::javascriptEngine);

			while (true)
			{
				v8::platform::PumpMessageLoop(jsEngine->platform, jsEngine->isolate);
				RadJav::runEventLoopSingleStep();
			}
		}

		void V8JavascriptEngine::runScript(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			V8JavascriptEngine *jsEngine = static_cast <V8JavascriptEngine *> (RadJav::javascriptEngine);
			String applicationSource = parseV8Value(args[0]);
			String fileName = parseV8Value(args[1]);

			jsEngine->unboundedExecuteScript(applicationSource, fileName);
		}

		v8::Handle<v8::Function> V8JavascriptEngine::v8GetFunction(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));
			v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

			return (func);
		}

		v8::Handle<v8::Value> V8JavascriptEngine::v8GetValue(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));

			return (value);
		}

		void V8JavascriptEngine::v8SetString(v8::Local<v8::Object> context, String functionName, String str)
		{
			context->Set(functionName.toV8String(isolate), str.toV8String (isolate));
		}

		String V8JavascriptEngine::v8GetString(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));
			String result = "";

			if (v8IsNull(value) == true)
				return (result);

			v8::Handle<v8::String> str = v8::Handle<v8::String>::Cast(value);

			return (parseV8Value(str));
		}

		void V8JavascriptEngine::v8SetNumber(v8::Local<v8::Object> context, String functionName, RDECIMAL number)
		{
			context->Set(functionName.toV8String(isolate), v8::Number::New(isolate, number));
		}

		RJINT V8JavascriptEngine::v8GetInt(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));
			RJINT result = 0;

			if (v8IsNull(value) == true)
				return (result);

			v8::Handle<v8::Number> val = v8::Handle<v8::Number>::Cast(value);

			return (val->Int32Value());
		}

		RDECIMAL V8JavascriptEngine::v8GetDecimal(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));
			RDECIMAL result = 0;

			if (v8IsNull(value) == true)
				return (result);

			v8::Handle<v8::Number> val = v8::Handle<v8::Number>::Cast(value);

			return (val->NumberValue());
		}

		void V8JavascriptEngine::v8SetBool(v8::Local<v8::Object> context, String functionName, bool value)
		{
			context->Set(functionName.toV8String(isolate), v8::Boolean::New (isolate, value));
		}

		RJBOOL V8JavascriptEngine::v8GetBool(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));
			RJBOOL result = false;

			if (v8IsNull(value) == true)
				return (result);

			v8::Handle<v8::Boolean> val = v8::Handle<v8::Boolean>::Cast(value);
			result = val->Value();

			return (result);
		}

		void V8JavascriptEngine::v8SetObject(v8::Local<v8::Object> context, String functionName, v8::Handle<v8::Object> obj)
		{
			context->Set(functionName.toV8String(isolate), obj);
		}

		v8::Handle<v8::Object> V8JavascriptEngine::v8GetObject(v8::Local<v8::Object> context, String functionName)
		{
			v8::Handle<v8::Value> value = context->Get(functionName.toV8String(isolate));
			v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(value);

			return (obj);
		}

		v8::Local<v8::Value> V8JavascriptEngine::v8CallFunction(
			v8::Local<v8::Object> context, String functionName, RJINT numArgs, v8::Local<v8::Value> *args)
		{
			v8::Handle<v8::Function> func = v8GetFunction(context, functionName);
			v8::Local<v8::Value> result = func->Call(context, numArgs, args);

			return (result);
		}

		v8::Local<v8::Object> V8JavascriptEngine::v8CallAsConstructor(v8::Local<v8::Object> function, RJINT numArgs, v8::Local<v8::Value> *args)
		{
			v8::Local<v8::Value> result = function->CallAsConstructor(
				V8_JAVASCRIPT_ENGINE->globalContext, numArgs, args).ToLocalChecked ();
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(result);

			return (obj);
		}

		CPP::ChainedPtr* V8JavascriptEngine::v8GetExternal(v8::Local<v8::Object> context, String functionName)
		{
			return externalsManager->get(context, functionName);
		}

		/* Replaced with method which utilize wrapper
		void V8JavascriptEngine::v8SetExternal(v8::Local<v8::Object> context, String functionName, void *obj)
		{
			v8::Local<v8::External> val = v8::External::New(isolate, obj);
			context->Set(functionName.toV8String(isolate), val);
		}
		*/

		void V8JavascriptEngine::v8SetExternal(v8::Local<v8::Object> context, String functionName, CPP::ChainedPtr *obj)
		{
			externalsManager->set(context, functionName, obj);
		}

		void V8JavascriptEngine::v8ClearExternal(v8::Local<v8::Object> context, String functionName)
		{
			externalsManager->clear(context, functionName);
		}

		void *V8JavascriptEngine::v8GetInternalField(v8::Local<v8::Object> context, String functionName)
		{
			v8::Local<v8::Value> value = context->Get(functionName.toV8String(isolate));

			if (v8IsNull(value) == true)
				return (NULL);

			v8::Local<v8::Object> val = v8::Local<v8::Object>::Cast (value);
			v8::Local<v8::External> ext = v8::Local<v8::External>::Cast (val->GetInternalField(0));

			return (ext->Value ());
		}

		v8::Handle<v8::Value> V8JavascriptEngine::v8GetArgument(const v8::FunctionCallbackInfo<v8::Value> &args, RJUINT index)
		{
			v8::Handle<v8::Value> value = args[index];

			return (value);
		}

		void V8JavascriptEngine::v8SetValue(v8::Local<v8::Object> context, String functionName, v8::Handle<v8::Value> obj)
		{
			context->Set(functionName.toV8String(isolate), obj);
		}

		bool V8JavascriptEngine::v8IsNull(v8::Local<v8::Value> val)
		{
			if (val.IsEmpty() == true)
				return (true);

			if (val->IsUndefined() == true)
				return (true);

			if (val->IsNull() == true)
				return (true);

			return (false);
		}

		RJBOOL V8JavascriptEngine::v8ParseBool(v8::Local<v8::Value> val)
		{
			return (val->BooleanValue());
		}

		RJINT V8JavascriptEngine::v8ParseInt(v8::Local<v8::Value> val)
		{
			return (val->Int32Value());
		}

		RDECIMAL V8JavascriptEngine::v8ParseDecimal(v8::Local<v8::Value> val)
		{
			return (val->NumberValue ());
		}

		v8::Local<v8::Object> V8JavascriptEngine::createPromise(v8::Local<v8::Function> function)
		{
			v8::Local<v8::Object> context = globalContext->Global();

			return (createPromise(context, function));
		}

		v8::Local<v8::Object> V8JavascriptEngine::createPromise(
			v8::Local<v8::Object> context, v8::Local<v8::Function> function, v8::Local<v8::Array> args)
		{
			v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_JAVASCRIPT_ENGINE->globalContext->Global(), "Promise");
			v8::Local<v8::Function> keepContext = V8_JAVASCRIPT_ENGINE->v8GetFunction(radJav->Get (isolate), "keepContext");
			RJINT contextArgs = 2;

			if (args.IsEmpty() == false)
				contextArgs = 3;

			v8::Local<v8::Value> *args2 = RJNEW v8::Local<v8::Value>[contextArgs];
			args2[0] = function;
			args2[1] = context;

			if (contextArgs > 2)
				args2[2] = args;

			v8::Local<v8::Value> newContext = keepContext->Call(context, contextArgs, args2);
			DELETE_ARRAY(args2);

			v8::Local<v8::Value> *args3 = RJNEW v8::Local<v8::Value>[1];
			args3[0] = newContext;
			v8::Local<v8::Value> result = promise->CallAsConstructor(V8_JAVASCRIPT_ENGINE->globalContext, 1, args3).ToLocalChecked ();
			v8::Local<v8::Object> promiseObject = v8::Local<v8::Object>::Cast(result);
			DELETE_ARRAY(args3);

			return (promiseObject);
		}

		V8JSChannel::V8JSChannel(v8::Local<v8::Context> context) {
			isolate_ = context->GetIsolate();
			context_.Reset(isolate_, context);
		}
	
	#endif
}
