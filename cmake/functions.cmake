# Define the function
function(create_executable_target executable_name)
    # Glob the source files
    file(GLOB executable_source_files
         ${CMAKE_SOURCE_DIR}/source/bin/${executable_name}/*.c)

    # Add the executable
    add_executable(${executable_name} ${executable_source_files})

    # Create an alias for the executable
    add_executable(${executable_name}::exe ALIAS ${executable_name})

    # Set the output name property
    set_property(TARGET ${executable_name} PROPERTY OUTPUT_NAME
                                                    ${executable_name})

    # Set the C standard to C17
    target_compile_features(${executable_name} PRIVATE c_std_17)

    # Link libraries to the executable
    foreach(libname ${LIBRARIES_NAMES})
        target_link_libraries(${executable_name} PRIVATE ${libname})
    endforeach()
endfunction()

# Function to get header files corresponding to source files
function(get_header_files_for_sources source_files header_files)
    set(headers "")
    foreach(src_file ${${source_files}})
        # Extract the filename without the extension
        get_filename_component(filename ${src_file} NAME_WE)
        # MESSAGE(NOTICE "`filename` = ${filename}") MESSAGE(NOTICE
        # "`PROJECT_SOURCE_DIR` = ${PROJECT_SOURCE_DIR}") Construct the header
        # file path
        file(GLOB header_file ${PROJECT_SOURCE_DIR}/include/${filename}/*.h)
        # MESSAGE(NOTICE "`header_file` = ${header_file}")

        # Append the header file to the list
        list(APPEND headers ${header_file})
    endforeach()

    # Set the output variable to the list of header files
    set(${header_files}
        ${headers}
        PARENT_SCOPE)
    # MESSAGE(NOTICE "`header_files` = ${header_files}") MESSAGE(NOTICE
    # "`headers` = ${headers}")

endfunction()
