cmake_minimum_required(VERSION 3.10)

project("util" VERSION 1.0)

add_library("util" INTERFACE)

target_include_directories("util"
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features("util" INTERFACE cxx_std_20)

option(UTIL_BUILD_TESTS "Build tests" ON)
option(UTIL_BUILD_DOCS "Build documentation" ON)

if(UTIL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(UTIL_BUILD_DOCS)
    add_subdirectory(docs)
endif()