# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(c_template_two_tests LANGUAGES C)
message(STATUS "Inside `test`: `CMAKE_BUILD_TYPE` = ${CMAKE_BUILD_TYPE}")
# ---- Tests ----
# https://honeytreelabs.com/posts/cmake-unity-integration/ use Unity
include(CTest)

include(FetchContent)
FetchContent_Declare(
    unity
    DOWNLOAD_EXTRACT_TIMESTAMP true
    URL https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v2.6.0.zip)

FetchContent_MakeAvailable(unity)

set(CMAKE_CTEST_ARGUMENTS "--verbose")

enable_testing()

add_executable(test_isprime source/test_isprime.c)
target_link_libraries(test_isprime unity PrimalityUtilities)
target_compile_features(test_isprime PRIVATE c_std_17)

add_executable(test_vector source/test_vector.c
                           ${CMAKE_SOURCE_DIR}/include/vector/vector.h)
target_link_libraries(test_vector unity VectorUtilities)
target_compile_features(test_vector PRIVATE c_std_17)

include(${CMAKE_SOURCE_DIR}/cmake/functions.cmake)
# CREATE_EXECUTABLE_TARGET(gcd) don't use this because on windows / macos it
# will create the `gcd` binary at ./build/release/test/Release and the resultant
# test cannot find the binary

add_executable(gcd ${CMAKE_SOURCE_DIR}/source/bin/gcd/main.c)
target_link_libraries(gcd GCDUtilities)
target_compile_features(gcd PRIVATE c_std_17)

add_custom_target(test_binaries ALL)
add_dependencies(test_binaries test_isprime gcd)

add_test(test_isprime_one test_isprime)
add_test(test_vector_one test_vector)
# Need to pass ${CMAKE_BUILD_TYPE} to the cmake script command or else the
# script executing won't know what is ${CMAKE_BUILD_TYPE}
add_test(NAME test_gcd
         COMMAND ${CMAKE_COMMAND} -DCMAKE_CONFIG=${CMAKE_BUILD_TYPE} -P
                 ${CMAKE_SOURCE_DIR}/test/cmake/tests.cmake)

# ---- End-of-file commands ----
