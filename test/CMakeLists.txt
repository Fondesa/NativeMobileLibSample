cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(lib_tests)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL
)

set(TEST_FILES
    main.cpp
    core/compat_bad_optional_access_exception_test.cpp
    database/database_client_test.cpp
    database/database_exception_test.cpp
    database/smart_c_statement_test.cpp
    database/sqlite_cursor_test.cpp
    database/sqlite_database_test.cpp
    database/sqlite_exception_test.cpp
    database/sqlite_statement_test.cpp
    note/draft_test.cpp
    note/drafts_repository_factory_test.cpp
    note/drafts_repository_impl_test.cpp
    note/incomplete_draft_exception_test.cpp
    note/mutable_draft_test.cpp
    note/note_database_initializer_test.cpp
    note/note_test.cpp
    note/notes_interactor_factory_test.cpp
    note/notes_interactor_impl_test.cpp
    note/notes_repository_factory_test.cpp
    note/notes_repository_impl_test.cpp
    )

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_QUALIFIER)

# Now simply link against gtest or gtest_main as needed.
add_executable(${PROJECT_NAME} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME}
    gtest
    gmock_main
    lib-${SYSTEM_QUALIFIER}
    )

if (CMAKE_DL_LIBS)
    # It adds the -ldl flag on the Unix machines which need dlopen and dlclose.
    target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
endif ()