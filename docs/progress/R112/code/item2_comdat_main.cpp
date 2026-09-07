#include <cstdio>
#include <cstring>
#include <exception>
#include <unistd.h>

extern "C" int old_entry();
extern "C" int new_entry();

namespace {
void terminate_marker() {
  constexpr char marker[] = "event=terminate\n";
  (void)!write(STDOUT_FILENO, marker, sizeof(marker) - 1);
  _Exit(86);
}
}

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::set_terminate(terminate_marker);
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "old") == 0)
    return old_entry();
  if (std::strcmp(argv[1], "new") == 0)
    return new_entry();
  return 64;
}
