cmake_minimum_required(VERSION 3.10)
project(lib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

option(BUILD_TESTS "Build test files" ON)

set(LIB_SOURCE_FILES
    src/foo.cpp
    src/core/compatible_bad_optional_access_exception.cpp
    src/note/note.cpp
    src/note/draft.cpp
    src/note/in_memory_note_repository.cpp
    src/note/notes_repository_impl.cpp
    src/database/sqlite_database.cpp
    src/database/database_exception.cpp
    src/database/sqlite_statement.cpp
    src/database/sqlite_cursor.cpp
    src/database/database_cursor.cpp
    src/database/sqlite_exception.cpp
    src/database/database_statement.cpp
    src/database/database_client.cpp
    src/note/note_database_initializer.cpp
    src/note/drafts_repository_impl.cpp
    src/note/incomplete_draft_exception.cpp
    src/note/notes_interactor_impl.cpp
    src/note/notes_interactor_factory.cpp
    src/note/drafts_repository_factory.cpp
    src/note/notes_repository_factory.cpp
    src/note/mutable_draft.cpp
    )

set(PUBLIC_HEADER_FILES
    include/database.hpp
    include/database_client.hpp
    include/database_cursor.hpp
    include/database_exception.hpp
    include/draft.hpp
    include/foo.hpp
    include/note.hpp
    include/note_database_initializer.hpp
    include/notes_interactor.hpp
    include/notes_interactor_factory.hpp
    )

set(SQLITE3_SOURCE_FILES
    libs/sqlite3/sqlite3.c
    )

set(MERGED_SOURCE_FILES ${LIB_SOURCE_FILES} ${SQLITE3_SOURCE_FILES})

if (BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
endif ()

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_QUALIFIER)
set(TARGET_NAME "lib-${SYSTEM_QUALIFIER}")

# Generate the target with the name "lib-{system}".
add_library(${TARGET_NAME} SHARED ${MERGED_SOURCE_FILES} ${PUBLIC_HEADER_FILES})

# Change the output name for the generated target.
set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME "nativemobile"
    )

target_include_directories(${TARGET_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/libs
    PUBLIC ${PROJECT_SOURCE_DIR}/src
    )

# Defines where the destination of tis library when it will be exported.
set(EXPORTED_LIB_PATH $ENV{PREBUILT_LIBS}/nativemobile/${SYSTEM_QUALIFIER})

if (${CMAKE_SYSTEM_NAME} STREQUAL Android)
    set(LIB_FILE_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libnativemobile.so)
    set(LIB_SYM_PATH ${EXPORTED_LIB_PATH}/${ANDROID_ABI}/libnativemobile.so)
    # For Android old APIs it's necessary to support both hash styles.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--hash-style=both")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--hash-style=both")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--hash-style=both")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL iOS)
    # Collect all the header files inside the "include" directory and join them with semicolon.
    foreach (header ${PUBLIC_HEADER_FILES})
        if (NOT DEFINED FRAMEWORK_PUBLIC_HEADERS)
            set(FRAMEWORK_PUBLIC_HEADERS ${header})
        else ()
            set(FRAMEWORK_PUBLIC_HEADERS "${FRAMEWORK_PUBLIC_HEADERS};${header}")
        endif ()
    endforeach ()

    set_target_properties(${TARGET_NAME} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION C
        MACOSX_FRAMEWORK_IDENTIFIER com.fondesa.nativemobilelib
        PUBLIC_HEADER "${FRAMEWORK_PUBLIC_HEADERS}"
        IOS_INSTALL_COMBINED YES
        OSX_ARCHITECTURES "armv7;armv7s;arm64;i386;x86_64"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
        )
elseif (${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    set(LIB_FILE_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libnativemobile.dylib)
    set(LIB_SYM_PATH ${EXPORTED_LIB_PATH}/libnativemobile.dylib)
else ()
    message(FATAL_ERROR "The system ${CMAKE_SYSTEM_NAME} is not supported")
endif ()

if (DEFINED LIB_FILE_PATH AND DEFINED LIB_SYM_PATH)
    add_custom_target(export-lib
        COMMAND make
        COMMENT "Building and exporting the library")

    # Get the directory containing the symlink.
    get_filename_component(LIB_SYM_PATH_DIR ${LIB_SYM_PATH} DIRECTORY)
    add_custom_command(
        TARGET export-lib POST_BUILD
        # Create the directory containing the symlink.
        COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_SYM_PATH_DIR}
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${LIB_FILE_PATH} ${LIB_SYM_PATH};
        COMMENT "Symlinking the ${CMAKE_SYSTEM_NAME} library from \"${LIB_FILE_PATH}\" to \"${LIB_SYM_PATH}\""
    )
endif ()
