#include <codecvt>
#include <locale>

using control_converter = std::wbuffer_convert<std::codecvt_utf8<wchar_t>>;

control_converter* make_control() {
  return nullptr;
}
