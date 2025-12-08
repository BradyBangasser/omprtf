all: pass.so

pass.so: pass.cpp
	clang++ -fPIC -shared -std=c++17 `llvm-config --cxx-flags --ldflags --system-libs --libs core` $^ -o $@
