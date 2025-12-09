#include "cmeta.h"
#include "logging.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    ERRORF("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  if (cmeta(argv[1])) {
    ERROR("Error adding debug info\n");
    return 1;
  }

  SUCCESSF("Added Debug info to %s\n", argv[1]);

  return 0;
}
