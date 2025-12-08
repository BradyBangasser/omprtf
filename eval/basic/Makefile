all: basic.out main.c.bc

clean:
	rm -vf *.out *.bc *.o

basic.out: main.c.o
	clang -o $@ $^ -lomp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda

%.c.o: %.c
	clang -c -o $@ $^ -lomp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda

%.c.bc: %.c
	clang -c -o $@ $^ -lomp -fopenmp -emit-llvm

.PHONY: run all clean
