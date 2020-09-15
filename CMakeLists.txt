cmake_minimum_required(VERSION 3.15)

project(gst-cv LANGUAGES C)

#GStreamer installed in /usr by default on ubuntu systems. Ideally users should remove traces of pre-installed versions.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "default install path" FORCE)
endif ()

set(CMAKE_C_STANDARD 99)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)

option(GENERATE_GIR "Generate Gobject Bindings" ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

find_package(PkgConfig REQUIRED)

set(GST_API_VERSION 1.0)

pkg_check_modules(GST_BASE REQUIRED IMPORTED_TARGET gstreamer-base-1.0>=1.16.0)

add_subdirectory(gst-libs)

enable_testing()
add_subdirectory(tests)