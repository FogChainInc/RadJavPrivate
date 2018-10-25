set (RADJAV_PACKAGER_DESCRIPTION "RadJav virtual machine for JavaScript based apps")

set (CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})

set (CPACK_PACKAGE_VENDOR "http://www.highersoftware.com")

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${RADJAV_PACKAGER_DESCRIPTION})
set (CPACK_PACKAGE_CONTACT "Nathanael Coonrod <ncoonrod@highersoftware.com>")
set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})

set (TEMP_ARCH_STR USE_ARCH)
set (DEB_ARCH "i386")

if (USE_ARCH STREQUAL "x86_64")
	set (TEMP_ARCH_STR "x64")
	set (DEB_ARCH "amd64")
endif ()

set (CPACK_RPM_PACKAGE_DEBUG 0)
set (RadJavVM_PACKAGER_TYPE "" CACHE STRING "Packager to use to create installer")
set (CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME_LAUNCHER}-Linux-${TEMP_ARCH_STR}-${CPACK_PACKAGE_VERSION})

#set (USELIBS "libc6 (>= 2.3.1-6), libboost-dev (>= 1.67.0)")
set (JS_LIBRARY_USED "")

if (USE_V8)
	#set (USELIBS "${USELIBS}, libv8 (>= 7.0.0)")
	set (JS_LIBRARY_USED "V8")
endif ()

if (USE_JAVASCRIPTCORE)
	#set (USELIBS "${USELIBS}, libjavascriptcoregtk-4.0-dev")
	set (JS_LIBRARY_USED "JavaScriptCore")
endif ()

if (USE_WXWIDGETS)
	#set (USELIBS "${USELIBS}, libwxgtk3.0-dev")
endif ()

if (INCLUDE_CRYPTOGRAPHY)
	#set (USELIBS "${USELIBS}, openssl")
endif ()

if (MSVC)
	message (ERROR "Use NSIS using the instructions located at ${RadJavVM_SOURCE_DIR}/installers/windows/installer.nsi")
endif ()

if (APPLE)
	if (IOS)
		message (WARNING "Application bundle not implemented for iOS build\n"
				"You can disable RadJavVM_CREATE_INSTALL_PACKAGES for now\n")
	else ()
		set (CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME_LAUNCHER}-MacOSX-${TEMP_ARCH_STR}-${JS_LIBRARY_USED}-${CPACK_PACKAGE_VERSION})
		set_property (CACHE RadJavVM_PACKAGER_TYPE PROPERTY STRINGS "DragNDrop")
	
		if (${RadJavVM_PACKAGER_TYPE} MATCHES "DragNDrop")
			set (CPACK_GENERATOR "DragNDrop")

			set (CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/installers/mac/RadJav.icns)
			
			set (CPACK_DMG_DS_STORE "${CMAKE_SOURCE_DIR}/installers/mac/DS_Store.in")
			set (CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/installers/mac/DmgBackground.png")
			
			set (CPACK_DMG_FORMAT "UDBZ")
			set (CPACK_DMG_VOLUME_NAME ${CMAKE_PROJECT_NAME_LAUNCHER})
		endif ()
	endif ()
endif ()

if (CPACK_BINARY_RPM)
	# RPM specific configuration (minimum)
	set(CPACK_RPM_PACKAGE_SUMMARY ${RADJAV_PACKAGER_DESCRIPTION})
	set(CPACK_RPM_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
	set(CPACK_RPM_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}.rpm)
	set(CPACK_RPM_PACKAGE_LICENSE "Proprietary")
	set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
	set(CPACK_RPM_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
	set(CPACK_RPM_PACKAGE_ARCHITECTURE ${USE_ARCH})
	set(CPACK_RPM_PACKAGE_VENDOR ${VENDOR})
	set(CPACK_RPM_PACKAGE_DESCRIPTION ${RADJAV_PACKAGER_DESCRIPTION})
	set(CPACK_RPM_PACKAGE_RELEASE 1)
	set(CPACK_RPM_COMPONENT_INSTALL ON)   # necessary even if CPACK_COMPONENT_INSTALL set to ON. A bug in my opinion.
	set (CPACK_RPM_PACKAGE_REQUIRES ${USELIBS})
endif()

if (CPACK_BINARY_DEB)
	# Debian specific configuration (minimum)
	set(CPACK_DEBIAN_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
	set(CPACK_DEBIAN_PACKAGE_VERSION ${PROJECT_VERSION})
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH})
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_CONTACT})
	set(CPACK_DEBIAN_PACKAGE_DEPENDS ${USELIBS})
	set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${RADJAV_PACKAGER_DESCRIPTION})
	set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
	set(CPACK_DEBIAN_COMPRESSION_TYPE "gzip")
	set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
endif()

include (InstallRequiredSystemLibraries)
include (CPack)

#Signing installation image
#if (APPLE)
#	execute_process (COMMAND codesign --force --deep --verbose --verify --sign "${XCODE_CODESIGN_IDENTITY}" "${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}.dmg")
#endif ()
