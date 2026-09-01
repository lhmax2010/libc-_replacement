// Positive calibration corpus for the unmodified R87 scanners.
// These declarations deliberately use the target ABI spellings without
// depending on the host standard library's current exception specifications.
namespace std {
class condition_variable {
public:
  void wait(int&) noexcept;
  void __do_timed_wait(long&) noexcept;
};

template <class Codecvt, class Elem>
class wbuffer_convert {
public:
  ~wbuffer_convert() noexcept;
};
} // namespace std

void std::condition_variable::wait(int&) noexcept {}
void std::condition_variable::__do_timed_wait(long&) noexcept {}

template <class Codecvt, class Elem>
std::wbuffer_convert<Codecvt, Elem>::~wbuffer_convert() noexcept = default;

template <int N, class T>
struct deep_layer {
  T value;
};

template <class T>
T&& calibration_declval() noexcept;

using wait_pointer = decltype(&std::condition_variable::wait);
using timed_pointer = decltype(&std::condition_variable::__do_timed_wait);
using explicit_wait_member_pointer =
    void (std::condition_variable::*)(int&) noexcept;

// Type-level use: the target method name is absent from this symbol, while the
// condition_variable member-pointer type and DoF encoding remain.
template <class Pointer, class Context>
__attribute__((noinline, used, visibility("default")))
int condition_type_only(Context, Pointer) {
  return sizeof(Pointer) + sizeof(Context);
}

template int condition_type_only<
    wait_pointer,
    deep_layer<7, deep_layer<8, deep_layer<9, int>>>>(
        deep_layer<7, deep_layer<8, deep_layer<9, int>>>, wait_pointer);

// Function-parameter use: a member function pointer carrying noexcept is part
// of the ordinary function signature.
__attribute__((noinline, used, visibility("default")))
int condition_pointer_parameter(explicit_wait_member_pointer pointer) {
  return pointer != nullptr;
}

// Combined real-address/type use: the address non-type argument contributes
// the real target method name, while Pointer contributes the noexcept type.
// The deep Context makes the symbol deliberately long and nested.
template <auto Address, class Pointer, class Context>
__attribute__((noinline, used, visibility("default")))
int deep_real_site(Context, Pointer) {
  return Address != nullptr;
}

template int deep_real_site<
    &std::condition_variable::wait,
    wait_pointer,
    deep_layer<11, deep_layer<12, deep_layer<13, deep_layer<14, int>>>>>(
        deep_layer<11, deep_layer<12, deep_layer<13, deep_layer<14, int>>>>,
        wait_pointer);

template int deep_real_site<
    &std::condition_variable::__do_timed_wait,
    timed_pointer,
    deep_layer<21, deep_layer<22, deep_layer<23, deep_layer<24, long>>>>>(
        deep_layer<21, deep_layer<22, deep_layer<23, deep_layer<24, long>>>>,
        timed_pointer);

using wbuffer_type = std::wbuffer_convert<char, wchar_t>;
using wbuffer_noexcept_function = void (*)(wbuffer_type&) noexcept;

// Dedicated type-only wbuffer branch: wbuffer_convert and DoF occur without a
// destructor or target member-function name.
template <class Owner, class Callable, class Context>
__attribute__((noinline, used, visibility("default")))
int wbuffer_type_only(Context, Callable) {
  return sizeof(Owner) + sizeof(Callable) + sizeof(Context);
}

template int wbuffer_type_only<
    wbuffer_type,
    wbuffer_noexcept_function,
    deep_layer<31, deep_layer<32, deep_layer<33, long>>>>(
        deep_layer<31, deep_layer<32, deep_layer<33, long>>>,
        wbuffer_noexcept_function);

// noexcept/trait result as a bool non-type template argument.  The Owner type
// keeps wbuffer_convert in the same symbol as Lb1E.
template <class Owner, bool IsNoexcept, class Context>
__attribute__((noinline, used, visibility("default")))
int wbuffer_destructor_bool(Context) {
  return IsNoexcept + sizeof(Owner) + sizeof(Context);
}

template int wbuffer_destructor_bool<
    wbuffer_type,
    noexcept(calibration_declval<wbuffer_type&>().~wbuffer_convert()),
    deep_layer<41, deep_layer<42, deep_layer<43, deep_layer<44, char>>>>>(
        deep_layer<41, deep_layer<42, deep_layer<43, deep_layer<44, char>>>>);

// A conventional entry point makes it possible to build an executable ELF in
// addition to the relocatable object and shared library.
int main() {
  return 0;
}
