
if (APPLE)
	#Configuring Info.plist for Mac Bundle
	set (RadJav_BUNDLE_DISPLAY_NAME ${CMAKE_PROJECT_NAME})
	set (RadJavVM_BUNDLE_SIGNATURE "rjvm")
	
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/installers/mac/Info.plist.in)
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "com.highersoftware.${CMAKE_PROJECT_NAME}")
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME})
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "RadJav.icns")
	set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_COPYRIGHT "© 2018, Higher Edge Software")
	
	#Copy specific files into a bundle
	set (V8_LIBRARIES_DIR "/Users/mikalaiturankou/Work/Compilation/v8/out.gn/x64.release")
	set (APP_EXECUTABLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.app/Contents/MacOS")
	
	add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
				COMMAND ${CMAKE_COMMAND} -E copy ${V8_LIBRARIES_DIR}/natives_blob.bin ${APP_EXECUTABLE_DIR}
				COMMAND ${CMAKE_COMMAND} -E copy ${V8_LIBRARIES_DIR}/snapshot_blob.bin ${APP_EXECUTABLE_DIR}
				COMMENT "Copying depencencies"
				VERBATIM )
	
	#Copy icons
	install (FILES ${CMAKE_SOURCE_DIR}/installers/mac/RadJav.icns DESTINATION ${CMAKE_PROJECT_NAME}.app/Contents/Resources)
	
	#Correct binary files to be able to execute on client side
	install (CODE "
			include(BundleUtilities)
			fixup_bundle(\"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.app\" \"\" \"\")
			"
		COMPONENT Runtime)

	#Installing as app bundle
	install(TARGETS ${CMAKE_PROJECT_NAME} BUNDLE DESTINATION . )
endif()
