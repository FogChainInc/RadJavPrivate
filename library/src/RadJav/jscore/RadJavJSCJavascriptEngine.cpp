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
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "RadJav.h"
#include "RadJavException.h"

#include "RadJavJSFile.h"
#include "RadJavJavascriptCode.h"

#ifdef GUI_USE_WXWIDGETS
	#include <wx/stdpaths.h>
	#include <wx/filename.h>
#else
#ifdef _WIN32
//#include <direct.h>
//#define getcwd _getcwd // stupid MSFT "deprecation" warning
#elif
#include <unistd.h>
#endif
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCGlobal.h"
    #include "jscore/RadJavJSCOS.h"

    #include "jscore/RadJavJSCConsole.h"
    #include "jscore/RadJavJSCThread.h"
#endif

#include "cpp/RadJavCPPIO.h"

#include <cstring>
#include <fstream>
#include "cpp/RadJavCPPAgent.h"

namespace RadJAV
{
	#ifdef USE_JAVASCRIPTCORE
        AsyncFunctionCall::AsyncFunctionCall (JSObjectRef newfunc,
                RJINT numArgs, JSValueRef *newargs, RJBOOL newDeleteOnComplete)
        {
            func = newfunc;
            this->numArgs = numArgs;
            args = newargs;
            deleteOnComplete = newDeleteOnComplete;
            result = NULL;
        }

        AsyncFunctionCall::~AsyncFunctionCall()
        {
        }

        JSValueRef AsyncFunctionCall::getResult(JSCJavascriptEngine *engine)
        {
            return (result);
        }

		JSCJavascriptEngine::JSCJavascriptEngine()
			: JavascriptEngine(),
			  externalsManager(nullptr)
		{
			externalsManager = RJNEW ExternalsManager();
			
			String execPath = "";

            #ifdef GUI_USE_WXWIDGETS
                execPath = parsewxString(wxStandardPaths::Get().GetExecutablePath());
            #else
                execPath = RadJAV::_radjav_exec_path;
            #endif

            String flags = "";
			exceptionsDisplayMessageBox = false;
			shutdownOnException = false;

			if (RadJav::radJavArguments.size() > 0)
			{
				for (size_t iIdx = 0; iIdx < RadJav::radJavArguments.size(); iIdx++)
				{
					String endSpace = " ";

					if (iIdx == (RadJav::radJavArguments.size() - 1))
						endSpace = "";

					String arg = RadJav::radJavArguments.at(iIdx);

					flags += arg + endSpace;
				}
			}

            JSContextGroupRef jsGroupContext = JSContextGroupCreate ();
            // Create the global context.
            globalContext = JSGlobalContextCreateInGroup (jsGroupContext, NULL);
            // The global "this" object used throughout JS.
            globalObj = JSContextGetGlobalObject (globalContext);

			#ifdef GUI_USE_WXWIDGETS
				criticalSection = RJNEW wxCriticalSection ();
			#endif
		}

		JSCJavascriptEngine::~JSCJavascriptEngine()
		{
			DELETEOBJ(externalsManager);
			
			destroyJSObjects();

			#ifdef GUI_USE_WXWIDGETS
				DELETEOBJ(criticalSection);
			#endif
		}

		int JSCJavascriptEngine::runApplication(String applicationSource, String fileName)
		{
			String parentDir = fileName;

			#ifdef GUI_USE_WXWIDGETS
				wxFileName file(parentDir.towxString());
				file.MakeAbsolute();
				wxString tempStr = file.GetPath();
				parentDir = parsewxString(tempStr);
				wxSetWorkingDirectory(parentDir);
			#endif

			loadJavascriptLibrary();

			// Insert the javascript libraries to be used.
			for (RJUINT iIdx = 0; iIdx < javascriptFiles.size(); iIdx++)
			{
				JSFile jsfile = javascriptFiles.at(iIdx);
				String contentStr = jsfile.getContent();

				executeScript(contentStr, jsfile.filename);
			}

			loadNativeCode();

            radJav = jscCastValueToObject (jscGetFunction(globalObj, "RadJav"));

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
                            JSObjectRef parsedObj = NULL;

							try
							{
                                JSStringRef appSrc = applicationSource.toJSCString();
                                JSValueRef parsedStr = JSValueMakeFromJSONString (globalContext, appSrc);
                                parsedObj = jscCastValueToObject(globalContext, parsedStr);

                                JSStringRelease(appSrc);
							}
							catch (Exception ex)
							{
								RadJav::showError(ex.getMessage(), true);
							}
                            
                            String name = jscGetString(parsedObj, "name");
                            String developer = jscGetString(parsedObj, "developer");
                            String license = jscGetString(parsedObj, "license");
                            JSValueRef executeFilesVal = jscGetValue(parsedObj, "execute_files");
                            JSObjectRef execute_files = jscCastValueToObject(globalContext, executeFilesVal);
                            JSValueRef dependenciesVal = jscGetValue(parsedObj, "dependencies");
                            JSObjectRef dependencies = jscCastValueToObject(globalContext, dependenciesVal);
                            RJUINT length = JSObjectGetTypedArrayLength (globalContext, execute_files, NULL);

                            // Get the list of JavaScript files to execute.
                            for (RJUINT iIdx = 0; iIdx < length; iIdx++)
                            {
                                JSValueRef jscFile = JSObjectGetPropertyAtIndex (globalContext, execute_files, iIdx, NULL);

                                if (JSValueIsString(globalContext, jscFile) == true)
                                {
                                    String filePath = parseJSCValue(globalContext, jscFile);
                                    filesToExecute.push_back(filePath);
                                }

                                if (JSValueIsObject (globalContext, jscFile) == true)
                                {
                                    JSObjectRef fileObj = jscCastValueToObject(globalContext, jscFile);
                                    String javascriptFilePath = jscGetString(fileObj, "javascript");
                                    filesToExecute.push_back(javascriptFilePath);
                                }
                            }

							// Once the list has been collected, execute each file.
							for (RJUINT iIdx = 0; iIdx < filesToExecute.size(); iIdx++)
							{
								String executeFile = filesToExecute.at(iIdx);
								String fileContents = CPP::IO::TextFile::readFile(executeFile);

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

				return EXIT_FAILURE;
			}

			int exitCode = EXIT_SUCCESS;
			#ifdef GUI_USE_WXWIDGETS
				// Here we enter main loop of wxApp and set our IdleEvent handler
				if(wxTheApp)
				{
					wxTheApp->Bind( wxEVT_IDLE,
								   &JSCJavascriptEngine::runApplicationInIdleEvent,
								   this,
								   wxID_ANY,
								   wxID_ANY);
				
					// Enter the main loop of app
					exitCode = wxTheApp->OnRun();
				}
			#else
				while (runApplicationSingleStep()) {}
			#endif

			return exitCode;
		}

        #ifdef GUI_USE_WXWIDGETS
            void JSCJavascriptEngine::runApplicationInIdleEvent(wxIdleEvent& event)
            {
                if(!runApplicationSingleStep())
                    return;

                event.RequestMore();
                wxMilliSleep(1);
            }
        #endif

		bool JSCJavascriptEngine::runApplicationSingleStep()
		{
			static RJBOOL firstRun = true;
			static RJBOOL startedBlockchainV1 = false;
			
			auto execCodeBegin = jsToExecuteNextCode.begin();
			auto execFilenameBegin = jsToExecuteNextFilename.begin();
            auto execContextBegin = jsToExecuteNextContext.begin();
			auto execCodeEnd = jsToExecuteNextCode.end();
			
			try
			{
				// Handle the on ready function.
				if (firstRun == true)
				{
                    if (JSC::OS::onReadyFunction != NULL)
                    {
                        if (JSValueIsNull(globalContext, JSC::OS::onReadyFunction) == false)
                        {
                            JSValueRef exception = jscCreateException (globalContext);
                            JSValueRef result = JSObjectCallAsFunction(globalContext, JSC::OS::onReadyFunction, globalObj, 0, NULL, &exception);

                            if (result == NULL)
                                jscHandleException(globalContext, exception);
                        }
                    }

					firstRun = false;
				}

				#ifdef USE_BLOCKCHAIN_V1
					if (JSC::BlockchainV1::hasBlockchainStarted == true)
					{
						if (startedBlockchainV1 == false)
						{
							JSC::BlockchainV1::startBlockchain();
							startedBlockchainV1 = true;
						}
					}
				#endif

				//Handle timers (setTimeout from JS)
				std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
				
                for (RJINT iIdx = 0; iIdx < timers.size (); iIdx++)
                {
                    auto timer = timers.at(iIdx);
                    JSObjectRef funcp = timer.first;
                    std::chrono::time_point<std::chrono::steady_clock> fireTime = timer.second;
                    
                    if (fireTime <= currentTime)
                    {
                        TimerVector::iterator delTimer = timers.begin ();
                        timers.erase(delTimer);

                        if ((JSValueIsNull(globalContext, funcp) == false) &&
                            (JSValueIsUndefined(globalContext, funcp) == false))
                        {
                            JSValueRef exception = jscCreateException (globalContext);
                            JSValueRef result = JSObjectCallAsFunction(globalContext, funcp, globalObj, 0, NULL, &exception);

                            if (result == NULL)
                                jscHandleException(globalContext, exception);
                        }
                    }
                }

				for (RJUINT iIdx = 0; iIdx < removeThreads.size(); iIdx++)
				{
					auto tbegin = threads.find (removeThreads.at (iIdx));
					auto tend = threads.end();
					
					if (tbegin == tend)
						continue;
					
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
                        JSObjectRef funcp = asyncCall->func;
                        JSValueRef *args = asyncCall->args;
                        RJINT numArgs = asyncCall->numArgs;
                        RJBOOL deleteOnComplete = asyncCall->deleteOnComplete;

                        if ((JSValueIsUndefined (globalContext, funcp) == false) &&
                            (JSValueIsNull(globalContext, funcp) == false))
                        {
                            JSValueRef exception = jscCreateException (globalContext);
                            JSValueRef result = JSObjectCallAsFunction(globalContext, funcp, globalObj, numArgs, args, &exception);

                            if (result == NULL)
                                jscHandleException(globalContext, exception);

                            asyncCall->result = result;
                        }
                        
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
				{
                    #ifdef GUI_USE_WXWIDGETS
                        wxExit();
                    #endif

					return false;
				}
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
				{
                    #ifdef GUI_USE_WXWIDGETS
                        wxExit();
                    #endif
					return false;
				}
			}
			
			return true;
		}
	
		int JSCJavascriptEngine::runApplicationFromFile(String file)
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
				return runApplication(content, file);
			
			return EXIT_FAILURE;
		}

		void JSCJavascriptEngine::executeScript(Array<String> code, String fileName)
		{
            String codeStr = "";

            for (RJINT iIdx = 0; iIdx < code.size (); iIdx++)
                codeStr += code.at (iIdx);

            executeScript (codeStr, fileName);
		}
    
        void JSCJavascriptEngine::executeScript(String code, String fileName, JSObjectRef context)
        {
            JSStringRef codeStr = code.toJSCString();
            JSStringRef fileNameStr = fileName.toJSCString();

            executeScript (codeStr, fileNameStr, context);

            JSStringRelease (codeStr);
            JSStringRelease (fileNameStr);
        }

		void JSCJavascriptEngine::executeScript(String code, String fileName)
		{
			JSStringRef codeStr = code.toJSCString();
			JSStringRef fileNameStr = fileName.toJSCString();

            executeScript (codeStr, fileNameStr);

			JSStringRelease (codeStr);
			JSStringRelease (fileNameStr);
		}

        void JSCJavascriptEngine::executeScript(JSStringRef code, JSStringRef fileName, JSObjectRef context)
        {
            JSValueRef exception = jscCreateException (globalContext);
            JSValueRef result = JSEvaluateScript (globalContext, code, NULL, fileName, 0, &exception);

            if (result == NULL)
                jscHandleException (exception);
        }

		#ifdef C3D_USE_OGRE
		void JSCJavascriptEngine::start3DEngine()
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

		void JSCJavascriptEngine::addTimeout (JSObjectRef func, RJINT time)
		{
			auto fireTime = std::chrono::steady_clock::now();
			fireTime += std::chrono::milliseconds(time);
			
			timers.push_back( std::make_pair(func, fireTime));
		}

		void JSCJavascriptEngine::blockchainEvent(String event, String dataType, void *data)
		{
			/// @todo Run the event.
		}

		/*void JSCJavascriptEngine::blockchainEvent(String event, RJINT numargs, v8::Local<v8::Value> *args, RJBOOL alreadyEnteredCritialSection)
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
				if (JSC::BlockchainV1::onReadyFunction != NULL)
					callFunctionOnNextTick(JSC::BlockchainV1::onReadyFunction, NULL, false);
			}

			if (event == "connectBlock")
			{
				if (JSC::BlockchainV1::connectBlockFunction != NULL)
				{
					v8::Persistent<v8::Array> *results = RJNEW v8::Persistent<v8::Array>();
					v8::Local<v8::Array> ary = v8::Array::New(isolate);

					for (RJINT iIdx = 0; iIdx < numargs; iIdx++)
						ary->Set(iIdx, args[iIdx]);

					results->Reset(V8_JAVASCRIPT_ENGINE->isolate, ary);

					callFunctionOnNextTick(JSC::BlockchainV1::connectBlockFunction, results, true);
				}
			}

			if (event == "proofOfWorkFound")
			{
				if (JSC::BlockchainV1::proofOfWorkFoundFunction != NULL)
					callFunctionOnNextTick(JSC::BlockchainV1::proofOfWorkFoundFunction, NULL, false);
			}

			if (event == "passphraseRequired")
			{
				if (JSC::BlockchainV1::passphraseRequiredFunction != NULL)
					callFunctionOnNextTick(JSC::BlockchainV1::passphraseRequiredFunction, NULL, false);
			}

			if (event == "error")
			{
				if (JSC::BlockchainV1::onErrorFunction != NULL)
				{
					v8::Persistent<v8::Array> *results = RJNEW v8::Persistent<v8::Array>();
					v8::Local<v8::Array> ary = v8::Array::New(isolate);

					for (RJINT iIdx = 0; iIdx < numargs; iIdx++)
						ary->Set(iIdx, args[iIdx]);

					results->Reset(V8_JAVASCRIPT_ENGINE->isolate, ary);

					callFunctionOnNextTick(JSC::BlockchainV1::onErrorFunction, results, true);
				}
			}

			DELETE_ARRAY(args);
			#endif

			#ifdef GUI_USE_WXWIDGETS
				criticalSection->Leave();
			#endif
		}*/

		void JSCJavascriptEngine::addThread(Thread *thread)
		{
			threads.insert (HashMapPair<RJULONG, Thread *> ((RJULONG)thread, thread));
		}

		void JSCJavascriptEngine::removeThread(Thread *thread)
		{
			removeThreads.push_back((RJULONG)thread);
		}

		void JSCJavascriptEngine::throwException(String message)
		{
			/// @todo Complete this.

			RadJav::printToOutputWindow(message);

			if (exceptionsDisplayMessageBox == true)
				RadJav::showMessageBox(message, "Error");

			if (shutdownOnException == true)
				shutdown = true;
		}

		void JSCJavascriptEngine::exit(RJINT exitCode)
		{
			shutdown = true;
		}

		void JSCJavascriptEngine::destroyJSObjects()
		{
		}
    
        void JSCJavascriptEngine::executeScriptNextTick(String code, String fileName, JSObjectRef context)
        {
            jsToExecuteNextCode.push_back(code);
            jsToExecuteNextFilename.push_back(fileName);
            jsToExecuteNextContext.push_back(context);
        }

        void JSCJavascriptEngine::callFunctionOnNextTick(AsyncFunctionCall *call)
        {
            funcs.push_back(call);
        }

        void JSCJavascriptEngine::collectGarbage()
        {
            if (exposeGC == false)
            {
                throw RadJAV::Exception("Unable to manually collect garbage, --expose_gc command line argument not set. Its best not to manually collect garbage anyway.");
                
                return;
            }

            JSGarbageCollect (globalContext);
        }

		void JSCJavascriptEngine::loadNativeCode()
		{
			// Globals
			{
				JSC::Global::createJSCCallbacks(globalContext, globalObj);

				if (exposeGC == true)
                {
					JSC_CCALLBACK(globalContext, globalObj, "collectGarbage", JSC::Global::collectGarbage);
                }
			}

			// RadJav
			{
				JSObjectRef radJavFunc = jscCastValueToObject (jscGetFunction(globalObj, "RadJav"));

				JSC::Console::createJSCCallbacks(globalContext, radJavFunc);
				JSC::Thread::createJSCCallbacks(globalContext, radJavFunc);

				// RadJav.OS
				{
					JSObjectRef osFunc = jscGetFunction(radJavFunc, "OS");

					JSC::OS::createJSCCallbacks(globalContext, osFunc);

					// Command line arguments
					{
						JSValueRef argsVal = jscGetValue(osFunc, "args");
                        JSObjectRef argsObj = jscCastValueToObject(globalContext, argsVal);

						for (RJINT iIdx = 0; iIdx < RadJav::arguments.size(); iIdx++)
						{
							String arg = RadJav::arguments.at(iIdx);
                            JSValueRef argVal = arg.toJSCValue(globalContext);

                            JSObjectSetPropertyAtIndex (globalContext, argsObj, iIdx, argVal, NULL);
                            
						}
					}
				}

				// RadJav.Console
				{
					JSObjectRef consoleFunc = jscGetFunction(radJavFunc, "Console");

                    JSC::Console::createJSCCallbacks(globalContext, consoleFunc, true);
				}
            }

				// RadJav.DB
				#ifdef USE_DATABASE
					{
						v8::Handle<v8::Function> dbFunc = v8GetFunction(radJavFunc, "DB");

						// RadJav.DB.KeyValueStorage
						{
							v8::Handle<v8::Function> keyValueStorageFunc = v8GetFunction(dbFunc, "KeyValueStorage");
							v8::Handle<v8::Object> keyValueStoragePrototype = v8GetObject(keyValueStorageFunc, "prototype");

							JSC::Database::KeyValueStorage::createJSCCallbacks(isolate, keyValueStoragePrototype);
						}
					}
				#endif

				// RadJav.IO
				/*{
					v8::Handle<v8::Function> ioFunc = v8GetFunction(radJavFunc, "IO");

					// RadJav.IO.FileIO
					{
						//v8::Handle<v8::Function> filemFunc = v8GetFunction(ioFunc, "FileIO");
						//v8::Handle<v8::Object> filePrototype = v8GetObject(filemFunc, "prototype");
					}

					// RadJav.IO.SerialComm
					{
						v8::Handle<v8::Function> serialCommFunc = v8GetFunction(ioFunc, "SerialComm");
						v8::Handle<v8::Object> serialPrototype = v8GetObject(serialCommFunc, "prototype");

						JSC::IO::SerialComm::createJSCCallbacks(isolate, serialPrototype);
					}

					// RadJav.IO.TextFile
					{
						v8::Handle<v8::Function> textFileFunc = v8GetFunction(ioFunc, "TextFile");
						v8::Handle<v8::Object> textPrototype = v8GetObject(textFileFunc, "prototype");

						JSC::IO::TextFile::createJSCCallbacks(isolate, textPrototype);
					}

					JSC::IO::createJSCCallbacks(isolate, ioFunc);
				}

				#ifdef HAS_XML_SUPPORT
				// RadJav.XML
				{
					v8::Handle<v8::Function> xmlFunc = v8GetFunction(radJavFunc, "XML");
					v8::Handle<v8::Function> xmlFileFunc = v8GetFunction(xmlFunc, "XMLFile");
					v8::Handle<v8::Object> xmlFilePrototype = v8GetObject(xmlFileFunc, "prototype");

					JSC::IO::XML::XMLFile::createJSCCallbacks(isolate, xmlFilePrototype);
				}
				#endif*/

				// RadJav.Net
				/*{
					v8::Handle<v8::Function> netFunc = v8GetFunction(radJavFunc, "Net");

					JSC::Net::NetCallbacks::createJSCCallbacks(isolate, netFunc);

					// WebServer
					{
						v8::Handle<v8::Function> webServerFunc = v8GetFunction(netFunc, "WebServer");
						v8::Handle<v8::Object> webServerPrototype = v8GetObject(webServerFunc, "prototype");

						JSC::Net::WebServer::createJSCCallbacks(isolate, webServerPrototype);
					}

					// WebSocketServer
					{
						v8::Handle<v8::Function> webSocketServerFunc = v8GetFunction(netFunc, "WebSocketServer");
						v8::Handle<v8::Object> webSocketServerPrototype = v8GetObject(webSocketServerFunc, "prototype");

						JSC::Net::WebSocketServer::createJSCCallbacks(isolate, webSocketServerPrototype);
					}

					// WebSocketClient
					{
						v8::Handle<v8::Function> webSocketClientFunc = v8GetFunction(netFunc, "WebSocketClient");
						v8::Handle<v8::Object> webSocketClientPrototype = v8GetObject(webSocketClientFunc, "prototype");

						JSC::Net::WebSocketClient::createJSCCallbacks(isolate, webSocketClientPrototype);
					}
				}*/

				#ifdef USE_BLOCKCHAIN_V1
				// RadJav.BlockchainV1
				{
					v8::Handle<v8::Function> blockchainFunc = v8GetFunction(radJavFunc, "BlockchainV1");

					JSC::BlockchainV1::createJSCCallbacks(isolate, blockchainFunc);
				}
				#endif

				// RadJav.GUI
				/*{
					v8::Handle<v8::Function> guiFunc = v8GetFunction(radJavFunc, "GUI");

					// RadJav.GUI.GObject
					{
						v8::Handle<v8::Function> gobjectFunc = v8GetFunction(guiFunc, "GObject");
						v8::Handle<v8::Object> gobjectPrototype = v8GetObject(gobjectFunc, "prototype");

						JSC::GUI::GObject::createJSCCallbacks(isolate, gobjectPrototype);
					}

					// RadJav.GUI.Window
					{
						v8::Handle<v8::Function> windowFunc = v8GetFunction(guiFunc, "Window");
						v8::Handle<v8::Object> windowPrototype = v8GetObject(windowFunc, "prototype");

						JSC::GUI::Window::createJSCCallbacks(isolate, windowPrototype);
					}

					// RadJav.GUI.Button
					{
						v8::Handle<v8::Function> buttonFunc = v8GetFunction(guiFunc, "Button");
						v8::Handle<v8::Object> buttonPrototype = v8GetObject(buttonFunc, "prototype");

						JSC::GUI::Button::createJSCCallbacks(isolate, buttonPrototype);
					}

					// RadJav.GUI.Label
					{
						v8::Handle<v8::Function> labelFunc = v8GetFunction(guiFunc, "Label");
						v8::Handle<v8::Object> labelPrototype = v8GetObject(labelFunc, "prototype");

						JSC::GUI::Label::createJSCCallbacks(isolate, labelPrototype);
					}

					// RadJav.GUI.Image
					{
						v8::Handle<v8::Function> imageFunc = v8GetFunction(guiFunc, "Image");
						v8::Handle<v8::Object> imagePrototype = v8GetObject(imageFunc, "prototype");

						JSC::GUI::Image::createJSCCallbacks(isolate, imagePrototype);
					}

					// RadJav.GUI.Container
					{
						v8::Handle<v8::Function> containerFunc = v8GetFunction(guiFunc, "Container");
						v8::Handle<v8::Object> containerPrototype = v8GetObject(containerFunc, "prototype");

						JSC::GUI::Container::createJSCCallbacks(isolate, containerPrototype);
					}

					// RadJav.GUI.Combobox
					{
						v8::Handle<v8::Function> comboboxFunc = v8GetFunction(guiFunc, "Combobox");
						v8::Handle<v8::Object> comboboxPrototype = v8GetObject(comboboxFunc, "prototype");

						JSC::GUI::Combobox::createJSCCallbacks(isolate, comboboxPrototype);
					}

					// RadJav.GUI.Textbox
					{
						v8::Handle<v8::Function> textboxFunc = v8GetFunction(guiFunc, "Textbox");
						v8::Handle<v8::Object> textboxPrototype = v8GetObject(textboxFunc, "prototype");

						JSC::GUI::Textbox::createJSCCallbacks(isolate, textboxPrototype);
					}

					// RadJav.GUI.Textarea
					{
						v8::Handle<v8::Function> textareaFunc = v8GetFunction(guiFunc, "Textarea");
						v8::Handle<v8::Object> textareaPrototype = v8GetObject(textareaFunc, "prototype");

						JSC::GUI::Textarea::createJSCCallbacks(isolate, textareaPrototype);
					}

					// RadJav.GUI.Checkbox
					{
						v8::Handle<v8::Function> checkboxFunc = v8GetFunction(guiFunc, "Checkbox");
						v8::Handle<v8::Object> checkboxPrototype = v8GetObject(checkboxFunc, "prototype");

						JSC::GUI::Checkbox::createJSCCallbacks(isolate, checkboxPrototype);
					}

					// RadJav.GUI.Radio
					{
						v8::Handle<v8::Function> radioFunc = v8GetFunction(guiFunc, "Radio");
						v8::Handle<v8::Object> radioPrototype = v8GetObject(radioFunc, "prototype");

						JSC::GUI::Radio::createJSCCallbacks(isolate, radioPrototype);
					}

					// RadJav.GUI.List
					{
						v8::Handle<v8::Function> listFunc = v8GetFunction(guiFunc, "List");
						v8::Handle<v8::Object> listPrototype = v8GetObject(listFunc, "prototype");

						JSC::GUI::List::createJSCCallbacks(isolate, listPrototype);
					}

					// RadJav.GUI.MenuBar
					{
						v8::Handle<v8::Function> menuBarFunc = v8GetFunction(guiFunc, "MenuBar");
						v8::Handle<v8::Object> menuBarPrototype = v8GetObject(menuBarFunc, "prototype");

						JSC::GUI::MenuBar::createJSCCallbacks(isolate, menuBarPrototype);
					}

					// RadJav.GUI.MenuItem
					{
						v8::Handle<v8::Function> menuItemFunc = v8GetFunction(guiFunc, "MenuItem");
						v8::Handle<v8::Object> menuItemPrototype = v8GetObject(menuItemFunc, "prototype");

						JSC::GUI::MenuItem::createJSCCallbacks(isolate, menuItemPrototype);
					}

					// RadJav.GUI.WebView
					{
						v8::Handle<v8::Function> webViewFunc = v8GetFunction(guiFunc, "WebView");
						v8::Handle<v8::Object> webViewPrototype = v8GetObject(webViewFunc, "prototype");
                        #ifdef WXWIDGETS_HAS_WEBVIEW
                            JSC::GUI::WebView::createJSCCallbacks(isolate, webViewPrototype);
                        #endif
					}

					#ifdef C3D_USE_OGRE
						// RadJav.GUI.Canvas3D
						{
							v8::Handle<v8::Function> canvas3DFunc = v8GetFunction(guiFunc, "Canvas3D");
							v8::Handle<v8::Object> canvas3DFuncPrototype = v8GetObject(canvas3DFunc, "prototype");

							JSC::GUI::Canvas3D::createJSCCallbacks(isolate, canvas3DFuncPrototype);
						}
					#endif
				}*/

				/*#ifdef C3D_USE_OGRE
				// RadJav.C3D
				{
					initV8Callback<JSC::C3D::Transform>(radJavFunc, "C3D", "Transform");
					initV8Callback<JSC::C3D::Object3D>(radJavFunc, "C3D", "Object3D");
					initV8Callback<JSC::C3D::Plane>(radJavFunc, "C3D", "Plane");
					initV8Callback<JSC::C3D::Cube>(radJavFunc, "C3D", "Cube");
					initV8Callback<JSC::C3D::Sphere>(radJavFunc, "C3D", "Sphere");
					initV8Callback<JSC::C3D::Camera>(radJavFunc, "C3D", "Camera");
					initV8Callback<JSC::C3D::Light>(radJavFunc, "C3D", "Light");
					initV8Callback<JSC::C3D::Model>(radJavFunc, "C3D", "Model");
				}
				#endif
				#ifdef USE_CRYPTOGRAPHY
				// RadJav.Crypto.Hash
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {

				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "Hash");
				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::Hash::getCapabilities);
				    //std::cout << "Obj FieldCount: " << func -> InternalFieldCount() << std::endl << std::flush;
				    //std::cout << "Obj ExtFieldCount: " << func -> GetIndexedPropertiesExternalArrayDataLength() << std::endl << std::flush;
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    //std::cout <<  "Len: " << str -> Length() << std::endl << std::flush;
				    //std::cout << "Obj FieldCount: " << prototype -> InternalFieldCount() << std::endl << std::flush;
				    

				    JSC::Crypto::Hash::createJSCCallbacks(isolate, prototype);
				  }

				}


				// RadJav.Crypto.HashMultipart
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "HashMultipart");
				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::HashMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    JSC::Crypto::HashMultipart::createJSCCallbacks(isolate, prototype);
				  }

				}
				// RadJav.Crypto.Cipher
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "Cipher");
				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::Cipher::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    JSC::Crypto::Cipher::createJSCCallbacks(isolate, prototype);
				  }
				}
				// RadJav.Crypto.Decipher
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "Decipher");
				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::Decipher::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    JSC::Crypto::Decipher::createJSCCallbacks(isolate, prototype);
				  }
				}
				// RadJav.Crypto.CipherMultipart
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "CipherMultipart");
				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::CipherMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    JSC::Crypto::CipherMultipart::createJSCCallbacks(isolate, prototype);
				  }
				}


				// RadJav.Crypto.DecipherMultipart
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "DecipherMultipart");
				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::DecipherMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    JSC::Crypto::DecipherMultipart::createJSCCallbacks(isolate, prototype);

				  }
				}

				// RadJav.Crypto.KeyGenerator
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "KeyGenerator");
				    //				    V8_CALLBACK(func, "getCapabilities", JSC::Crypto::DecipherMultipart::getCapabilities);
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");
				    //				    v8::Local<v8::String> str = String("_init").toV8String(isolate);
				    JSC::Crypto::KeyGenerator::createJSCCallbacks(isolate, prototype);
				  }
				}

				// RadJav.Crypto.PrivateKey
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "PrivateKey");
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");

				    JSC::Crypto::PrivateKey::createJSCCallbacks(isolate, prototype);
				    JSC::Crypto::PrivateKey::setConstructor(isolate, func);

				    v8::Handle<v8::Object> init = v8GetObject(prototype, "_init");
				    
				  }
				}

				// RadJav.Crypto.PublicKey
				{
				  v8::Handle<v8::Function> cryptoFunc = v8GetFunction(radJavFunc, "Crypto");

				  {
				    v8::Handle<v8::Function> func = v8GetFunction(cryptoFunc, "PublicKey");
				    v8::Handle<v8::Object> prototype = v8GetObject(func, "prototype");

				    JSC::Crypto::PublicKey::createJSCCallbacks(isolate, prototype);
				    JSC::Crypto::PublicKey::setConstructor(isolate, func);

				  }
				}

				#endif
				
			}*/
		}

		/*void JSCJavascriptEngine::loadTemplates(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			static_cast <JSCJavascriptEngine *> (RadJav::javascriptEngine)->loadNativeCode();
		}

		void JSCJavascriptEngine::runEventLoopSingleStep(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			JSCJavascriptEngine *jsEngine = static_cast <JSCJavascriptEngine *> (RadJav::javascriptEngine);

			while (true)
			{
				/// @todo Pump the JSC event loop.
			}
		}*/
    
        JSObjectRef JSCJavascriptEngine::jscCreateExternal (void *external, std::function<void (JSObjectRef jsObject)> onDelete)
        {
            return (jscCreateExternal (globalContext, external, onDelete));
        }

        JSObjectRef JSCJavascriptEngine::jscCreateExternal (JSContextRef context, void *external, std::function<void (JSObjectRef jsObject)> onDelete)
        {
            //Define External class
            JSClassRef externalClassRef;

            JSClassDefinition externalClassDef = kJSClassDefinitionEmpty;
            externalClassDef.className = "External";
            externalClassDef.attributes = kJSClassAttributeNone;
            externalClassDef.finalize = onDelete.target<void (JSObjectRef jsObject)> ();
            
            externalClassRef = JSClassCreate(&externalClassDef);
            
            //Create new External object with private data pointed to this wrapper object
            JSObjectRef externalJavascriptObject = JSObjectMake(context, externalClassRef, this);
            JSClassRelease(externalClassRef);

            /*JSObjectRef javascriptObject = JSObjectMake(context, NULL, NULL);

            //Assign External class object to requested property
            JSStringRef property = JSStringCreateWithUTF8CString("external");
            JSObjectSetProperty(context, javascriptObject, property, externalJavascriptObject, kJSPropertyAttributeNone, NULL);
            JSStringRelease(property);*/

            return (externalJavascriptObject);
        }
    
        JSValueRef JSCJavascriptEngine::jscCreateException ()
        {
            return (jscCreateException (globalContext));
        }

        JSValueRef JSCJavascriptEngine::jscCreateException (JSContextRef context)
        {
            JSValueRef exception = JSObjectMakeError (globalContext, 0, NULL, NULL);

            return (exception);
        }
    
        RJBOOL JSCJavascriptEngine::jscHandleException (JSValueRef exception)
        {
            return (jscHandleException (globalContext, exception));
        }
    
        /// If necessary, handle an exception.
        RJBOOL JSCJavascriptEngine::jscHandleException (JSContextRef context, JSValueRef exception)
        {
            if (exception == NULL)
                return (false);

            if (JSValueIsNull(context, exception) == false)
            {
                JSObjectRef exObj = jscCastValueToObject(exception);
                String exStr = jscGetString (exObj, "message");
                throwException (exStr);
                
                return (true);
            }
            
            return (false);
        }
    
        /// Cast a value to an object.
        JSObjectRef JSCJavascriptEngine::jscCastValueToObject (JSValueRef value)
        {
            return (jscCastValueToObject (globalContext, value));
        }

        /// Cast a value to an object.
        JSObjectRef JSCJavascriptEngine::jscCastValueToObject (JSContextRef context, JSValueRef value)
        {
			if (!context || !value)
				return nullptr;
			
            JSValueRef exception = nullptr;
            JSObjectRef obj = JSValueToObject (context, value, &exception);

			jscHandleException (exception);

            return (obj);
        }

        JSObjectRef JSCJavascriptEngine::jscCreateArray (RJINT numArgs, JSValueRef args[])
        {
            return (jscCreateArray (globalContext, numArgs, args));
        }

        JSObjectRef JSCJavascriptEngine::jscCreateArray (JSContextRef context, RJINT numArgs, JSValueRef args[])
        {
            JSValueRef exception = NULL;
            JSObjectRef ary = JSObjectMakeArray (context, numArgs, args, &exception);

            jscHandleException(exception);

            return (ary);
        }
    
        /// Cast a value to a RJINT.
        RJINT JSCJavascriptEngine::jscValueToNumber (JSValueRef value)
        {
            return (jscValueToNumber (globalContext, value));
        }
    
        /// Cast a value to a RJINT.
        RJINT JSCJavascriptEngine::jscValueToNumber (JSContextRef context, JSValueRef value)
        {
            JSValueRef exception = NULL;
            RJNUMBER result = JSValueToNumber (context, value, &exception);
            
            jscHandleException (exception);
            
            return (result);
        }
    
        RJBOOL JSCJavascriptEngine::jscIsNull(JSValueRef val)
        {
            return (jscIsNull (globalContext, val));
        }
    
        RJBOOL JSCJavascriptEngine::jscIsNull(JSContextRef context, JSValueRef val)
        {
            RJBOOL isNull = false;

            if (JSValueIsNull(context, val) == true)
                isNull = true;

            if (JSValueIsUndefined (context, val) == true)
                isNull = true;

            return (isNull);
        }

        /// Cast a value to a RJBOOL.
        RJBOOL JSCJavascriptEngine::jscValueToBoolean (JSValueRef value)
        {
            return (jscValueToBoolean (globalContext, value));
        }

        /// Cast a value to a RJBOOL.
        RJBOOL JSCJavascriptEngine::jscValueToBoolean (JSContextRef context, JSValueRef value)
        {
            RJBOOL result = JSValueToBoolean (context, value);
            
            return (result);
        }

        /// Cast a value to a JSStringRef.
        JSStringRef JSCJavascriptEngine::jscValueToJSStringRef (JSValueRef value)
        {
            return (jscValueToJSStringRef (globalContext, value));
        }

        /// Cast a value to a RJBOOL.
        JSStringRef JSCJavascriptEngine::jscValueToJSStringRef (JSContextRef context, JSValueRef value)
        {
            JSValueRef exception = NULL;
            JSStringRef result = JSValueToStringCopy (context, value, &exception);
            
            jscHandleException (exception);
            
            return (result);
        }

        JSObjectRef JSCJavascriptEngine::jscGetFunction (JSObjectRef context, String functionName)
        {
            JSValueRef val = jscGetValue (context, functionName);
            JSObjectRef result = jscCastValueToObject(val);

            return (result);
        }

        JSValueRef JSCJavascriptEngine::jscGetValue (JSObjectRef context, String functionName)
        {
            JSValueRef exception = NULL;
            JSStringRef funcStr = functionName.toJSCString();
            JSValueRef result = JSObjectGetProperty (globalContext, context, funcStr, &exception);

            JSStringRelease(funcStr);
            jscHandleException(exception);

            return (result);
        }

        void JSCJavascriptEngine::jscSetString(JSObjectRef context, String functionName, String str)
        {
            JSValueRef exception = NULL;
            JSStringRef funcStr = functionName.toJSCString();
            JSStringRef strStr = str.toJSCString();
            JSValueRef value = JSValueMakeString (globalContext, strStr);
            JSObjectSetProperty (globalContext, context, funcStr, value, kJSPropertyAttributeNone, &exception);

            JSStringRelease(funcStr);
            JSStringRelease(strStr);

            jscHandleException(exception);
        }

        String JSCJavascriptEngine::jscGetString(JSObjectRef context, String functionName)
        {
            JSValueRef value = jscGetValue (context, functionName);
            String result = parseJSCValue(globalContext, value);

            return (result);
        }

        void JSCJavascriptEngine::jscSetNumber(JSObjectRef context, String functionName, RDECIMAL number)
        {
            JSValueRef exception = NULL;
            JSStringRef funcStr = functionName.toJSCString();
            JSValueRef value = JSValueMakeNumber(globalContext, number);
            JSObjectSetProperty (globalContext, context, funcStr, value, kJSPropertyAttributeNone, &exception);

            JSStringRelease(funcStr);

            jscHandleException(exception);
        }

        RJINT JSCJavascriptEngine::jscGetInt(JSObjectRef context, String functionName)
        {
            JSValueRef value = jscGetValue (context, functionName);
            RJINT result = jscValueToInt (globalContext, value);

            return (result);
        }
    
        RDECIMAL JSCJavascriptEngine::jscGetDecimal(JSObjectRef context, String functionName)
        {
            JSValueRef value = jscGetValue (context, functionName);
            RDECIMAL result = jscValueToNumber (globalContext, value);
            
            return (result);
        }
    
        void JSCJavascriptEngine::jscSetBool(JSObjectRef context, String functionName, bool value)
        {
            JSValueRef exception = NULL;
            JSStringRef funcStr = functionName.toJSCString();
            JSValueRef jsval = JSValueMakeBoolean(globalContext, value);
            JSObjectSetProperty (globalContext, context, funcStr, jsval, kJSPropertyAttributeNone, &exception);

            JSStringRelease(funcStr);

            jscHandleException(exception);
        }

        RJBOOL JSCJavascriptEngine::jscGetBool(JSObjectRef context, String functionName)
        {
            JSValueRef value = jscGetValue (context, functionName);
            RJBOOL result = jscValueToBoolean (globalContext, value);

            return (result);
        }

        void JSCJavascriptEngine::jscSetObject(JSObjectRef context, String functionName, JSObjectRef obj)
        {
            JSValueRef exception = NULL;
            JSStringRef funcStr = functionName.toJSCString();
            JSObjectSetProperty (globalContext, context, funcStr, obj, kJSPropertyAttributeNone, &exception);
            
            JSStringRelease(funcStr);
            
            jscHandleException(exception);
        }
    
        JSObjectRef JSCJavascriptEngine::jscGetObject(JSObjectRef context, String functionName)
        {
            JSValueRef value = jscGetValue (context, functionName);
            JSObjectRef result = jscCastValueToObject(globalContext, value);
            
            return (result);
        }

        JSValueRef JSCJavascriptEngine::jscCallFunction(JSObjectRef context, String functionName, RJINT numArgs, JSValueRef args[])
        {
            JSValueRef exception = jscCreateException (globalContext);
            JSValueRef value = jscGetValue (context, functionName);
            JSObjectRef func = jscCastValueToObject(globalContext, value);

            JSValueRef result = JSObjectCallAsFunction (globalContext, func, context, numArgs, args, &exception);

            if (result == NULL)
                jscHandleException(exception);

            return (result);
        }
    
        JSObjectRef JSCJavascriptEngine::jscCallAsConstructor(JSObjectRef function, RJINT numArgs, JSValueRef args[])
        {
            JSValueRef exception = jscCreateException (globalContext);

            JSObjectRef result = JSObjectCallAsConstructor (globalContext, function, numArgs, args, &exception);

            if (result == NULL)
                jscHandleException(exception);

            return (result);
        }
    
        JSObjectRef JSCJavascriptEngine::jscCreateNewObject(String objectName, RJINT numArgs, JSValueRef args[])
        {
            JSValueRef exception = jscCreateException (globalContext);
            JSObjectRef context = jscGetObjectFromJSClass(objectName);

            JSObjectRef result = JSObjectCallAsConstructor (globalContext, context, numArgs, args, &exception);

            if (result == NULL)
                jscHandleException(exception);

            return (result);
        }

        JSObjectRef JSCJavascriptEngine::jscGetObjectFromJSClass(String objectName, JSContextRef context)
        {
            Array<String> classes = objectName.split(".");
            JSObjectRef foundObj = NULL;

            if (context == NULL)
                foundObj = globalObj;
            else
                foundObj = JSContextGetGlobalObject (context);

            for (RJINT iIdx = 0; iIdx < classes.size(); iIdx++)
            {
                String objClass = classes.at(iIdx);

                foundObj = jscGetObject (foundObj, objClass);
            }

            return (foundObj);
        }

		RDECIMAL JSCJavascriptEngine::jscParseDecimal(JSValueRef val)
		{
			return jscParseDecimal(globalContext, val);
		}

		RDECIMAL JSCJavascriptEngine::jscParseDecimal(JSContextRef context, JSValueRef val)
		{
			double value = JSValueToNumber(context, val, nullptr);
			
			if(std::isnan(value))
				return 0.0;
			
			return value;
		}

        JSObjectRef JSCJavascriptEngine::createPromise(JSObjectRef function)
        {
            return (createPromise (globalObj, function));
        }

        JSObjectRef JSCJavascriptEngine::createPromise(JSObjectRef context, JSObjectRef function, RJINT numArgs, JSValueRef args[])
        {
            JSObjectRef promise = jscGetObject (globalObj, "Promise");
            JSObjectRef keepContext = jscGetFunction (radJav, "keepContext");
            RJINT contextArgs = 2;

            if (numArgs > 0)
                contextArgs = 3;

            JSValueRef *args2 = RJNEW JSValueRef[contextArgs];
            args2[0] = function;
            args2[1] = context;

            if (contextArgs > 2)
                args2[2] = jscCreateArray (numArgs, args);

            JSValueRef exception = jscCreateException (globalContext);
            JSValueRef newContext = JSObjectCallAsFunction(globalContext, keepContext, context, contextArgs, args2, &exception);

            if (newContext == NULL)
                jscHandleException(exception);

            DELETEARRAY(args2);

            JSValueRef *args3 = RJNEW JSValueRef[1];
            args3[0] = newContext;
            JSValueRef result = JSObjectCallAsConstructor(globalContext, promise, 1, args3, &exception);

            if (result == NULL)
                jscHandleException(exception);

            DELETEARRAY(args3);

            JSObjectRef promiseObject = jscCastValueToObject (result);

            return (promiseObject);
        }

		CPP::ChainedPtr* JSCJavascriptEngine::jscGetExternal(JSContextRef context, JSObjectRef handle, String functionName)
		{
			return externalsManager->get(context, handle, functionName);
		}

		void JSCJavascriptEngine::jscSetExternal(JSContextRef context, JSObjectRef handle, String functionName, CPP::ChainedPtr *obj)
		{
			externalsManager->set(context, handle, functionName, obj);
		}

		void JSCJavascriptEngine::jscClearExternal(JSContextRef context, JSObjectRef handle, String functionName)
		{
			externalsManager->clear(context, handle, functionName);
		}
	
		JSValueRef JSCJavascriptEngine::jscGetArgument(const JSValueRef arguments[], RJUINT argumentCount, RJUINT index)
		{
			if (argumentCount == 0 ||
				index >= argumentCount)
			{
				return nullptr;
			}
			
			JSValueRef argument = arguments[index];
			
			return jscIsNull(argument) ? nullptr : argument;
		}

	#endif
}
