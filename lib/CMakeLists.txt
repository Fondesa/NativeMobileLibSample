project(lib)

set(SOURCE_FILES
	src/foo.cpp
)

# They are needed only for XCode, otherwise the header files don't show up in the project.
set(HEADER_FILES_FOR_XCODE 
	include/foo.hpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES_FOR_XCODE})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "nativemobile")

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)