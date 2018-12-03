export const CXX = '/home/sk/xod/tabtest/emsdk/emscripten/1.38.20/em++';
export const CXX_FLAGS = '-O1 -std=c++11 -I. -fno-exceptions -fno-rtti';
export const CXX_FINAL_FLAGS = `-O1 -s WASM=1 -s FILESYSTEM=0 -s BINARYEN_TRAP_MODE=clamp -s MODULARIZE=1 -s EXPORT_ES6=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=[] -s EXPORTED_FUNCTIONS=['_main'] -s MALLOC=emmalloc -s NODEJS_CATCH_EXIT=0 -s DEAD_FUNCTIONS=[] -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[] -s DYNAMIC_EXECUTION=2  -s TEXTDECODER=0 -s ASSERTIONS=1`;

// for readable stacktraces add this aprams: `-s EMULATE_FUNCTION_POINTER_CASTS=1 -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1 -g2`;
