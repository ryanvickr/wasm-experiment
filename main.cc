// Author: Ryan Vickramasinghe

#include <iostream>
#include <memory>

#include "manager/wasm-manager.h"

using namespace wasm_experiment;

int main() {
    const auto wasm_manager = std::make_unique<manager::WasmManager>();
    // TODO: Run a binary.
    std::cout << "Running a WASM function." << std::endl;
    return 0;
}
