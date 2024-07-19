# https://stackoverflow.com/a/54675911/19504610
# https://stackoverflow.com/questions/70539001/how-to-ctest-an-interactive-command-line-utility
# https://discourse.cmake.org/t/attempting-to-write-a-cmake-function-that-does-testing-with-an-interactive-prompt/11190

FUNCTION(interactive_command_line_should_pass test_executable input_arguments
         expected_output expected_error)
    MESSAGE(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
    MESSAGE(STATUS "CMAKE_HOST_WIN32 = ${CMAKE_HOST_WIN32}")
    MESSAGE(STATUS "CMAKE_HOST_APPLE = ${CMAKE_HOST_APPLE}")
    MESSAGE(STATUS "CMAKE_HOST_UNIX = ${CMAKE_HOST_UNIX}")
    MESSAGE(STATUS "CMAKE_CONFIGURATION_TYPES = ${CMAKE_CONFIGURATION_TYPES}")
    MESSAGE(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS "CMAKE_CONFIG = ${CMAKE_CONFIG}")
    # This is to fix the issue of the executable NOT found under `./build/test` but is found in `./build/test/${CMAKE_BUILD_TYPE}` directory
    # on MacOS and Win32
    # Use `CMAKE_CONFIGURATION_TYPES` because MacOS and Win32 are multi-configurational.
    IF((CMAKE_HOST_WIN32 STREQUAL "1") OR (CMAKE_HOST_APPLE STREQUAL "1"))
        EXECUTE_PROCESS(
            COMMAND
                bash -c
                "echo \"${input_arguments}\" | ${CMAKE_BINARY_DIR}/${CMAKE_CONFIG}/${test_executable}"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error
            COMMAND_ECHO STDOUT)
    ELSE()
    EXECUTE_PROCESS(
        COMMAND
            bash -c
            "echo \"${input_arguments}\" | ${CMAKE_BINARY_DIR}/${test_executable}"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error)
    ENDIF()

    # Find if ${expected_output} is in ${output}; if cannot be found, found =
    # "-1"
    # STRING(STRIP ${output} output)
    IF(output STREQUAL "")
        MESSAGE(FATAL_ERROR "output = ${output}; error = ${error}")
    ENDIF()
    STRING(FIND ${output} ${expected_output} found)
    # MESSAGE(found ${found})
    IF((NOT (${result} STREQUAL "0")) OR (${found} STREQUAL "-1"))
        MESSAGE(
            FATAL_ERROR
                "Test failed with return value '${result}'; output '${output}'; expected_output '${expected_output}' and found '${found}'; error = '${error}'";
        )
    ENDIF()
ENDFUNCTION()

FUNCTION(interactive_command_line_should_fail test_executable input_arguments
         expected_error)
    MESSAGE(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
    MESSAGE(STATUS "CMAKE_HOST_WIN32 = ${CMAKE_HOST_WIN32}")
    MESSAGE(STATUS "CMAKE_HOST_APPLE = ${CMAKE_HOST_APPLE}")
    MESSAGE(STATUS "CMAKE_HOST_UNIX = ${CMAKE_HOST_UNIX}")
    MESSAGE(STATUS "CMAKE_CONFIGURATION_TYPES = ${CMAKE_CONFIGURATION_TYPES}")
    # This is to fix the issue of the executable NOT found under `./build/test` but is found in `./build/test/${CMAKE_BUILD_TYPE}` directory
    # on MacOS and Win32
    # Use `CMAKE_CONFIGURATION_TYPES` because MacOS and Win32 are multi-configurational.

    IF((CMAKE_HOST_WIN32 STREQUAL "1") OR (CMAKE_HOST_APPLE STREQUAL "1"))
        EXECUTE_PROCESS(
            COMMAND
                bash -c
                "echo \"${input_arguments}\" | ${CMAKE_BINARY_DIR}/${CMAKE_CONFIG}/${test_executable}"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error
            COMMAND_ECHO STDOUT)
    ELSE()
    EXECUTE_PROCESS(
        COMMAND
            bash -c
            "echo \"${input_arguments}\" | ${CMAKE_BINARY_DIR}/${test_executable}"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error)
    ENDIF()

    # Find if ${expected_output} is in ${output}; if cannot be found, found =
    # "-1"
    IF(error STREQUAL "")
        MESSAGE(FATAL_ERROR "output = ${output}; error = ${error}")
    ENDIF()
    STRING(STRIP ${error} error)
    # FOR DEBUGGING MESSAGE(NOTICE
    # "output=${output};err_output=${err_output};input_arguments=${input_arguments};expected_error=${expected_error};")
    STRING(FIND ${error} ${expected_error} found)
    IF((NOT (${result} STREQUAL "1")) OR (${found} STREQUAL "-1"))
        MESSAGE(
            FATAL_ERROR
                "Test failed with return value '${result}'; error '${error}'; expected_error '${expected_error}' and found '${found}'; output = '${output}'";
        )
    ENDIF()
ENDFUNCTION()

INTERACTIVE_COMMAND_LINE_SHOULD_PASS(gcd "5 20" "GCD of 5 and 20 is 5" "")
INTERACTIVE_COMMAND_LINE_SHOULD_FAIL(
    gcd "4343 4ds5940"
    "Invalid input. Please enter two integers.\nYou have entered 4343 4ds5940"
    "")
