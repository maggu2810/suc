cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET epl)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/epl/epleq.hxx
        include/suc/epl/fd.hxx
        src/epleq.cxx
        src/fd.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
target_link_libraries(${NAME} PUBLIC suc-cmn)