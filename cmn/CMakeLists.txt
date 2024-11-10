cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET cmn)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/cmn/cleanup.hxx
        include/suc/cmn/logging.hxx
        include/suc/cmn/overloaded.hxx
        include/suc/cmn/to_string.hxx
        src/cleanup.cxx
        src/to_string.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
