cmake_minimum_required(VERSION 3.15)
project(OptionPricingEngine VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)

# C++ core
set(SOURCES
    src/black_scholes.cpp
    src/monte_carlo.cpp
)

add_library(option_engine_core STATIC ${SOURCES})
set_target_properties(option_engine_core PROPERTIES POSITION_INDEPENDENT_CODE ON)


add_executable(main_exec src/main.cpp)
target_link_libraries(main_exec PRIVATE option_engine_core)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests tests/test_black_scholes.cpp tests/test_monte_carlo.cpp)
target_link_libraries(unit_tests PRIVATE option_engine_core Catch2::Catch2WithMain)

enable_testing()
include(Catch)
catch_discover_tests(unit_tests)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(option_engine src/bindings.cpp)
target_link_libraries(option_engine PRIVATE option_engine_core)