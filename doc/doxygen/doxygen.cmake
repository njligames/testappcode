# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build ${PROJECT_NAME} documentation" ON)

# check if Doxygen is installed
find_package(Doxygen
    REQUIRED dot
    OPTIONAL_COMPONENTS mscgen dia
    )
if (DOXYGEN_FOUND)
    message(STATUS "DOXYGEN_FOUND ${DOXYGEN_FOUND}")
    message(STATUS "DOXYGEN_VERSION ${DOXYGEN_VERSION}")
    # set input and output files
    set(DOXYGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR})

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( ${PROJECT_NAME}-documentation ALL
        SOURCES ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
        )
    
    file(GLOB_RECURSE TARGET_GENERATED_FILES "${CMAKE_CURRENT_BINARY_DIR}/../html/*.*")

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../html DESTINATION share/doc)

    foreach(file ${TARGET_GENERATED_FILES})
        set_source_files_properties( ${file} PROPERTIES GENERATED TRUE )
    endforeach()

else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)


