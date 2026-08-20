// R47 分类探针:令消费者 TU 实际 odr-use 34 处站点的所在函数,
// 供 nm 检查消费者目标文件中各符号为 UND / 本地定义 / 缺失。
// 本探针只需编译,不需要运行;各 exercise 为外部链接函数,防止整体被删。
// 无任何 catch。
#include <future>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

// ---- istream 成员(15 站点)----
extern "C" void r47_istream_members() {
  char buf[64];
  std::stringbuf sb;
  std::cin >> &sb;                                  // istream:644 operator>>(basic_streambuf*)
  (void)std::cin.get();                             // istream:679 get()
  std::cin.get(buf, 8, 'x');                        // istream:717 get(char*,n,delim)
  std::cin.get(sb, 'x');                            // istream:765 get(streambuf&,delim)
  std::cin.getline(buf, 8, 'x');                    // istream:808 getline(char*,n,delim)
  std::cin.ignore(3, 'x');                          // istream:862 ignore(n,delim)
  (void)std::cin.peek();                            // istream:889 peek()
  std::cin.read(buf, 4);                            // istream:915 read(char*,n)
  (void)std::cin.readsome(buf, 4);                  // istream:954 readsome(char*,n)
  std::cin.putback('a');                            // istream:982 putback(char)
  std::cin.unget();                                 // istream:1010 unget()
  (void)std::cin.sync();                            // istream:1042 sync()
  (void)std::cin.tellg();                           // istream:1066 tellg()
  std::cin.seekg(std::streampos(0));                // istream:1091 seekg(pos_type)
  std::cin.seekg(0, std::ios_base::beg);            // istream:1116 seekg(off,dir)
}

// ---- istream 自由函数模板(7 站点;:1389 需 C++20,另见补充 TU)----
extern "C" void r47_istream_free() {
  char c;
  char arr[8];
  double d;
  int i;
  std::string s;
  std::cin >> d;      // istream:377 __input_arithmetic<double> 经成员 operator>>(double&)
  std::cin >> i;      // istream:468 __input_arithmetic_with_numeric_limits<int> 经成员 operator>>(int&)
  std::cin >> arr;    // istream:519 __input_c_string 经自由 operator>>(istream&, CharT*)(C++17)
  std::cin >> c;      // istream:594 自由 operator>>(istream&, char&)
  std::cin >> std::ws;                              // istream:1149 ws()
  std::cin >> s;                                    // istream:1255 operator>>(istream&, string&)
  std::getline(std::cin, s, ':');                   // istream:1328 getline(istream&, string&, delim)
}

// ---- future(9 站点)----
static int r47_ret1() { return 1; }
static void r47_retv() {}

extern "C" void r47_future_sites() {
  auto fd_int = std::async(std::launch::deferred, r47_ret1);   // future:1865 async; get 触发 future:792
  (void)fd_int.get();
  auto fd_void = std::async(std::launch::deferred, r47_retv);  // future:823
  fd_void.get();
  auto fa_int = std::async(std::launch::async, r47_ret1);      // future:853
  (void)fa_int.get();
  auto fa_void = std::async(std::launch::async, r47_retv);     // future:890
  fa_void.get();

  std::packaged_task<int()> pt_int(r47_ret1);
  pt_int();                                                    // future:1664
  std::packaged_task<int()> pt_int2(r47_ret1);
  pt_int2.make_ready_at_thread_exit();                         // future:1681
  std::packaged_task<void()> pt_void(r47_retv);
  pt_void();                                                   // future:1764
  std::packaged_task<void()> pt_void2(r47_retv);
  pt_void2.make_ready_at_thread_exit();                        // future:1782
}

// ---- ostream sentry 析构(1 站点)与 string shrink_to_fit(1 站点)----
extern "C" void r47_ostream_string_sites() {
  std::cout << "x";                                 // basic_ostream.h:217 sentry::~sentry()
  std::string s(1000, 'y');
  s.resize(10);
  s.shrink_to_fit();                                // string:3466 shrink_to_fit()
}

int main() {
  r47_istream_members();
  r47_istream_free();
  r47_future_sites();
  r47_ostream_string_sites();
  return 0;
}
