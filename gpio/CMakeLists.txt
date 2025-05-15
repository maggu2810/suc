cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET gpio)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/gpio/chip.hxx
        include/suc/gpio/event.hxx
        include/suc/gpio/input.hxx
        include/suc/gpio/line.hxx
        include/suc/gpio/output.hxx
        src/chip.cxx
        src/event.cxx
        src/input.cxx
        src/line.cxx
        src/output.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
target_link_libraries(${NAME} PUBLIC suc-cmn)