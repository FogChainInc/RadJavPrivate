#android toolchain file
set (CMAKE_SYSTEM_NAME Android)
set (CMAKE_SYSTEM_VERSION 21)
set (CMAKE_SYSROOT "/tmp/arm64-21stllib-android-toolchain/sysroot")
set (CMAKE_ANDROID_ARCH_ABI arm64-v8a)
set (CMAKE_ANDROID_NDK "/home/radjav/v8/third_party/android_ndk")
set (CMAKE_ANDROID_STL_TYPE c++_static) #gnustl_static)
set (Boost_USE_STATIC_LIBS ON CACHE BOOL "use boost static libraries")