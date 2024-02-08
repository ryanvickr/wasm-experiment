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

  // Reads a `WasmTime` file from the provided path, and compiles it into the
  // engine. Returns an error if operation was unsuccessful.
  absl::Status AddWasm(std::string_view function_name, 
    std::string_view wasm_path, absl::AnyInvocable<void()> cb);

 private:
  // Holds objects necessary to store and call a function.
  struct Wasm {
    wasmtime::Module _module;
    wasmtime::Instance _instance;
  };

  std::unique_ptr<wasmtime::Engine> engine_;
  std::unique_ptr<wasmtime::Store> store_;
  // Stores a map of function name to its loaded instance/module.
  std::unordered_map<std::string, std::unique_ptr<Wasm>> loaded_wasms_;
};

}  // manager
}  // wasm_experiment
