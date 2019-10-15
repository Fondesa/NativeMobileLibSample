cmake_minimum_required(VERSION 3.10)
project(lib)
set(CMAKE_CXX_STANDARD 17)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
# The lib name will be used for:
# - the output library files
# - the iOS framework identifier
# - the name of the directory inside $PREBUILT_LIBS (if defined)
set(LIB_NAME notesnative)

# Download and unpack sqlite3 at configure time.
configure_file(CMakeLists.txt.in sqlite3-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sqlite3-download)
if (result)
    message(FATAL_ERROR "CMake step for sqlite failed: ${result}")
endif ()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sqlite3-download)
if (result)
    message(FATAL_ERROR "Build step for sqlite failed: ${result}")
endif ()

option(ENABLE_TESTS "Enable the test target" ON)
option(ENABLE_TESTS_COVERAGE "Enable the coverage for tests" ON)

set(LIB_SOURCE_FILES
    src/core/compat_bad_optional_access_exception.cpp
    src/database/smart_c_statement.cpp
    src/note/note.cpp
    src/note/draft.cpp
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
    include/note.hpp
    include/note_database_initializer.hpp
    include/notes_interactor.hpp
    include/notes_interactor_factory.hpp
    )

set(SQLITE3_SOURCE_FILES
    # This will be downloaded using ExternalProject_Add().
    ${CMAKE_CURRENT_BINARY_DIR}/libs/src/sqlite3/sqlite3.c
    )

set(MERGED_SOURCE_FILES ${LIB_SOURCE_FILES} ${SQLITE3_SOURCE_FILES})

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_QUALIFIER)
set(TARGET_NAME "lib-${SYSTEM_QUALIFIER}")

# Generate the target with the name "lib-{system}".
add_library(${TARGET_NAME} SHARED ${MERGED_SOURCE_FILES} ${PUBLIC_HEADER_FILES})

# Change the output name for the generated target.
set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
    )

target_include_directories(${TARGET_NAME}
    PUBLIC include
    PUBLIC src
    # Includes the directories downloaded with ExternalProject_Add().
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/src
    )

if (DEFINED ENV{PREBUILT_LIBS})
    # Defines the destination of this library when it will be exported.
    set(EXPORTED_LIB_PATH $ENV{PREBUILT_LIBS}/${LIB_NAME})
    set(QUALIFIED_EXPORTED_LIB_PATH ${EXPORTED_LIB_PATH}/${SYSTEM_QUALIFIER})
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL Android)
    if (DEFINED ENV{PREBUILT_LIBS})
        set(LIB_FILE_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${LIB_NAME}.so)
        set(LIB_SYM_PATH ${QUALIFIED_EXPORTED_LIB_PATH}/${ANDROID_ABI}/lib${LIB_NAME}.so)
    endif ()
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
        MACOSX_FRAMEWORK_IDENTIFIER com.fondesa.${LIB_NAME}
        PUBLIC_HEADER "${FRAMEWORK_PUBLIC_HEADERS}"
        IOS_INSTALL_COMBINED YES
        OSX_ARCHITECTURES "armv7;armv7s;arm64;i386;x86_64"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
        )

elseif (${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    if (DEFINED ENV{PREBUILT_LIBS})
        set(LIB_FILE_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${LIB_NAME}.dylib)
        set(LIB_SYM_PATH ${QUALIFIED_EXPORTED_LIB_PATH}/lib${LIB_NAME}.dylib)
    endif ()
elseif (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    if (DEFINED ENV{PREBUILT_LIBS})
        set(LIB_FILE_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${LIB_NAME}.so)
        set(LIB_SYM_PATH ${QUALIFIED_EXPORTED_LIB_PATH}/lib${LIB_NAME}.so)
    endif ()
else ()
    message(FATAL_ERROR "The system ${CMAKE_SYSTEM_NAME} is not supported")
endif ()

add_custom_target(build-lib)

if (DEFINED LIB_FILE_PATH AND DEFINED LIB_SYM_PATH)
    # Get the directory containing the symlink.
    get_filename_component(LIB_SYM_PATH_DIR ${LIB_SYM_PATH} DIRECTORY)

    set(INCLUDE_DIR_PATH ${CMAKE_SOURCE_DIR}/include)
    set(INCLUDE_SYM_PATH ${EXPORTED_LIB_PATH}/include)

    add_custom_command(OUTPUT export-headers SYMBOLIC
        # Create the directory containing the include dir.
        COMMAND ${CMAKE_COMMAND} -E make_directory ${EXPORTED_LIB_PATH}
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${INCLUDE_DIR_PATH} ${INCLUDE_SYM_PATH};
        COMMENT "Symlinking the public headers from \"${INCLUDE_DIR_PATH}\" to \"${INCLUDE_SYM_PATH}\""
        )

    # It symlinks the public headers before.
    add_custom_target(export-lib
        # Create the directory containing the symlink.
        COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_SYM_PATH_DIR}
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${LIB_FILE_PATH} ${LIB_SYM_PATH};
        COMMENT "Symlinking the ${CMAKE_SYSTEM_NAME} library from \"${LIB_FILE_PATH}\" to \"${LIB_SYM_PATH}\""
        DEPENDS export-headers)

    # The library must be built before the library is symlinked because the library file must be generated.
    add_dependencies(export-lib ${TARGET_NAME})
    add_dependencies(build-lib export-lib)
else ()
    add_dependencies(build-lib ${TARGET_NAME})
endif ()

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)

    add_custom_target(run-tests
        # Run lib-tests.
        COMMAND lib_tests
        COMMENT "Running all the tests..."
        )

    add_dependencies(run-tests lib_tests)

    if (ENABLE_TESTS_COVERAGE)
        find_program(LCOV_PATH lcov)
        if (NOT LCOV_PATH)
            message(FATAL_ERROR "lcov not found! Aborting...")
        endif ()

        add_library(coverage-config INTERFACE)

        # Add the required coverage flags.
        target_compile_options(${TARGET_NAME} PUBLIC
            # Skip the optimization.
            -O0
            # Generate the debug info.
            -g
            -fprofile-arcs
            -ftest-coverage
            )

        if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
            target_link_options(coverage-config INTERFACE --coverage)
        else ()
            target_link_libraries(coverage-config INTERFACE --coverage)
        endif ()

        target_link_libraries(${TARGET_NAME} PUBLIC coverage-config)

        set(COVERAGE_OUTPUT_DIR ${OUTPUT_DIR}/coverage)
        set(COVERAGE_OUTPUT_RAW_FILE_PATH ${COVERAGE_OUTPUT_DIR}/raw.info)
        set(COVERAGE_OUTPUT_FILTERED_FILE_PATH ${COVERAGE_OUTPUT_DIR}/filtered.info)
        set(COVERAGE_OUTPUT_HTML_DIR ${COVERAGE_OUTPUT_DIR}/html)

        add_custom_target(coverage-clean
            COMMAND ${LCOV_PATH}
            --directory ${CMAKE_BINARY_DIR}
            --zerocounters
            COMMENT "Removing the coverage tracefiles from the previous run...")

        # Clean the coverage tracefiles from the previous files, if any, before running all the tests.
        # In this way, we can be sure the new report will use only the tracefiles from the latest run.
        add_dependencies(run-tests coverage-clean)

        add_custom_target(coverage-raw
            # Create the output dir of the coverage reports.
            COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_DIR}
            # Generate the raw coverage report containing all the files.
            COMMAND ${LCOV_PATH}
            --directory ${CMAKE_BINARY_DIR}
            --gcov-tool ${CMAKE_SOURCE_DIR}/llvm-gcov.sh
            --capture
            --rc lcov_branch_coverage=1
            -o ${COVERAGE_OUTPUT_RAW_FILE_PATH}
            # Exclude the unwanted files from the generated report.
            COMMAND ${LCOV_PATH}
            --remove ${COVERAGE_OUTPUT_RAW_FILE_PATH}
            --rc lcov_branch_coverage=1
            -o ${COVERAGE_OUTPUT_FILTERED_FILE_PATH}
            # The default /usr/include dir of Xcode shouldn't be included since we are not testing those files.
            '*/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/*'
            # The build files shouldn't be included.
            '${CMAKE_BINARY_DIR}/*'
            # All the files in the libs folder are external so they shoudn't be tested.
            '${CMAKE_CURRENT_SOURCE_DIR}/libs/*'
            # All the test files are hit during the tests so it's pointless to include them.
            '${CMAKE_CURRENT_SOURCE_DIR}/test/*'
            COMMENT "Generating the raw code coverage report...")

        # Run all the tests before generating the report.
        add_dependencies(coverage-raw run-tests)

        # Just an empty target to avoid to change the target name based on the report type.
        add_custom_target(coverage-report)

        if (GENERATE_HTML_REPORT)
            find_program(GENHTML_PATH genhtml)
            if (NOT GENHTML_PATH)
                message(FATAL_ERROR "genhtml not found! Aborting...")
            endif ()

            add_custom_target(coverage-html
                # Use genhtml to generate the HTML report from the raw one.
                COMMAND ${GENHTML_PATH}
                ${COVERAGE_OUTPUT_FILTERED_FILE_PATH}
                -o ${COVERAGE_OUTPUT_HTML_DIR}
                --rc lcov_branch_coverage=1
                COMMENT "Generating the HTML code coverage report...")

            add_custom_command(TARGET coverage-html POST_BUILD
                COMMENT "The HTML coverage report is available at:\n${COVERAGE_OUTPUT_HTML_DIR}/index.html")

            # The HTML report is based on the raw one.
            add_dependencies(coverage-html coverage-raw)
            add_dependencies(coverage-report coverage-html)
        else ()
            add_dependencies(coverage-report coverage-raw)
        endif ()
    endif ()
endif ()