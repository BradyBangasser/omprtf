#include <cassert>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

void safe_setenv(const char *name, const char *value, int overwrite) {
  assert(name != nullptr);
  assert(value != nullptr);
  if (setenv(name, value, overwrite)) {
    std::cerr << "error: failed to set environment variable " << name << ". "
              << strerror(errno) << "\n";
    exit(EXIT_FAILURE);
  }
  return;
}

/* Attempts to set up the OMP_TOOL environment variable for ompdataperf. Returns
 * on success, otherwise prints an error message and exits.
 */
void setenv_omp_tool() {
  const char *env_omp_tool = getenv("OMP_TOOL");
  if (env_omp_tool != nullptr && strcmp(env_omp_tool, "enabled") != 0) {
    std::cerr << "warning: OMP_TOOL is defined but is not set to \'enabled\'. "
                 "Ignoring set value.\n";
  }
  // Ensure OpenMP runtime will try to register tools
  safe_setenv("OMP_TOOL", "enabled", 1 /*overwrite*/);
  return;
}

/* Attempts to set up the OMP_TOOL_LIBRARIES environment variable for
 * ompdataperf. Assumes that libompdataperf.so is in the same directory as the
 * current running executable. Returns on success, otherwise prints an error
 * message and exits.
 */
void setenv_omp_tool_libraries(const char *exec_path) {
  namespace fs = std::filesystem;
  const char *lib_name = "libprofiler.so";
  fs::path lib_path;
  try {
    fs::path exec_full_path = fs::canonical(exec_path);
    lib_path = exec_full_path.parent_path().append(lib_name);
  } catch (const std::exception &ex) {
    std::cerr << "error: failed to resolve canonical path for " << lib_name
              << ". " << ex.what() << "\n";
    exit(EXIT_FAILURE);
  }

  const char *env_omp_tool_libraries = getenv("OMP_TOOL_LIBRARIES");
  std::string new_env_omp_tool_libraries;
  if (env_omp_tool_libraries == nullptr) {
    new_env_omp_tool_libraries = lib_path.string();
  } else {
    new_env_omp_tool_libraries =
        std::string(env_omp_tool_libraries) + ":" + lib_path.string();
  }
  safe_setenv("OMP_TOOL_LIBRARIES", new_env_omp_tool_libraries.c_str(),
              1 /*overwrite*/);
  return;
}

/* Attempts to set up the OMP_TOOL_VERBOSE_INIT environment variable for
 * ompdataperf. Returns on success, otherwise prints an error message and exits.
 */
void setenv_omp_tool_verbose_init(int verbose) {
  // If OMP_TOOL_VERBOSE_INIT is already set, don't overwrite it.
  if (verbose) {
    safe_setenv("OMP_TOOL_VERBOSE_INIT", "stderr", 0 /*overwrite*/);
  } else {
    safe_setenv("OMP_TOOL_VERBOSE_INIT", "disabled", 0 /*overwrite*/);
  }
  return;
}
