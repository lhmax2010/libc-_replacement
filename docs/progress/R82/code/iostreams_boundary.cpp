#include <boost/iostreams/device/mapped_file.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <unistd.h>

static void fail(const char* step) {
  std::fprintf(stderr, "FAIL step=%s\n", step);
  std::_Exit(50);
}

int main() {
  char path[] = "/tmp/r82_io_XXXXXX";
  int fd = ::mkstemp(path);
  if (fd < 0) fail("mkstemp");
  const char payload[] = "R82-BOOST-IOSTREAMS";
  const std::size_t expected = sizeof(payload) - 1;
  if (::write(fd, payload, expected) != static_cast<ssize_t>(expected)) fail("write");
  if (::close(fd) != 0) fail("close");

  int result = 0;
  {
    boost::iostreams::mapped_file_params params;
    params.path = path;
    boost::iostreams::mapped_file_source source;
    source.open(params);
    bool open_ok = source.is_open();
    bool size_ok = source.size() == expected;
    bool value_ok = open_ok && size_ok && std::memcmp(source.data(), payload, expected) == 0;
    std::printf("sizeof_params=%zu open_ok=%d size_ok=%d value_ok=%d observed_size=%zu\n",
                sizeof(params), open_ok ? 1 : 0, size_ok ? 1 : 0,
                value_ok ? 1 : 0, open_ok ? source.size() : 0);
    if (!value_ok) result = 51;
  }

  ::unlink(path);
  std::printf("destruction_completed=1 result=%d\n", result);
  return result;
}
