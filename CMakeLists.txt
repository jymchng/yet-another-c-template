cmake_minimum_required(VERSION 3.21)

include(cmake/prelude.cmake)
include(cmake/macros.cmake)

project(
    c_template_two
    VERSION 0.1.0
    DESCRIPTION "A simple C template made using `cmake-init`."
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C)

message(NOTICE " " CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE})
message(NOTICE " " CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR})

include(cmake/variables.cmake)
include(cmake/functions.cmake)

# ---- Declare library ----

# add_library( PrimalityUtilities OBJECT
# source/libs/PrimalityUtilities/isprime.c )

# add_library( GCDUtilities OBJECT source/libs/GCDUtilities/gcd.c )

# Find all source files under each subdirectory of source/libs and add each of
# them as a library named after the name of the directory
# ADD_LIBRARY_FOR_EACH_SUB_DIR("source/libs")

# ########### BUILD ALL THE LIBRARIES FIRST ##########
# Use the macro `GET_SUB_DIRS_NAMES` to get a list of Library Directories Names
# => `SUB_DIR_NAMES` = GCDUtilitiesPrimalityUtilities
get_sub_dirs_names(LIBRARIES_NAMES "source/libs")
message(NOTICE "`LIBRARIES_NAMES` = " ${LIBRARIES_NAMES})

foreach(libname ${LIBRARIES_NAMES})
    file(GLOB library_source_files source/libs/${libname}/*.c)

    # Get the corresponding header file for each of the source files
    get_header_files_for_sources(library_source_files header_source_files)

    # Get the first source file from the list
    list(GET header_source_files 0 first_header_file)

    # Get the directory name of the first source file
    get_filename_component(first_header_dir_fullpath ${first_header_file}
                           DIRECTORY)
    message(STATUS "`first_header_dir_fullpath` = ${first_header_dir_fullpath}")
    get_filename_component(first_header_dir_name ${first_header_dir_fullpath}
                           NAME_WE)

    # Print the directory name for debugging purposes
    message(
        STATUS "Name of the header files directory: ${first_header_dir_name}")
    message(NOTICE header_source_files=${header_source_files})
    if(library_source_files STREQUAL "")
        set(all_files ${header_source_files})
    else()
        list(APPEND library_source_files ${header_source_files})
        set(all_files ${library_source_files})
    endif()
    add_library(${libname} STATIC ${all_files})
    target_include_directories(
        ${libname} ${warning_guard}
        PUBLIC
            "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/libs/${libname}>"
            "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${first_header_dir_name}>"
    )
    target_compile_features(${libname} PUBLIC c_std_17)
endforeach()

# single header library
add_library(VectorUtilities STATIC ${CMAKE_SOURCE_DIR}/include/vector/vector.h)

target_include_directories(
    VectorUtilities ${warning_guard}
    PUBLIC "\$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/vector>")
target_compile_features(VectorUtilities PUBLIC c_std_17)
set_target_properties(VectorUtilities PROPERTIES LINKER_LANGUAGE C)
message(STATUS "LIBRARIES = ${LIBRARIES_NAMES} ARE ADDED")

set(TARGET_GROUP
    NotTest
    CACHE STRING "Group to build")

message(STATUS "`TARGET_GROUP` = ${TARGET_GROUP}")

if(TARGET_GROUP STREQUAL NotTest)

    # target_include_directories( PrimalityUtilities ${warning_guard} PUBLIC
    # "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/libs/PrimalityUtilities>"
    # )

    # target_include_directories( GCDUtilities ${warning_guard} PUBLIC
    # "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/libs/GCDUtilities>" )

    # target_compile_features(PrimalityUtilities PUBLIC c_std_17)
    # target_compile_features(GCDUtilities PUBLIC c_std_17)

    # ---- Declare executables ----

    get_sub_dirs_names(EXECUTABLE_NAMES "source/bin")
    message(STATUS "`EXECUTABLE_NAMES` = " ${EXECUTABLE_NAMES})

    foreach(executable_name ${EXECUTABLE_NAMES})
        file(GLOB executable_source_files source/bin/${executable_name}/*.c)
        add_executable(${executable_name} ${executable_source_files})
        add_executable(${executable_name}::exe ALIAS ${executable_name})

        set_property(TARGET ${executable_name} PROPERTY OUTPUT_NAME
                                                        ${executable_name})

        target_compile_features(${executable_name} PRIVATE c_std_17)

        foreach(libname ${LIBRARIES_NAMES})
            target_link_libraries(${executable_name} PRIVATE ${libname})
        endforeach()
    endforeach()

    # add_executable(primetest source/bin/primetest/main.c)
    # add_executable(primetest::exe ALIAS primetest)

    # set_property(TARGET primetest PROPERTY OUTPUT_NAME primetest)

    # target_compile_features(primetest PRIVATE c_std_17)

    # target_link_libraries(primetest PRIVATE PrimalityUtilities)
    # target_link_libraries(primetest PRIVATE GCDUtilities)

    # ---- Declare executable ----

    # add_executable(gcd source/bin/gcd/main.c) add_executable(gcd::exe ALIAS
    # gcd)

    # set_property(TARGET gcd PROPERTY OUTPUT_NAME gcd)

    # target_compile_features(gcd PRIVATE c_std_17)

    # target_link_libraries(gcd PRIVATE PrimalityUtilities)
    # target_link_libraries(gcd PRIVATE GCDUtilities)

    # ---- Install rules ----

    if(NOT CMAKE_SKIP_INSTALL_RULES)
        include(cmake/install-rules.cmake)
    endif()
elseif((TARGET_GROUP STREQUAL Test))
    message(NOTICE "Adding `test` as subdirectory")
    add_subdirectory(test)
else()
    message(FATAL_ERROR "Given TARGET_GROUP = ${TARGET_GROUP} unknown")
endif()

# ---- Developer mode ----

if(NOT c_template_two_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(
        AUTHOR_WARNING
            "Developer mode is intended for developers of c_template_two")
endif()

include(cmake/dev-mode.cmake)
