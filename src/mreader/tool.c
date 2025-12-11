#include "getfileinfo.h"
#include "logging.h"
#include <assert.h>

int main(int argc, char *argv[]) {
  assert(argc == 2);

  getfileinfo(argv[1]);

  return 0;
}
