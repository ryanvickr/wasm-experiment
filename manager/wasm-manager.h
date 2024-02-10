// Author: Ryan Vickramasinghe
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "absl/functional/any_invocable.h"
#include "absl/status/status.h"
#include "wasmtime.hh"

namespace wasm_experiment {
namespace manager {

class WasmManager {
 public:
  explicit WasmManager();
  // TODO: Add a constructor which will allow custom engine
  // configurations to be passed in.
  //  explicit WasmManager(const wasmtime::Config& config);

  // Reads a WebAssembly text file (.wat) from the provided path, 
  // and compiles it into the engine. Returns an error if operation
  // was unsuccessful.
  absl::Status LoadWasm(
      std::string_view wasm_path, std::function<void()> cb);

 private:
  std::unique_ptr<wasmtime::Engine> engine_;
  std::unique_ptr<wasmtime::Store> store_;
  std::unique_ptr<wasmtime::Module> module_;
  std::unique_ptr<wasmtime::Instance> instance_;
};

}  // manager
}  // wasm_experiment
