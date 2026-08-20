// R47 补充探针(仅 istream:1389):operator>>(istream&, CharT(&)[N]) 仅在 C++20 存在。
#include <iostream>

extern "C" void r47_array_extraction() {
  char arr[8];
  std::cin >> arr;   // istream:1389(C++20 数组安全重载,内部调 __input_c_string,即 :519 的另一载体)
}

int main() {
  r47_array_extraction();
  return 0;
}
