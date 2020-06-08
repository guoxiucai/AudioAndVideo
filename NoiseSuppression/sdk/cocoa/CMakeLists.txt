include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/sdk/src)

list(APPEND OBJC_ADAPTER_SRC
    src/SWNoiseSuppression.h
    src/SWNoiseSuppression.mm
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fobjc-abi-version=2 -fobjc-arc")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-abi-version=2 -fobjc-arc")

add_library(ObjcAdaptor STATIC ${OBJC_ADAPTER_SRC})

