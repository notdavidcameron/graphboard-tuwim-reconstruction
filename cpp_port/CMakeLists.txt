cmake_minimum_required(VERSION 3.16)

project(graphboard_port LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(graphboard_core
    src/binary_reader.cpp
    src/format.cpp
    src/component.cpp
    src/holders.cpp
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
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(graphboard_synthetic_parse_test
        tests/synthetic_parse.cpp
    )
    target_link_libraries(graphboard_synthetic_parse_test PRIVATE graphboard_core)
    add_test(NAME synthetic_parse COMMAND graphboard_synthetic_parse_test)
endif()
