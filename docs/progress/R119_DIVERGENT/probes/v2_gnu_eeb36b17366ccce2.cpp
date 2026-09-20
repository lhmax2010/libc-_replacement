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

template<class C,class=void> struct Probe_T0012{static void run(){std::printf("T0012\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0012<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0012");}};

template<class C,class=void> struct Probe_T0013{static void run(){std::printf("T0013\tUNAVAILABLE=member int_type\n");}};
template<class C> struct Probe_T0013<C,std::void_t<typename C::int_type>>{static void run(){emit<typename C::int_type>("T0013");}};

template<class C,class=void> struct Probe_T0014{static void run(){std::printf("T0014\tUNAVAILABLE=member off_type\n");}};
template<class C> struct Probe_T0014<C,std::void_t<typename C::off_type>>{static void run(){emit<typename C::off_type>("T0014");}};

template<class C,class=void> struct Probe_T0015{static void run(){std::printf("T0015\tUNAVAILABLE=member pos_type\n");}};
template<class C> struct Probe_T0015<C,std::void_t<typename C::pos_type>>{static void run(){emit<typename C::pos_type>("T0015");}};

template<class C,class=void> struct Probe_T0016{static void run(){std::printf("T0016\tUNAVAILABLE=member state_type\n");}};
template<class C> struct Probe_T0016<C,std::void_t<typename C::state_type>>{static void run(){emit<typename C::state_type>("T0016");}};

template<class C,class=void> struct Probe_T0017{static void run(){std::printf("T0017\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0017<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0017");}};

template<class C,class=void> struct Probe_T0018{static void run(){std::printf("T0018\tUNAVAILABLE=member int_type\n");}};
template<class C> struct Probe_T0018<C,std::void_t<typename C::int_type>>{static void run(){emit<typename C::int_type>("T0018");}};

template<class C,class=void> struct Probe_T0019{static void run(){std::printf("T0019\tUNAVAILABLE=member off_type\n");}};
template<class C> struct Probe_T0019<C,std::void_t<typename C::off_type>>{static void run(){emit<typename C::off_type>("T0019");}};

template<class C,class=void> struct Probe_T0020{static void run(){std::printf("T0020\tUNAVAILABLE=member pos_type\n");}};
template<class C> struct Probe_T0020<C,std::void_t<typename C::pos_type>>{static void run(){emit<typename C::pos_type>("T0020");}};

template<class C,class=void> struct Probe_T0021{static void run(){std::printf("T0021\tUNAVAILABLE=member state_type\n");}};
template<class C> struct Probe_T0021<C,std::void_t<typename C::state_type>>{static void run(){emit<typename C::state_type>("T0021");}};

template<class C,class=void> struct Probe_T0022{static void run(){std::printf("T0022\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0022<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0022");}};

template<class C,class=void> struct Probe_T0023{static void run(){std::printf("T0023\tUNAVAILABLE=member int_type\n");}};
template<class C> struct Probe_T0023<C,std::void_t<typename C::int_type>>{static void run(){emit<typename C::int_type>("T0023");}};

template<class C,class=void> struct Probe_T0024{static void run(){std::printf("T0024\tUNAVAILABLE=member off_type\n");}};
template<class C> struct Probe_T0024<C,std::void_t<typename C::off_type>>{static void run(){emit<typename C::off_type>("T0024");}};

template<class C,class=void> struct Probe_T0025{static void run(){std::printf("T0025\tUNAVAILABLE=member pos_type\n");}};
template<class C> struct Probe_T0025<C,std::void_t<typename C::pos_type>>{static void run(){emit<typename C::pos_type>("T0025");}};

template<class C,class=void> struct Probe_T0026{static void run(){std::printf("T0026\tUNAVAILABLE=member state_type\n");}};
template<class C> struct Probe_T0026<C,std::void_t<typename C::state_type>>{static void run(){emit<typename C::state_type>("T0026");}};

template<class C,class=void> struct Probe_T0027{static void run(){std::printf("T0027\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0027<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0027");}};

template<class C,class=void> struct Probe_T0028{static void run(){std::printf("T0028\tUNAVAILABLE=member int_type\n");}};
template<class C> struct Probe_T0028<C,std::void_t<typename C::int_type>>{static void run(){emit<typename C::int_type>("T0028");}};

template<class C,class=void> struct Probe_T0029{static void run(){std::printf("T0029\tUNAVAILABLE=member off_type\n");}};
template<class C> struct Probe_T0029<C,std::void_t<typename C::off_type>>{static void run(){emit<typename C::off_type>("T0029");}};

template<class C,class=void> struct Probe_T0030{static void run(){std::printf("T0030\tUNAVAILABLE=member pos_type\n");}};
template<class C> struct Probe_T0030<C,std::void_t<typename C::pos_type>>{static void run(){emit<typename C::pos_type>("T0030");}};

template<class C,class=void> struct Probe_T0031{static void run(){std::printf("T0031\tUNAVAILABLE=member state_type\n");}};
template<class C> struct Probe_T0031<C,std::void_t<typename C::state_type>>{static void run(){emit<typename C::state_type>("T0031");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
emit<std::streamoff>("T0001");
emit<std::streamsize>("T0002");
emit<std::streampos>("T0003");
emit<std::wstreampos>("T0004");
emit<std::u16streampos>("T0005");
emit<std::u32streampos>("T0006");
emit<std::fpos<std::mbstate_t>>("T0007");
Probe_T0008<std::ios_base>::run();
Probe_T0009<std::ios_base>::run();
Probe_T0010<std::ios_base>::run();
Probe_T0011<std::ios_base>::run();
Probe_T0012<std::char_traits<char>>::run();
Probe_T0013<std::char_traits<char>>::run();
Probe_T0014<std::char_traits<char>>::run();
Probe_T0015<std::char_traits<char>>::run();
Probe_T0016<std::char_traits<char>>::run();
Probe_T0017<std::char_traits<wchar_t>>::run();
Probe_T0018<std::char_traits<wchar_t>>::run();
Probe_T0019<std::char_traits<wchar_t>>::run();
Probe_T0020<std::char_traits<wchar_t>>::run();
Probe_T0021<std::char_traits<wchar_t>>::run();
Probe_T0022<std::char_traits<char16_t>>::run();
Probe_T0023<std::char_traits<char16_t>>::run();
Probe_T0024<std::char_traits<char16_t>>::run();
Probe_T0025<std::char_traits<char16_t>>::run();
Probe_T0026<std::char_traits<char16_t>>::run();
Probe_T0027<std::char_traits<char32_t>>::run();
Probe_T0028<std::char_traits<char32_t>>::run();
Probe_T0029<std::char_traits<char32_t>>::run();
Probe_T0030<std::char_traits<char32_t>>::run();
Probe_T0031<std::char_traits<char32_t>>::run();
emit<std::int16_t>("T0032");
emit<std::int32_t>("T0033");
emit<std::int64_t>("T0034");
emit<std::int8_t>("T0035");
emit<std::int_fast16_t>("T0036");
emit<std::int_fast32_t>("T0037");
emit<std::int_fast64_t>("T0038");
emit<std::int_fast8_t>("T0039");
emit<std::int_least16_t>("T0040");
emit<std::int_least32_t>("T0041");
emit<std::int_least64_t>("T0042");
emit<std::int_least8_t>("T0043");
emit<std::intmax_t>("T0044");
emit<std::intptr_t>("T0045");
emit<std::max_align_t>("T0046");
emit<std::nullptr_t>("T0047");
emit<std::ptrdiff_t>("T0048");
}
