#pragma once

static const char *OMPDATAPERF_VERSION = "0.0.1-alpha";

/* Attempts to set an environment variable using setenv. Returns on success,
 * otherwise prints an error message and exits.
 */
void safe_setenv(const char *name, const char *value, int overwrite);

void setenv_omp_tool();

void setenv_omp_tool_libraries(const char *exec_path);

/* Attempts to set up the OMP_TOOL_VERBOSE_INIT environment variable for
 * ompdataperf. Returns on success, otherwise prints an error message and exits.
 */
void setenv_omp_tool_verbose_init(int verbose);
