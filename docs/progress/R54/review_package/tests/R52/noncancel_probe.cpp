#include <cstdio>
#include <cstring>
#include <future>
#include <sstream>
#include <stdexcept>
#include <string>

static int getline_success() {
  std::istringstream input("alpha beta\n");
  std::string line;
  const bool ok = static_cast<bool>(std::getline(input, line));
  std::fprintf(stderr, "MODE=getline_success OK=%d VALUE=%s STATE=%d\n", ok, line.c_str(), input.rdstate());
  return ok && line == "alpha beta" ? 0 : 2;
}

static int getline_failure() {
  std::istringstream input("");
  std::string line("sentinel");
  const bool ok = static_cast<bool>(std::getline(input, line));
  std::fprintf(stderr, "MODE=getline_failure OK=%d EOF=%d FAIL=%d VALUE_SIZE=%zu\n", ok, input.eof(), input.fail(), line.size());
  return !ok && input.eof() && input.fail() ? 0 : 3;
}

static int packaged_exception() {
  std::packaged_task<int()> task([]() -> int { throw std::runtime_error("expected"); });
  std::future<int> future = task.get_future();
  task();
  try {
    (void)future.get();
  } catch (const std::runtime_error& exception) {
    const bool match = std::strcmp(exception.what(), "expected") == 0;
    std::fprintf(stderr, "MODE=packaged_exception CAUGHT_RUNTIME_ERROR=1 MESSAGE=%s\n", exception.what());
    return match ? 0 : 4;
  }
  std::fprintf(stderr, "MODE=packaged_exception CAUGHT_RUNTIME_ERROR=0\n");
  return 5;
}

static int string_normal() {
  std::string value(1024, 'x');
  value.reserve(65536);
  value.shrink_to_fit();
  const bool ok = value.size() == 1024 && value.front() == 'x' && value.back() == 'x';
  std::fprintf(stderr, "MODE=string_normal OK=%d SIZE=%zu CAPACITY=%zu\n", ok, value.size(), value.capacity());
  return ok ? 0 : 6;
}

extern "C" __attribute__((visibility("default"))) int r52_noncancel_run(const char* mode) {
  if (std::strcmp(mode, "getline_success") == 0) return getline_success();
  if (std::strcmp(mode, "getline_failure") == 0) return getline_failure();
  if (std::strcmp(mode, "packaged_exception") == 0) return packaged_exception();
  if (std::strcmp(mode, "string_normal") == 0) return string_normal();
  return 64;
}

int main(int argc, char** argv) {
  if (argc != 2) return 64;
  return r52_noncancel_run(argv[1]);
}
