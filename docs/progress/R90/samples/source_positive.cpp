// Source-only positive calibration input for scan_noexcept_type_uses.py.
#include <condition_variable>
#include <codecvt>
#include <locale>
#include <type_traits>
#include <utility>

using cv_alias = std::condition_variable;
using wait_lock = std::unique_lock<std::mutex>;
using explicit_member_pointer =
    void (std::condition_variable::*)(wait_lock&);
using explicit_address_type = decltype(
    static_cast<explicit_member_pointer>(&std::condition_variable::wait));
using explicit_trait = std::is_invocable<explicit_address_type,
                                          std::condition_variable&, wait_lock&>;

auto direct_address =
    static_cast<explicit_member_pointer>(&std::condition_variable::wait);
auto alias_address = static_cast<explicit_member_pointer>(&cv_alias::wait);
auto broad_address = static_cast<explicit_member_pointer>(&cv_alias::wait);

void private_helpers() {
  // Calibration token for D02-D04's private helper reference branch.
  // It is intentionally source-only because the real helper is private.
  (void)"__do_timed_wait";
}

template <class Facet, class Elem>
constexpr bool destructor_query() {
  using target = std::wbuffer_convert<Facet, Elem>;
  return noexcept(std::declval<target&>().~wbuffer_convert());
}

template <class CV, class Lock>
constexpr bool realistic_nested_query(CV& cv, Lock& lock) {
  return noexcept(cv.wait(lock));
}

static_assert(std::is_nothrow_destructible<
              std::wbuffer_convert<std::codecvt_utf8<wchar_t>, wchar_t>>::value ||
              !destructor_query<std::codecvt_utf8<wchar_t>, wchar_t>() || true);
