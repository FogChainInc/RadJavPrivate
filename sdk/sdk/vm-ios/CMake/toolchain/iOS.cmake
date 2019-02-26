#
# Toolchain for cross-compiling to iOS using Xcode
#
# This script will setup toolchain for selected architecture
#
# Generic workflow:
#  mkdir build-ios && cd build-ios
#  cmake .. \
#       -DCMAKE_TOOLCHAIN_FILE=<full-path>RadJavPrivate/library/CMake/toolchain/iOS.cmake \
#       -DRJ_ARCHITECTURE="x86_64" -G Xcode
# Note:
#  While setting RJ_ARCHITECTURE please provide only one desired arhitecture.
#  x86_64 is for iOS simulator
#  arm64, armv7, armv7s is for iOS real device
#
# TODO: Add ability to specify SDK version to be used
#

# Set different system name so CMake will be notified that we are crosscompiling, CMAKE_CROSSCOMPILING will be set in that case.
set(CMAKE_SYSTEM_NAME "iOS")

set(IOS 1)

# Get the SDK versions information
execute_process(COMMAND xcodebuild -showsdks
			OUTPUT_VARIABLE _XCODE_SDKS
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE)

# Put SDKs into the list
string(REGEX MATCHALL "-sdk iphone[a-z]+[0-9]+\\.[0-9]+"
       _XCODE_SDKS "${_XCODE_SDKS}")
string(REPLACE "-sdk " "" _XCODE_SDKS "${_XCODE_SDKS}")

#message(STATUS "Available iOS(iPhone) SDKs: ${_XCODE_SDKS}")

# Check desired architecture
#message (STATUS "Desired iOS device architecture: ${CMAKE_OSX_ARCHITECTURES}")
set (CMAKE_OSX_ARCHITECTURES ${RJ_ARCHITECTURE} CACHE STRING "Desired architecture")
mark_as_advanced(FORCE CMAKE_OSX_ARCHITECTURES)

if (CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
    string (REGEX MATCH "iphonesimulator[0-9]+\\.[0-9]+" IOS_SDK ${_XCODE_SDKS})
elseif (CMAKE_OSX_ARCHITECTURES MATCHES "arm")
    string (REGEX MATCH "iphoneos[0-9]+\\.[0-9]+" IOS_SDK ${_XCODE_SDKS})
else ()
    message (FATAL_ERROR "Please specify CMAKE_OSX_ARCHITECTURES to match one of the available architectures: arm64, armv7, armv7s, x86_64")
endif ()

#message (STATUS "IOS_SDK to be used -  ${IOS_SDK}")
set(IOS_SDK ${IOS_SDK} CACHE STRING "SDK name for iOS device to be used" FORCE)
mark_as_advanced (FORCE IOS_SDK)

# Configuring sysroot
#message (STATUS "iOS SDK: ${IOS_SDK}")

# Get the SDK versions information
execute_process(COMMAND xcodebuild -sdk ${IOS_SDK} -version Path
			OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE)

# Sanity check for sysroot
if (CMAKE_OSX_SYSROOT MATCHES "error:")
    message (FATAL_ERROR "Unable to find sysroot of iOS SDK ${IOS_SDK} for architecture ${CMAKE_OSX_ARCHITECTURES}")
endif ()

# Help CMake find the platform file
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

# Required to make CMake's test_compile stuff pass
# https://cmake.org/Bug/view.php?id=15329
set(CMAKE_MACOSX_BUNDLE ON)
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")

set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH}
    ${CMAKE_OSX_SYSROOT}
    ${CMAKE_OSX_SYSROOT}/../..
    ${CMAKE_PREFIX_PATH}
    ${CMAKE_INSTALL_PREFIX})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
