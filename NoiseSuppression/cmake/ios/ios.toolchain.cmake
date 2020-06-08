if(DEFINED CMAKE_OSX_SYSROOT)
  message(STATUS "Using manually set SDK path: ${CMAKE_OSX_SYSROOT}")
else()  
  message(FATAL_ERROR "Invalid CMAKE_OSX_SYSROOT: ${CMAKE_OSX_SYSROOT} does not exist.")
endif()

if (NOT CMAKE_C_COMPILER)
  execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find clang
    OUTPUT_VARIABLE CMAKE_C_COMPILER
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
endif()

if (NOT CMAKE_CXX_COMPILER)
  execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find clang++
    OUTPUT_VARIABLE CMAKE_CXX_COMPILER
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Using CXX compiler: ${CMAKE_CXX_COMPILER}")
endif()

execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find libtool
  OUTPUT_VARIABLE IOS_LIBTOOL
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Using libtool: ${IOS_LIBTOOL}")
# Configure libtool to be used instead of ar + ranlib to build static libraries.
# This is required on Xcode 7+, but should also work on previous versions of Xcode.
set(CMAKE_C_CREATE_STATIC_LIBRARY
  "${IOS_LIBTOOL} -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY
  "${IOS_LIBTOOL} -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")

# Standard settings.
set(UNIX TRUE CACHE BOOL "")
set(APPLE TRUE CACHE BOOL "")
set(IOS TRUE CACHE BOOL "")
set(CMAKE_AR ar CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB ranlib CACHE FILEPATH "" FORCE)

set(CMAKE_C_COMPILER_ABI ELF)
set(CMAKE_CXX_COMPILER_ABI ELF)

set(CMAKE_C_FLAGS "-fobjc-abi-version=2 -fobjc-arc -fobjc-weak ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-fobjc-abi-version=2 -fobjc-arc -fobjc-weak ${CMAKE_CXX_FLAGS}")

set(CMAKE_MACOSX_BUNDLE YES)

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE "NO")
set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "9.0")