# Main RadJav
set (SOURCES_files_RADJAV__Main__Sources
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJav.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavC.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavHashMap.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavArray.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavString.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavDate.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavThread.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavLang.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavTheme.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavJSExternals.cpp")
source_group ("RadJav\\Main" FILES ${SOURCES_files_RADJAV__Main__Sources})

set (SOURCES ${SOURCES_files_RADJAV__Main__Sources} ${SOURCES})

# Main RadJav Android
if (ANDROID)
	set (SOURCES_files_RADJAV__Android__Sources
		"${libRadJav_SOURCE_DIR}/src/RadJav/android/Jni.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/android/ClassesCache.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/android/Utils.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/android/RadJavAndroid.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/android/UiThreadCallbackFunction.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/android/NativeCallbackFunction.cpp")
	source_group ("RadJav\\Main\\Android" FILES ${SOURCES_files_RADJAV__Android__Sources})
	
	set (SOURCES ${SOURCES_files_RADJAV__Android__Sources} ${SOURCES})
endif ()

# wxWidgets
set (SOURCES_files_wxWidgets__Main__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavWxWidgets.cpp")
source_group ("RadJav\\wxWidgets" FILES ${SOURCES_files_wxWidgets__Main__Sources})

set (SOURCES ${SOURCES_files_wxWidgets__Main__Sources} ${SOURCES})

if (USE_V8)
	# V8 JavaScript Engine
	set (SOURCES_files_RadJav__v8__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8JavascriptEngine.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8ExternalsDetails.cpp")
	source_group ("RadJav\\V8" FILES ${SOURCES_files_RadJav__v8__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Sources} ${SOURCES})
	
	# V8 common GUI
	if (USE_WXWIDGETS OR IS_MOBILE)
		set (SOURCES_files_RadJav__v8__GUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIGObject.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIButton.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUILabel.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIImage.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUITextbox.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUITextarea.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUICheckbox.cpp")
		source_group ("RadJav\\V8\\GUI" FILES ${SOURCES_files_RadJav__v8__GUI__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__GUI__Sources} ${SOURCES})
	endif ()
	
	# V8 GUI
	if (USE_WXWIDGETS)
		set (SOURCES_files_RadJav__v8__GUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIWindow.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIContainer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUICombobox.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIRadio.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIList.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIMenuBar.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIMenuItem.cpp")
		# V8 GUI WebView
		if (USE_WXWIDGETS_WEBVIEW)
			set (SOURCES_files_RadJav__v8__GUI__Sources
				"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUIWebView.cpp"
				${SOURCES_files_RadJav__v8__GUI__Sources})
		endif ()
		# V8 GUI Canvas3D
		if (USE_OGRE)
			set (SOURCES_files_RadJav__v8__GUI__Sources
				"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8GUICanvas3D.cpp"
				${SOURCES_files_RadJav__v8__GUI__Sources})
		endif ()
		source_group ("RadJav\\V8\\GUI" FILES ${SOURCES_files_RadJav__v8__GUI__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__GUI__Sources} ${SOURCES})
	endif ()

	# V8 MUI
	if (IS_MOBILE)
		set (SOURCES_files_RadJav__v8__MUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8MUIView.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8MUINavigator.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8MUITableView.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8MUITableViewModel.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8MUIScrollView.cpp"
			#"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8MUIWebView.cpp"
			)

		source_group ("RadJav\\V8\\MUI" FILES ${SOURCES_files_RadJav__v8__MUI__Sources})

		set (SOURCES ${SOURCES_files_RadJav__v8__MUI__Sources} ${SOURCES})
	endif ()

	# V8 C3D
	if (USE_OGRE)
		set (SOURCES_files_RadJav__v8__C3D__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DTransform.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DObject3D.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DPlane.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DCube.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DSphere.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DCamera.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DLight.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8C3DModel.cpp")
		source_group ("RadJav\\V8\\C3D" FILES ${SOURCES_files_RadJav__v8__C3D__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__C3D__Sources} ${SOURCES})
	endif ()
	
	# V8 Global
	set (SOURCES_files_RadJav__v8__Global__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8Global.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8Thread.cpp")
	source_group ("RadJav\\V8\\Global" FILES ${SOURCES_files_RadJav__v8__Global__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Global__Sources} ${SOURCES})

	# V8 OS
	set (SOURCES_files_RadJav__v8__OS__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8OS.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8OSScreenInfo.cpp")
	source_group ("RadJav\\V8\\OS" FILES ${SOURCES_files_RadJav__v8__OS__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__OS__Sources} ${SOURCES})

	# V8 Testing
	set (SOURCES_files_RadJav__v8__Testing__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8Testing.cpp")
	source_group ("RadJav\\V8\\Testing" FILES ${SOURCES_files_RadJav__v8__Testing__Sources})

	set (SOURCES ${SOURCES_files_RadJav__v8__Testing__Sources} ${SOURCES})

	# V8 Console
	set (SOURCES_files_RadJav__v8__Console__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8Console.cpp")
	source_group ("RadJav\\V8\\Console" FILES ${SOURCES_files_RadJav__v8__Console__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Console__Sources} ${SOURCES})
	
	# V8 IO
	set (SOURCES_files_RadJav__v8__IO__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8IO.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8IOXML.cpp")
	source_group ("RadJav\\V8\\IO" FILES ${SOURCES_files_RadJav__v8__IO__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__IO__Sources} ${SOURCES})
	
	# V8 Networking
	if (libRadJav_ALLOW_NETWORKING)
		set (SOURCES_files_RadJav__v8__Net__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8Net.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetHttpRequest.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetWebServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetWebSocket.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetWebSocketSsl.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetUdpServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetUdpClient.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetTcpServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8NetTcpClient.cpp")
		source_group ("RadJav\\V8\\Net" FILES ${SOURCES_files_RadJav__v8__Net__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__Net__Sources} ${SOURCES})
	endif ()
	
	# V8 Database
	if (INCLUDE_DATABASES)
		set (SOURCES_files_RadJav__v8__DB__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8DBKeyValueStorage.cpp")
		source_group ("RadJav\\V8\\Database" FILES ${SOURCES_files_RadJav__v8__DB__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__DB__Sources} ${SOURCES})
	endif ()
	
	# V8 Crypto
	if (INCLUDE_CRYPTOGRAPHY)
		set (SOURCES_files_RadJav__v8__Crypto__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoCipher.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoDecipher.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoCipherMultipart.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoDecipherMultipart.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoKeyGenerator.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoPrivateKey.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoPublicKey.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoHash.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8CryptoHashMultipart.cpp")
		source_group ("RadJav\\V8\\Crypto" FILES ${SOURCES_files_RadJav__v8__Crypto__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__Crypto__Sources} ${SOURCES})
	endif ()
	
	# V8 Blockchain
	if (libRadJav_INCLUDE_BLOCKCHAIN_V1)
		set (SOURCES_files_RadJav__v8__Blockchain__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/v8/RadJavV8BlockchainV1.cpp")
		source_group ("RadJav\\V8\\Blockchain" FILES ${SOURCES_files_RadJav__v8__Blockchain__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__Blockchain__Sources} ${SOURCES})
	endif ()
endif ()

if (USE_JAVASCRIPTCORE)
	# JavaScriptCore JavaScript Engine
	set (SOURCES_files_RadJav__JSC__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCJavascriptEngine.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCExternalsDetails.cpp")
	source_group ("RadJav\\JavaScriptCore" FILES ${SOURCES_files_RadJav__JSC__Sources})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Sources} ${SOURCES})

	# JavaScriptCore common GUI
	if (USE_WXWIDGETS OR IS_MOBILE)
		set (SOURCES_files_RadJav__JSC__GUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIGObject.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIButton.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUILabel.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIImage.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUITextbox.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUITextarea.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUICheckbox.cpp")
		source_group ("RadJav\\JavaScriptCore\\GUI" FILES ${SOURCES_files_RadJav__JSC__GUI__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__GUI__Sources} ${SOURCES})
	endif ()
	
	# JavaScriptCore GUI
	if (USE_WXWIDGETS)
		set (SOURCES_files_RadJav__JSC__GUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIWindow.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIContainer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUICombobox.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIRadio.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIList.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIMenuBar.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIMenuItem.cpp")
		# JavaScriptCore GUI WebView
		if (USE_WXWIDGETS_WEBVIEW)
			set (SOURCES_files_RadJav__JSC__GUI__Sources
				"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUIWebView.cpp"
				${SOURCES_files_RadJav__JSC__GUI__Sources})
		endif ()
		# JavaScriptCore GUI Canvas3D
		if (USE_OGRE)
			set (SOURCES_files_RadJav__JSC__GUI__Sources
				"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGUICanvas3D.cpp"
				${SOURCES_files_RadJav__JSC__GUI__Sources})
		endif ()
		source_group ("RadJav\\JavaScriptCore\\GUI" FILES ${SOURCES_files_RadJav__JSC__GUI__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__GUI__Sources} ${SOURCES})
	endif ()

	# JavaScriptCore MUI
	if (IS_MOBILE)
		set (SOURCES_files_RadJav__JSC__MUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUIView.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUIBottomNavigator.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUINavigator.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUITableView.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUITableViewModel.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUIScrollView.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCMUIWebView.cpp")
		source_group ("RadJav\\JavaScriptCore\\MUI" FILES ${SOURCES_files_RadJav__JSC__MUI__Sources})

		set (SOURCES ${SOURCES_files_RadJav__JSC__MUI__Sources} ${SOURCES})
	endif ()

	# JavaScriptCore C3D
	if (USE_OGRE)
		set (SOURCES_files_RadJav__JSC__C3D__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DTransform.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DObject3D.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DPlane.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DCube.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DSphere.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DCamera.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DLight.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCC3DModel.cpp")
		source_group ("RadJav\\JavaScriptCore\\C3D" FILES ${SOURCES_files_RadJav__JSC__C3D__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__C3D__Sources} ${SOURCES})
	endif ()
		
	# JavaScriptCore Global
	set (SOURCES_files_RadJav__JSC__Global__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCGlobal.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCThread.cpp")
	source_group ("RadJav\\JavaScriptCore\\Global" FILES ${SOURCES_files_RadJav__JSC__Global__Sources})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Global__Sources} ${SOURCES})

	# JavaScriptCore OS
	set (SOURCES_files_RadJav__JSC__OS__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCOS.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCOSScreenInfo.cpp")
	source_group ("RadJav\\JavaScriptCore\\OS" FILES ${SOURCES_files_RadJav__JSC__OS__Sources})

	set (SOURCES ${SOURCES_files_RadJav__JSC__OS__Sources} ${SOURCES})

	# JavaScriptCore Testing
	set (SOURCES_files_RadJav__JSC__Testing__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCTesting.cpp")
	source_group ("RadJav\\JavaScriptCore\\Testing" FILES ${SOURCES_files_RadJav__JSC__Testing__Sources})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Testing__Sources} ${SOURCES})

	# JavaScriptCore Console
	set (SOURCES_files_RadJav__JSC__Console__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCConsole.cpp")
	source_group ("RadJav\\JavaScriptCore\\Console" FILES ${SOURCES_files_RadJav__JSC__Console__Sources})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Console__Sources} ${SOURCES})

	# JavaScriptCore IO
	set (SOURCES_files_RadJav__JSC__IO__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCIO.cpp"
		#"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCIOXML.cpp"
		)
	source_group ("RadJav\\JavaScriptCore\\IO" FILES ${SOURCES_files_RadJav__JSC__IO__Sources})

	set (SOURCES ${SOURCES_files_RadJav__JSC__IO__Sources} ${SOURCES})

	# JavaScriptCore Networking
	if (libRadJav_ALLOW_NETWORKING)
		set (SOURCES_files_RadJav__JSC__Net__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNet.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetWebSocket.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetWebSocketSsl.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetUdpServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetUdpClient.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetTcpServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetTcpClient.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCNetWebServer.cpp")
		source_group ("RadJav\\JavaScriptCore\\Net" FILES ${SOURCES_files_RadJav__JSC__Net__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__Net__Sources} ${SOURCES})
	endif ()
	
	# JavaScriptCore Database
	if (INCLUDE_DATABASES)
		set (SOURCES_files_RadJav__JSC__DB__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCDBKeyValueStorage.cpp")
		source_group ("RadJav\\JavaScriptCore\\Database" FILES ${SOURCES_files_RadJav__JSC__DB__Sources})

		set (SOURCES ${SOURCES_files_RadJav__JSC__DB__Sources} ${SOURCES})
	endif ()

	# JavaScriptCore Crypto
	if (INCLUDE_CRYPTOGRAPHY)
		set (SOURCES_files_RadJav__JSC__Crypto__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoCipher.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoDecipher.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoCipherMultipart.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoDecipherMultipart.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoKeyGenerator.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoPrivateKey.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoPublicKey.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoHash.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCCryptoHashMultipart.cpp")
		source_group ("RadJav\\JavaScriptCore\\Crypto" FILES ${SOURCES_files_RadJav__JSC__Crypto__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__Crypto__Sources} ${SOURCES})
	endif ()

	# JavaScriptCore Blockchain
	if (libRadJav_INCLUDE_BLOCKCHAIN_V1)
		set (SOURCES_files_RadJav__JSC__Blockchain__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/jscore/RadJavJSCBlockchainV1.cpp")
		source_group ("RadJav\\JavaScriptCore\\Blockchain" FILES ${SOURCES_files_RadJav__JSC__Blockchain__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__Blockchain__Sources} ${SOURCES})
	endif ()
endif ()

# V8
set (SOURCES_files_Javascript__Main__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavJavascriptEngine.cpp")
source_group ("RadJav\\Javascript" FILES ${SOURCES_files_Javascript__Main__Sources})

set (SOURCES ${SOURCES_files_Javascript__Main__Sources} ${SOURCES})

# Networking
set (SOURCES_files_Networking__Main__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/RadJavNetworking.cpp")
source_group ("RadJav\\Networking" FILES ${SOURCES_files_Networking__Main__Sources})

set (SOURCES ${SOURCES_files_Networking__Main__Sources} ${SOURCES})

# C++ Global
set (SOURCES_files_RadJav__cpp__Global__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPColor.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPFont.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMath.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPRectangle.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPVector2.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPVector3.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPVector4.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPThread.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPChainedPtr.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPPersistent.cpp")
source_group ("RadJav\\CPP\\Global" FILES ${SOURCES_files_RadJav__cpp__Global__Sources})

set (SOURCES ${SOURCES_files_RadJav__cpp__Global__Sources} ${SOURCES})

# C++ IO
set (SOURCES_files_RadJav__cpp__IO__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPIO.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPIOXML.cpp")
source_group ("RadJav\\CPP\\IO" FILES ${SOURCES_files_RadJav__cpp__IO__Sources})

set (SOURCES ${SOURCES_files_RadJav__cpp__IO__Sources} ${SOURCES})

# C++ OS
set (SOURCES_files_RadJav__cpp__OS__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPOS.cpp"
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPOSScreenInfo.cpp")
source_group ("RadJav\\CPP\\OS" FILES ${SOURCES_files_RadJav__cpp__OS__Sources})

set (SOURCES ${SOURCES_files_RadJav__cpp__OS__Sources} ${SOURCES})

# C++ Testing
set (SOURCES_files_RadJav__cpp__Testing__Sources 
	"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPTesting.cpp")
source_group ("RadJav\\CPP\\Testing" FILES ${SOURCES_files_RadJav__cpp__Testing__Sources})

set (SOURCES ${SOURCES_files_RadJav__cpp__Testing__Sources} ${SOURCES})

if (USE_V8)
	# C++ Networking
	if (libRadJav_ALLOW_NETWORKING)
		set (SOURCES_files_RadJav__cpp__Net__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetUriParser.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNet.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetHttpRequest.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetWebSocketServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetWebSocketClient.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetWebSocketServerSsl.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetWebSocketClientSsl.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetUdpServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetUdpClient.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetTcpServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetTcpClient.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetWebServer.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPNetWebServerUpgradable.cpp")
		source_group ("RadJav\\CPP\\Net" FILES ${SOURCES_files_RadJav__cpp__Net__Sources})
		
		set (SOURCES ${SOURCES_files_RadJav__cpp__Net__Sources} ${SOURCES})
	endif ()
	
	if (libRadJav_DEBUG_INSPECTOR)
	# C++ Inspector Agent
		set (SOURCES_files_RadJav__cpp__Agent__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPAgent.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPAgentIO.cpp")
		source_group ("RadJav\\CPP\\Agent" FILES ${SOURCES_files_RadJav__cpp__Agent__Sources})
	
		set (SOURCES ${SOURCES_files_RadJav__cpp__Agent__Sources} ${SOURCES})
	endif()
endif ()

# C++ Database
if (INCLUDE_DATABASES)
	set (SOURCES_files_RadJav__cpp__Database__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPDBKeyValueStorage.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPDBNuDB.cpp")
	source_group ("RadJav\\CPP\\Database" FILES ${SOURCES_files_RadJav__cpp__Database__Sources})

	set (SOURCES ${SOURCES_files_RadJav__cpp__Database__Sources} ${SOURCES})
endif ()


# C++ Crypto
if (INCLUDE_CRYPTOGRAPHY)
	set (SOURCES_files_RadJav__cpp__Crypto__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoBase.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoCipher.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoDecipher.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoCipherMultipart.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoDecipherMultipart.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoKeyGenerator.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoPrivateKey.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoPublicKey.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoHash.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPCryptoHashMultipart.cpp")
	source_group ("RadJav\\CPP\\Crypto" FILES ${SOURCES_files_RadJav__cpp__Crypto__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__cpp__Crypto__Sources} ${SOURCES})
endif ()

# C++ common GUI
if (USE_WXWIDGETS OR IS_MOBILE)
	set (SOURCES_files_RadJav__cpp__GUI__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIGObject.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIButton.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUILabel.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIImage.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUITextbox.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUITextarea.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUICheckbox.cpp")
	
	source_group ("RadJav\\CPP\\GUI" FILES ${SOURCES_files_RadJav__cpp__GUI__Sources})
	
	set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__Sources} ${SOURCES})
endif ()

# C++ GUI
if (USE_WXWIDGETS)
	# Main sources
	set (SOURCES_files_RadJav__cpp__GUI__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIWindow.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIContainer.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUICombobox.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIRadio.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIList.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIMenuBar.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIMenuItem.cpp")

	# wxWidgets implementation of GUI controls
	set (SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIGObjectWidget.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIWindowFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIButtonFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUILabelFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIImageFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIContainerFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIComboboxFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUITextboxFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUITextareaFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUICheckboxFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIRadioFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIListFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIMenuBarFrame.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIMenuItem.cpp")

	# C++ GUI WebView
	if (USE_WXWIDGETS_WEBVIEW)
		set (SOURCES_files_RadJav__cpp__GUI__Sources
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUIWebView.cpp"
			${SOURCES_files_RadJav__cpp__GUI__Sources})

		set (SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIWebViewFrame.cpp"
			${SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources})
	endif ()
	# C++ GUI Canvas3D
	if (USE_OGRE)
		set (SOURCES_files_RadJav__cpp__GUI__Sources
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPGUICanvas3D.cpp"
			${SOURCES_files_RadJav__cpp__GUI__Sources})

		set (SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUICanvas3DFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/RadJavCPPGUIRenderWindow.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/desktop/wxOgreRenderWindow.cpp"
			${SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources})
	endif ()

	source_group ("RadJav\\CPP\\GUI" FILES ${SOURCES_files_RadJav__cpp__GUI__Sources})
	set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__Sources} ${SOURCES})

	source_group ("RadJav\\CPP\\GUI\\desktop" FILES ${SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources})
	set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__wxWidgets_Sources} ${SOURCES})
endif ()

# C++ MUI
if (IS_MOBILE)
	set (SOURCES_files_RadJav__cpp__MUI__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUIView.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUIBottomNavigator.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUINavigator.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUITableView.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUITableViewModel.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUIScrollView.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPMUIWebView.cpp")

	source_group ("RadJav\\CPP\\MUI" FILES ${SOURCES_files_RadJav__cpp__MUI__Sources})
	set (SOURCES ${SOURCES_files_RadJav__cpp__MUI__Sources} ${SOURCES})

	if (IOS)
		set (SOURCES_files_RadJav__cpp__GUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPGUIGObjectWidget.mm")

		source_group ("RadJav\\CPP\\GUI\\iOS" FILES ${SOURCES_files_RadJav__cpp__GUI__Sources})
		set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__Sources} ${SOURCES})

		set (SOURCES_files_RadJav__cpp__MUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIButtonFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIViewFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIBottomNavigatorFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUINavigatorFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUITableViewFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIScrollViewFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIAlertFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIEventDelegates.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIUtils.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUILabelFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIImageFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUICheckboxFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUITextboxFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUITextareaFrame.mm"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/ios/RadJavCPPMUIWebView.mm")

		source_group ("RadJav\\CPP\\MUI\\iOS" FILES ${SOURCES_files_RadJav__cpp__MUI__Sources})
		set (SOURCES ${SOURCES_files_RadJav__cpp__MUI__Sources} ${SOURCES})

	elseif (ANDROID)
		set (SOURCES_files_RadJav__cpp__GUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPGUIGObjectWidget.cpp")

		source_group ("RadJav\\CPP\\GUI\\Android" FILES ${SOURCES_files_RadJav__cpp__GUI__Sources})
		set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__Sources} ${SOURCES})

		set (SOURCES_files_RadJav__cpp__MUI__Sources 
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUIButtonFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUIViewFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUINavigatorFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUITableViewFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUIScrollViewFrame.cpp"
			#"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUIAlertFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUILabelFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUIImageFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUICheckboxFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUITextboxFrame.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUITextareaFrame.cpp"
			#"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUIWebView.cpp"
			"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/android/RadJavCPPMUITableViewDelegate.cpp"
			)

		source_group ("RadJav\\CPP\\MUI\\Android" FILES ${SOURCES_files_RadJav__cpp__MUI__Sources})
		set (SOURCES ${SOURCES_files_RadJav__cpp__MUI__Sources} ${SOURCES})
	endif ()
endif ()

# C++ C3D
if (USE_OGRE)
	set (SOURCES_files_RadJav__cpp__C3D__Sources 
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DTransform.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DCamera.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DObject3D.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DLight.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DPlane.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DCube.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DSphere.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DModel.cpp"
		"${libRadJav_SOURCE_DIR}/src/RadJav/cpp/RadJavCPPC3DModelFactory.cpp")
	source_group ("RadJav\\CPP\\C3D" FILES ${SOURCES_files_RadJav__cpp__C3D__Sources})

	set (SOURCES ${SOURCES_files_RadJav__cpp__C3D__Sources} ${SOURCES})
endif ()

if (libRadJav_INCLUDE_BLOCKCHAIN_V1)
	# BlockchainV1
	set (SOURCES_files_BlockchainV1__Main__Sources 
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/arith_uint256.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/amount.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/addrdb.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/addrman.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/base58.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/blockencodings.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/bloom.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chain.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chainparams.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chainparamsbase.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/checkpoints.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/clientversion.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/coins.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat/glibcxx_sanity.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat/glibc_sanity.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat/strnlen.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compressor.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/consensus/merkle.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/core_read.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/core_write.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/aes.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/hmac_sha256.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/hmac_sha512.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/ripemd160.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/sha1.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/sha256.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/sha512.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/dbwrapper.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/hash.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/httprpc.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/httpserver.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/init.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/key.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/keystore.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/merkleblock.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/miner.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/net.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/netaddress.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/netbase.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/net_processing.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/noui.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/policy/fees.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/policy/policy.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/policy/rbf.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/pow.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/primitives/block.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/primitives/transaction.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/protocol.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/pubkey.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/random.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rest.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/blockchain.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/client.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/mining.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/misc.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/net.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/protocol.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/rawtransaction.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/server.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/scheduler.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/xrjv1consensus.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/interpreter.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/ismine.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/script.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/script_error.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/sigcache.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/sign.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/standard.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/cleanse.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/lockedpool.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/sync.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/threadinterrupt.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/timedata.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/torcontrol.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/txdb.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/txmempool.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/uint256.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/ui_interface.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/lib/univalue.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/lib/univalue_read.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/lib/univalue_write.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/util.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/utilmoneystr.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/utilstrencodings.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/utiltime.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/validation.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/validationinterface.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/versionbits.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/crypter.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/db.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/rpcdump.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/rpcwallet.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/wallet.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/walletdb.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/warnings.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqabstractnotifier.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqnotificationinterface.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqpublishnotifier.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/xrjv1d.cpp"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/xrjv1-cli.cpp")

	if (MSVC)
		set (SOURCES_files_BlockchainV1__Main__Sources 
		"${libRadJav_SOURCE_DIR}/blockchainV1/stdafx.cpp"
			${SOURCES_files_BlockchainV1__Main__Sources})
	endif ()

	source_group ("RadJav\\BlockchainV1" FILES ${SOURCES_files_BlockchainV1__Main__Sources})

	set (SOURCES ${SOURCES_files_BlockchainV1__Main__Sources} ${SOURCES})
endif ()

