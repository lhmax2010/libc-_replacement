#include <codecvt>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

// P1 文件流子段；额外参数只验证运行库身份，不改变转换调用。
int main(int argc, char** argv) {
  if (argc != 5) return 2;
  const std::string mode = argv[2];
  if (mode != "destructor" && mode != "explicit") return 2;
  bool cxx = false, abi = false, gnu = false, wrong = false;
  FILE* maps = std::fopen("/proc/self/maps", "r");
  if (!maps) return 90;
  char line[4096];
  while (std::fgets(line, sizeof(line), maps)) {
    bool c = std::strstr(line, "libc++.so");
    bool a = std::strstr(line, "libc++abi.so");
    bool g = std::strstr(line, "libstdc++.so");
    if (c || a || g) {
      std::fputs(line, stdout);
      cxx |= c; abi |= a; gnu |= g;
      wrong |= !std::strstr(line, argv[3]);
    }
  }
  std::fclose(maps);
  bool identity = !wrong && (std::strcmp(argv[4], "gnu") == 0
       ? gnu && !cxx && !abi : cxx && abi && !gnu);
  std::printf("RUNTIME_IDENTITY=%s cxx=%d abi=%d gnu=%d wrong=%d\n",
              identity ? "PASS" : "FAIL", cxx, abi, gnu, wrong);
  if (!identity) return 91;
  using Conv = std::wbuffer_convert<std::codecvt_utf8<wchar_t>>;
  using Traits = std::char_traits<wchar_t>;
  {
    std::ofstream bytes(argv[1], std::ios::binary);
    if (!bytes) return 3;
    {
      Conv converter(bytes.rdbuf());
      for (wchar_t c : {wchar_t(0x4e51), wchar_t(0x4e52), wchar_t(0x4e53)}) {
        auto value = converter.sputc(c);
        if (!Traits::eq_int_type(value, Traits::to_int_type(c))) return 4;
      }
      if (mode == "explicit" && converter.pubsync() != 0) return 5;
    }
    bytes.close();
    if (!bytes) return 6;
  }
  std::ifstream input(argv[1], std::ios::binary);
  if (!input) return 7;
  const std::string got((std::istreambuf_iterator<char>(input)),
                        std::istreambuf_iterator<char>());
  if (input.bad()) return 8;
  const std::string expected("\xe4\xb9\x91\xe4\xb9\x92\xe4\xb9\x93", 9);
  std::cout << "mode=" << mode << " bytes=" << got.size()
            << " exact_utf8=" << (got == expected) << " hex=";
  for (unsigned char c : got) std::printf("%02x", c);
  std::cout << '\n';
  return got == expected ? 0 : 1;
}
