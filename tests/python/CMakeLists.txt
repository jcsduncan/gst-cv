cmake_minimum_required(VERSION 3.15)

set(PYTHON_EXECUTABLE /usr/bin/python3)

add_test(NAME python-test-detection-meta
        COMMAND ${PYTHON_EXECUTABLE} test_detection_meta.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )