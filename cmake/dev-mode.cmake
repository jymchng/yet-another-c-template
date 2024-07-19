include(cmake/folders.cmake)

# INCLUDE(CTest) IF(BUILD_TESTING) ADD_SUBDIRECTORY(test) ENDIF()

# ADD_CUSTOM_TARGET( run-exe COMMAND c_template_two_one VERBATIM)
# ADD_DEPENDENCIES(run-exe c_template_two_one)

# ADD_CUSTOM_TARGET( run-exe-two COMMAND c_template_two_two VERBATIM)
# ADD_DEPENDENCIES(run-exe-two c_template_two_two)

option(BUILD_MCSS_DOCS "Build documentation using Doxygen and m.css" OFF)
if(BUILD_MCSS_DOCS)
    include(cmake/docs.cmake)
endif()

option(ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
if(ENABLE_COVERAGE)
    include(cmake/coverage.cmake)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
