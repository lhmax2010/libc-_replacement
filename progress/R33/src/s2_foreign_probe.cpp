#include <unwind.h>

#include <cxxabi.h>
#include <exception>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <typeinfo>

namespace {

struct ForeignException {
  _Unwind_Exception unwind;
};

void cleanup(_Unwind_Reason_Code reason, _Unwind_Exception* exception) {
  std::fprintf(stderr, "foreign:cleanup:reason=%d\n", static_cast<int>(reason));
  std::free(exception);
}

int run_foreign() {
  auto* foreign = static_cast<ForeignException*>(std::calloc(1, sizeof(ForeignException)));
  if (!foreign)
    return 20;
  std::memcpy(&foreign->unwind.exception_class, "R33XTEST", 8);
  foreign->unwind.exception_cleanup = cleanup;
  try {
    std::fprintf(stderr, "foreign:raise\n");
    const _Unwind_Reason_Code rc = _Unwind_RaiseException(&foreign->unwind);
    std::fprintf(stderr, "foreign:raise-returned=%d\n", static_cast<int>(rc));
    _Unwind_DeleteException(&foreign->unwind);
    return 21;
  } catch (...) {
    const std::type_info* type = __cxxabiv1::__cxa_current_exception_type();
    const std::exception_ptr current = std::current_exception();
    std::fprintf(stderr, "foreign:caught:type=%s:exception_ptr=%s\n",
                 type ? type->name() : "NULL", current ? "NONEMPTY" : "EMPTY");
    return 0;
  }
}

int run_native() {
  try {
    throw std::runtime_error("r33-native");
  } catch (...) {
    const std::type_info* type = __cxxabiv1::__cxa_current_exception_type();
    const std::exception_ptr current = std::current_exception();
    std::fprintf(stderr, "native:caught:type=%s:exception_ptr=%s\n",
                 type ? type->name() : "NULL", current ? "NONEMPTY" : "EMPTY");
    return type && current ? 0 : 30;
  }
}

} // namespace

int main(int argc, char** argv) {
  if (argc != 2)
    return 64;
  if (std::strcmp(argv[1], "foreign") == 0)
    return run_foreign();
  if (std::strcmp(argv[1], "native") == 0)
    return run_native();
  return 65;
}
