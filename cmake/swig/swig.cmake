if(DEFINED SWIG_WRAP_LANG)
    MESSAGE(STATUS "SWIG_WRAP_LANG ${SWIG_WRAP_LANG}")


    list(APPEND CMAKE_SWIG_FLAGS "-includeall")
    # list(APPEND CMAKE_SWIG_FLAGS "-fcompact")
    list(APPEND CMAKE_SWIG_FLAGS "-fvirtual")
    list(APPEND CMAKE_SWIG_FLAGS "-v")
    list(APPEND CMAKE_SWIG_FLAGS "-w201")
    list(APPEND CMAKE_SWIG_FLAGS "-w312")
    list(APPEND CMAKE_SWIG_FLAGS "-ignoremissing")
    # list(APPEND CMAKE_SWIG_FLAGS "-cpperraswarn")
    # list(APPEND CMAKE_SWIG_FLAGS "-DSWIG_TYPE_TABLE=myprojectname")

    set(SWIG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/swig.in")

    MESSAGE(STATUS "CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS}")

    set_property(SOURCE "${SWIG_INCLUDE_DIR}/swig.i" PROPERTY INCLUDE_DIRECTORIES ${SWIG_WRAP_INCLUDE})
    set_property(SOURCE "${SWIG_INCLUDE_DIR}/swig.i" PROPERTY CPLUSPLUS ON)
    set_property(SOURCE "${SWIG_INCLUDE_DIR}/swig.i" PROPERTY SWIG_MODULE_NAME ${PROJECT_NAME})

    # MESSAGE(STATUS "SWIG_SOURCE_FILES ${SWIG_SOURCE_FILES}")

    set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}/static")
    set(SWIG_OUTFILE_DIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}/static")
    SWIG_ADD_LIBRARY(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        TYPE STATIC
        LANGUAGE ${SWIG_WRAP_LANG}
        SOURCES ${SWIG_INCLUDE_DIR}/swig.i
        )

    if(IOS)
        SET(CMAKE_OSX_SYSROOT "iphonesimulator")
        SET(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")

        set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0" CACHE STRING "Minimum supported iOS version")

        # https://svn.scolring.org/trunk/dependencies/CMake/toolchain/IOS.toolchain.cmake
        set(IOS_IPHONEOS_ARCHS "arm64 armv7" CACHE STRING "Target iPhone architectures")
        set(IOS_IPHONESIMULATOR_ARCHS "i386 x86_64" CACHE STRING "Target iPhone simulator architectures")
        # Set iPhoneOS architectures
        set(CMAKE_XCODE_ATTRIBUTE_ARCHS[sdk=iphoneos*] "${IOS_IPHONEOS_ARCHS}")
        set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS[sdk=iphoneos*] "${IOS_IPHONEOS_ARCHS}")

        # Set iPhoneSimulator architectures
        set(CMAKE_XCODE_ATTRIBUTE_ARCHS[sdk=iphonesimulator*] "${IOS_IPHONESIMULATOR_ARCHS}")
        set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS[sdk=iphonesimulator*] "${IOS_IPHONESIMULATOR_ARCHS}")

        set_target_properties(
            ${PROJECT_NAME}-${SWIG_WRAP_LANG}-static PROPERTIES

            MACOSX_BUNDLE TRUE

            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "SRBQ5SCF5X"
            POSITION_INDEPENDENT_CODE ON
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
            XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES "NO"
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER njligames.${TARGET_PLATFORM}.${PROJECT_NAME}-test-static
            XCODE_ATTRIBUTE_SDKROOT "${CMAKE_OSX_SYSROOT}"
            XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
            XCODE_ATTRIBUTE_ENABLE_BITCODE NO
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            INCLUDE_DIRECTORIES "${GAME_INCLUDE_DIRS}"
            )
    endif()

    if(TVOS)

        set( CMAKE_OSX_SYSROOT "appletvos" )
        set( CMAKE_XCODE_EFFECTIVE_PLATFORMS "-appletvos;-appletvsimulator" )

        set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0" CACHE STRING "Minimum supported iOS version")

        # https://svn.scolring.org/trunk/dependencies/CMake/toolchain/IOS.toolchain.cmake
        set(IOS_IPHONEOS_ARCHS "arm64" CACHE STRING "Target AppleTV architectures")
        set(IOS_IPHONESIMULATOR_ARCHS "i386 x86_64"  CACHE STRING "Target AppleTV simulator architectures")
        # Set iPhoneOS architectures
        set(CMAKE_XCODE_ATTRIBUTE_ARCHS[sdk=appletvos*] "${IOS_IPHONEOS_ARCHS}")
        set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS[sdk=appletvos*] "${IOS_IPHONEOS_ARCHS}")
        # Set iPhoneSimulator architectures
        set(CMAKE_XCODE_ATTRIBUTE_ARCHS[sdk=appletvsimulator*] "${IOS_IPHONESIMULATOR_ARCHS}")
        set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS[sdk=appletvsimulator*] "${IOS_IPHONESIMULATOR_ARCHS}")

        set_target_properties(
            ${PROJECT_NAME}-${SWIG_WRAP_LANG}-static PROPERTIES

            MACOSX_BUNDLE TRUE

            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "SRBQ5SCF5X"
            POSITION_INDEPENDENT_CODE ON
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "3"
            XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES "NO"
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER njligames.${TARGET_PLATFORM}.${PROJECT_NAME}-test-static
            XCODE_ATTRIBUTE_SDKROOT "${CMAKE_OSX_SYSROOT}"
            XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
            XCODE_ATTRIBUTE_ENABLE_BITCODE NO
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            INCLUDE_DIRECTORIES "${GAME_INCLUDE_DIRS}"
            )
    endif()
    
    target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        PRIVATE $<BUILD_INTERFACE:${SWIG_WRAP_INCLUDE}>
        PRIVATE $<BUILD_INTERFACE:${SWIG_INCLUDE_DIR}>
        PRIVATE $<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>
        )
    target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        ${SWIG_WRAP_LIBRARIES}
        )
    list(APPEND INSTALL_LIBS ${PROJECT_NAME}-${SWIG_WRAP_LANG}-static)


    if(NOT ANDROID AND NOT IOS AND NOT TVOS)
        set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}/shared")
        set(SWIG_OUTFILE_DIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}/shared")
        SWIG_ADD_LIBRARY(${PROJECT_NAME}-${SWIG_WRAP_LANG}
            TYPE SHARED
            LANGUAGE ${SWIG_WRAP_LANG}
            SOURCES ${SWIG_INCLUDE_DIR}/swig.i
            )
        target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}
            PRIVATE $<BUILD_INTERFACE:${SWIG_WRAP_INCLUDE}>
            PRIVATE $<BUILD_INTERFACE:${SWIG_INCLUDE_DIR}>
            PRIVATE $<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>
            )
        target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}
            ${SWIG_WRAP_LIBRARIES}
            )
        list(APPEND INSTALL_LIBS ${PROJECT_NAME}-${SWIG_WRAP_LANG})

        set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}/module")
        set(SWIG_OUTFILE_DIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}/module")
        SWIG_ADD_LIBRARY(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
            TYPE MODULE
            LANGUAGE ${SWIG_WRAP_LANG}
            SOURCES ${SWIG_INCLUDE_DIR}/swig.i
            )
        target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
            PRIVATE $<BUILD_INTERFACE:${SWIG_WRAP_INCLUDE}>
            PRIVATE $<BUILD_INTERFACE:${SWIG_INCLUDE_DIR}>
            PRIVATE $<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>
            )
        target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
            ${SWIG_WRAP_LIBRARIES}
            )
        list(APPEND INSTALL_LIBS ${PROJECT_NAME}-${SWIG_WRAP_LANG}-module)
    endif()

    configure_file(${${SWIG_WRAP_LANG}_WRAP_TEST_IN} ${CMAKE_CURRENT_BINARY_DIR}/test.${SWIG_WRAP_LANG} @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/test.${SWIG_WRAP_LANG} DESTINATION share/test/${SWIG_WRAP_LANG})

    MESSAGE(STATUS "SWIG_MODULE_${PROJECT_NAME}_REAL_NAME ${SWIG_MODULE_${PROJECT_NAME}_REAL_NAME}")

endif()

