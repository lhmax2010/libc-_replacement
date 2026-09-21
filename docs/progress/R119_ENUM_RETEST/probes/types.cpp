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
template<class C,class=void> struct Probe_T0008{static void run(){std::printf("T0008\tUNAVAILABLE=member fmtflags\n");}};
template<class C> struct Probe_T0008<C,std::void_t<typename C::fmtflags>>{static void run(){emit<typename C::fmtflags>("T0008");}};

template<class C,class=void> struct Probe_T0009{static void run(){std::printf("T0009\tUNAVAILABLE=member iostate\n");}};
template<class C> struct Probe_T0009<C,std::void_t<typename C::iostate>>{static void run(){emit<typename C::iostate>("T0009");}};

template<class C,class=void> struct Probe_T0010{static void run(){std::printf("T0010\tUNAVAILABLE=member openmode\n");}};
template<class C> struct Probe_T0010<C,std::void_t<typename C::openmode>>{static void run(){emit<typename C::openmode>("T0010");}};

template<class C,class=void> struct Probe_T0011{static void run(){std::printf("T0011\tUNAVAILABLE=member seekdir\n");}};
template<class C> struct Probe_T0011<C,std::void_t<typename C::seekdir>>{static void run(){emit<typename C::seekdir>("T0011");}};

int main(){
#ifdef _LIBCPP_VERSION
 std::printf("LIBRARY=libc++ VERSION=%d\n",_LIBCPP_VERSION);
#else
 std::printf("LIBRARY=libstdc++ RELEASE=%d\n",_GLIBCXX_RELEASE);
#endif
 std::printf("TARGET_POINTER=%zu TARGET_LONG=%zu TARGET_LONG_LONG=%zu\n",sizeof(void*),sizeof(long),sizeof(long long));
emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0008<std::ios_base>::run();
Probe_T0009<std::ios_base>::run();
Probe_T0010<std::ios_base>::run();
Probe_T0011<std::ios_base>::run();
}
