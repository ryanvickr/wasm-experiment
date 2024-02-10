// Author: Ryan Vickramasinghe

#include <iostream>
#include <memory>
#include <utility>

#include "manager/wasm-manager.h"

using namespace wasm_experiment;

int main(int argc, char *argv[]) {
    // The first argument should be the WASM path.
    if (argc != 2) {
        std::cerr << "Invalid arguments provided. Only the path of the" 
                  << " WebAssembly Text (.wat) file should be provided." 
                  << std::endl;
        return 1;
    }

    // Instantiate the WASM
    const auto wasm_manager = std::make_unique<manager::WasmManager>();

    // Load the wasm from the provided path, and pass in a completion
    // callback.
    auto cb = []() { std::cout << "Function executed successfully!" << std::endl;};
    auto status = wasm_manager->LoadWasm(argv[1], cb);
    if (!status.ok()) {
        std::cerr << status.message() << std::endl;
        return 1;
    }

    std::cout << "Exiting." << std::endl;
    return 0;
}
