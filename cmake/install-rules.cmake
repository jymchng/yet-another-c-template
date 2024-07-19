get_sub_dirs_names(EXECUTABLE_NAMES ${PROJECT_SOURCE_DIR}/source/bin)
message(NOTICE "inside `install-rules.cmake`, `PROJECT_SOURCE_DIRECTORY` = "
        ${PROJECT_SOURCE_DIR})
if(NOT DEFINED EXECUTABLE_NAMES)
    message(FATAL_ERROR "`EXECUTABLE_NAMES` must not be empty")
else()
    message(NOTICE "inside `install-rules.cmake`, `EXECUTABLE_NAMES` = "
            ${EXECUTABLE_NAMES})
endif()

install(TARGETS ${EXECUTABLE_NAMES} RUNTIME COMPONENT c_template_two_Runtime)

if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
