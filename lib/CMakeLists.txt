cmake_minimum_required(VERSION 3.10)

include("${CMAKE_CURRENT_LIST_DIR}/lib-macro.cmake")

set(CMAKE_CXX_STANDARD 17)

project(lib)

set(ANDROID_PLATFORM Android)
set(IOS_PLATFORM iOS)
set(UNIX_PLATFORM Unix)
set(PLATFORMS ${ANDROID_PLATFORM} ${IOS_PLATFORM} ${UNIX_PLATFORM})
set(PLATFORM ${UNIX_PLATFORM} CACHE STRING "The platform which will use the lib")
set_property(CACHE PLATFORM PROPERTY STRINGS ${PLATFORMS})
if (NOT ${PLATFORM} IN_LIST PLATFORMS)
    message(FATAL_ERROR "${PLATFORM} must be one of ${PLATFORMS}")
endif ()

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

if (${PLATFORM} STREQUAL ${IOS_PLATFORM})
    # Collect all the header files inside the "include" directory and join them with semicolon.
    foreach (header ${PUBLIC_HEADER_FILES})
        if (NOT DEFINED FRAMEWORK_PUBLIC_HEADERS)
            set(FRAMEWORK_PUBLIC_HEADERS ${header})
        else ()
            set(FRAMEWORK_PUBLIC_HEADERS "${FRAMEWORK_PUBLIC_HEADERS};${header}")
        endif ()
    endforeach ()

    set(LIB_FRAMEWORK_TARGET lib-framework)

    generate_lib_target(${LIB_FRAMEWORK_TARGET} "${MERGED_SOURCE_FILES}")

    set_target_properties(${LIB_FRAMEWORK_TARGET} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION C
        MACOSX_FRAMEWORK_IDENTIFIER com.fondesa.nativemobilelib
        PUBLIC_HEADER "${FRAMEWORK_PUBLIC_HEADERS}"
        IOS_INSTALL_COMBINED YES
        OSX_ARCHITECTURES "armv7;armv7s;arm64;i386;x86_64"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
        )
else ()
    generate_lib_target(${PROJECT_NAME} "${MERGED_SOURCE_FILES}")
endif ()

# For Android old APIs it's necessary to support both hash styles.
if (${PLATFORM} STREQUAL ${ANDROID_PLATFORM})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--hash-style=both")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--hash-style=both")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--hash-style=both")
endif ()