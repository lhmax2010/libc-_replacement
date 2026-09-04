#include <codecvt>
#include <locale>
#include <sstream>

using wide_adapter = std::wbuffer_convert<std::codecvt_utf8<wchar_t>>;

extern "C" __attribute__((visibility("default"))) int use_wbuffer_convert() {
  std::stringbuf bytes;
  wide_adapter converted(&bytes);
  converted.sputn(L"positive", 8);
  return converted.pubsync();
}
