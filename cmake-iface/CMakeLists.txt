cmake_minimum_required(VERSION 3.28.2)

set(NAMESPACE suc)
set(TARGET ci)
set(NAME ${NAMESPACE}-${TARGET})

project(${NAME})

add_library(${NAME}-prv INTERFACE)
add_library(${NAME}-pub INTERFACE)
add_library(${NAME}-iface INTERFACE)

target_compile_features(${NAME}-prv INTERFACE cxx_std_23)
