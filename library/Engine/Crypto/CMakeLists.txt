

cmake_minimum_required(VERSION 3.11)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project (RadJav_Crypto)

set (${PROJECT_NAME}_CPP_DEFS USE_CRYPTOGRAPHY ${${PROJECT_NAME}_CPP_DEFS})
set (${PROJECT_NAME}_CPP_DEFS USE_OPENSSL ${${PROJECT_NAME}_CPP_DEFS})

file (GLOB OpenSSLSources
  OpenSSL/src/*.cpp
  OpenSSL/orbimpl/*.cpp
  orbimpl/*.cpp
  )

file (GLOB OpenSSLTests OpenSSL/tests/*.cpp)

set (SOURCES
  ${OpenSSLSources}
  )

if (WIN32)
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
else ()
  add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif ()
set_property (TARGET ${PROJECT_NAME} PROPERTY FOLDER "RadJav_Crypto")



set (OPENSSL_ROOT_DIR "" CACHE PATH "OPENSSL_ROOT_DIR")
include (FindOpenSSL)

if (OPENSSL_FOUND)
  message("OpenSSL found")
  message("Include: ${OPENSSL_INCLUDE_DIR}")
  message("Crypto lib: ${OPENSSL_CRYPTO_LIBRARY}")

  target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${OPENSSL_INCLUDE_DIR}
    )
  
else()
  message (FATAL_ERROR "Cannot find OpenSSL")
endif()

set (libCrypto_INTERFACE_DIR "${PROJECT_SOURCE_DIR}/Interface" CACHE PATH "Crypto Interface")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set (libCrypto_LIBRARIES "${PROJECT_SOURCE_DIR}/build/lib${PROJECT_NAME}_d.so" CACHE STRING "Crypto libraries" FORCE)
set (libCrypto_NAME "lib${PROJECT_NAME}_d.so" CACHE STRING "Crypto library" FORCE)
endif()

if (WIN32) 
set (libCrypto_LIBRARIES "${PROJECT_SOURCE_DIR}/build/Debug/${PROJECT_NAME}_d.lib" CACHE STRING "Crypto libraries" FORCE)
set (libCrypto_NAME "${PROJECT_NAME}_d.lib" CACHE STRING "Crypto library" FORCE)
endif()

target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  )


target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
)



# -------------------------
add_executable(t1 OpenSSL/tests/t1.cpp)
set_property (TARGET t1 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
set_property (TARGET t1 PROPERTY FOLDER "RadJav_Crypto/Tests")

target_include_directories(t1
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  )

target_link_libraries(t1
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${PROJECT_NAME}
)

# -------------------------
add_executable(t2 OpenSSL/tests/t2.cpp)
set_property (TARGET t2 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
set_property (TARGET t2 PROPERTY FOLDER "RadJav_Crypto/Tests")

target_include_directories(t2
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  ${PROJECT_SOURCE_DIR}/orb
  )

target_link_libraries(t2
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${PROJECT_NAME}
)

# -------------------------
add_executable(encodeHex tests/encodeHex.cpp)
set_property (TARGET encodeHex APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
set_property (TARGET encodeHex PROPERTY FOLDER "RadJav_Crypto/Tests")

target_include_directories(encodeHex
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  ${PROJECT_SOURCE_DIR}/orb
  )

target_link_libraries(encodeHex
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${PROJECT_NAME}
)

# -------------------------
add_executable(encodeBase64 tests/encodeBase64.cpp)
set_property (TARGET encodeBase64 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
set_property (TARGET encodeBase64 PROPERTY FOLDER "RadJav_Crypto/Tests")


target_include_directories(encodeBase64
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  ${PROJECT_SOURCE_DIR}/orb
  )

target_link_libraries(encodeBase64
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${PROJECT_NAME}
  )

# -------------------------
add_executable(cipher1 OpenSSL/tests/cipher1.cpp)
set_property (TARGET cipher1 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
set_property (TARGET cipher1 PROPERTY FOLDER "RadJav_Crypto/Tests")

target_include_directories(cipher1
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  ${PROJECT_SOURCE_DIR}/orb
  )

target_link_libraries(cipher1
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${PROJECT_NAME}
)

# -------------------------
add_executable(cipher2 OpenSSL/tests/cipher2.cpp)
set_property (TARGET cipher2 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
set_property (TARGET cipher2 PROPERTY FOLDER "RadJav_Crypto/Tests")

target_include_directories(cipher2
  PRIVATE
  ${PROJECT_SOURCE_DIR}/Interface
  ${PROJECT_SOURCE_DIR}/orb
  )

target_link_libraries(cipher2
  PRIVATE
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${PROJECT_NAME}
)


set_property (TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set (${PROJECT_NAME}_CPP_DEFS_DEBUG _DEBUG ${$PROJECT_NAME}_CPP_DEFS_DEBUG})
	set (${PROJECT_NAME}_CPP_DEFS_DEBUG ${${PROJECT_NAME}_CPP_DEFS} ${${PROJECT_NAME}_CPP_DEFS_DEBUG})
endif ()

message("${PROJECT_NAME} uses the following definitions: ${${PROJECT_NAME}_CPP_DEFS}")


