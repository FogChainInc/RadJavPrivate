# Main RadJav
set (SOURCES_files_RADJAV__Main__Headers
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJav.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavunistd.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavPreprocessor.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavLang.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavException.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavLang.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavHashMap.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavArray.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavString.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavDate.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavThread.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavTheme.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavJSExternals.h")
source_group ("RadJav\\Main" FILES ${SOURCES_files_RADJAV__Main__Headers})

set (SOURCES ${SOURCES_files_RADJAV__Main__Headers} ${SOURCES})

# wxWidgets
set (SOURCES_files_wxWidgets__Main__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavWxWidgets.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavWxWidgetsNetworking.h")
source_group ("RadJav\\wxWidgets" FILES ${SOURCES_files_wxWidgets__Main__Headers})

set (SOURCES ${SOURCES_files_wxWidgets__Main__Headers} ${SOURCES})

if (USE_V8)
	# V8 JavaScript Engine
	set (SOURCES_files_RadJav__v8__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8JavascriptEngine.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8ExternalsDetails.h")
	source_group ("RadJav\\V8" FILES ${SOURCES_files_RadJav__v8__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Headers} ${SOURCES})
	
	# V8 GUI
	set (SOURCES_files_RadJav__v8__GUI__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIGObject.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIWindow.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIWebView.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIButton.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUILabel.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIImage.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIContainer.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUICombobox.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUITextbox.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUITextarea.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUICheckbox.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIRadio.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIList.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIMenuBar.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUIMenuItem.h")
	source_group ("RadJav\\V8\\GUI" FILES ${SOURCES_files_RadJav__v8__GUI__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__GUI__Headers} ${SOURCES})
	
	# V8 Global
	set (SOURCES_files_RadJav__v8__Global__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8Global.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8Thread.h")
	source_group ("RadJav\\V8\\Global" FILES ${SOURCES_files_RadJav__v8__Global__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Global__Headers} ${SOURCES})
	
	# V8 OS
	set (SOURCES_files_RadJav__v8__OS__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8OS.h")
	source_group ("RadJav\\V8\\OS" FILES ${SOURCES_files_RadJav__v8__OS__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__OS__Headers} ${SOURCES})
	
	# V8 Console
	set (SOURCES_files_RadJav__v8__Console__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8Console.h")
	source_group ("RadJav\\V8\\Console" FILES ${SOURCES_files_RadJav__v8__Console__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Console__Headers} ${SOURCES})
	
	# V8 IO
	set (SOURCES_files_RadJav__v8__IO__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8IO.h")
	source_group ("RadJav\\V8\\IO" FILES ${SOURCES_files_RadJav__v8__IO__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__IO__Headers} ${SOURCES})
	
	# V8 Networking
	set (SOURCES_files_RadJav__v8__Net__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8Net.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8NetWebSocket.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8NetWebServer.h")
	source_group ("RadJav\\V8\\Net" FILES ${SOURCES_files_RadJav__v8__Net__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Net__Headers} ${SOURCES})
	
	# V8 Database
	set (SOURCES_files_RadJav__v8__DB__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8DBKeyValueStorage.h")
	source_group ("RadJav\\V8\\Database" FILES ${SOURCES_files_RadJav__v8__DB__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__DB__Headers} ${SOURCES})
	
	# V8 Crypto
	set (SOURCES_files_RadJav__v8__Crypto__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoCipher.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoDecipher.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoCipherMultipart.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoDecipherMultipart.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoKeyGenerator.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoPrivateKey.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoPublicKey.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoHash.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8CryptoHashMultipart.h")
	source_group ("RadJav\\V8\\Crypto" FILES ${SOURCES_files_RadJav__v8__Crypto__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Crypto__Headers} ${SOURCES})
	
	# V8 Blockchain
	set (SOURCES_files_RadJav__v8__Blockchain__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8BlockchainV1.h")
	source_group ("RadJav\\V8\\Blockchain" FILES ${SOURCES_files_RadJav__v8__Blockchain__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__v8__Blockchain__Headers} ${SOURCES})
endif ()

if (USE_JAVASCRIPTCORE)
	# JavaScriptCore JavaScript Engine
	set (SOURCES_files_RadJav__JSC__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCJavascriptEngine.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCExternalsDetails.h")
	source_group ("RadJav\\JavaScriptCore" FILES ${SOURCES_files_RadJav__JSC__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Headers} ${SOURCES})

	# V8 GUI
	set (SOURCES_files_RadJav__JSC__GUI__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIGObject.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIWindow.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIWebView.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIButton.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUILabel.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIImage.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIContainer.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUICombobox.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUITextbox.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUITextarea.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUICheckbox.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIRadio.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIList.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIMenuBar.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUIMenuItem.h")
	source_group ("RadJav\\JavaScriptCore\\GUI" FILES ${SOURCES_files_RadJav__JSC__GUI__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__GUI__Headers} ${SOURCES})

	# JavaScriptCore Global
	set (SOURCES_files_RadJav__JSC__Global__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGlobal.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCThread.h")
	source_group ("RadJav\\JavaScriptCore\\Global" FILES ${SOURCES_files_RadJav__JSC__Global__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Global__Headers} ${SOURCES})

	# JavaScriptCore OS
	set (SOURCES_files_RadJav__JSC__OS__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCOS.h")
	source_group ("RadJav\\JavaScriptCore\\OS" FILES ${SOURCES_files_RadJav__JSC__OS__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__OS__Headers} ${SOURCES})

	# JavaScriptCore Console
	set (SOURCES_files_RadJav__JSC__Console__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCConsole.h")
	source_group ("RadJav\\JavaScriptCore\\Console" FILES ${SOURCES_files_RadJav__JSC__Console__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Console__Headers} ${SOURCES})

	# JavaScriptCore IO
	set (SOURCES_files_RadJav__JSC__IO__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCIO.h")
	source_group ("RadJav\\JavaScriptCore\\IO" FILES ${SOURCES_files_RadJav__JSC__IO__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__JSC__IO__Headers} ${SOURCES})

	# JavaScriptCore Networking
	set (SOURCES_files_RadJav__JSC__Net__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCNet.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCNetWebSocket.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCNetWebServer.h")
	source_group ("RadJav\\JavaScriptCore\\Net" FILES ${SOURCES_files_RadJav__JSC__Net__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__Net__Headers} ${SOURCES})

	# JavaScriptCore Database
	set (SOURCES_files_RadJav__JSC__DB__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCDBKeyValueStorage.h")
	source_group ("RadJav\\JavaScriptCore\\Database" FILES ${SOURCES_files_RadJav__JSC__DB__Headers})

	set (SOURCES ${SOURCES_files_RadJav__JSC__DB__Headers} ${SOURCES})

	# JavaScriptCore Crypto
	set (SOURCES_files_RadJav__JSC__Crypto__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoCipher.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoDecipher.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoCipherMultipart.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoDecipherMultipart.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoKeyGenerator.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoPrivateKey.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoPublicKey.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoHash.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCCryptoHashMultipart.h")
	source_group ("RadJav\\JavaScriptCore\\Crypto" FILES ${SOURCES_files_RadJav__JSC__Crypto__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__JSC__Crypto__Headers} ${SOURCES})
endif ()

# V8
set (SOURCES_files_Javascript__Main__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavJavascriptEngine.h")
source_group ("RadJav\\Javascript" FILES ${SOURCES_files_Javascript__Main__Headers})

set (SOURCES ${SOURCES_files_Javascript__Main__Headers} ${SOURCES})

# Networking
set (SOURCES_files_Networking__Main__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/RadJavNetworking.h")
source_group ("RadJav\\Networking" FILES ${SOURCES_files_Networking__Main__Headers})

set (SOURCES ${SOURCES_files_Networking__Main__Headers} ${SOURCES})

# C++ Global
set (SOURCES_files_RadJav__cpp__Global__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPColor.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPFont.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPMath.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPRectangle.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPVector2.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPVector3.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPVector4.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPThread.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPChainedPtr.h")
source_group ("RadJav\\CPP\\Global" FILES ${SOURCES_files_RadJav__cpp__Global__Headers})

set (SOURCES ${SOURCES_files_RadJav__cpp__Global__Headers} ${SOURCES})

# C++ IO
set (SOURCES_files_RadJav__cpp__IO__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPIO.h")
source_group ("RadJav\\CPP\\IO" FILES ${SOURCES_files_RadJav__cpp__IO__Headers})

set (SOURCES ${SOURCES_files_RadJav__cpp__IO__Headers} ${SOURCES})

# C++ OS
set (SOURCES_files_RadJav__cpp__OS__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPOS.h")
source_group ("RadJav\\CPP\\OS" FILES ${SOURCES_files_RadJav__cpp__OS__Headers})

set (SOURCES ${SOURCES_files_RadJav__cpp__OS__Headers} ${SOURCES})

if (USE_V8)
	# C++ Networking
	set (SOURCES_files_RadJav__cpp__Net__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPNet.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPNetWebSocketServer.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPNetWebSocketClient.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPNetWebServer.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPNetWebServerUpgradable.h")
	source_group ("RadJav\\CPP\\Net" FILES ${SOURCES_files_RadJav__cpp__Net__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__cpp__Net__Headers} ${SOURCES})
	
	if (libRadJav_DEBUG_INSPECTOR)
	# C++ Inspector Agent
		set (SOURCES_files_RadJav__cpp__Agent__Headers 
			"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPAgent.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPAgentIO.h")
		source_group ("RadJav\\CPP\\Agent" FILES ${SOURCES_files_RadJav__cpp__Agent__Headers})
	
		set (SOURCES ${SOURCES_files_RadJav__cpp__Agent__Headers} ${SOURCES})
	endif()
endif ()

# C++ Database
set (SOURCES_files_RadJav__cpp__Database__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPDBKeyValueStorage.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPDBNuDB.h")
source_group ("RadJav\\CPP\\Database" FILES ${SOURCES_files_RadJav__cpp__Database__Headers})

set (SOURCES ${SOURCES_files_RadJav__cpp__Database__Headers} ${SOURCES})

# C++ Crypto
set (SOURCES_files_RadJav__cpp__Crypto__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoBase.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoCipher.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoDecipher.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoCipherMultipart.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoDecipherMultipart.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoKeyGenerator.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoPrivateKey.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoPublicKey.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoHash.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPCryptoHashMultipart.h")
source_group ("RadJav\\CPP\\Crypto" FILES ${SOURCES_files_RadJav__cpp__Crypto__Headers})

set (SOURCES ${SOURCES_files_RadJav__cpp__Crypto__Headers} ${SOURCES})

# C++ GUI
set (SOURCES_files_RadJav__cpp__GUI__Headers 
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIGObject.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIWindow.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIWebView.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIButton.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUILabel.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIImage.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIContainer.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUICombobox.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUITextbox.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUITextarea.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUICheckbox.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIRadio.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIList.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIMenuBar.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIMenuItem.h"
	"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIEvent.h")
source_group ("RadJav\\CPP\\GUI" FILES ${SOURCES_files_RadJav__cpp__GUI__Headers})

set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__Headers} ${SOURCES})

if (USE_OGRE)
	if (USE_V8)
		# V8 C3D
		set (SOURCES_files_RadJav__v8__C3D__Headers 
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DTransform.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DObject3D.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DPlane.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DCube.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DSphere.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DCamera.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DLight.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8C3DModel.h")
		source_group ("RadJav\\V8\\C3D" FILES ${SOURCES_files_RadJav__v8__C3D__Headers})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__C3D__Headers} ${SOURCES})
		
		# V8 GUI Canvas3D
		set (SOURCES_files_RadJav__v8__GUI__Headers 
			"${libRadJav_SOURCE_DIR}/include/RadJav/v8/RadJavV8GUICanvas3D.h")
		source_group ("RadJav\\V8\\GUI" FILES ${SOURCES_files_RadJav__v8__GUI__Headers})
		
		set (SOURCES ${SOURCES_files_RadJav__v8__GUI__Headers} ${SOURCES})
	elseif (USE_JAVASCRIPTCORE)
		# JavaScriptCore C3D
		set (SOURCES_files_RadJav__JSC__C3D__Headers 
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DTransform.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DObject3D.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DPlane.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DCube.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DSphere.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DCamera.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DLight.h"
			"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCC3DModel.h")
		source_group ("RadJav\\JavaScriptCore\\C3D" FILES ${SOURCES_files_RadJav__JSC__C3D__Headers})
		
		set (SOURCES ${SOURCES_files_RadJav__JSC__C3D__Headers} ${SOURCES})
		
		# JavaScriptCore GUI Canvas3D
		#set (SOURCES_files_RadJav__JSC__GUI__Headers 
		#	"${libRadJav_SOURCE_DIR}/include/RadJav/jscore/RadJavJSCGUICanvas3D.h")
		#source_group ("RadJav\\JavaScriptCore\\GUI" FILES ${SOURCES_files_RadJav__JSC__GUI__Headers})
		
		#set (SOURCES ${SOURCES_files_RadJav__JSC__GUI__Headers} ${SOURCES})
	endif ()

	# C++ C3D
	set (SOURCES_files_RadJav__cpp__C3D__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DTransform.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DCamera.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DObject3D.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DLight.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DPlane.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DCube.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DSphere.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DModel.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPC3DModelFactory.h")
	source_group ("RadJav\\CPP\\C3D" FILES ${SOURCES_files_RadJav__cpp__C3D__Headers})
	
	set (SOURCES ${SOURCES_files_RadJav__cpp__C3D__Headers} ${SOURCES})

	# C++ GUI Canvas3D
	set (SOURCES_files_RadJav__cpp__GUI__Headers 
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUICanvas3D.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/cpp/RadJavCPPGUIRenderWindow.h"
		"${libRadJav_SOURCE_DIR}/include/RadJav/wxOgreRenderWindow.h")
	source_group ("RadJav\\CPP\\GUI" FILES ${SOURCES_files_RadJav__cpp__GUI__Headers})

	set (SOURCES ${SOURCES_files_RadJav__cpp__GUI__Headers} ${SOURCES})
endif ()

if (libRadJav_INCLUDE_BLOCKCHAIN_V1)
	# BlockchainV1
	set (SOURCES_files_BlockchainV1__Main__Headers 
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/arith_uint256.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/addrdb.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/addrman.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/amount.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/base58.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/blockencodings.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/bloom.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chain.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chainparams.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chainparamsbase.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/chainparamsseeds.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/checkpoints.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/checkqueue.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/clientversion.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/coins.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat/byteswap.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat/endian.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compat/sanity.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/compressor.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/consensus/consensus.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/consensus/merkle.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/consensus/params.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/consensus/validation.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/core_io.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/core_memusage.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/aes.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/common.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/hmac_sha256.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/hmac_sha512.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/ripemd160.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/sha1.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/sha256.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/crypto/sha512.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/cuckoocache.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/dbwrapper.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/hash.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/httprpc.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/httpserver.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/indirectmap.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/init.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/key.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/keystore.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/limitedmap.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/memusage.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/merkleblock.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/miner.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/net.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/netaddress.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/netbase.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/netmessagemaker.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/net_processing.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/noui.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/policy/fees.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/policy/policy.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/policy/rbf.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/pow.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/prevector.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/primitives/block.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/primitives/transaction.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/protocol.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/pubkey.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/random.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/reverselock.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/client.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/protocol.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/register.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/rpc/server.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/scheduler.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/xrjv1consensus.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/interpreter.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/ismine.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/script.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/script_error.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/sigcache.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/sign.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/script/standard.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/serialize.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/streams.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/allocators/secure.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/allocators/zeroafterfree.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/cleanse.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/events.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/support/lockedpool.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/sync.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/threadinterrupt.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/threadsafety.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/timedata.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/tinyformat.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/torcontrol.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/txdb.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/txmempool.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/uint256.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/ui_interface.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/undo.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/include/univalue.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/lib/univalue_escapes.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/lib/univalue_utffilter.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/univalue/univalue-config.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/util.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/utilmoneystr.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/utilstrencodings.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/utiltime.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/validation.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/validationinterface.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/version.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/versionbits.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/coincontrol.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/crypter.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/db.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/rpcwallet.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/wallet.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wallet/walletdb.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/warnings.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqabstractnotifier.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqconfig.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqnotificationinterface.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/zmq/zmqpublishnotifier.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/wintype.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/xrjv1d.h"
		"${libRadJav_SOURCE_DIR}/blockchainV1/src/xrjv1-cli.h")

	if (MSVC)
		set (SOURCES_files_BlockchainV1__Main__Headers 
			"${libRadJav_SOURCE_DIR}/blockchainV1/src/config/xrjv1-config-win.h"
			"${libRadJav_SOURCE_DIR}/blockchainV1/stdafx.h"
			${SOURCES_files_BlockchainV1__Main__Headers})
	endif ()

	source_group ("RadJav\\BlockchainV1" FILES ${SOURCES_files_BlockchainV1__Main__Headers})

	set (SOURCES ${SOURCES_files_BlockchainV1__Main__Headers} ${SOURCES})
endif ()

