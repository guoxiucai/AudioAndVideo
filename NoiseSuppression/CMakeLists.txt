cmake_minimum_required(VERSION 3.8.0)

project(ns)

set(CMAKE_OBJECT_PATH_MAX 512)
set(SDK_VERSION 1.0.0.x)

if(WIN32)
	set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else(WIN32)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g -static-libstdc++ ${CMAKE_CXX_FLAGS}")
	#set(CMAKE_CXX_FLAGS_RELEASE "-s ${CMAKE_CXX_FLAGS_RELEASE}")
	#set(CMAKE_C_FLAGS_RELEASE "-s ${CMAKE_C_FLAGS_RELEASE}")
	set(CMAKE_C_FLAGS "-std=c11 ${CMAKE_C_FLAGS}")
endif(WIN32)

if(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
	message(STATUS "Complie on apple computer use clang.")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-shorten-64-to-32")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(ANDROID)
	string(REPLACE "-fno-rtti" "" FIXED_C_FLAGS ${CMAKE_C_FLAGS})
	string(REPLACE "-fno-exceptions" "" FIXED_C_FLAGS ${FIXED_C_FLAGS})
	if(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7-a")
		string(REPLACE "-mfpu=vfpv3-d16" "" FIXED_C_FLAGS ${FIXED_C_FLAGS})
	endif()
	set(CMAKE_C_FLAGS ${FIXED_C_FLAGS})


	string(REPLACE "-fno-rtti" "" FIXED_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	string(REPLACE "-fno-exceptions" "" FIXED_CXX_FLAGS ${FIXED_CXX_FLAGS})
	if(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7-a")
		string(REPLACE "-mfpu=vfpv3-d16" "" FIXED_CXX_FLAGS ${FIXED_CXX_FLAGS})
	endif()
	set(CMAKE_CXX_FLAGS ${FIXED_CXX_FLAGS})

endif()

if(APPLE)
	if(IOS)
		set(CMAKE_SYSTEM_PROCESSOR aarch64)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=arm64-apple-darwin-eabi-as")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=arm64-apple-darwin-eabi-as")
		set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --target=arm64-apple-darwin-eabi-as")
	else()
		set(CMAKE_SYSTEM_PROCESSOR x86_64)
	endif()
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG=3")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG=4")
endif()

execute_process(
  COMMAND git rev-list --count HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_CODE_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_INFO
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REPLACE "x" "${GIT_CODE_VERSION}" SDK_VERSION ${SDK_VERSION})
set_property(GLOBAL PROPERTY COMPILE_DEFINITIONS "-DSDK_VERSION=${SDK_VERSION}")
set_property(GLOBAL PROPERTY COMPILE_DEFINITIONS "-DGIT_COMMIT_INFO=${GIT_COMMIT_INFO}")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMakePredefinedTargets")
				
configure_file("${CMAKE_SOURCE_DIR}/sdk/src/Commons/inc/Version.h.in" "${CMAKE_SOURCE_DIR}/sdk/src/Commons/inc/Version.h")

add_subdirectory(sdk/src)
if(APPLE)
	add_subdirectory(sdk/cocoa)
endif()


