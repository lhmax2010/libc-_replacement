#include <absl/strings/str_cat.h>

#include <iostream>
#include <string>

int main() {
  const std::string value = absl::StrCat("absl-", 104);
  if (value != "absl-104")
    return 2;
  std::cout << value << '\n';
  return 0;
}
