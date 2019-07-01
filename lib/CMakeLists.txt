project(lib)

set(SOURCE_FILES
	src/foo.cpp
)

# They are needed only for XCode, otherwise the header files don't show up in the project.
set(HEADER_FILES_FOR_XCODE 
	include/foo.hpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES_FOR_XCODE})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
