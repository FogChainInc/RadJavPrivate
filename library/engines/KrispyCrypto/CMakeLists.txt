cmake_minimum_required(VERSION 3.0)

if (WIN32)
	set (CMAKE_USE_RELATIVE_PATHS TRUE)
endif ()

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

project (KrispyCrypto)

if (UNIX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

set (${PROJECT_NAME}_CPP_DEFS USE_CRYPTOGRAPHY ${${PROJECT_NAME}_CPP_DEFS})
set (${PROJECT_NAME}_CPP_DEFS USE_OPENSSL ${${PROJECT_NAME}_CPP_DEFS})

set (${PROJECT_NAME}_BUILD_TESTS FALSE CACHE BOOL "Build tests")

file (GLOB OpenSSLSources
  OpenSSL/src/*.cpp
  OpenSSL/orbimpl/*.cpp
  orbimpl/*.cpp
  )

if (${PROJECT_NAME}_BUILD_TESTS)
	file (GLOB OpenSSLTests OpenSSL/tests/*.cpp)
endif ()

set (SOURCES
  ${OpenSSLSources}
  )

if (WIN32)
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
else ()
  add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif ()

set_property (TARGET ${PROJECT_NAME} PROPERTY FOLDER "KrispyCrypto")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set (OPENSSL_ROOT_DIR "${KrispyCrypto}_SOURCE_DIR/../../../../sdk/openssl/build/install" CACHE PATH "OPENSSL_ROOT_DIR" FORCE)
set(OPENSSL_CRYPTO_LIBRARY "${KrispyCrypto_SOURCE_DIR}/../../../../sdk/openssl/build/install/lib/libcrypto.so" CACHE FILEPATH "libcrypto" FORCE) 
set(OPENSSL_SSL_LIBRARY "${KrispyCrypto_SOURCE_DIR}/../../../../sdk/openssl/build/install/lib/libssl.so" CACHE FILEPATH "libssl" FORCE) 
set(OPENSSL_INCLUDE_DIR "${KrispyCrypto_SOURCE_DIR}/../../../../sdk/openssl/build/install/include" CACHE PATH "OpenSSL include" FORCE)
endif()



include (FindOpenSSL)

if (OPENSSL_FOUND)
  message("OpenSSL found")
  message("Include: ${OPENSSL_INCLUDE_DIR}")
  message("KrispyCrypto lib: ${OPENSSL_CRYPTO_LIBRARY}")

  target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${OPENSSL_INCLUDE_DIR}
    )
  
else()
  message (FATAL_ERROR "Cannot find OpenSSL")
endif()

set (libKrispyCrypto_INTERFACE_DIR "${PROJECT_SOURCE_DIR}/Interface" CACHE PATH "KrispyCrypto Interface")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set (libKrispyCrypto_LIBRARIES "${PROJECT_SOURCE_DIR}/build/lib${PROJECT_NAME}_d.so" CACHE STRING "KrispyCrypto libraries" FORCE)
	set (libKrispyCrypto_NAME "lib${PROJECT_NAME}_d.so" CACHE STRING "KrispyCrypto library" FORCE)
endif()

if (WIN32) 
	set (libKrispyCrypto_LIBRARIES "${PROJECT_SOURCE_DIR}/build/Debug/${PROJECT_NAME}_d.lib" CACHE STRING "KrispyCrypto libraries" FORCE)
	set (libKrispyCrypto_NAME "${PROJECT_NAME}_d.lib" CACHE STRING "KrispyCrypto library" FORCE)
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



if (${PROJECT_NAME}_BUILD_TESTS)
	# -------------------------
	add_executable(t1 OpenSSL/tests/t1.cpp)
	set_property (TARGET t1 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
	set_property (TARGET t1 PROPERTY FOLDER "KrispyCrypto/Tests")

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
	set_property (TARGET t2 PROPERTY FOLDER "KrispyCrypto/Tests")

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
	set_property (TARGET encodeHex PROPERTY FOLDER "KrispyCrypto/Tests")

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
	set_property (TARGET encodeBase64 PROPERTY FOLDER "KrispyCrypto/Tests")


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
	set_property (TARGET cipher1 PROPERTY FOLDER "KrispyCrypto/Tests")

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
	set_property (TARGET cipher2 PROPERTY FOLDER "KrispyCrypto/Tests")

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

	# -------------------------
	add_executable(rsa1 OpenSSL/tests/rsa1.cpp)
	set_property (TARGET rsa1 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
	set_property (TARGET rsa1 PROPERTY FOLDER "KrispyCrypto/Tests")

	target_include_directories(rsa1
	  PRIVATE
	  ${PROJECT_SOURCE_DIR}/Interface
	  ${PROJECT_SOURCE_DIR}/orb
	  )

	target_link_libraries(rsa1
	  PRIVATE
	  ${OPENSSL_CRYPTO_LIBRARY}
	  ${OPENSSL_SSL_LIBRARY}
	  ${PROJECT_NAME}
	)

	# -------------------------
	add_executable(OpenSSL_rsa1 tests/OpenSSL-rsa1.cpp)
	set_property (TARGET OpenSSL_rsa1 APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})
	set_property (TARGET OpenSSL_rsa1 PROPERTY FOLDER "KrispyCrypto/Tests")

	target_include_directories(OpenSSL_rsa1
	  PRIVATE
	  ${PROJECT_SOURCE_DIR}/Interface
	  ${PROJECT_SOURCE_DIR}/orb
	  )

	target_link_libraries(OpenSSL_rsa1
	  PRIVATE
	  ${OPENSSL_CRYPTO_LIBRARY}
	  ${OPENSSL_SSL_LIBRARY}
	  ${PROJECT_NAME}
	)
      
endif ()


set_property (TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${PROJECT_NAME}_CPP_DEFS})

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set (${PROJECT_NAME}_CPP_DEFS_DEBUG _DEBUG ${${PROJECT_NAME}_CPP_DEFS_DEBUG})
	set (${PROJECT_NAME}_CPP_DEFS_DEBUG ${${PROJECT_NAME}_CPP_DEFS} ${${PROJECT_NAME}_CPP_DEFS_DEBUG})
endif ()

message("${PROJECT_NAME} uses the following definitions: ${${PROJECT_NAME}_CPP_DEFS}")


