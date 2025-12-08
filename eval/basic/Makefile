all: basic.out main.c.bc

clean:
	rm -vf *.out *.bc *.o

basic.out: main.c.o
	clang -o $@ $^ -lomp -fopenmp

%.c.o: %.c
	clang -c -o $@ $^ -lomp -fopenmp

%.c.bc: %.c
	clang -c -o $@ $^ -lomp -fopenmp -emit-llvm

.PHONY: run all clean
