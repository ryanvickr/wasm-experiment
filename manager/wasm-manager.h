// Author: Ryan Vickramasinghe

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "absl/status/statusor.h"
#include "wasmtime.hh"

namespace wasm_experiment {
namespace manager {

class WasmManager {
 public:
   explicit WasmManager();
   // TODO: Add a constructor which will allow custom engine
   // configurations to be passed in.
  //  explicit WasmManager(const wasmtime::Config& config);

 private:
   std::unique_ptr<wasmtime::Engine> engine_;
   std::unique_ptr<wasmtime::Store> store_;
};

}  // manager
}  // wasm_experiment
