#include <string>
#include <memory>
#include <cstdlib>
#if CASE == 1
#include <app_control.hpp>
extern "C" int native_probe(void* h) {
  tizen_appfw::AppControl c(static_cast<app_control_h>(h), false);
  const std::string expected="http://tizen.org/appcontrol/operation/view";
  const int rc=c.SetOperation(expected);
  if (rc) return rc;
  return c.GetOperation()==expected ? 0 : 91;
}
#elif CASE == 2
#include <app_common.hpp>
using A=tizen_appfw::app_common::AppBase<int,int>;
extern "C" int native_probe(void*) {
  A a(0,0);
  return a.GetId().empty() ? 92 : 0;
}
#elif CASE == 3
#include <service_app.hpp>
extern "C" int native_probe(void*) {
  tizen_appfw::ServiceAppBase app;
  char name[]="api0911"; char* argv[]={name,nullptr};
  return app.Run(1,argv);
}
#elif CASE == 4
#include <app.hpp>
extern "C" int native_probe(void*) {
  tizen_appfw::UiAppBase app;
  char name[]="api0911"; char* argv[]={name,nullptr};
  return app.Run(1,argv);
}
#endif
// 正向对照：本地 wrapper 编译实例确实会产生 C++ 标准库符号，
// 检索不能把全部 C++ 未定义符号误报为平台接口边。
extern "C" std::string cpp_symbol_positive(const std::string& x) {
  return x.substr(1);
}
