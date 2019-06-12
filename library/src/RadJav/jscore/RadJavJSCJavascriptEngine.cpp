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
	#else
		#include <unistd.h>
	#endif
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCGlobal.h"
    #include "jscore/RadJavJSCOS.h"
    #include "jscore/RadJavJSCOSScreenInfo.h"
    #include "jscore/RadJavJSCTesting.h"

	#include "jscore/RadJavJSCIO.h"
    #include "jscore/RadJavJSCConsole.h"
    #include "jscore/RadJavJSCThread.h"

	// Common GUI (desktop/mobile platforms)
	#if defined GUI_USE_WXWIDGETS || defined USE_IOS
		#include "jscore/RadJavJSCGUIGObject.h"
		#include "jscore/RadJavJSCGUIButton.h"
		#include "jscore/RadJavJSCGUILabel.h"
		#include "jscore/RadJavJSCGUIImage.h"
		#include "jscore/RadJavJSCGUITextbox.h"
		#include "jscore/RadJavJSCGUITextarea.h"
		#include "jscore/RadJavJSCGUICheckbox.h"
	#endif

	// Desktop specific GUI
	#ifdef GUI_USE_WXWIDGETS
		#include "jscore/RadJavJSCGUIWindow.h"
		#include "jscore/RadJavJSCGUIContainer.h"
		#include "jscore/RadJavJSCGUICombobox.h"
		#include "jscore/RadJavJSCGUIRadio.h"
		#include "jscore/RadJavJSCGUIList.h"
		#include "jscore/RadJavJSCGUIMenuBar.h"
		#include "jscore/RadJavJSCGUIMenuItem.h"
		#include "jscore/RadJavJSCGUICanvas3D.h"
		#ifdef WXWIDGETS_HAS_WEBVIEW
			#include "jscore/RadJavJSCGUIWebView.h"
		#endif
	#endif

	// Mobile specific GUI
	#ifdef USE_IOS
		#include "jscore/RadJavJSCMUIView.h"
		#include "jscore/RadJavJSCMUINavigator.h"
		#include "jscore/RadJavJSCMUIBottomNavigator.h"
		#include "jscore/RadJavJSCMUITableView.h"
		#include "jscore/RadJavJSCMUITableViewModel.h"
		#include "jscore/RadJavJSCMUIScrollView.h"
		#include "jscore/RadJavJSCMUIWebView.h"
	#endif

	// Database
	#ifdef USE_DATABASE
		#include "jscore/RadJavJSCDBKeyValueStorage.h"
	#endif

	// Net
	#ifdef NET_ON
		#include "jscore/RadJavJSCNet.h"
		#include "jscore/RadJavJSCNetWebServer.h"
		#include "jscore/RadJavJSCNetWebSocket.h"
//		#include "jscore/RadJavJSCNetWebSocket.h"
//		#include "jscore/RadJavJSCNetWebServer.h"
	#endif

	// C3D
	#ifdef C3D_USE_OGRE
		#include "jscore/RadJavJSCC3DTransform.h"
		#include "jscore/RadJavJSCC3DObject3D.h"
		#include "jscore/RadJavJSCC3DPlane.h"
		#include "jscore/RadJavJSCC3DCube.h"
		#include "jscore/RadJavJSCC3DSphere.h"
		#include "jscore/RadJavJSCC3DCamera.h"
		#include "jscore/RadJavJSCC3DLight.h"
		#include "jscore/RadJavJSCC3DModel.h"
	#endif

	// Crypto
	#ifdef USE_CRYPTOGRAPHY
		#include "jscore/RadJavJSCCryptoCipher.h"
		#include "jscore/RadJavJSCCryptoDecipher.h"
		#include "jscore/RadJavJSCCryptoCipherMultipart.h"
		#include "jscore/RadJavJSCCryptoDecipherMultipart.h"
		#include "jscore/RadJavJSCCryptoKeyGenerator.h"
		#include "jscore/RadJavJSCCryptoPrivateKey.h"
		#include "jscore/RadJavJSCCryptoPublicKey.h"
		#include "jscore/RadJavJSCCryptoHash.h"
		#include "jscore/RadJavJSCCryptoHashMultipart.h"
	#endif
#endif

#include "cpp/RadJavCPPIO.h"
#include "cpp/RadJavCPPOS.h"
#include "cpp/RadJavCPPOSScreenInfo.h"

#include <cstring>
#include <fstream>
#include "cpp/RadJavCPPAgent.h"
#include "cpp/RadJavCPPContextManager.h"

//This is here only to bring the mac app to the foreground
#ifdef __APPLE__
	#if TARGET_OS_OSX == 1
		#include <AppKit/NSApplication.h>
	#endif
#endif


namespace RadJAV
{
	#ifdef USE_JAVASCRIPTCORE
        AsyncFunctionCall::AsyncFunctionCall (JSObjectRef newfunc, JSObjectRef newargs, RJBOOL newDeleteOnComplete)
        {
            func = newfunc;
            args = newargs;
            deleteOnComplete = newDeleteOnComplete;
            result = NULL;
        }
    
        AsyncFunctionCall::AsyncFunctionCall(JSObjectRef newfunc, RJINT numArgs, JSValueRef *newargs, RJBOOL newDeleteOnComplete)
        {
            func = newfunc;
            args = JSC_JAVASCRIPT_ENGINE->jscCreateArray(numArgs, newargs);
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

		void bringAppToForeground()
		{
			// This code will bring Mac GUI app to foreground
			#ifdef __APPLE__
				#if TARGET_OS_OSX == 1
					[[NSApplication sharedApplication] activateIgnoringOtherApps:true];
				#endif
			#endif
		}
	
		// WorkNotificator is needed to notify system that app has some activity
		// and system should not slow down our process when we are in the background.
		// Currently we added support for MacOS, maybe later other OS will require such
		// notification as well.
		class WorkNotificator
		{
		public:
			WorkNotificator()
			{
				#ifdef __APPLE__
					#if TARGET_OS_OSX == 1
						NSActivityOptions options = NSActivityBackground;
						activity = [[NSProcessInfo processInfo]
									beginActivityWithOptions:options
									reason:String("mainloop").toNSString()];
						[activity retain];
					#endif
				#endif
			}
			
			~WorkNotificator()
			{
				#ifdef __APPLE__
					#if TARGET_OS_OSX == 1
						[[NSProcessInfo processInfo]
						 endActivity:activity];
						activity = nil;
					#endif
				#endif
			}
		private:
			#ifdef __APPLE__
				#if TARGET_OS_OSX == 1
					NSObject* activity;
				#endif
			#endif
		};
	
		JSCJavascriptEngine::JSCJavascriptEngine()
			: JavascriptEngine()
			, externalsManager(nullptr)
			, workNotificator(nullptr)
		{
			externalsManager = RJNEW ExternalsManager();
			
			contextManager = RJNEW CPP::ContextManager();
			
			String execPath = "";

            #ifdef GUI_USE_WXWIDGETS
                execPath = parsewxString(wxStandardPaths::Get().GetExecutablePath());
            #else
                execPath = RadJAV::_radjav_exec_path;
            #endif

            String flags = "";
			exceptionsDisplayMessageBox = false;
			shutdownOnException = false;

			if (CPP::OS::args.size() > 0)
			{
				for (size_t iIdx = 0; iIdx < CPP::OS::args.size(); iIdx++)
				{
					String endSpace = " ";

					if (iIdx == (CPP::OS::args.size() - 1))
						endSpace = "";

					String arg = CPP::OS::args.at(iIdx);

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
			
			DELETEOBJ(contextManager);

			destroyJSObjects();

			#ifdef GUI_USE_WXWIDGETS
				DELETEOBJ(criticalSection);
			#endif
		}

		int JSCJavascriptEngine::runApplication(String applicationSource, String fileName)
		{
			applicationScriptFilePath = fileName;
			
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

            radJav = jscCastValueToObject (jscGetFunction(globalObj, "RadJav"));

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
					if (!workNotificator)
						workNotificator = RJNEW WorkNotificator();
					
					wxTheApp->Bind( wxEVT_IDLE,
								   &JSCJavascriptEngine::runApplicationInIdleEvent,
								   this,
								   wxID_ANY,
								   wxID_ANY);
				
					// Enter the main loop of app
					exitCode = wxTheApp->OnRun();
				}
			#else
                #ifdef USE_IOS
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                    runApplicationSingleStep();
     
                    });
                #else
                while (runApplicationSingleStep()) {}
               #endif
			#endif

			return exitCode;
		}

        #ifdef GUI_USE_WXWIDGETS
            void JSCJavascriptEngine::runApplicationInIdleEvent(wxIdleEvent& event)
            {
                if(!runApplicationSingleStep())
				{
					DELETEOBJ(workNotificator);
                    return;
				}

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
				// Run ContextManager messages loop
				contextManager->run_one();
				
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

					// This is needed because sometimes app appear in the end of
					// the stack of active windows (at least on macOS)
					bringAppToForeground();

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
				//Now implementation is a bit slow because timers
				//can be removed during execution of timer function itself,
				//so instead of iterating over the timers we search next
				//valid timer by it's ID
				std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
				
				std::vector<RJUINT> timersIds;
				timersIds.reserve(timers.size());
				for(auto mapItem: timers)
				{
					timersIds.push_back(mapItem.first);
				}
				
				for(auto timerId: timersIds)
				{
					auto timer = timers.find(timerId);
					if (timer != timers.end())
					{
						auto timerData = timer->second;
						JSObjectRef funcp = timerData.first;
						std::chrono::time_point<std::chrono::steady_clock> fireTime = timerData.second;
						
						if (fireTime <= currentTime)
						{
							timers.erase(timer);
							
							if (!jscIsNull(funcp))
								JSObjectCallAsFunction(globalContext, funcp, globalObj, 0, 0, nullptr);
							
							JSValueUnprotect(globalContext, funcp);
						}
					}
				}
				
				timersIds.clear();

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

					thread->run();
					
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
                        JSObjectRef argsP = asyncCall->args;
                        RJBOOL deleteOnComplete = asyncCall->deleteOnComplete;
                        RJINT numArgs = 0;
                        JSValueRef *args = jscCreateArray (argsP, &numArgs);

                        if ((JSValueIsUndefined (globalContext, funcp) == false) &&
                            (JSValueIsNull(globalContext, funcp) == false))
                        {
                            JSValueRef exception = jscCreateException (globalContext);
                            JSValueRef result = JSObjectCallAsFunction(globalContext, funcp, globalObj, numArgs, args, &exception);

                            if (result == NULL)
                                jscHandleException(globalContext, exception);

                            asyncCall->result = result;
                        }

                        DELETEARRAY(args);

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
                else {
                    
                #ifdef USE_IOS
                    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.03 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                        runApplicationSingleStep();
                    });
                #endif
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

		RJUINT JSCJavascriptEngine::addTimeout (JSObjectRef func, RJINT time)
		{
			static RJUINT nextTimerId;
			
			auto fireTime = std::chrono::steady_clock::now();
			fireTime += std::chrono::milliseconds(time);

			//Protecting timeout function from GC
			JSValueProtect(globalContext, func);
			
			while(timers.find(nextTimerId) != timers.end())
			{
				nextTimerId++;
			}
			
			timers[nextTimerId] = std::make_pair(func, fireTime);
			return nextTimerId++;
		}

		void JSCJavascriptEngine::clearTimeout(RJUINT timerId)
		{
			auto index = timers.find(timerId);
			if( index != timers.end())
			{
				auto timerData = index->second;
				JSObjectRef funcp = timerData.first;
				timers.erase(index);

				//Unprotecting timeout function
				JSValueUnprotect(globalContext, funcp);
			}
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

		void JSCJavascriptEngine::throwException(JSContextRef ctx, JSValueRef* exception, String message)
		{
			if (!ctx || !exception)
				return;

			size_t argsCount = 0;
			JSValueRef messageValue = nullptr;
			
			if (message.size())
			{
				messageValue = message.toJSCValue(ctx);
				argsCount = 1;
			}
			
			*exception = JSObjectMakeError(ctx, argsCount, &messageValue, nullptr);
			
			throwException(message);
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

		template<class T>
		void JSCJavascriptEngine::initJSCCallback(JSObjectRef parent, const char* nameSpace, const char* typeName)
		{
			JSObjectRef namespaceFunc = jscGetFunction(parent, nameSpace);
			JSObjectRef function = jscGetFunction(namespaceFunc, typeName);
			JSObjectRef prototype = jscGetObject(function, "prototype");

			T::createJSCCallbacks(globalContext, prototype);
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

				JSC_CCALLBACK(globalContext, radJavFunc, "exit", JSC::Global::exit);
				JSC_CCALLBACK(globalContext, radJavFunc, "quit", JSC::Global::exit);
				JSC_CCALLBACK(globalContext, radJavFunc, "include", JSC::Global::include);

				// RadJav.OS
				{
					JSObjectRef osFunc = jscGetFunction(radJavFunc, "OS");

					JSC::OS::createJSCCallbacks(globalContext, osFunc);
                    
                    // RadJav.OS.ScreenInfo
                    {
                        JSObjectRef screenInfoFunc = jscGetFunction(osFunc, "ScreenInfo");

                        for (RJINT iIdx = 0; iIdx < RadJav::screens.size (); iIdx++)
                        {
                            RadJAV::CPP::OS::ScreenInfo screen = RadJav::screens.at (iIdx);
                            JSObjectRef jsScreen = screen.toJSCObject();
                            JSValueRef args[1];

                            args[0] = jsScreen;

                            jscCallFunction(radJavFunc, "addScreen", 1, args);
                        }

                        // This loads only the static functions.
                        JSC::OS::ScreenInfo::createJSCCallbacks(globalContext, screenInfoFunc);
                    }

					// RadJav.OS.SystemProcess
					{
						JSObjectRef systemProcessFunc = jscGetFunction(osFunc, "SystemProcess");
						JSObjectRef systemProcessFuncPrototype = jscGetObject(systemProcessFunc, "prototype");

						JSC::OS::SystemProcess::createJSCCallbacks(globalContext, systemProcessFuncPrototype);
					}

					// Command line arguments
					{
						JSObjectRef argsArray = convertArrayToJSCArray(RadJav::arguments, globalContext);
						jscSetObject(osFunc, "args", argsArray);
					}
					
					// The application main file path
					{
						jscSetString(osFunc, "executingFile", applicationScriptFilePath);
					}
				}

                // RadJav.Testing
                {
                    JSObjectRef testingFunc = jscGetFunction(radJavFunc, "Testing");

                    // RadJav.Testing.KeyboardSimulator
                    {
                        JSObjectRef keyboardSimulatorFunc = jscGetFunction(testingFunc, "KeyboardSimulator");
                        JSC::Testing::KeyboardSimulator::createJSCCallbacks(globalContext, keyboardSimulatorFunc);
                    }

                    // RadJav.Testing.MouseSimulator
                    {
                        JSObjectRef mouseSimulatorFunc = jscGetFunction(testingFunc, "MouseSimulator");
                        JSC::Testing::MouseSimulator::createJSCCallbacks(globalContext, mouseSimulatorFunc);
                    }
                }

				// RadJav.Console
				{
					JSObjectRef consoleFunc = jscGetFunction(radJavFunc, "Console");

                    JSC::Console::createJSCCallbacks(globalContext, consoleFunc, true);
				}

				// RadJav.DB
				#ifdef USE_DATABASE
					{
						JSObjectRef dbFunc = jscGetFunction(radJavFunc, "DB");

						// RadJav.DB.KeyValueStorage
						{
							JSObjectRef keyValueStorageFunc = jscGetFunction(dbFunc, "KeyValueStorage");
							JSObjectRef keyValueStoragePrototype = jscGetObject(keyValueStorageFunc, "prototype");

							JSC::Database::KeyValueStorage::createJSCCallbacks(globalContext, keyValueStoragePrototype);
						}
					}
				#endif

				// RadJav.IO
				{
					JSObjectRef ioFunc = jscGetFunction(radJavFunc, "IO");

					// RadJav.IO.FileIO
					{
						//JSObjectRef filemFunc = jscGetFunction(ioFunc, "FileIO");
						//JSObjectRef filePrototype = jscGetObject(filemFunc, "prototype");
					}

					// RadJav.IO.SerialComm
					{
						JSObjectRef serialCommFunc = jscGetFunction(ioFunc, "SerialComm");
						JSObjectRef serialPrototype = jscGetObject(serialCommFunc, "prototype");

						JSC::IO::SerialComm::createJSCCallbacks(globalContext, serialPrototype);
					}

					// RadJav.IO.TextFile
					{
						JSObjectRef textFileFunc = jscGetFunction(ioFunc, "TextFile");
						JSObjectRef textPrototype = jscGetObject(textFileFunc, "prototype");

						JSC::IO::TextFile::createJSCCallbacks(globalContext, textPrototype);
					}

					JSC::IO::createJSCCallbacks(globalContext, ioFunc);
				}

				// RadJav.XML
				#ifdef HAS_XML_SUPPORT
				{
					JSObjectRef xmlFunc = jscGetFunction(radJavFunc, "XML");
					JSObjectRef xmlFileFunc = jscGetFunction(xmlFunc, "XMLFile");
					JSObjectRef xmlFilePrototype = jscGetObject(xmlFileFunc, "prototype");

					JSC::IO::XML::XMLFile::createJSCCallbacks(globalContext, xmlFilePrototype);
				}
				#endif

				// RadJav.Net
				#ifdef NET_ON
				{
					JSObjectRef netFunc = jscGetFunction(radJavFunc, "Net");

					JSC::Net::NetCallbacks::createJSCCallbacks(globalContext, netFunc);

					// WebServer
					{
						JSObjectRef webServerFunc = jscGetFunction(netFunc, "WebServer");
						JSObjectRef webServerPrototype = jscGetObject(webServerFunc, "prototype");

						JSC::Net::WebServer::createJSCCallbacks(globalContext, webServerPrototype);
					}

					// WebSocketConnection
					{
						JSObjectRef webSocketConnectionFunc = jscGetFunction(netFunc, "WebSocketConnection");
						JSObjectRef webSocketConnectionPrototype = jscGetObject(webSocketConnectionFunc, "prototype");
						
						JSC::Net::WebSocketConnection::createJSCCallbacks(globalContext, webSocketConnectionPrototype);
					}
					
//					// WebSocketServer
//					{
//						JSObjectRef webSocketServerFunc = jscGetFunction(netFunc, "WebSocketServer");
//						JSObjectRef webSocketServerPrototype = jscGetObject(webSocketServerFunc, "prototype");
//
//						JSC::Net::WebSocketServer::createJSCCallbacks(globalContext, webSocketServerPrototype);
//					}
//
//					// WebSocketClient
//					{
//						JSObjectRef webSocketClientFunc = jscGetFunction(netFunc, "WebSocketClient");
//						JSObjectRef webSocketClientPrototype = jscGetObject(webSocketClientFunc, "prototype");
//
//						JSC::Net::WebSocketClient::createJSCCallbacks(globalContext, webSocketClientPrototype);
//					}
				}
				#endif
				
				// RadJav.BlockchainV1
				#ifdef USE_BLOCKCHAIN_V1
				{
					JSObjectRef blockchainFunc = jscGetFunction(radJavFunc, "BlockchainV1");

					JSC::BlockchainV1::createJSCCallbacks(globalContext, blockchainFunc);
				}
				#endif

				// RadJav.GUI common controls for desktop/mobile
				#if defined GUI_USE_WXWIDGETS || defined USE_IOS
				{
					JSObjectRef guiFunc = jscGetFunction(radJavFunc, "GUI");

					// RadJav.GUI.GObject
					{
						JSObjectRef gobjectFunc = jscGetFunction(guiFunc, "GObject");
						JSObjectRef gobjectPrototype = jscGetObject(gobjectFunc, "prototype");
						
						JSC::GUI::GObject::createJSCCallbacks(globalContext, gobjectPrototype);
					}
					
					// RadJav.GUI.Button
					{
						JSObjectRef buttonFunc = jscGetFunction(guiFunc, "Button");
						JSObjectRef buttonPrototype = jscGetObject(buttonFunc, "prototype");
						
						JSC::GUI::Button::createJSCCallbacks(globalContext, buttonPrototype);
					}

					// RadJav.GUI.Label
					{
						JSObjectRef labelFunc = jscGetFunction(guiFunc, "Label");
						JSObjectRef labelPrototype = jscGetObject(labelFunc, "prototype");
						
						JSC::GUI::Label::createJSCCallbacks(globalContext, labelPrototype);
					}
					
					// RadJav.GUI.Image
					{
						JSObjectRef imageFunc = jscGetFunction(guiFunc, "Image");
						JSObjectRef imagePrototype = jscGetObject(imageFunc, "prototype");
						
						JSC::GUI::Image::createJSCCallbacks(globalContext, imagePrototype);
					}
					
					// RadJav.GUI.Textbox
					{
						JSObjectRef textboxFunc = jscGetFunction(guiFunc, "Textbox");
						JSObjectRef textboxPrototype = jscGetObject(textboxFunc, "prototype");
						
						JSC::GUI::Textbox::createJSCCallbacks(globalContext, textboxPrototype);
					}
					
					// RadJav.GUI.Textarea
					{
						JSObjectRef textareaFunc = jscGetFunction(guiFunc, "Textarea");
						JSObjectRef textareaPrototype = jscGetObject(textareaFunc, "prototype");
						
						JSC::GUI::Textarea::createJSCCallbacks(globalContext, textareaPrototype);
					}
					
					// RadJav.GUI.Checkbox
					{
						JSObjectRef checkboxFunc = jscGetFunction(guiFunc, "Checkbox");
						JSObjectRef checkboxPrototype = jscGetObject(checkboxFunc, "prototype");
						
						JSC::GUI::Checkbox::createJSCCallbacks(globalContext, checkboxPrototype);
					}
				}
				#endif
				
				// RadJav.GUI desktop specific controls
				#ifdef GUI_USE_WXWIDGETS
				{
					JSObjectRef guiFunc = jscGetFunction(radJavFunc, "GUI");

					// RadJav.GUI.Window
					{
						JSObjectRef windowFunc = jscGetFunction(guiFunc, "Window");
						JSObjectRef windowPrototype = jscGetObject(windowFunc, "prototype");

						JSC::GUI::Window::createJSCCallbacks(globalContext, windowPrototype);
					}

					// RadJav.GUI.Container
					{
						JSObjectRef containerFunc = jscGetFunction(guiFunc, "Container");
						JSObjectRef containerPrototype = jscGetObject(containerFunc, "prototype");

						JSC::GUI::Container::createJSCCallbacks(globalContext, containerPrototype);
					}

					// RadJav.GUI.Combobox
					{
						JSObjectRef comboboxFunc = jscGetFunction(guiFunc, "Combobox");
						JSObjectRef comboboxPrototype = jscGetObject(comboboxFunc, "prototype");

						JSC::GUI::Combobox::createJSCCallbacks(globalContext, comboboxPrototype);
					}

					// RadJav.GUI.Radio
					{
						JSObjectRef radioFunc = jscGetFunction(guiFunc, "Radio");
						JSObjectRef radioPrototype = jscGetObject(radioFunc, "prototype");

						JSC::GUI::Radio::createJSCCallbacks(globalContext, radioPrototype);
					}

					// RadJav.GUI.List
					{
						JSObjectRef listFunc = jscGetFunction(guiFunc, "List");
						JSObjectRef listPrototype = jscGetObject(listFunc, "prototype");

						JSC::GUI::List::createJSCCallbacks(globalContext, listPrototype);
					}

					// RadJav.GUI.MenuBar
					{
						JSObjectRef menuBarFunc = jscGetFunction(guiFunc, "MenuBar");
						JSObjectRef menuBarPrototype = jscGetObject(menuBarFunc, "prototype");

						JSC::GUI::MenuBar::createJSCCallbacks(globalContext, menuBarPrototype);
					}

					// RadJav.GUI.MenuItem
					{
						JSObjectRef menuItemFunc = jscGetFunction(guiFunc, "MenuItem");
						JSObjectRef menuItemPrototype = jscGetObject(menuItemFunc, "prototype");

						JSC::GUI::MenuItem::createJSCCallbacks(globalContext, menuItemPrototype);
					}

					// RadJav.GUI.WebView
					#ifdef WXWIDGETS_HAS_WEBVIEW
					{
						JSObjectRef webViewFunc = jscGetFunction(guiFunc, "WebView");
						JSObjectRef webViewPrototype = jscGetObject(webViewFunc, "prototype");
						JSC::GUI::WebView::createJSCCallbacks(globalContext, webViewPrototype);
					}
					#endif

					// RadJav.GUI.Canvas3D
					#ifdef C3D_USE_OGRE
					{
						JSObjectRef canvas3DFunc = jscGetFunction(guiFunc, "Canvas3D");
						JSObjectRef canvas3DFuncPrototype = jscGetObject(canvas3DFunc, "prototype");
						
						JSC::GUI::Canvas3D::createJSCCallbacks(globalContext, canvas3DFuncPrototype);
					}
					#endif
				}
				#endif

				// RadJav.MUI mobile specific controls
				#ifdef USE_IOS
				{
					JSObjectRef muiFunc = jscGetFunction(radJavFunc, "MUI");

					// RadJav.MUI.View
					{
						JSObjectRef viewFunc = jscGetFunction(muiFunc, "View");
						JSObjectRef viewPrototype = jscGetObject(viewFunc, "prototype");
						
						JSC::MUI::View::createJSCCallbacks(globalContext, viewPrototype);
					}

					// RadJav.MUI.Navigator
					{
						JSObjectRef navigatorFunc = jscGetFunction(muiFunc, "Navigator");
						JSObjectRef navigatorPrototype = jscGetObject(navigatorFunc, "prototype");

						JSC::MUI::Navigator::createJSCCallbacks(globalContext, navigatorPrototype);
					}
					
					// RadJav.MUI.BottomNavigator
					{
						JSObjectRef navigatorFunc = jscGetFunction(muiFunc, "BottomNavigator");
						JSObjectRef navigatorPrototype = jscGetObject(navigatorFunc, "prototype");
						
						JSC::MUI::BottomNavigator::createJSCCallbacks(globalContext, navigatorPrototype);
					}

                    // RadJav.MUI.TableView
                    {
                        JSObjectRef viewFunc = jscGetFunction(muiFunc, "TableView");
                        JSObjectRef viewPrototype = jscGetObject(viewFunc, "prototype");
                        
                        JSC::MUI::TableView::createJSCCallbacks(globalContext, viewPrototype);
                    }
                    
                    // RadJav.MUI.TableViewModel
                    {
                        JSObjectRef viewFunc = jscGetFunction(muiFunc, "TableViewModel");
                        JSObjectRef viewPrototype = jscGetObject(viewFunc, "prototype");
                        
                        JSC::MUI::TableViewModel::createJSCCallbacks(globalContext, viewPrototype);
                    }
                    
					// RadJav.MUI.ScrollView
					{
						JSObjectRef scrollviewFunc = jscGetFunction(muiFunc, "ScrollView");
						JSObjectRef scrollviewPrototype = jscGetObject(scrollviewFunc, "prototype");
						
						JSC::MUI::ScrollView::createJSCCallbacks(globalContext, scrollviewPrototype);
					}

                    // RadJav.MUI.WebView
                    {
                        JSObjectRef webViewFunc = jscGetFunction(muiFunc, "WebView");
                        JSObjectRef webViewPrototype = jscGetObject(webViewFunc, "prototype");
                        
                        JSC::MUI::WebView::createJSCCallbacks(globalContext, webViewPrototype);
                    }
				}
				#endif

				// RadJav.C3D
				#ifdef C3D_USE_OGRE
				{
					initJSCCallback<JSC::C3D::Transform>(radJavFunc, "C3D", "Transform");
					initJSCCallback<JSC::C3D::Object3D>(radJavFunc, "C3D", "Object3D");
					initJSCCallback<JSC::C3D::Plane>(radJavFunc, "C3D", "Plane");
					initJSCCallback<JSC::C3D::Cube>(radJavFunc, "C3D", "Cube");
					initJSCCallback<JSC::C3D::Sphere>(radJavFunc, "C3D", "Sphere");
					initJSCCallback<JSC::C3D::Camera>(radJavFunc, "C3D", "Camera");
					initJSCCallback<JSC::C3D::Light>(radJavFunc, "C3D", "Light");
					initJSCCallback<JSC::C3D::Model>(radJavFunc, "C3D", "Model");
				}
				#endif

				#ifdef USE_CRYPTOGRAPHY
				// RadJav.Crypto
				{
					JSObjectRef cryptoFunc = jscGetFunction(radJavFunc, "Crypto");

					// Callbacks
					{
						JSC::Crypto::Base::createJSCCallbacks(globalContext, cryptoFunc);
					}

					// RadJav.Crypto.Hash
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "Hash");
						JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::Hash::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::Hash::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.HashMultipart
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "HashMultipart");
						JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::HashMultipart::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::HashMultipart::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.Cipher
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "Cipher");
						JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::Cipher::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::Cipher::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.Decipher
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "Decipher");
						JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::Decipher::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::Decipher::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.CipherMultipart
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "CipherMultipart");
						JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::CipherMultipart::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::CipherMultipart::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.DecipherMultipart
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "DecipherMultipart");
						JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::DecipherMultipart::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::DecipherMultipart::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.KeyGenerator
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "KeyGenerator");
						//JSC_CCALLBACK(globalContext, func, "getCapabilities", JSC::Crypto::DecipherMultipart::getCapabilities);
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::KeyGenerator::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.PrivateKey
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "PrivateKey");
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::PrivateKey::createJSCCallbacks(globalContext, prototype);
					}

					// RadJav.Crypto.PublicKey
					{
						JSObjectRef func = jscGetFunction(cryptoFunc, "PublicKey");
						JSObjectRef prototype = jscGetObject(func, "prototype");
						JSC::Crypto::PublicKey::createJSCCallbacks(globalContext, prototype);
					}
				}
				#endif
			}
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

        JSValueRef *JSCJavascriptEngine::jscCreateArray (JSObjectRef args, RJINT *numArgs)
        {
            return (jscCreateArray (globalContext, args, numArgs));
        }

        JSValueRef *JSCJavascriptEngine::jscCreateArray (JSContextRef context, JSObjectRef args, RJINT *numArgs)
        {
            RJINT length = jscGetInt (args, "length");
            JSValueRef *ary = RJNEW JSValueRef[length];

            for (RJINT iIdx = 0; iIdx < length; iIdx++)
            {
                JSValueRef elm = jscGetElement (context, args, iIdx);
                ary[iIdx] = elm;
            }

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
			
			if (jscIsNull(value))
				return nullptr;
			
            return (jscCastValueToObject(globalContext, value));
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

        RJBOOL JSCJavascriptEngine::jscParseBool(JSValueRef val)
        {
            return jscParseBool(globalContext, val);
        }

        RJBOOL JSCJavascriptEngine::jscParseBool(JSContextRef context, JSValueRef val)
        {
            RJBOOL value = JSValueToBoolean(context, val);

            return value;
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

        RJINT JSCJavascriptEngine::jscParseInt(JSValueRef val)
        {
            return (jscParseInt(globalContext, val));
        }

        RJINT JSCJavascriptEngine::jscParseInt(JSContextRef context, JSValueRef val)
        {
            RJINT value = JSValueToNumber(context, val, nullptr);
            
            if(std::isnan(value))
                return 0.0;
            
            return value;
        }

		std::vector<std::string> JSCJavascriptEngine::jscGetObjectPropertyNames(JSObjectRef context)
		{
			std::vector<std::string> properties;
			
			if (!context)
				return properties;
			
			JSPropertyNameArrayRef propertiesJs = JSObjectCopyPropertyNames(globalContext, context);
			std::size_t propertiesSize= JSPropertyNameArrayGetCount(propertiesJs);
			
			for(int i = 0; i < propertiesSize; i++)
			{
				JSStringRef propertyNameJs = JSPropertyNameArrayGetNameAtIndex(propertiesJs, i);
				properties.push_back(parseJSCString(globalContext, propertyNameJs));
				JSStringRelease(propertyNameJs);
			}

			JSPropertyNameArrayRelease(propertiesJs);

			return properties;
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
    
        JSValueRef JSCJavascriptEngine::jscGetElement(JSObjectRef array, RJUINT index)
        {
            return (jscGetElement (globalContext, array, index));
        }

        JSValueRef JSCJavascriptEngine::jscGetElement(JSContextRef context, JSObjectRef array, RJUINT index)
        {
            JSValueRef elm = JSObjectGetPropertyAtIndex (context, array, index, NULL);

            return (elm);
        }

		JSValueRef JSCJavascriptEngine::jscGetArgument(const JSValueRef arguments[], RJULONG argumentCount, RJULONG index)
		{
			if (argumentCount == 0 ||
				index >= argumentCount)
			{
				return nullptr;
			}
			
			JSValueRef argument = arguments[index];
			
			return jscIsNull(argument) ? nullptr : argument;
		}
	
		String JSCJavascriptEngine::jscGetArgumentAsString(JSContextRef ctx, const JSValueRef arguments[], RJULONG argumentCount, RJULONG index)
		{
			String data;
			
			JSValueRef argument = jscGetArgument(arguments, argumentCount, index);
			if (!argument)
				return data;
			
			if (JSValueIsString(ctx, argument))
			{
				data = parseJSCValue(ctx, argument);
			}
			else if (JSValueIsArray(ctx, argument))
			{
				// TODO add support for arrays
			}
			else if (JSValueIsObject(ctx, argument))
			{
				//Convert to ArrayBuffer object
				JSObjectRef arrayBufferObject = JSValueToObject(ctx, argument, nullptr);
				
				const size_t dataSize = JSObjectGetArrayBufferByteLength(ctx, arrayBufferObject, nullptr);
				if (dataSize)
				{
					const char *dataPtr = static_cast<const char*>(JSObjectGetArrayBufferBytesPtr(ctx, arrayBufferObject, nullptr));
					data = String(dataPtr, dataSize);
				}
			}
			
			return data;
		}


	#endif
}
