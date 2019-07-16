cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(lib)

set(SOURCE_FILES
	src/foo.cpp
	src/note.cpp
	src/draft_note.cpp
	src/in_memory_note_repository.cpp
)

# They are needed only for XCode, otherwise the header files don't show up in the project.
set(HEADER_FILES_FOR_XCODE 
	include/foo.hpp
	include/note.hpp
	include/draft_note.hpp
	include/note_repository.hpp
	include/in_memory_note_repository.hpp
)

if(${BUILD_TESTS}) 
	subdirs(test)
	enable_testing()
endif()

if(${BUILD_FRAMEWORK}) 
	set(LIB_FRAMEWORK_TARGET lib-framework)

	add_library(${LIB_FRAMEWORK_TARGET} SHARED ${SOURCE_FILES} ${HEADER_FILES_FOR_XCODE})

	set_target_properties(${LIB_FRAMEWORK_TARGET} PROPERTIES
		OUTPUT_NAME "nativemobile"
		FRAMEWORK TRUE
		FRAMEWORK_VERSION C
		MACOSX_FRAMEWORK_IDENTIFIER com.fondesa.nativemobilelib
		PUBLIC_HEADER ${HEADER_FILES_FOR_XCODE}
		XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
	)

	target_include_directories(${LIB_FRAMEWORK_TARGET}
   		PUBLIC ${PROJECT_SOURCE_DIR}/include
	)
else()
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES_FOR_XCODE})

	set_target_properties(${PROJECT_NAME} PROPERTIES 
		OUTPUT_NAME "nativemobile"
	)

	target_include_directories(${PROJECT_NAME}
		PUBLIC ${PROJECT_SOURCE_DIR}/include
	)
endif()