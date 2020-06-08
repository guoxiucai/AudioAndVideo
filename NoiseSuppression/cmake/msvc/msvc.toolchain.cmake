cmake_minimum_required(VERSION 3.8.0)

set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
	MSVC_VERSION_YEAR
	TARGET_CPU)
    
set(MSVC TRUE)
if(NOT MSVC_VERSION_YEAR)
    set(MSVC_VERSION_YEAR 2017)#default vs2017
endif()
if(NOT TARGET_CPU)
    set(TARGET_CPU x86)#default x86
endif()
set(CMAKE_SYSTEM_PROCESSOR TARGET_CPU)

if(${MSVC_VERSION_YEAR} EQUAL 2017)
    set(MSVC_VERSION 1910)
elseif(${MSVC_VERSION_YEAR} EQUAL 2015)
    set(MSVC_VERSION 1900)
else()
    message(FATAL_ERROR "Unsupport vs version.")
endif()

get_filename_component(BAT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set (python_cmd "python")
set(python_bat "visual_studio_detect.py")
set (arg_vs_version ${MSVC_VERSION_YEAR})
set (arg_target_cpu ${TARGET_CPU})
set (arg_function "cl")
execute_process(COMMAND ${python_cmd} ${python_bat} ${arg_vs_version} ${arg_target_cpu} ${arg_function}
    WORKING_DIRECTORY ${BAT_DIR}
    RESULT_VARIABLE bat_result
    OUTPUT_VARIABLE bat_output)

if(${bat_result} EQUAL 0)
    message(STATUS ${bat_output})
    set(CMAKE_C_COMPILER ${bat_output})
    set(CMAKE_CXX_COMPILER ${bat_output})    
else()
    message(FATAL_ERROR "get vs cl path failed: ${bat_output}")
endif()