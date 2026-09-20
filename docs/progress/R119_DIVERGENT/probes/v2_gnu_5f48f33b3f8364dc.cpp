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
template<class C,class=void> struct Probe_T0064{static void run(){std::printf("T0064\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0064<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0064");}};

template<class C,class=void> struct Probe_T0065{static void run(){std::printf("T0065\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0065<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0065");}};

template<class C,class=void> struct Probe_T0066{static void run(){std::printf("T0066\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0066<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0066");}};

template<class C,class=void> struct Probe_T0067{static void run(){std::printf("T0067\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0067<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0067");}};

template<class C,class=void> struct Probe_T0068{static void run(){std::printf("T0068\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0068<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0068");}};

template<class C,class=void> struct Probe_T0069{static void run(){std::printf("T0069\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0069<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0069");}};

template<class C,class=void> struct Probe_T0070{static void run(){std::printf("T0070\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0070<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0070");}};

template<class C,class=void> struct Probe_T0071{static void run(){std::printf("T0071\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0071<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0071");}};

template<class C,class=void> struct Probe_T0072{static void run(){std::printf("T0072\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0072<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0072");}};

template<class C,class=void> struct Probe_T0073{static void run(){std::printf("T0073\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0073<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0073");}};

template<class C,class=void> struct Probe_T0074{static void run(){std::printf("T0074\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0074<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0074");}};

template<class C,class=void> struct Probe_T0076{static void run(){std::printf("T0076\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0076<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0076");}};

template<class C,class=void> struct Probe_T0077{static void run(){std::printf("T0077\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0077<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0077");}};

template<class C,class=void> struct Probe_T0078{static void run(){std::printf("T0078\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0078<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0078");}};

template<class C,class=void> struct Probe_T0079{static void run(){std::printf("T0079\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0079<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0079");}};

template<class C,class=void> struct Probe_T0080{static void run(){std::printf("T0080\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0080<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0080");}};

template<class C,class=void> struct Probe_T0081{static void run(){std::printf("T0081\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0081<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0081");}};

template<class C,class=void> struct Probe_T0082{static void run(){std::printf("T0082\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0082<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0082");}};

template<class C,class=void> struct Probe_T0083{static void run(){std::printf("T0083\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0083<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0083");}};

template<class C,class=void> struct Probe_T0084{static void run(){std::printf("T0084\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0084<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0084");}};

template<class C,class=void> struct Probe_T0085{static void run(){std::printf("T0085\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0085<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0085");}};

template<class C,class=void> struct Probe_T0086{static void run(){std::printf("T0086\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0086<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0086");}};

template<class C,class=void> struct Probe_T0088{static void run(){std::printf("T0088\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0088<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0088");}};

template<class C,class=void> struct Probe_T0089{static void run(){std::printf("T0089\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0089<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0089");}};

template<class C,class=void> struct Probe_T0090{static void run(){std::printf("T0090\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0090<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0090");}};

template<class C,class=void> struct Probe_T0091{static void run(){std::printf("T0091\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0091<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0091");}};

template<class C,class=void> struct Probe_T0092{static void run(){std::printf("T0092\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0092<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0092");}};

template<class C,class=void> struct Probe_T0093{static void run(){std::printf("T0093\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0093<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0093");}};

template<class C,class=void> struct Probe_T0094{static void run(){std::printf("T0094\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0094<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0094");}};

template<class C,class=void> struct Probe_T0095{static void run(){std::printf("T0095\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0095<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0095");}};

template<class C,class=void> struct Probe_T0096{static void run(){std::printf("T0096\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0096<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0096");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
emit<std::size_t>("T0049");
emit<std::uint16_t>("T0050");
emit<std::uint32_t>("T0051");
emit<std::uint64_t>("T0052");
emit<std::uint8_t>("T0053");
emit<std::uint_fast16_t>("T0054");
emit<std::uint_fast32_t>("T0055");
emit<std::uint_fast64_t>("T0056");
emit<std::uint_fast8_t>("T0057");
emit<std::uint_least16_t>("T0058");
emit<std::uint_least32_t>("T0059");
emit<std::uint_least64_t>("T0060");
emit<std::uint_least8_t>("T0061");
emit<std::uintmax_t>("T0062");
emit<std::uintptr_t>("T0063");
Probe_T0064<std::string>::run();
Probe_T0065<std::string>::run();
Probe_T0066<std::string>::run();
Probe_T0067<std::string>::run();
Probe_T0068<std::string>::run();
Probe_T0069<std::string>::run();
Probe_T0070<std::string>::run();
Probe_T0071<std::string>::run();
Probe_T0072<std::string>::run();
Probe_T0073<std::string>::run();
Probe_T0074<std::string>::run();
emit<decltype(std::string::npos)>("T0075");
Probe_T0076<std::wstring>::run();
Probe_T0077<std::wstring>::run();
Probe_T0078<std::wstring>::run();
Probe_T0079<std::wstring>::run();
Probe_T0080<std::wstring>::run();
Probe_T0081<std::wstring>::run();
Probe_T0082<std::wstring>::run();
Probe_T0083<std::wstring>::run();
Probe_T0084<std::wstring>::run();
Probe_T0085<std::wstring>::run();
Probe_T0086<std::wstring>::run();
emit<decltype(std::wstring::npos)>("T0087");
Probe_T0088<std::u16string>::run();
Probe_T0089<std::u16string>::run();
Probe_T0090<std::u16string>::run();
Probe_T0091<std::u16string>::run();
Probe_T0092<std::u16string>::run();
Probe_T0093<std::u16string>::run();
Probe_T0094<std::u16string>::run();
Probe_T0095<std::u16string>::run();
Probe_T0096<std::u16string>::run();
}
