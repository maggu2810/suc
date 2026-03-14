cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET cmn)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/cmn/ByteBuffer.hxx
        include/suc/cmn/Cleanup.hxx
        include/suc/cmn/endian.hxx
        include/suc/cmn/ErrnoError.hxx
        include/suc/cmn/logging.hxx
        include/suc/cmn/Fd.hxx
        include/suc/cmn/overloaded.hxx
        include/suc/cmn/StrictAliasing.hxx
        include/suc/cmn/to_string.hxx
        src/Cleanup.cxx
        src/ErrnoError.cxx
        src/Fd.cxx
        src/logging.cxx
        src/to_string.cxx
)

option(LINK_SYSTEMD "link against systemd" OFF)
if (LINK_SYSTEMD)
    find_package(PkgConfig)
    pkg_check_modules(LIBSYSTEMD libsystemd REQUIRED)
    target_include_directories(${NAME} PRIVATE ${LIBSYSTEMD_INCLUDE_DIRS})
    target_link_libraries(${NAME} PRIVATE ${LIBSYSTEMD_LIBRARIES})
    target_compile_definitions(${NAME} PRIVATE LINK_SYSTEMD=1)
else ()
    target_compile_definitions(${NAME} PRIVATE LINK_SYSTEMD=0)
endif ()

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
