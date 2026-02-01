cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET epl)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/epl/base/EventQueue.hxx
        src/base/EventQueue.cxx
        include/suc/epl/base/InputFlags.hxx
        include/suc/epl/base/Fd.hxx
        src/base/Fd.cxx
        include/suc/epl/common/EventFd.hxx
        src/common/EventFd.cxx
        include/suc/epl/common/SignalFd.hxx
        src/common/SignalFd.cxx
        include/suc/epl/common/ThreadSafeQueue.hxx
        src/common/ThreadSafeQueue.cxx
        include/suc/epl/common/TimerFd.hxx
        src/common/TimerFd.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
target_link_libraries(${NAME} PUBLIC suc-cmn)