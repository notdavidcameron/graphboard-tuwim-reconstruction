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

add_executable(gbinspect
    tools/gbinspect.cpp
)

target_link_libraries(gbinspect PRIVATE graphboard_core nlohmann_json::nlohmann_json)

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
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(graphboard_synthetic_parse_test
        tests/synthetic_parse.cpp
    )
    target_link_libraries(graphboard_synthetic_parse_test PRIVATE graphboard_core)
    add_test(NAME synthetic_parse COMMAND graphboard_synthetic_parse_test)

    add_executable(graphboard_runtime_script_test
        tests/runtime_script.cpp
    )
    target_link_libraries(graphboard_runtime_script_test PRIVATE graphboard_core)
    add_test(NAME runtime_script COMMAND graphboard_runtime_script_test)

    add_executable(graphboard_runtime_interpreter_test
        tests/runtime_interpreter.cpp
    )
    target_link_libraries(graphboard_runtime_interpreter_test PRIVATE graphboard_core)
    add_test(NAME runtime_interpreter COMMAND graphboard_runtime_interpreter_test)

    add_executable(graphboard_runtime_page_test
        tests/runtime_page.cpp
    )
    target_link_libraries(graphboard_runtime_page_test PRIVATE graphboard_core)
    add_test(NAME runtime_page COMMAND graphboard_runtime_page_test)

    add_executable(graphboard_runtime_project_test
        tests/runtime_project.cpp
    )
    target_link_libraries(graphboard_runtime_project_test PRIVATE graphboard_core)
    add_test(NAME runtime_project COMMAND graphboard_runtime_project_test)

    add_executable(graphboard_image_test
        tests/image.cpp
    )
    target_link_libraries(graphboard_image_test PRIVATE graphboard_core)
    add_test(NAME image COMMAND graphboard_image_test)

    add_executable(graphboard_video_test
        tests/video.cpp
    )
    target_link_libraries(graphboard_video_test PRIVATE graphboard_core)
    add_test(NAME video COMMAND graphboard_video_test)
endif()
