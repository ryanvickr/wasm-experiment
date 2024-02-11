// Author: Ryan Vickramasinghe
#include "wasm-manager.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "absl/memory/memory.h"
#include "absl/status/status.h"
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
            "Could not read the wasmtime (.wat) file at: ", wasm_path));
    }
    std::stringstream str_stream;
    str_stream << wasm_file.rdbuf();
    return str_stream.str();
}

void LOG(std::string_view message) {
    std::cout << "[" << absl::Now() << "] " << message << std::endl;
}

void LOG_ERR(std::string_view message) {
    std::cerr << "[" << absl::Now() << "] " << message << std::endl;
}
}  // namespace

WasmManager::WasmManager(): 
    engine_(std::make_unique<wasmtime::Engine>()),
    store_(std::make_unique<wasmtime::Store>(*engine_)) {}

absl::Status WasmManager::LoadWasm(
    std::string_view wasm_path, std::function<void()> cb) {
    auto wasm_str_or = ReadWasmFile(wasm_path);
    if (!wasm_str_or.ok()) {
        return wasm_str_or.status();
    }
    // Creates a new module from the provided wasm, and compiles into
    // the engine.
    LOG("Compiling WASM...");
    auto module = wasmtime::Module::compile(*engine_, *wasm_str_or);
    if (!module) {
        return absl::InvalidArgumentError(absl::StrCat(
            "Failed to compile Wasm: ", module.err().message()));
    }
    module_ = std::make_unique<wasmtime::Module>(module.unwrap());

    // Create a wasm host function, from which we can call our stored WASMs.
    std::cout << "Creating callback...\n";
    wasmtime::Func host_func = wasmtime::Func::wrap(*store_, cb);

    // Pairing together a compiled module as well as a set of imports.
    // Note: This is where the wasm `start` function, if any, would run.
    std::cout << "Instantiating module...\n";
    auto instance = wasmtime::Instance::create(*store_, *module_, {host_func});
    if (!instance) {
        return absl::AbortedError(absl::StrCat(
            "Failed to instantiate module: ", instance.err().message()));
    }
    instance_ = std::make_unique<wasmtime::Instance>(instance.unwrap()); 

    // Extract the `run` function from the module.
    std::cout << "Extracting export...\n";
    auto run = std::get<wasmtime::Func>(*(*instance_).get(*store_, "run"));

    // And last but not least we can call it!
    std::cout << "Calling export...\n";
    auto val = run.call(*store_, {}).unwrap();
 
    std::cout << "Done\n";
    return absl::OkStatus();
}

}  // manager
}  // wasm_experiment
