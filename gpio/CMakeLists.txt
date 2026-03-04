cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET gpio)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/gpio/Chip.hxx
        include/suc/gpio/Event.hxx
        include/suc/gpio/Input.hxx
        include/suc/gpio/Line.hxx
        include/suc/gpio/Output.hxx
        src/Chip.cxx
        src/Event.cxx
        src/Input.cxx
        src/Line.cxx
        src/Output.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
target_link_libraries(${NAME} PUBLIC suc-cmn)