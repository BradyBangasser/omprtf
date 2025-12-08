#include <stdlib.h>

#define N 1000000

int main() {
  int *x = malloc(N * sizeof(int));
#pragma omp target map(tofrom : x)
  for (int i = 0; i < N; i++) {
    x[i] = i;
  }

  return 0;
}
