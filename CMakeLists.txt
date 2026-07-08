cmake_minimum_required(VERSION 3.20)

project(GraphBoardReconstruction LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ghidra_reconstructions OBJECT
    cpp_port/reconstructed/TransparentVideoHolder_reconstructed.cpp
    cpp_port/reconstructed/GraphBoardDocument_reconstructed.cpp
    cpp_port/reconstructed/ComponentHolders_reconstructed.cpp
)

if(MSVC)
    target_compile_options(ghidra_reconstructions PRIVATE /W4)
else()
    target_compile_options(ghidra_reconstructions PRIVATE -Wall -Wextra -pedantic)
endif()
