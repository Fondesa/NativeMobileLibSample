cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(lib)

set(LIB_SOURCE_FILES
    src/foo.cpp
    src/note.cpp
    src/draft_note.cpp
    src/in_memory_note_repository.cpp
    src/sqlite_note_repository.cpp
    src/sqlite_database.cpp
    src/sqlite_exception.cpp
    src/sqlite_statement.cpp
    src/sqlite_cursor.cpp
    src/cursor.cpp
    src/statement.cpp
    )

set(LIB_HEADER_FILES
    include/foo.hpp
    include/note.hpp
    include/draft_note.hpp
    include/note_repository.hpp
    include/in_memory_note_repository.hpp
    include/sqlite_note_repository.hpp
    include/sqlite_database.hpp
    include/sqlite_exception.hpp
    include/sqlite_statement.hpp
    include/sqlite_cursor.hpp
    include/cursor.hpp
    include/statement.hpp
    )

set(SQLITE3_SOURCE_FILES
    sqlite3/sqlite3.c
    )

set(SQLITE3_HEADER_FILES
    sqlite3/sqlite3.h
    )

set(MERGED_SOURCE_FILES ${LIB_SOURCE_FILES} ${SQLITE3_SOURCE_FILES})
set(MERGED_HEADER_FILES ${LIB_HEADER_FILES} ${SQLITE3_HEADER_FILES})

if (${BUILD_TESTS})
    add_subdirectory(test)
    enable_testing()
endif ()

if (${BUILD_FRAMEWORK})
    # Collect all the header files inside the "include" directory and join them with semicolon.
    foreach (header ${LIB_HEADER_FILES})
        if (NOT DEFINED FRAMEWORK_PUBLIC_HEADERS)
            set(FRAMEWORK_PUBLIC_HEADERS ${header})
        else ()
            set(FRAMEWORK_PUBLIC_HEADERS "${FRAMEWORK_PUBLIC_HEADERS};${header}")
        endif ()
    endforeach ()

    set(LIB_FRAMEWORK_TARGET lib-framework)

    add_library(${LIB_FRAMEWORK_TARGET} SHARED ${MERGED_SOURCE_FILES} ${MERGED_HEADER_FILES})

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
        )
else ()
    add_library(${PROJECT_NAME} SHARED ${MERGED_SOURCE_FILES} ${MERGED_HEADER_FILES})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "nativemobile"
        )

    target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )
endif ()