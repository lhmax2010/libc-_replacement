#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <cmath>
#include <type_traits>
#include <typeinfo>
#include <limits>
#include <ios>
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <iterator>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <memory>
#include <atomic>
#include <regex>
#include <exception>
#include <typeindex>
#include <initializer_list>
template<class T> struct TypeToken {};
template<class T> __attribute__((used,noinline)) void abi_identity(TypeToken<T>) {}
template<class T> const char* actual(){return __PRETTY_FUNCTION__;}
template<class T> void emit(const char* id){
  abi_identity(TypeToken<T>{});
  std::printf("%s\tTYPE=%s\tTOKEN=%s\tDIRECT=%s\t",id,actual<T>(),typeid(TypeToken<T>).name(),typeid(T).name());
  if constexpr(std::is_void<T>::value||std::is_function<T>::value) std::printf("SIZE=NA\tALIGN=NA\n");
  else std::printf("SIZE=%zu\tALIGN=%zu\n",sizeof(T),alignof(T));
}

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
emit<std::atomic_uint_least8_t>("T0505");
emit<std::atomic_int_least16_t>("T0506");
emit<std::atomic_uint_least16_t>("T0507");
emit<std::atomic_int_least32_t>("T0508");
emit<std::atomic_uint_least32_t>("T0509");
emit<std::atomic_int_least64_t>("T0510");
emit<std::atomic_uint_least64_t>("T0511");
emit<std::atomic_int_fast8_t>("T0512");
emit<std::atomic_uint_fast8_t>("T0513");
emit<std::atomic_int_fast16_t>("T0514");
emit<std::atomic_uint_fast16_t>("T0515");
emit<std::atomic_int_fast32_t>("T0516");
}
