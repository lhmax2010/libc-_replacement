#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <locale>
#include <ostream>
#include <sstream>
#include <string>

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  if (argc != 3)
    return 64;
  size_t length = static_cast<size_t>(std::strtoull(argv[1], nullptr, 10));
  std::string action = argv[2];
  std::stringbuf sink;
  int action_rc = -999;
  {
    std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converted(&sink);
    {
      std::wostream output(&converted);
      output << std::wstring(length, L'x');
    }
    if (action == "pubsync")
      action_rc = converted.pubsync();
    else if (action == "seekoff")
      action_rc = converted.pubseekoff(0, std::ios_base::cur, std::ios_base::out) ==
                          std::wstreampos(std::streamoff(-1))
                      ? -1
                      : 0;
    else if (action != "destructor")
      return 64;
  }
  std::string bytes = sink.str();
  bool all_x = true;
  for (char c : bytes)
    all_x = all_x && c == 'x';
  std::printf("event=result input_wchars=%zu output_bytes=%zu all_x=%d action=%s action_rc=%d\n",
              length,
              bytes.size(),
              all_x ? 1 : 0,
              action.c_str(),
              action_rc);
  return all_x ? 0 : 66;
}
