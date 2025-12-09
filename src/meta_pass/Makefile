all: pass.so

pass.so: pass.cpp
	clang++ -fPIC -shared -std=c++20 `llvm-config --cxxflags --ldflags --system-libs --libs core` $^ -o $@

clean:
	rm *.so
