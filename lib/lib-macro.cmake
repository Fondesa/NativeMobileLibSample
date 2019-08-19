cmake_minimum_required(VERSION 3.10)

macro(generate_lib_target targetName mergedSourceFiles)
    add_library(${targetName} SHARED ${mergedSourceFiles})

    set_target_properties(${targetName} PROPERTIES
        OUTPUT_NAME "nativemobile"
        )

    target_include_directories(${targetName}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/libs
        PUBLIC ${PROJECT_SOURCE_DIR}/src
        )
endmacro()