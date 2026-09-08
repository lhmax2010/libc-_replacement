#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <locale>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <cstdio>
#include <utility>

static_assert(!noexcept(std::declval<std::condition_variable&>().wait(
                          std::declval<std::unique_lock<std::mutex>&>())), "wait must permit unwinding");
int main() {
  std::shared_mutex ordinary;
  ordinary.lock(); ordinary.unlock(); ordinary.lock_shared(); ordinary.unlock_shared();
  std::shared_timed_mutex timed;
  if (!timed.try_lock_for(std::chrono::seconds(1))) return 1;
  timed.unlock();
  std::stringbuf explicit_output;
  {
    std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converted(&explicit_output);
    std::wostream output(&converted); output << L"abcdef";
    if (converted.pubsync() != 0 || explicit_output.str() != "abcdef") return 2;
  }
  if (explicit_output.str() != "abcdef") return 3;
  std::stringbuf pending_output;
  {
    std::wbuffer_convert<std::codecvt_utf8<wchar_t>> converted(&pending_output);
    std::wostream output(&converted); output << L"abcdef";
  }
  printf("explicit_bytes=%zu pending_bytes=%zu\n",explicit_output.str().size(),pending_output.str().size());
  if (!pending_output.str().empty()) return 4;
  puts("HEADER_VALUE_ASSERTIONS=PASS");
}
