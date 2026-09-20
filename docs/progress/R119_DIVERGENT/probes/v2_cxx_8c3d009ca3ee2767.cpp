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
template<class C,class=void> struct Probe_T0580{static void run(){std::printf("T0580\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0580<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0580");}};

template<class C,class=void> struct Probe_T0581{static void run(){std::printf("T0581\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0581<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0581");}};

template<class C,class=void> struct Probe_T0582{static void run(){std::printf("T0582\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0582<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0582");}};

template<class C,class=void> struct Probe_T0583{static void run(){std::printf("T0583\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0583<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0583");}};

template<class C,class=void> struct Probe_T0584{static void run(){std::printf("T0584\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0584<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0584");}};

template<class C,class=void> struct Probe_T0585{static void run(){std::printf("T0585\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0585<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0585");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
emit<std::type_index>("T0577");
emit<std::float_t>("T0578");
emit<std::double_t>("T0579");
Probe_T0580<std::initializer_list<int>>::run();
Probe_T0581<std::initializer_list<int>>::run();
Probe_T0582<std::initializer_list<int>>::run();
Probe_T0583<std::initializer_list<int>>::run();
Probe_T0584<std::initializer_list<int>>::run();
Probe_T0585<std::initializer_list<int>>::run();
}
