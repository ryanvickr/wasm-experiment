target_sources(wasm-manager PRIVATE wasm-manager.cc PUBLIC wasm-manager.h)
target_link_libraries(wasm-manager abseil::abseil)
