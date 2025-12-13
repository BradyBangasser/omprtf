#include "preload.hh"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <unistd.h>
#include <vector>

void print_help() {
  std::cout << "Usage: ompdataperf [options] [program] [program arguments]\n";
  std::cout << "Options:\n";
  std::cout << "  -h, --help              Show this help message\n";
  std::cout << "  -q, --quiet             Suppress warnings\n";
  std::cout << "  -v, --verbose           Enable verbose output\n";
  std::cout << "  --version               Print the version of ompdataperf\n";
}

void print_version() {
  std::cout << "ompdataperf version " << OMPDATAPERF_VERSION << "\n";
}

void print_env(const char *name) {
  assert(name != nullptr);
  const char *value = getenv(name);
  if (value == nullptr) {
    std::cerr << "info: " << name << " not set" << "\n";
    return;
  }
  std::cerr << "info: " << name << "=" << value << "\n";
  return;
}

int main(int argc, char *argv[]) {
  // default values for options
  int verbose = false;
  // std::string outfile;

  // clang-format off
  static struct option long_options[] = {
    {"help",    no_argument,       nullptr, 'h'},
    {"verbose", no_argument,       nullptr, 'v'},
    {"version", no_argument,       nullptr,  0 },
 // {"outfile", required_argument, nullptr, 'o'},
    {nullptr,   0,                 nullptr,  0 }
  };
  // clang-format on

  int option_index = 0;
  int c;
  // parse options
  while ((c = getopt_long(argc, argv, "++hv", long_options, &option_index)) !=
         -1) {
    switch (c) {
    case 'h':
      print_help();
      return 0;
    case 'v':
      verbose = true;
      break;
    // example of output file which takes a path parameter
    // case 'o': // add o: to optstring
    //   output_file = optarg;
    //   break;
    case 0: // handle long options with no short equivalent
      if (strcmp(long_options[option_index].name, "version") == 0) {
        print_version();
        return 0;
      }
      break;
    case '?':
      // getopt_long already printed an error message.
      return 1;
    default:
      return 1;
    }
  }

  // remaining arguments after options should be the user's program and its args
  if (optind == argc) {
    std::cerr << "error: no program specified to profile\n";
    return 1;
  }

  std::vector<char *> program_args;
  // the first argument is the program to be executed
  char *program = argv[optind];
  program_args.push_back(program);
  // collect the remaining arguments (if any) as the program's arguments
  for (int i = optind + 1; i < argc; ++i) {
    program_args.push_back(argv[i]);
  }
  // end the argument list with a nullptr for execvp
  program_args.push_back(nullptr);

  setenv_omp_tool();
  setenv_omp_tool_libraries(argv[0]);
  setenv_omp_tool_verbose_init(verbose);

  if (verbose) {
    print_env("OMP_TOOL");
    print_env("OMP_TOOL_LIBRARIES");
    print_env("OMP_TOOL_VERBOSE_INIT");

    // print command being profiled
    std::cout << "info: profiling \'" << argv[optind];
    for (int i = optind + 1; i < argc; ++i) {
      std::cout << " " << argv[i];
    }
    std::cout << "\'\n";
  }

  // on success, execvp() does not return
  execvp(program, program_args.data());

  // if execvp returns, an error occurred and errno is set
  std::cerr << "error: failed to execute program. " << strerror(errno) << "\n";
  return 1;
}
