set (CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})

set (CPACK_PACKAGE_VENDOR "http://www.highersoftware.com")

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "RadJav virtual machine for JavaScript based apps")
set (CPACK_PACKAGE_CONTACT "Nathanael Coonrod <ncoonrod@highersoftware.com>")
set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})

set (CPACK_RPM_PACKAGE_DEBUG 0)
set (RadJavVM_PACKAGER_TYPE "" CACHE STRING "Packager to use to create installer")

if (MSVC)
	set_property (CACHE RadJavVM_PACKAGER_TYPE PROPERTY STRINGS NSIS)

	if (${RadJavVM_PACKAGER_TYPE} MATCHES "NSIS")
		set (CPACK_NSIS_INSTALL_ROOT "" CACHE STRING "V8 libraries directory")
	endif ()
endif ()

if (APPLE)
	if (IOS)
		message (WARNING "Application bundle not implemented for iOS build\n"
				"You can disable RadJavVM_CREATE_INSTALL_PACKAGES for now\n")
	else ()
		set_property (CACHE RadJavVM_PACKAGER_TYPE PROPERTY STRINGS "DragNDrop")
	
		if (${RadJavVM_PACKAGER_TYPE} MATCHES "DragNDrop")
			set (CPACK_GENERATOR "DragNDrop")
			
			set (CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME_LAUNCHER}-${CPACK_PACKAGE_VERSION})
			set (CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/installers/mac/RadJav.icns)
			
			set (CPACK_DMG_DS_STORE "${CMAKE_SOURCE_DIR}/installers/mac/DS_Store.in")
			set (CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/installers/mac/DmgBackground.png")
			
			set (CPACK_DMG_FORMAT "UDBZ")
			set (CPACK_DMG_VOLUME_NAME ${CMAKE_PROJECT_NAME_LAUNCHER})
		endif ()
	endif ()
endif ()

set (CPACK_GENERATOR ${RadJavVM_PACKAGER_TYPE})

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	set_property (CACHE RadJavVM_PACKAGER_TYPE PROPERTY STRINGS RPM DEB)

	if (${RadJavVM_PACKAGER_TYPE} MATCHES "RPM")
		set(CPACK_GENERATOR "TGZ;DEB;RPM")

		# RPM specific configuration (minimum)
		#set(CPACK_RPM_PACKAGE_LICENSE "Proprietary")
		#set(CPACK_RPM_PACKAGE_GROUP "Applications/Multimedia")
		#set(CPACK_RPM_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
		#set(CPACK_RPM_COMPONENT_INSTALL ON)   # necessary even if CPACK_COMPONENT_INSTALL set to ON. A bug in my opinion.
	endif()

	if (${RadJavVM_PACKAGER_TYPE} MATCHES "DEB")
		set(CPACK_GENERATOR "TGZ;DEB")

		# Debian specific configuration (minimum)
		#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
		#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libboost-test-dev")
		#set(CPACK_DEBIAN_PACKAGE_CONFLICTS "some-apps")
	endif()
endif()

include (InstallRequiredSystemLibraries)
include (CPack)

#Signing installation image
#if (APPLE)
#	execute_process (COMMAND codesign --force --deep --verbose --verify --sign "${MAC_CODESIGN_IDENTITY}" "${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}.dmg")
#endif ()
