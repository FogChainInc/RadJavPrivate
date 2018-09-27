set (ANDROID_ARCH armeabi-v7a arm64-v8a x86 x86_64)
set (CMAKE_ANDROID_ARCH_ABI armeabi-v7a CACHE STRING "Android architecture")
set_property (CACHE CMAKE_ANDROID_ARCH_ABI PROPERTY STRINGS ${ANDROID_ARCH})

#android toolchain file
set (CMAKE_SYSTEM_NAME Android)
set (CMAKE_SYSTEM_VERSION 28 CACHE STRING "OS Version")
set (CMAKE_SYSROOT "/tmp/arm64-21stllib-android-toolchain/sysroot" CACHE PATH "sysroot path")
set (CMAKE_ANDROID_NDK "/home/radjav/v8/third_party/android_ndk" CACHE PATH "Android NDK path")
set (CMAKE_ANDROID_STL_TYPE c++_static) #gnustl_static)
set (Boost_USE_STATIC_LIBS ON CACHE BOOL "use boost static libraries")