#include <any>
#include <array>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <exception>
#include <forward_list>
#include <functional>
#include <future>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <cstdio>

template<class T>
void emit(const char* family, const char* name) {
  std::printf("%s\t%s\t%zu\t%zu\n", family, name, sizeof(T), alignof(T));
}

int main() {
  std::puts("family\ttype\tsize\talign");
  emit<std::array<int, 4>>("aggregate", "array<int,4>");
  emit<std::pair<int, int>>("aggregate", "pair<int,int>");
  emit<std::tuple<int, char, double>>("aggregate", "tuple<int,char,double>");
  emit<std::optional<int>>("vocabulary", "optional<int>");
  emit<std::variant<int, double>>("vocabulary", "variant<int,double>");
  emit<std::any>("vocabulary", "any");
  emit<std::function<void()>>("callable", "function<void()>");
  emit<std::unique_ptr<int>>("ownership", "unique_ptr<int>");
  emit<std::shared_ptr<int>>("ownership", "shared_ptr<int>");
  emit<std::weak_ptr<int>>("ownership", "weak_ptr<int>");
  emit<std::atomic<int>>("atomic", "atomic<int>");
  emit<std::atomic<long long>>("atomic", "atomic<long long>");
  emit<std::vector<int>>("sequence", "vector<int>");
  emit<std::deque<int>>("sequence", "deque<int>");
  emit<std::list<int>>("sequence", "list<int>");
  emit<std::forward_list<int>>("sequence", "forward_list<int>");
  emit<std::map<int, int>>("associative", "map<int,int>");
  emit<std::set<int>>("associative", "set<int>");
  emit<std::unordered_map<int, int>>("unordered", "unordered_map<int,int>");
  emit<std::unordered_set<int>>("unordered", "unordered_set<int>");
  emit<std::string>("text", "string");
  emit<std::string_view>("view", "string_view");
  emit<std::span<int>>("view", "span<int>");
  emit<std::mutex>("sync", "mutex");
  emit<std::recursive_mutex>("sync", "recursive_mutex");
  emit<std::condition_variable>("sync", "condition_variable");
  emit<std::thread>("thread", "thread");
  emit<std::future<int>>("future", "future<int>");
  emit<std::promise<int>>("future", "promise<int>");
  emit<std::logic_error>("d09", "logic_error");
  emit<std::runtime_error>("d09", "runtime_error");
  emit<std::nested_exception>("d09", "nested_exception");
  emit<std::exception_ptr>("d09", "exception_ptr");
  return 0;
}
