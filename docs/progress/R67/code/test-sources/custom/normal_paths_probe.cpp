#include <cstdio>
#include <cstring>
#include <future>
#include <sstream>
#include <stdexcept>

namespace {
int packaged_normal() {
  std::packaged_task<int()> task([] { return 24; });
  std::future<int> future = task.get_future();
  task();
  const int value = future.get();
  std::fprintf(stderr, "MODE=packaged_normal VALUE=%d\n", value);
  return value == 24 ? 0 : 2;
}

int packaged_exception() {
  std::packaged_task<int()> task([]() -> int { throw std::runtime_error("packaged-real-error"); });
  std::future<int> future = task.get_future();
  task();
  try {
    (void)future.get();
    std::fprintf(stderr, "MODE=packaged_exception GET=value\n");
    return 3;
  } catch (const std::runtime_error& error) {
    std::fprintf(stderr, "MODE=packaged_exception GET=runtime_error MESSAGE=%s\n", error.what());
    return std::strcmp(error.what(), "packaged-real-error") == 0 ? 0 : 4;
  } catch (...) {
    std::fprintf(stderr, "MODE=packaged_exception GET=other_exception\n");
    return 5;
  }
}

int arithmetic_format_error() {
  std::istringstream input("not-a-number");
  int value = 17;
  input >> value;
  std::fprintf(stderr, "MODE=arithmetic_format_error FAIL=%d BAD=%d EOF=%d VALUE=%d STATE=%d\n",
               input.fail(), input.bad(), input.eof(), value, input.rdstate());
  return input.fail() && !input.bad() ? 0 : 6;
}
} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "packaged_normal") == 0)
    return packaged_normal();
  if (std::strcmp(argv[1], "packaged_exception") == 0)
    return packaged_exception();
  if (std::strcmp(argv[1], "arithmetic_format_error") == 0)
    return arithmetic_format_error();
  return 65;
}

