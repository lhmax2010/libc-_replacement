#include <clang-c/Index.h>
#include <cstdio>
#include <cstring>
int main() {
  CXString version = clang_getClangVersion();
  std::printf("version=%s\n", clang_getCString(version));
  clang_disposeString(version);
  CXIndex index = clang_createIndex(0, 0);
  const char *source = "int answer(void) { return 42; }\n";
  CXUnsavedFile file = {"probe.c", source, std::strlen(source)};
  CXTranslationUnit unit = nullptr;
  CXErrorCode result = clang_parseTranslationUnit2(index, "probe.c", nullptr, 0,
      &file, 1, CXTranslationUnit_None, &unit);
  unsigned diagnostics = unit ? clang_getNumDiagnostics(unit) : 999;
  std::printf("parse_result=%d diagnostics=%u\n", int(result), diagnostics);
  if (unit) clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
  return result == CXError_Success && diagnostics == 0 ? 0 : 1;
}
