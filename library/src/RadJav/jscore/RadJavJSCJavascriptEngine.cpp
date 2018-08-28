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
    #include "jscore/RadJavJSCConsole.h"
#endif

#include "cpp/RadJavCPPIO.h"

#include <cstring>
#include <fstream>
#include "cpp/RadJavCPPAgent.h"

namespace RadJAV
{
	#ifdef USE_JAVASCRIPTCORE
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
			/*for (RJUINT iIdx = 0; iIdx < javascriptFiles.size(); iIdx++)
			{
				JSFile jsfile = javascriptFiles.at(iIdx);
				String contentStr = jsfile.getContent();

				executeScript(contentStr, jsfile.filename);
			}*/

			loadNativeCode();
			
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

							try
							{
                                // Parse JSON here.
							}
							catch (Exception ex)
							{
								RadJav::showError(ex.getMessage(), true);
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
			auto execCodeEnd = jsToExecuteNextCode.end();
			
			try
			{
				// Handle the on ready function.
				if (firstRun == true)
				{
                    /// @todo Check if RadJav::OS has a function, if so, execute it.
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
				
				/// @todo Handle timers.

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

                    /// @todo Execute any scripts.

					execCodeBegin++;
					execFilenameBegin++;
				}
				
				jsToExecuteNextCode.clear();
				jsToExecuteNextFilename.clear();
				
				#ifdef GUI_USE_WXWIDGETS
				if (criticalSection->TryEnter() == true)
				{
				#endif

					/// @todo Handle any functions that need to be executed from a thread.

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
			/// @todo Complete this.
		}

		void JSCJavascriptEngine::executeScript(String code, String fileName)
		{
            JSValueRef exception;
            JSStringRef codeStr = code.toJSCString();
            JSStringRef fileNameStr = fileName.toJSCString();
            JSValueRef result = JSEvaluateScript (globalContext,
                codeStr, NULL, fileNameStr, 0, &exception);

            JSStringRelease(codeStr);
            JSStringRelease (fileNameStr);

            if (JSValueIsNull(globalContext, exception) == false)
            {
                String exStr = parseJSCValue(globalContext, exception);
                throwException (exStr);
            }
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

        /// @todo Do the JavaScriptCore version of this.
		/*void JSCJavascriptEngine::addTimeout (v8::Persistent<v8::Function> *func, RJINT time)
		{
			auto fireTime = std::chrono::steady_clock::now();
			fireTime += std::chrono::milliseconds(time);
			
			timers.push_back( std::make_pair(func, fireTime));
		}*/

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
    
        void JSCJavascriptEngine::collectGarbage()
        {
        }

		void JSCJavascriptEngine::loadNativeCode()
		{
            /// @todo Convert this all to JSC.
			// Globals
			{
				JSC::Global::createJSCCallbacks(globalContext, globalObj);

				if (exposeGC == true)
				{
					//V8_CALLBACK(globalContext->Global(), "collectGarbage", JSC::Global::collectGarbage);
				}
			}

			// RadJav
			/*{
				v8::Handle<v8::Function> radJavFunc = v8GetFunction(globalContext->Global(), "RadJav");

				JSC::Console::createJSCCallbacks(isolate, radJavFunc);
				JSC::Thread::createJSCCallbacks(isolate, radJavFunc);

				V8_CALLBACK(radJavFunc, "exit", JSC::Global::exit);
				V8_CALLBACK(radJavFunc, "quit", JSC::Global::exit);

				// RadJav.OS
				{
					v8::Handle<v8::Function> osFunc = v8GetFunction(radJavFunc, "OS");

					JSC::OS::createJSCCallbacks(isolate, osFunc);

					// Command line arguments
					{
						v8::Handle<v8::Array> args = v8::Handle<v8::Array>::Cast (v8GetValue(osFunc, "args"));

						for (RJINT iIdx = 0; iIdx < RadJav::arguments.size(); iIdx++)
						{
							String arg = RadJav::arguments.at(iIdx);

							args->Set(iIdx, arg.toV8String(isolate));
						}
					}
				}*/

				// RadJav.Console
				{
					//v8::Handle<v8::Function> consoleFunc = v8GetFunction(radJavFunc, "Console");

					//JSC::Console::createJSCCallbacks(isolate, consoleFunc);
                    JSC::Console::createJSCCallbacks(globalContext, globalObj, true);
				}

				// RadJav.DB
				/*#ifdef USE_DATABASE
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
				{
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
				#endif

				// RadJav.Net
				{
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
				}

				#ifdef USE_BLOCKCHAIN_V1
				// RadJav.BlockchainV1
				{
					v8::Handle<v8::Function> blockchainFunc = v8GetFunction(radJavFunc, "BlockchainV1");

					JSC::BlockchainV1::createJSCCallbacks(isolate, blockchainFunc);
				}
				#endif

				// RadJav.GUI
				{
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
				}

				#ifdef C3D_USE_OGRE
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

		CPP::ChainedPtr* JSCJavascriptEngine::getExternal(JSObjectRef context, String functionName)
		{
			return externalsManager->get(context, functionName);
		}

		void JSCJavascriptEngine::setExternal(JSObjectRef context, String functionName, CPP::ChainedPtr *obj)
		{
			externalsManager->set(context, functionName, obj);
		}

		void JSCJavascriptEngine::clearExternal(JSObjectRef context, String functionName)
		{
			externalsManager->clear(context, functionName);
		}


        /// @todo Create the JSC version.
		/*v8::Local<v8::Object> JSCJavascriptEngine::createPromise(v8::Local<v8::Function> function)
		{
			v8::Local<v8::Object> context = globalContext->Global();

			return (createPromise(context, function));
		}

		v8::Local<v8::Object> JSCJavascriptEngine::createPromise(
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
		}*/

	#endif
}
