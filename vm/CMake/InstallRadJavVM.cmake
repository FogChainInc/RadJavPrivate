
if (APPLE)
	set (RadJavVM_DEPENDENCY_OBJECTS_LIST "./natives_blob.bin;./snapshot_blob.bin" CACHE STRING "Semicolon separated list of paths to dependencies \
												which are indirectly will be used by application, i.e. loading \
												during runtime")
	
	#Configuring Info.plist for Mac Bundle
	set (RadJavVM_BUNDLE_DISPLAY_NAME ${CMAKE_PROJECT_NAME})
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
	set (APP_BUNDLE_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.app")
	set (APP_EXECUTABLE_DIR "${APP_BUNDLE_LOCATION}/Contents/MacOS")
	set (APP_RESOURCES_DIR "${APP_BUNDLE_LOCATION}/Contents/Resources")
	
	foreach(dependency ${RadJavVM_DEPENDENCY_OBJECTS_LIST})
		add_custom_command (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy ${dependency} ${APP_EXECUTABLE_DIR}
					COMMENT "Adding extra dependency into application bundle: ${dependency}"
					VERBATIM )
	endforeach(dependency)
	
	
	#Copy icons
	install (FILES ${CMAKE_SOURCE_DIR}/installers/mac/RadJav.icns DESTINATION ${CMAKE_PROJECT_NAME}.app/Contents/Resources)
	
	#Correct binary files to be able to execute on client side
	install (CODE "
			include(BundleUtilities)
			fixup_bundle(\"${APP_BUNDLE_LOCATION}\" \"\" \"\")
			"
		COMPONENT Runtime)

	#Installing as app bundle
	install(TARGETS ${CMAKE_PROJECT_NAME} BUNDLE DESTINATION . )
endif()
