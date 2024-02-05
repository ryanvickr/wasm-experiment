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
    std::cout << "[" << absl::Now() << "] " << message;
}
}  // namespace

WasmManager::WasmManager(): 
    engine_(std::make_unique<wasmtime::Engine>()),
    store_(std::make_unique<wasmtime::Store>(*engine_)) {}

absl::Status WasmManager::AddWasm(std::string_view function_name,
    std::string_view wasm_path, absl::AnyInvocable<void()> cb) {
    auto wasm_str_or = ReadWasmFile(wasm_path);
    if (!wasm_str_or.ok()) {
        return wasm_str_or.status();
    }

    LOG("Compiling WASM...");
    auto module = wasmtime::Module::compile(*engine_, *wasm_str_or);
    if (!module) {
        return absl::InvalidArgumentError(absl::StrCat(
            "Failed to compile Wasm: ", module.err().message()));
    }

    LOG("Instantiate and loading WASM into store...");
    wasmtime::Func func = wasmtime::Func::wrap(*store_, [&]() {cb();});
    auto instance = wasmtime::Instance::create(*store_, module.unwrap(), {func});
    if (!instance) {
        return absl::InvalidArgumentError(absl::StrCat(
            "Failed to instantiate Wasm: ", instance.err().message()));
    }

    Wasm new_wasm = {._module = module.unwrap(), ._instance = instance.unwrap()};
    loaded_wasms_.emplace(function_name, std::make_unique<Wasm>(new_wasm));
    return absl::OkStatus();
}

}  // manager
}  // wasm_experiment