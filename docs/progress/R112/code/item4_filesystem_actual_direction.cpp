#include <boost/filesystem.hpp>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PROVIDER_TOKEN
#define PROVIDER_TOKEN "libboost_filesystem.so.1.83.0"
#endif

static bool maps_contains(const char* token) {
  std::FILE* maps = std::fopen("/proc/self/maps", "r");
  if (!maps)
    return false;
  char line[4096];
  while (std::fgets(line, sizeof(line), maps)) {
    if (std::strstr(line, token)) {
      std::fclose(maps);
      return true;
    }
  }
  std::fclose(maps);
  return false;
}

static void fail(const char* step, const char* detail) {
  std::fprintf(stderr, "FAIL step=%s detail=%s errno=%d\n", step, detail, errno);
  std::_Exit(40);
}

int main() {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::printf("runtime_maps=libcxx:%d,libstdcxx:%d,provider:%d\n",
              maps_contains("libc++.so.1") ? 1 : 0,
              maps_contains("libstdc++.so.6") ? 1 : 0,
              maps_contains(PROVIDER_TOKEN) ? 1 : 0);
  char root_template[] = "/tmp/r112_fs_XXXXXX";
  char* root = ::mkdtemp(root_template);
  if (!root)
    fail("mkdtemp", "null");

  std::string target = std::string(root) + "/payload.txt";
  std::string link = std::string(root) + "/payload.link";
  int fd = ::open(target.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);
  if (fd < 0)
    fail("open", target.c_str());
  const char payload[] = "R112-ACTUAL-DIRECTION";
  if (::write(fd, payload, sizeof(payload) - 1) != static_cast<ssize_t>(sizeof(payload) - 1))
    fail("write", target.c_str());
  if (::close(fd) != 0)
    fail("close", target.c_str());
  if (::symlink("payload.txt", link.c_str()) != 0)
    fail("symlink", link.c_str());

  int result = 0;
  {
    boost::system::error_code ec;
    boost::filesystem::path target_path(target.c_str());
    boost::filesystem::file_status status = boost::filesystem::status(target_path, ec);
    bool status_ok = !ec && boost::filesystem::is_regular_file(status);

    boost::filesystem::path link_path(link.c_str());
    boost::filesystem::path returned = boost::filesystem::read_symlink(link_path, ec);
    std::string returned_text = returned.native();
    bool value_ok = !ec && returned_text == "payload.txt";
    std::printf("sizeof_path=%zu status_ok=%d value_ok=%d returned_size=%zu returned=%s\n",
                sizeof(boost::filesystem::path),
                status_ok ? 1 : 0,
                value_ok ? 1 : 0,
                returned_text.size(),
                returned_text.c_str());
    if (!status_ok || !value_ok)
      result = 41;
  }
  std::puts("event=destruction_completed");
  ::unlink(link.c_str());
  ::unlink(target.c_str());
  ::rmdir(root);
  std::printf("event=result code=%d\n", result);
  return result;
}
