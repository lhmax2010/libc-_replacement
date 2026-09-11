#include <boost/program_options/detail/convert.hpp>
#include <stdexcept>
#include "bridge.h"
namespace {
int receive(void* p, const char* data, size_t n) {
  try { static_cast<std::string*>(p)->assign(data,n); return 0; }
  catch (...) { return 41; }
}
}
namespace boost { namespace program_options {
// 与应用原始 API 相同的声明；本文件只看调用方那套 std 类型。
__attribute__((visibility("default")))
std::string to_internal(const std::wstring& input) {
  std::string output;
  if(api0911_convert(input.data(),input.size(),receive,&output))
    throw std::runtime_error("api0911 bridge conversion failed");
  return output;
}
}}
