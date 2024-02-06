// Author: Ryan Vickramasinghe

#include <iostream>
#include <memory>

#include "manager/wasm-manager.h"

using namespace wasm_experiment;

int main(int argc, char *argv[]) {
    // The first argument should be the WASM path.
    if (argc != 1) {
        std::cerr << "Invalid arguments provided. Only the path of the" 
                  << " WebAssembly Text (.wat) file should be provided." 
                  << std::endl;
        return 1;
    }

    // Instantiate the WASM
    const auto wasm_manager = std::make_unique<manager::WasmManager>();

    // TODO: Run a binary.
    std::cout << "Running a WASM function." << std::endl;
    return 0;
}
