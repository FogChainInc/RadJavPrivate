
if (APPLE)
	if (NOT IOS)
		#Correct binary files to be able to execute on client side
		install (CODE "
				include(BundleUtilities)
				fixup_bundle(\"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME_LAUNCHER}.app\" \"\" \"\")
				"
			COMPONENT Runtime)
		
		#Signing
		install (CODE 
			"execute_process(COMMAND codesign --force --deep --verbose --verify --sign \"${MAC_CODESIGN_IDENTITY}\" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME_LAUNCHER}.app)")
		
		#Installing as app bundle
		install (TARGETS ${CMAKE_PROJECT_NAME_LAUNCHER} BUNDLE DESTINATION . )
		
	else ()
		message (WARNING "Support for iOS installation package not implemented\n"
				"You can disable RadJavVM_CREATE_INSTALL_PACKAGES for now\n")
	endif ()
endif()
