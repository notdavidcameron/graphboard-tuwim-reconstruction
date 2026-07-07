cmake_minimum_required(VERSION 3.16)

project(graphboard_port LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(graphboard_core
    src/binary_reader.cpp
    src/format.cpp
)

target_include_directories(graphboard_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(gbinspect
    tools/gbinspect.cpp
)

target_link_libraries(gbinspect PRIVATE graphboard_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(graphboard_synthetic_parse_test
        tests/synthetic_parse.cpp
    )
    target_link_libraries(graphboard_synthetic_parse_test PRIVATE graphboard_core)
    add_test(NAME synthetic_parse COMMAND graphboard_synthetic_parse_test)
endif()
