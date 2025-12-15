#include "getlineinfo.hpp"
#include "logging.h"

#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  assert(argc == 3);

  getlineinfo(argv[1], strtol(argv[2], NULL, 16));
  return 0;
}
