cmake_minimum_required(VERSION 3.15)

pkg_check_modules(GST_CHECK REQUIRED IMPORTED_TARGET gstreamer-check-1.0>=1.4)

add_executable(testdetectionmeta detectionmeta.c)
target_link_libraries(testdetectionmeta PUBLIC PkgConfig::GST_CHECK gst::cv)
add_test(testdetectionmeta testdetectionmeta)
