# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required (VERSION 3.6.0)

project (RadJavWrapper)

message (STATUS "Building for architecture ${ANDROID_ABI}")

# Instruct CMake to look into special architecture related folders while searching libraries, i.e. ROOT/lib/armeabi-v7a/
set (CMAKE_LIBRARY_ARCHITECTURE ${ANDROID_ABI})

# Specify root paths for cross-compiling, CMake will search start search libs and headers in these folders
list (APPEND CMAKE_FIND_ROOT_PATH ${ANDROID_NDK} "${RJ_DEPENDENCIES_PATH}")

message (STATUS "Searching for libRadJav...")
find_path (radjav_INCLUDE_DIR NAMES RadJavC.h RadJavPreprocessor.h PATH_SUFFIXES radjav)

find_library (radjav_LIBRARY_RELEASE NAMES libRadJav)
find_library (radjav_LIBRARY_DEBUG NAMES libRadJav_d)

message (STATUS "Searching for Boost...")
find_path (boost_INCLUDE_DIR NAMES asio.hpp PATH_SUFFIXES boost)

find_library (boost_thread_LIBRARY boost_thread-mt)
find_library (boost_system_LIBRARY boost_system-mt)
find_library (boost_filesystem_LIBRARY boost_filesystem-mt)
find_library (boost_chrono_LIBRARY boost_chrono-mt)

message (STATUS "Searching for V8...")
find_path (v8_INCLUDE_DIR NAMES v8.h PATH_SUFFIXES v8)
find_path (icu_INCLUDE_DIR NAMES ucnvmbcs.h PATH_SUFFIXES icu)

find_library (v8_main_LIBRARY v8.cr)
find_library (v8_platform_LIBRARY v8_libplatform.cr)
find_library (v8_base_LIBRARY v8_libbase.cr)
find_library (icu_i18n_LIBRARY icui18n.cr)
find_library (icu_uc_LIBRARY icuuc.cr)

message (STATUS "Searching for libLog...")
find_library (libLog_LIBRARY log)

# Add libRadJav headers location
include_directories (${radjav_INCLUDE_DIR})

# Add boost headers location
include_directories (${boost_INCLUDE_DIR})

# Add wrapper target
add_library (${CMAKE_PROJECT_NAME} SHARED src/main/cpp/RadJavWrapper.cpp)

# Since libRadJav distributed as static library, here are the flags we built libRadJav with
target_compile_definitions (${CMAKE_PROJECT_NAME} PUBLIC LIBRADJAV
                                                         USE_V8
                                                         USE_BOOST
                                                         BOOST_SPIRIT_THREADSAFE
                                                         HAVE_WORKING_BOOST_SLEEP_FOR
                                                         BOOST_SYSTEM_NO_DEPRECATED
                                                         LINUX
                                                         USE_ANDROID
                                                         RADJAV_LIB_STATIC
                                                         LANG_EN_US
                                                         THREADS_USE_STD_THREAD)

# set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${libRadJav_CPP_DEFS2})
# set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:${libRadJav_CPP_DEFS_DEBUG2}>)

# Add dependency to libRadJav
target_link_libraries (${CMAKE_PROJECT_NAME} debug ${radjav_LIBRARY_DEBUG})
target_link_libraries (${CMAKE_PROJECT_NAME} optimized ${radjav_LIBRARY_RELEASE})
target_link_libraries (${CMAKE_PROJECT_NAME} ${boost_thread_LIBRARY}
                                             ${boost_system_LIBRARY}
                                             ${boost_filesystem_LIBRARY}
                                             ${boost_chrono_LIBRARY})

target_link_libraries (${CMAKE_PROJECT_NAME} ${v8_main_LIBRARY}
                                             ${v8_platform_LIBRARY}
                                             ${v8_base_LIBRARY}
                                             ${icu_i18n_LIBRARY}
                                             ${icu_uc_LIBRARY})

# Add dependency to libLog
target_link_libraries ( ${CMAKE_PROJECT_NAME} ${libLog_LIBRARY})
