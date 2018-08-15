#
# Toolchain for cross-compiling to iOS using Xcode
#
# Set CMAKE_OSX_ROOT to SDK you want to target and enable all desired
# architectures in CMAKE_OSX_ARCHITECTURES.
#
#  mkdir build-ios && cd build-ios
#  cmake .. \
#       -DCMAKE_TOOLCHAIN_FILE=<full-path>RadJavPrivate/library/CMake/toolchain/iOS.cmake \
#       -DCMAKE_OSX_ARCHITECTURES="arm64;armv7;armv7s" -G Xcode
#

# Here we set different system name so CMake will be notified that we crosscompiling, CMAKE_CROSSCOMPILING will be set in that case.
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

set (IOS_SDK "iphoneos11.4" CACHE STRING "SDK name for iOS device to be used")
set_property (CACHE IOS_SDK PROPERTY STRINGS ${_XCODE_SDKS})

# Get the SDK versions information
execute_process(COMMAND xcodebuild -sdk ${IOS_SDK} -version Path
			OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE)

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

#TODO: Properties below needs to be set to ONLY so we will not interfere with on host libs,
#      but in that case we need to update CMAKE_FIND_ROOT_PATH for each lib which we trying to find
#      using find_path or other CMake commands
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
