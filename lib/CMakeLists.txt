cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(lib)

set(LIB_SOURCE_FILES
    src/foo.cpp
    src/note/note.cpp
    src/note/draft_note.cpp
    src/note/in_memory_note_repository.cpp
    src/note/database_note_repository.cpp
    src/database/sqlite_database.cpp
    src/database/sqlite_exception.cpp
    src/database/sqlite_statement.cpp
    src/database/sqlite_cursor.cpp
    src/database/database_cursor.cpp
    src/database/database_statement.cpp
    src/database/database_client.cpp
    src/core/initializer.cpp
    src/core/illegal_state_exception.cpp
    )

set(PUBLIC_HEADER_FILES
    include/foo.hpp
    include/note.hpp
    include/draft_note.hpp
    include/note_repository.hpp
    include/in_memory_note_repository.hpp
    include/database_note_repository.hpp
    include/database_client.hpp
    )

set(SQLITE3_SOURCE_FILES
    libs/sqlite3/sqlite3.c
    )

set(MERGED_SOURCE_FILES ${LIB_SOURCE_FILES} ${SQLITE3_SOURCE_FILES})

if (${BUILD_TESTS})
    add_subdirectory(test)
    enable_testing()
endif ()

if (${BUILD_FRAMEWORK})
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