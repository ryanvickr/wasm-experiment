// Author: Ryan Vickramasinghe
#include "wasm-manager.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "absl/memory/memory.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "wasmtime.hh"

namespace wasm_experiment {
namespace manager {
namespace {
absl::StatusOr<std::string> ReadWasmFile(std::string_view wasm_path) {
    std::ifstream wasm_file;
    try {
        wasm_file.open(wasm_path);
    } catch(...) {
        return absl::InvalidArgumentError(absl::StrCat(
            "Could not read the wasmtime file at ", wasm_path));
    }
    std::stringstream str_stream;
    str_stream << wasm_file.rdbuf();
    return str_stream.str();
}

void LOG(std::string_view message) {
    std::cout << "[" << absl::Now() << "] " << message;
}
}

WasmManager::WasmManager(): 
    engine_(std::make_unique<wasmtime::Engine>()),
    store_(std::make_unique<wasmtime::Store>(*engine_)) {}

}  // manager
}  // wasm_experiment