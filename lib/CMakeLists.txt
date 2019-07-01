# Set the project name
project(lib)

add_library(${PROJECT_NAME} src/foo.cpp)
add_library(sub::lib ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)