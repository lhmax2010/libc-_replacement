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
  std::printf("%s\tTYPE=%s\tTOKEN=%s\t",id,actual<T>(),typeid(TypeToken<T>).name());
  if constexpr(std::is_void<T>::value||std::is_function<T>::value) std::printf("SIZE=NA\tALIGN=NA\n");
  else std::printf("SIZE=%zu\tALIGN=%zu\n",sizeof(T),alignof(T));
}
template<class C,class=void> struct Probe_T0528{static void run(){std::printf("T0528\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0528<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0528");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
emit<std::atomic_char>("T0481");
emit<std::atomic_schar>("T0482");
emit<std::atomic_uchar>("T0483");
emit<std::atomic_short>("T0484");
emit<std::atomic_ushort>("T0485");
emit<std::atomic_int>("T0486");
emit<std::atomic_uint>("T0487");
emit<std::atomic_long>("T0488");
emit<std::atomic_ulong>("T0489");
emit<std::atomic_llong>("T0490");
emit<std::atomic_ullong>("T0491");
emit<std::atomic_wchar_t>("T0492");
emit<std::atomic_char8_t>("T0493");
emit<std::atomic_char16_t>("T0494");
emit<std::atomic_char32_t>("T0495");
emit<std::atomic_int8_t>("T0496");
emit<std::atomic_uint8_t>("T0497");
emit<std::atomic_int16_t>("T0498");
emit<std::atomic_uint16_t>("T0499");
emit<std::atomic_int32_t>("T0500");
emit<std::atomic_uint32_t>("T0501");
emit<std::atomic_int64_t>("T0502");
emit<std::atomic_uint64_t>("T0503");
emit<std::atomic_int_least8_t>("T0504");
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
emit<std::atomic_uint_fast32_t>("T0517");
emit<std::atomic_int_fast64_t>("T0518");
emit<std::atomic_uint_fast64_t>("T0519");
emit<std::atomic_intptr_t>("T0520");
emit<std::atomic_uintptr_t>("T0521");
emit<std::atomic_size_t>("T0522");
emit<std::atomic_ptrdiff_t>("T0523");
emit<std::atomic_intmax_t>("T0524");
emit<std::atomic_uintmax_t>("T0525");
emit<std::atomic_signed_lock_free>("T0526");
emit<std::atomic_unsigned_lock_free>("T0527");
Probe_T0528<std::regex>::run();
}
