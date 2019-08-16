cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(lib)

option(BUILD_TESTS "Build test files" ON)
option(BUILD_FRAMEWORK "Build iOS framework" OFF)
option(BUILD_WITH_PORTABLE_HASH_STYLE "Support both hash styles" OFF)

set(LIB_SOURCE_FILES
    src/foo.cpp
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
    include/foo.hpp
    include/note.hpp
    include/draft.hpp
    include/database_client.hpp
    include/database_exception.hpp
    include/note_database_initializer.hpp
    )

set(SQLITE3_SOURCE_FILES
    libs/sqlite3/sqlite3.c
    )

set(MERGED_SOURCE_FILES ${LIB_SOURCE_FILES} ${SQLITE3_SOURCE_FILES})

if (BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
endif ()

if (BUILD_FRAMEWORK)
    message(CIAOOOOO)
    # Collect all the header files inside the "include" directory and join them with semicolon.
    foreach (header ${PUBLIC_HEADER_FILES})
        if (NOT DEFINED FRAMEWORK_PUBLIC_HEADERS)
            set(FRAMEWORK_PUBLIC_HEADERS ${header})
        else ()
            set(FRAMEWORK_PUBLIC_HEADERS "${FRAMEWORK_PUBLIC_HEADERS};${header}")
        endif ()
    endforeach ()

    set(LIB_FRAMEWORK_TARGET lib-framework)

    add_library(${LIB_FRAMEWORK_TARGET} SHARED ${MERGED_SOURCE_FILES} ${PUBLIC_HEADER_FILES})

    set_target_properties(${LIB_FRAMEWORK_TARGET} PROPERTIES
        OUTPUT_NAME "nativemobile"
        FRAMEWORK TRUE
        FRAMEWORK_VERSION C
        MACOSX_FRAMEWORK_IDENTIFIER com.fondesa.nativemobilelib
        PUBLIC_HEADER "${FRAMEWORK_PUBLIC_HEADERS}"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        )

    target_include_directories(${LIB_FRAMEWORK_TARGET}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/libs
        PUBLIC ${PROJECT_SOURCE_DIR}/src
        )
else ()
    add_library(${PROJECT_NAME} SHARED ${MERGED_SOURCE_FILES})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "nativemobile"
        )

    target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/libs
        PUBLIC ${PROJECT_SOURCE_DIR}/src
        )
endif ()

# For some builds it's necessary to support both hash styles (e.g. for Android old APIs).
if (BUILD_WITH_PORTABLE_HASH_STYLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--hash-style=both")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--hash-style=both")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--hash-style=both")
endif ()