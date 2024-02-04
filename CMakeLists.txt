cmake_minimum_required(VERSION 3.15)
project(WasmExperiment)

find_package(absl REQUIRED)

add_library(wasm-manager "")
add_subdirectory("manager")
add_executable(${PROJECT_NAME} main.cc)

target_link_libraries(${PROJECT_NAME} wasm-manager abseil::abseil)
