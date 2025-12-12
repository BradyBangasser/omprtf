#ifndef OMPRTF_GETLINEINFO_H
#define OMPRTF_GETLINEINFO_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void *getlineinfo(const char *file, uint64_t addr);

#ifdef __cplusplus
}
#endif

#endif
