#include <type_traits>
#include <utility>

#if defined(MAY_THROW)
#  define R87_SPEC noexcept(false)
#else
#  define R87_SPEC noexcept
#endif

struct Site {
  void wait(int&) R87_SPEC {}
  ~Site() R87_SPEC {}
};

template <class T>
int type_template_use() { return sizeof(T); }

template <auto P>
int value_template_use() { return P != nullptr; }

template <bool B>
int bool_template_use() { return B; }

void parameter_use(void (Site::*)(int&) R87_SPEC) {}

int force_instantiations() {
  return type_template_use<decltype(&Site::wait)>()
       + value_template_use<&Site::wait>()
       + bool_template_use<noexcept(std::declval<Site&>().wait(std::declval<int&>()))>()
       + bool_template_use<std::is_nothrow_destructible<Site>::value>();
}
