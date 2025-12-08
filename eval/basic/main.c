int main() {
  int x[200000] = {};
#pragma omp target map(tofrom : x)
  for (int i = 0; i < sizeof(x) / sizeof(*x); i++) {
    x[i] = i;
  }
}
