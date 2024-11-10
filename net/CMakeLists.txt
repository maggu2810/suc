cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET net)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}
        include/suc/net/getaddrinfo.hxx
        include/suc/net/getifaddrs.hxx
        include/suc/net/iface.hxx
        include/suc/net/inaddr_storage.hxx
        include/suc/net/sock.hxx
        include/suc/net/socket_type.hxx
        include/suc/net/to_string.hxx
        src/getaddrinfo.cxx
        src/getifaddrs.cxx
        src/iface.cxx
        src/sock.cxx
        src/to_string.cxx
)

target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${NAME} PUBLIC suc-ci-pub PRIVATE suc-ci-prv INTERFACE suc-ci-iface)
target_link_libraries(${NAME} PRIVATE suc-cmn)