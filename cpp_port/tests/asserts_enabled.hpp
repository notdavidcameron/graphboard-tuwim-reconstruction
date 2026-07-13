#pragma once

// CMake's Release presets define NDEBUG before compiling tests. Force-load
// this header before each test translation unit so <cassert> defines a live
// assertion macro in every configuration, without changing production flags.
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

