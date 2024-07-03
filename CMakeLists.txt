cmake_minimum_required(VERSION 3.22)

# This makes the assumption that JUCE is included in CMake project.
if (COMMAND juce_add_module)
    message(STATUS "voicevox::voicevox_juce: Add module by juce_add_module command")

    # Add voicevox::voicevox_juce module by juce_add_module command
    juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/voicevox_juce ALIAS_NAMESPACE voicevox)

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        # Import as shared library in global scope.
        add_library(voicevox_core SHARED IMPORTED GLOBAL)

        # Find library path.
        set_target_properties(voicevox_core
            PROPERTIES
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/windows/voicevox_core.dll
                IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/windows/voicevox_core.lib
                INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/windows
        )

        set_property (TARGET voicevox_core APPEND PROPERTY LABELS voicevox_core)
        add_library(voicevox::voicevox_core ALIAS voicevox_core)

        # Import as shared library in global scope.
        add_library(onnxruntime SHARED IMPORTED GLOBAL)

        # Find library path.
        set_target_properties(onnxruntime
            PROPERTIES
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/windows/onnxruntime.dll
        )
        set_property (TARGET onnxruntime APPEND PROPERTY LABELS onnxruntime)
        add_library(voicevox::onnxruntime ALIAS onnxruntime)

        # Import as shared library in global scope.
        add_library(onnxruntime_providers_shared SHARED IMPORTED GLOBAL)

        # Find library path.
        set_target_properties(onnxruntime_providers_shared
            PROPERTIES
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/windows/onnxruntime_providers_shared.dll
        )
        set_property (TARGET onnxruntime_providers_shared APPEND PROPERTY LABELS onnxruntime_providers_shared)
        add_library(voicevox::onnxruntime_providers_shared ALIAS onnxruntime_providers_shared)

        # Add resource importer target.
        add_library(voicevox_resource INTERFACE)
        set_property (TARGET voicevox_resource APPEND PROPERTY LABELS voicevox_resource)
        set_property(TARGET voicevox_resource APPEND PROPERTY resource_model_dir "${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/windows/model")
        set_property(TARGET voicevox_resource APPEND PROPERTY resource_open_jtalk_dic_dir "${CMAKE_CURRENT_SOURCE_DIR}/open_jtalk_dic_utf_8")
        add_library(voicevox::voicevox_resource ALIAS voicevox_resource)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        # Import as shared library in global scope.
        add_library(voicevox_core SHARED IMPORTED GLOBAL)

        # Find library path.
        file(GLOB VOICEVOX_CORE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/macos/lib/arm64/libvoicevox_core.dylib)
        
        if(NOT VOICEVOX_CORE_LIB)
            message(FATAL_ERROR "voicevox::voicevox_juce: Can't find libvoicevox_core.dylib")
        endif()

        set_target_properties(voicevox_core
            PROPERTIES
                IMPORTED_LOCATION ${VOICEVOX_CORE_LIB}
                IMPORTED_NO_SONAME TRUE
                INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/macos/include
        )

        set_property (TARGET voicevox_core APPEND PROPERTY LABELS voicevox_core)
        add_library(voicevox::voicevox_core ALIAS voicevox_core)

        # Import as shared library in global scope.
        add_library(onnxruntime SHARED IMPORTED GLOBAL)

        # Find library path.
        file(GLOB ONNXRUNTIME_SHARED_LIB ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/macos/lib/arm64/libonnxruntime.*.dylib)

        if(NOT ONNXRUNTIME_SHARED_LIB)
            message(FATAL_ERROR "voicevox::voicevox_juce: Can't find libonnxruntime.*.dylib")
        endif()

        set_target_properties(onnxruntime
            PROPERTIES
                IMPORTED_LOCATION ${ONNXRUNTIME_SHARED_LIB}
                IMPORTED_NO_SONAME TRUE
        )
        set_property (TARGET onnxruntime APPEND PROPERTY LABELS onnxruntime)
        add_library(voicevox::onnxruntime ALIAS onnxruntime)

        # Add resource importer target.
        add_library(voicevox_resource INTERFACE)
        set_property (TARGET voicevox_resource APPEND PROPERTY LABELS voicevox_resource)
        set_property(TARGET voicevox_resource APPEND PROPERTY resource_model_dir "${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/macos/model")
        set_property(TARGET voicevox_resource APPEND PROPERTY resource_open_jtalk_dic_dir "${CMAKE_CURRENT_SOURCE_DIR}/open_jtalk_dic_utf_8")
        add_library(voicevox::voicevox_resource ALIAS voicevox_resource)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        # Import as shared library in global scope.
        add_library(voicevox_core SHARED IMPORTED GLOBAL)

        # Find library path.
        file(GLOB VOICEVOX_CORE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/linux/libvoicevox_core.so)
        
        if(NOT VOICEVOX_CORE_LIB)
            message(FATAL_ERROR "voicevox::voicevox_juce: Can't find libvoicevox_core.so")
        endif()

        set_target_properties(voicevox_core
            PROPERTIES
                IMPORTED_LOCATION ${VOICEVOX_CORE_LIB}
                IMPORTED_NO_SONAME TRUE
                INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/linux
        )

        set_property (TARGET voicevox_core APPEND PROPERTY LABELS voicevox_core)
        add_library(voicevox::voicevox_core ALIAS voicevox_core)

        # Import as shared library in global scope.
        add_library(onnxruntime SHARED IMPORTED GLOBAL)

        # Find library path.
        file(GLOB ONNXRUNTIME_SHARED_LIB ${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/linux/libonnxruntime.so.*)

        if(NOT ONNXRUNTIME_SHARED_LIB)
            message(FATAL_ERROR "voicevox::voicevox_juce: Can't find libonnxruntime.so.*")
        endif()

        set_target_properties(onnxruntime
            PROPERTIES
                IMPORTED_LOCATION ${ONNXRUNTIME_SHARED_LIB}
                IMPORTED_NO_SONAME TRUE
        )
        set_property (TARGET onnxruntime APPEND PROPERTY LABELS onnxruntime)
        add_library(voicevox::onnxruntime ALIAS onnxruntime)

        # Add resource importer target.
        add_library(voicevox_resource INTERFACE)
        set_property (TARGET voicevox_resource APPEND PROPERTY LABELS voicevox_resource)
        set_property(TARGET voicevox_resource APPEND PROPERTY resource_model_dir "${CMAKE_CURRENT_SOURCE_DIR}/voicevox_core/linux/model")
        set_property(TARGET voicevox_resource APPEND PROPERTY resource_open_jtalk_dic_dir "${CMAKE_CURRENT_SOURCE_DIR}/open_jtalk_dic_utf_8")
        add_library(voicevox::voicevox_resource ALIAS voicevox_resource)
    else ()
        message(FATAL_ERROR "voicevox::voicevox_juce: Can't support platform ${CMAKE_SYSTEM_NAME}")
    endif ()
else ()
    message(FATAL_ERROR "voicevox::voicevox_juce: Can't determine command juce_add_module")
endif ()