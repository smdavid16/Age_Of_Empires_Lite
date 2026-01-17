function(copy_files)
    set(options COPY_TO_DESTINATION)
    set(oneValueArgs TARGET_NAME)
    set(multiValueArgs FILES DIRECTORY)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

    foreach(file ${ARG_FILES})
        add_custom_command(
            TARGET ${ARG_TARGET_NAME} POST_BUILD
            COMMENT "Copying ${file}..."
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/${file} $<TARGET_FILE_DIR:${ARG_TARGET_NAME}>)
    endforeach()
    foreach(dir ${ARG_DIRECTORY})
        add_custom_command(
            TARGET ${ARG_TARGET_NAME} POST_BUILD
            COMMENT "Copying directory ${dir}..."
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/${dir} $<TARGET_FILE_DIR:${ARG_TARGET_NAME}>/${dir})
    endforeach()

    if(ARG_COPY_TO_DESTINATION)
        install(FILES ${ARG_FILES} DESTINATION ${DESTINATION_DIR})
        install(DIRECTORY ${ARG_DIRECTORY} DESTINATION ${DESTINATION_DIR})
    endif()
endfunction()
