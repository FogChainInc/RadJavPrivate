message (STATUS "Searching for libRadJav...")

find_path (radjav_INCLUDE_DIR NAMES RadJavC.h RadJavPreprocessor.h PATH_SUFFIXES radjav)

find_library (radjav_LIBRARY_RELEASE NAMES libRadJav)
find_library (radjav_LIBRARY_DEBUG NAMES libRadJav_d)
