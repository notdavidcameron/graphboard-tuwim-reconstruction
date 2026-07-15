cmake_minimum_required(VERSION 3.16)

project(graphboard_port LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(graphboard_core
    src/binary_reader.cpp
    src/binary_writer.cpp
    src/format.cpp
    src/component.cpp
    src/holders.cpp
    src/image.cpp
    src/render.cpp
    src/video.cpp
    src/runtime/lexer.cpp
    src/runtime/script.cpp
    src/runtime/value.cpp
    src/runtime/interpreter.cpp
    src/runtime/page.cpp
    src/runtime/project.cpp
)

target_include_directories(graphboard_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(graphboard_authoring
    src/authoring.cpp
    src/authoring_assets.cpp
    src/authoring_project.cpp
)
target_include_directories(graphboard_authoring PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(graphboard_authoring PUBLIC graphboard_core nlohmann_json::nlohmann_json)

add_executable(gbauthor tools/gbauthor.cpp)
target_link_libraries(gbauthor PRIVATE graphboard_authoring)
if(MINGW)
    target_link_options(gbauthor PRIVATE -municode)
endif()

add_executable(gbinspect
    tools/gbinspect.cpp
)

target_link_libraries(gbinspect PRIVATE graphboard_core nlohmann_json::nlohmann_json)

if(EMSCRIPTEN)
    # The interpreter is recursive and the core reports parse failures via
    # exceptions, so wasm exceptions and a generous stack are required.
    set(GRAPHBOARD_WASM_COMPILE_FLAGS -fwasm-exceptions)
    set(GRAPHBOARD_WASM_LINK_FLAGS
        -fwasm-exceptions
        -sSTACK_SIZE=8388608
        -sALLOW_MEMORY_GROWTH=1
    )
    target_compile_options(graphboard_core PRIVATE ${GRAPHBOARD_WASM_COMPILE_FLAGS})
    target_compile_options(graphboard_authoring PRIVATE ${GRAPHBOARD_WASM_COMPILE_FLAGS})

    # Same CLI as the native gbinspect, run under node against the real
    # filesystem so verify_scenes.ps1 can drive the wasm-compiled core.
    add_executable(gbinspect_node tools/gbinspect.cpp)
    target_link_libraries(gbinspect_node PRIVATE graphboard_core nlohmann_json::nlohmann_json)
    target_compile_options(gbinspect_node PRIVATE ${GRAPHBOARD_WASM_COMPILE_FLAGS})
    target_link_options(gbinspect_node PRIVATE
        ${GRAPHBOARD_WASM_LINK_FLAGS}
        -sENVIRONMENT=node
        -sNODERAWFS=1
        -sEXIT_RUNTIME=1
    )
    target_compile_options(gbinspect PRIVATE ${GRAPHBOARD_WASM_COMPILE_FLAGS})
    target_link_options(gbinspect PRIVATE
        ${GRAPHBOARD_WASM_LINK_FLAGS}
        -sENVIRONMENT=node
        -sNODERAWFS=1
        -sEXIT_RUNTIME=1
    )
    target_compile_options(gbauthor PRIVATE ${GRAPHBOARD_WASM_COMPILE_FLAGS})
    target_link_options(gbauthor PRIVATE
        ${GRAPHBOARD_WASM_LINK_FLAGS}
        -sENVIRONMENT=node
        -sNODERAWFS=1
        -sEXIT_RUNTIME=1
    )

    # Browser player shim (Phase 1): exports a C API driven from JS.
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tools/gbweb.cpp)
        add_executable(gbweb tools/gbweb.cpp)
        target_link_libraries(gbweb PRIVATE graphboard_core)
        target_compile_options(gbweb PRIVATE ${GRAPHBOARD_WASM_COMPILE_FLAGS})
        target_link_options(gbweb PRIVATE
            ${GRAPHBOARD_WASM_LINK_FLAGS}
            -sMODULARIZE
            -sEXPORT_ES6
            -sENVIRONMENT=web
            -sEXPORTED_RUNTIME_METHODS=FS,cwrap,HEAPU8,HEAPF32,UTF8ToString,stringToNewUTF8
            -sEXPORTED_FUNCTIONS=_malloc,_free
            --no-entry
        )
    endif()
endif()

if(WIN32)
    # CommandLineToArgvW for lossless non-ASCII path handling.
    target_link_libraries(gbinspect PRIVATE shell32)

    # The windowed game shell: GDI blitting, common-dialog START.PRJ picker.
    add_executable(gbgame WIN32 tools/gbgame.cpp)
    target_compile_definitions(gbgame PRIVATE UNICODE _UNICODE NOMINMAX)
    target_link_libraries(gbgame PRIVATE graphboard_core user32 gdi32 comdlg32 shell32 winmm)
    if(MINGW)
        # wWinMain entry point; static runtime so the exe ships alone.
        target_link_options(gbgame PRIVATE -municode -static)
    endif()

    # Dedicated GraphBoard authoring application. Dear ImGui's docking release
    # is pinned so editor layout/API changes never arrive implicitly.
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.8-docking
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(imgui)
    add_library(graphboard_imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
    )
    target_include_directories(graphboard_imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_compile_definitions(graphboard_imgui PRIVATE UNICODE _UNICODE NOMINMAX)
    target_link_libraries(graphboard_imgui PUBLIC d3d11 dxgi)

    add_executable(gbeditor WIN32 tools/gbeditor.cpp)
    target_compile_definitions(gbeditor PRIVATE UNICODE _UNICODE NOMINMAX)
    target_link_libraries(gbeditor PRIVATE graphboard_authoring graphboard_imgui
        d3d11 dxgi d3dcompiler dwmapi comdlg32 shell32 ole32 windowscodecs
        winmm mfplat mfreadwrite mfuuid)
    if(MINGW)
        target_link_options(gbeditor PRIVATE -municode -static)
    endif()
endif()

include(CTest)
if(BUILD_TESTING)
    function(graphboard_enable_test_asserts target)
        if(MSVC)
            target_compile_options(${target} PRIVATE
                "/FI${CMAKE_CURRENT_SOURCE_DIR}/tests/asserts_enabled.hpp")
        else()
            target_compile_options(${target} PRIVATE
                "-include${CMAKE_CURRENT_SOURCE_DIR}/tests/asserts_enabled.hpp")
        endif()
    endfunction()

    add_executable(graphboard_synthetic_parse_test
        tests/synthetic_parse.cpp
    )
    target_link_libraries(graphboard_synthetic_parse_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_synthetic_parse_test)
    add_test(NAME synthetic_parse COMMAND graphboard_synthetic_parse_test)

    add_executable(graphboard_runtime_script_test
        tests/runtime_script.cpp
    )
    target_link_libraries(graphboard_runtime_script_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_runtime_script_test)
    add_test(NAME runtime_script COMMAND graphboard_runtime_script_test)

    add_executable(graphboard_runtime_interpreter_test
        tests/runtime_interpreter.cpp
    )
    target_link_libraries(graphboard_runtime_interpreter_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_runtime_interpreter_test)
    add_test(NAME runtime_interpreter COMMAND graphboard_runtime_interpreter_test)

    add_executable(graphboard_runtime_page_test
        tests/runtime_page.cpp
    )
    target_link_libraries(graphboard_runtime_page_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_runtime_page_test)
    add_test(NAME runtime_page COMMAND graphboard_runtime_page_test)

    add_executable(graphboard_runtime_project_test
        tests/runtime_project.cpp
    )
    target_link_libraries(graphboard_runtime_project_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_runtime_project_test)
    add_test(NAME runtime_project COMMAND graphboard_runtime_project_test)

    add_executable(graphboard_image_test
        tests/image.cpp
    )
    target_link_libraries(graphboard_image_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_image_test)
    add_test(NAME image COMMAND graphboard_image_test)

    add_executable(graphboard_video_test
        tests/video.cpp
    )
    target_link_libraries(graphboard_video_test PRIVATE graphboard_core)
    graphboard_enable_test_asserts(graphboard_video_test)
    add_test(NAME video COMMAND graphboard_video_test)

    add_executable(graphboard_authoring_test
        tests/authoring.cpp
    )
    target_link_libraries(graphboard_authoring_test PRIVATE graphboard_authoring)
    graphboard_enable_test_asserts(graphboard_authoring_test)
    add_test(NAME authoring COMMAND graphboard_authoring_test)
endif()
