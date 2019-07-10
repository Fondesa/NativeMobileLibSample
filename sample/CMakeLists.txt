cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(sample)

set(SOURCE_FILES
	main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    lib
)