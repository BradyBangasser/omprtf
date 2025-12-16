#include <assert.h>
#include <clang/Driver/Driver.h>
#include <llvm/Support/Duration.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Support/Program.h>
#include <llvm/TargetParser/Triple.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fcntl.h>
#include <vector>

#include "analyze.hh"
#include "cmeta.h"
#include "getlineinfo.hpp"
#include "logging.h"
#include "pass.hpp"
#include "preload.hh"

int main(int argc, const char *argv[]) {
  assert(argc >= 2);

  std::shared_ptr<analyzer_results_t> profile_results =
      std::make_shared<analyzer_results_t>();

  pid_t pid;

  cmeta(argv[1]);

  llvm::ErrorOr<std::string> clang_p = llvm::sys::findProgramByName("clang");
  if (!clang_p) {
    ERROR("Failed to locate Clang\n");
    return 1;
  }

  char const *c_arg_arr[] = {clang_p->c_str(),
                             "-g",
                             "-lomp",
                             "-fopenmp",
                             "-fopenmp-targets=nvptx64-nvidia-cuda",
                             "-o",
                             "/tmp/out.out",
                             argv[1],
                             nullptr};

  pid = fork();
  if (pid == 0) {
    execv(clang_p->c_str(), (char **)c_arg_arr);
    perror("execv clang");
    _exit(127);
  }
  waitpid(pid, nullptr, 0);

  int pipefd[2];
  assert(pipe(pipefd) == 0);

  // Pass write FD to child via env
  char fd_buf[16];
  snprintf(fd_buf, sizeof(fd_buf), "%d", pipefd[1]);
  setenv("ANALYZER_PIPE_FD", fd_buf, 1);

  std::vector<char *> pargs;
  char *program = (char *)"/tmp/out.out";
  pargs.push_back(program);
  for (int i = 2; i < argc; ++i)
    pargs.push_back((char *)argv[i]);
  pargs.push_back(nullptr);

  pid = fork();

  if (pid == 0) {
    close(pipefd[0]); // child doesn't read

    setenv_omp_tool();
    setenv_omp_tool_libraries(argv[0]);
    setenv_omp_tool_verbose_init(0);

    execvp(program, pargs.data());
    perror("execvp target");
    _exit(127);
  }

  close(pipefd[1]); // parent doesn't write

  while (true) {
    uint32_t magic;
    ssize_t r = read(pipefd[0], &magic, sizeof(magic));

    if (r == 0)
      break; // EOF
    if (r != sizeof(magic) || magic != 0xA11A11A1)
      break;

    uint32_t type;
    uint64_t n_addrs;

    read(pipefd[0], &type, sizeof(type));
    read(pipefd[0], &n_addrs, sizeof(n_addrs));

    std::unique_ptr<analyzer_result_t> res =
        std::make_unique<analyzer_result_t>();
    res->result_type = static_cast<analyzer_result_type_e>(type);

    res->code.resize(n_addrs);
    read(pipefd[0], res->code.data(), n_addrs * sizeof(uint64_t));

    profile_results->push_back(std::move(res));
  }

  waitpid(pid, nullptr, 0);

  for (const auto &res : *profile_results) {
    INFO("RESULT\n");
    std::vector<std::unique_ptr<llvm::DILineInfo>> lines;
    for (uint64_t addr : res->code) {
      lines.push_back(getlineinfo(program, addr));
    }

    replaceLinesWithFiveFunctions(argv[1], lines);
  }

  SUCCESS("DONE\n");
  return 0;
}
