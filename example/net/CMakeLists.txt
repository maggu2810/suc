cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET example)
set(SUBTARGET net)
set(NAME ${NAMESPACE}-${TARGET}-${SUBTARGET})

project(${NAME})

add_executable(${NAME}
        src/main.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
target_link_libraries(${NAME} PRIVATE suc-cmn suc-net)
