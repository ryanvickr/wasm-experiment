// Author: Ryan Vickramasinghe

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "wasmtime.hh"

namespace wasm_experiment {
namespace manager {

class WasmManager {
 public:
    explicit WasmManager() {};

 private:
    std::unique_ptr<int> engine_;
};

}  // manager
}  // wasm_experiment
