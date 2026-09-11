#include <boost/program_options/detail/convert.hpp>
#include <cstdio>
#include <dlfcn.h>
#include "bridge.h"
static int completed=0;
extern "C" __attribute__((visibility("default"))) int api0911_completed() { return completed; }
extern "C" __attribute__((visibility("hidden")))
int api0911_convert(const wchar_t* p,size_t n,api0911_sink sink,void* context) {
  try {
    int result;
    {
      std::wstring input(p,n);
      auto fn=static_cast<std::string(*)(const std::wstring&)>(&boost::program_options::to_internal);
      Dl_info info{};
      if(!dladdr(reinterpret_cast<void*>(fn),&info)) return 42;
      std::printf("REAL_CALLEE=%s\n",info.dli_fname);
      // 真正调用未改动的平台 Boost provider；不自行实现其转换算法。
      std::string output=fn(input);
      result=sink(context,output.data(),output.size());
    }
    ++completed; // provider 侧局部 string/wstring 已离开作用域。
    return result;
  } catch (...) { return 43; } // 普通异常留在 provider 一侧；取消不在本探针范围。
}
