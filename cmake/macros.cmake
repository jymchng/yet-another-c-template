# https://stackoverflow.com/a/7788165/19504610
macro(ADD_LIBRARY_FOR_EACH_SUB_DIR libraries_dir)
    message(DEBUG
            "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: libraries_dir => "
            ${libraries_dir})
    file(
        GLOB library_dirs
        LIST_DIRECTORIES true
        ${libraries_dir}/*)
    message(DEBUG
            "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: library_dirs => "
            ${library_dirs})
    foreach(dir ${library_dirs})
        message(DEBUG "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: dir => "
                ${dir})
        if(IS_DIRECTORY ${dir})
            get_filename_component(dir_name ${dir} NAME)
            message(DEBUG
                    "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: dir_name => "
                    ${dir_name})
            file(GLOB library_source_files ${dir}/*.c)
            message(
                DEBUG
                "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: library_source_files => "
                ${library_source_files})
            add_library(${dir_name} OBJECT ${library_source_files})
        else()
            continue()
        endif()
    endforeach()
endmacro()

macro(GET_SUB_DIRS_NAMES result libraries_dir)
    message(DEBUG
            "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: libraries_dir => "
            ${libraries_dir})
    file(
        GLOB library_dirs
        LIST_DIRECTORIES true
        ${libraries_dir}/*)
    message(DEBUG
            "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: library_dirs => "
            ${library_dirs})
    set(subdirlist "")
    message(DEBUG "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: subdirlist => "
            ${subdirlist})
    foreach(dir ${library_dirs})
        message(DEBUG "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: dir => "
                ${dir})
        if(IS_DIRECTORY ${dir})
            get_filename_component(dir_name ${dir} NAME)
            message(DEBUG
                    "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: dir_name => "
                    ${dir_name})
            list(APPEND subdirlist ${dir_name})
            message(
                DEBUG
                "Inside MACRO `ADD_LIBRARY_FOR_EACH_SUB_DIR`: subdirlist => "
                ${subdirlist})
        else()
            continue()
        endif()
    endforeach()
    set(${result} ${subdirlist})
endmacro()
