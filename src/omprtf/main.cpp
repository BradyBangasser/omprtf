#include <assert.h>
#include <clang/Driver/Driver.h>
#include <llvm/Support/Duration.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Support/Program.h>
#include <llvm/TargetParser/Triple.h>
#include <sys/wait.h>
#include <unistd.h>

#include "analyze.hh"
#include "cmeta.h"
#include "getlineinfo.h"
#include "logging.h"
#include "preload.hh"

int main(int argc, const char *argv[]) {
  assert(argc == 2);
  std::shared_ptr<analyzer_results_t> profile_results =
      std::make_shared<analyzer_results_t>();
  pid_t pid;

  // Augment metadata
  cmeta(argv[1]);

  llvm::ErrorOr<std::string> clang_p = llvm::sys::findProgramByName("clang");

  if (!clang_p) {
    ERROR("Failed to locate Clang\n");
    return 1;
  }

  char const *c_arg_arr[] = {clang_p->data(),
                             "-g",
                             "-lomp",
                             "-fopenmp",
                             "-fopenmp-targets=nvptx64-nvidia-cuda",
                             "-o",
                             "/tmp/out.out",
                             argv[1],
                             NULL};

  pid = fork();

  if (pid == 0) {
    DEBUGF("%s\n", clang_p->data());
    execv(clang_p->data(), (char **)c_arg_arr) || (SUCCESS("Worked\n"));
    ERROR("ERROR\n");
    exit(127);
  } else {
    waitpid(pid, 0, 0);
  }

  std::vector<char *> pargs;
  char *program = (char *)"/tmp/out.out";
  pargs.push_back(program);
  for (int i = 2; i < argc; ++i) {
    pargs.push_back((char *)argv[i]);
  }
  pargs.push_back(nullptr);

  setenv_omp_tool();
  setenv_omp_tool_libraries(argv[0]);
  setenv_omp_tool_verbose_init(0);

  set_analyzer_vector(profile_results);

  pid = fork();

  if (pid == 0) {
    DEBUGF("%s\n", clang_p->data());
    execvp(program, pargs.data());
    ERROR("ERROR\n");
    exit(127);
  } else {
    waitpid(pid, 0, 0);
  }

  for (const auto &res : *profile_results) {
    for (uint64_t addr : res->code) {
      getlineinfo(argv[1], addr);
    }
  }

  return 0;
}
