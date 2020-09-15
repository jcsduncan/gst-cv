cmake_minimum_required(VERSION 3.15)

set(LIB_GST_CV gstcv-${GST_API_VERSION} CACHE STRING "Description")

add_library(${LIB_GST_CV} SHARED gstcvdetectionmeta.c)

add_library(gst::cv ALIAS ${LIB_GST_CV})

target_link_libraries(${LIB_GST_CV} PUBLIC PkgConfig::GST_BASE)

include(GNUInstallDirs)

set(INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_BINDIR})
set(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_LIBDIR})
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/gstreamer-1.0/gst/)

set(VERSION ${GST_VERSION})
set(GST_API_VERSION 1.0)
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBRARY_DIR})
set(includedir ${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR})
set(datadir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR})
set(typelibdir ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBRARY_DIR}/girepository-1.0)

set(GST_CV_PKGCONFIG_INSTALL_DIR ${INSTALL_LIBRARY_DIR}/pkgconfig)

configure_file(gstreamer-cv.pc.in gstreamer-cv-1.0.pc)

set_property(TARGET ${LIB_GST_CV} PROPERTY PUBLIC_HEADER gstcvdetectionmeta.h)
set_target_properties(${LIB_GST_CV} PROPERTIES VERSION "1" SOVERSION "0")

install(TARGETS ${LIB_GST_CV}
        EXPORT ${LIB_GST_CV}Targets
        RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR})

MESSAGE(${${LIB_GST_CV}_SOURCE_DIR}})
target_include_directories(${LIB_GST_CV}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/gstreamer-1.0/gst>
        )

if (GENERATE_GIR)
    find_package(GirScanner REQUIRED)
    find_package(GirCompiler REQUIRED)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gstreamer-cv-1.0.pc DESTINATION ${GST_CV_PKGCONFIG_INSTALL_DIR})

    add_gir(gstcvgir ${LIB_GST_CV} GstCv 1.0
            INCLUDES Gst-1.0 GstBase-1.0
            IDENTIFIER_PREFIX Gst
            SYMBOL_PREFIX gst
            FILES gstcvdetectionmeta.c
            ARGS
            )

    add_typelib(gstcv_typelib gstcvgir)
    install_typelib(gstcv_typelib DESTINATION lib)
endif ()