
set (CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})

set (CPACK_PACKAGE_VENDOR "http://www.highersoftware.com")

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "RadJav virtual machine for JavaScript based apps")
set (CPACK_PACKAGE_CONTACT "Nathanael Coonrod <ncoonrod@highersoftware.com>")
set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})

set (CPACK_RPM_PACKAGE_DEBUG 0)

if(MSVC)
    set (CPACK_GENERATOR "NSIS")
endif(MSVC)

if(APPLE)
	set (CPACK_GENERATOR "DragNDrop")
	
	set (CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})
	set (CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/installers/mac/RadJav.icns)
	
	set (CPACK_DMG_DS_STORE "${CMAKE_SOURCE_DIR}/installers/mac/DS_Store.in")
	set (CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/installers/mac/DmgBackground.png")
	
	set (CPACK_DMG_FORMAT "UDBZ")
	set (CPACK_DMG_VOLUME_NAME ${CMAKE_PROJECT_NAME})
	
endif(APPLE)

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	find_program(RPM_EXE rpmbuild)
	
	if(${RPM_EXE} MATCHES RPM_EXE-NOTFOUND)
	    set(CPACK_GENERATOR "TGZ;DEB")
	else()
	    set(CPACK_GENERATOR "TGZ;DEB;RPM")
	endif()
endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)

# Debian specific configuration (minimum)
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libboost-test-dev")
#set(CPACK_DEBIAN_PACKAGE_CONFLICTS "some-apps")

# RPM specific configuration (minimum)
#set(CPACK_RPM_PACKAGE_LICENSE "Proprietary")
#set(CPACK_RPM_PACKAGE_GROUP "Applications/Multimedia")
#set(CPACK_RPM_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
#set(CPACK_RPM_COMPONENT_INSTALL ON)   # necessary even if CPACK_COMPONENT_INSTALL set to ON. A bug in my opinion.

include (InstallRequiredSystemLibraries)
include (CPack)
