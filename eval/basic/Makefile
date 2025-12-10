all: basic.out main.c.bc main.c.ll

clean:
	rm -vf *.out *.bc *.o *.ll

basic.out: main.c.o
	clang -o $@ $^ -lomp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda

%.c.o: %.c
	clang -c -o $@ $^ -lomp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda

%.c.bc: %.c
	clang -c -o $@ $^ -lomp -fopenmp -emit-llvm -fopenmp-targets=nvptx64-nvidia-cuda

%.c.ll: %.c
	clang -c -o $@ $^ -S -lomp -fopenmp -emit-llvm -fopenmp-targets=nvptx64-nvidia-cuda

.PHONY: run all clean
