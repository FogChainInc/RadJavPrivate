# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#block: include directories for debug only
#looks like this block is necessary only for debug session and can be removed
#    include_directories ("d:/Radjav/RadJavPrivate/library/include/")
#    include_directories ("d:/Radjav/RadJavPrivate/library/src/")
#block:

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the glue library for android
             radjav-native

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to glue lib source file(s).
             src/main/cpp/radjav-native.cpp )

#add libRadJav as an external dependency for glue library, static or
#  shared depending on the library build, radjav-static is a new library name
#  that will be used in java code (in case of 'shared' option)

add_library( radjav-static
             STATIC
             IMPORTED)

#set source of the libRadJav binary file, jniLibs is a common shared file folder where binary
#  artifacts of all dependencies reside. ${ANDROID_ABI} used in gradle files to separate binaries
#  for different architectures, see abiFilters in build.gradle:
#  android {
#    buildTypes {
#      release{
#        ...
#        ndk {
#          abiFilters "arm64-v8a"
#        }
#      }
#    }
#  }

set_target_properties (
             radjav-static
             PROPERTIES IMPORTED_LOCATION
             ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/liblibRadJav_d.a )

#add libRadJav dependencies
add_library( v8 SHARED IMPORTED)
add_library( v8base SHARED IMPORTED)
add_library( v8platform SHARED IMPORTED)
add_library( boostfs STATIC IMPORTED)
add_library( boosts STATIC IMPORTED)
set_target_properties ( v8 PROPERTIES IMPORTED_LOCATION
             ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libv8.cr.so )
set_target_properties ( v8base PROPERTIES IMPORTED_LOCATION
             ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libv8_libbase.cr.so )
set_target_properties ( v8platform PROPERTIES IMPORTED_LOCATION
             ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libv8_libplatform.cr.so )
set_target_properties ( boostfs PROPERTIES IMPORTED_LOCATION
             ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libboost_filesystem-mt.a )
set_target_properties ( boosts PROPERTIES IMPORTED_LOCATION
             ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libboost_system-mt.a )

#search for native android library that enables logcat output
find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_include_directories(radjav-native PRIVATE
                            "d:/RadJav/RadJavPrivate/library/include")

target_link_libraries( # Specifies the target library.
                       radjav-native
                       radjav-static
                       v8
                       v8base
                       v8platform
                       boostfs
                       boosts
                       ${log-lib}
                      )