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
template<class C,class=void> struct Probe_T0097{static void run(){std::printf("T0097\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0097<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0097");}};

template<class C,class=void> struct Probe_T0098{static void run(){std::printf("T0098\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0098<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0098");}};

template<class C,class=void> struct Probe_T0100{static void run(){std::printf("T0100\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0100<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0100");}};

template<class C,class=void> struct Probe_T0101{static void run(){std::printf("T0101\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0101<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0101");}};

template<class C,class=void> struct Probe_T0102{static void run(){std::printf("T0102\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0102<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0102");}};

template<class C,class=void> struct Probe_T0103{static void run(){std::printf("T0103\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0103<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0103");}};

template<class C,class=void> struct Probe_T0104{static void run(){std::printf("T0104\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0104<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0104");}};

template<class C,class=void> struct Probe_T0105{static void run(){std::printf("T0105\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0105<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0105");}};

template<class C,class=void> struct Probe_T0106{static void run(){std::printf("T0106\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0106<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0106");}};

template<class C,class=void> struct Probe_T0107{static void run(){std::printf("T0107\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0107<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0107");}};

template<class C,class=void> struct Probe_T0108{static void run(){std::printf("T0108\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0108<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0108");}};

template<class C,class=void> struct Probe_T0109{static void run(){std::printf("T0109\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0109<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0109");}};

template<class C,class=void> struct Probe_T0110{static void run(){std::printf("T0110\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0110<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0110");}};

template<class C,class=void> struct Probe_T0112{static void run(){std::printf("T0112\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0112<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0112");}};

template<class C,class=void> struct Probe_T0113{static void run(){std::printf("T0113\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0113<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0113");}};

template<class C,class=void> struct Probe_T0114{static void run(){std::printf("T0114\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0114<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0114");}};

template<class C,class=void> struct Probe_T0115{static void run(){std::printf("T0115\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0115<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0115");}};

template<class C,class=void> struct Probe_T0116{static void run(){std::printf("T0116\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0116<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0116");}};

template<class C,class=void> struct Probe_T0117{static void run(){std::printf("T0117\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0117<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0117");}};

template<class C,class=void> struct Probe_T0118{static void run(){std::printf("T0118\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0118<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0118");}};

template<class C,class=void> struct Probe_T0119{static void run(){std::printf("T0119\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0119<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0119");}};

template<class C,class=void> struct Probe_T0120{static void run(){std::printf("T0120\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0120<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0120");}};

template<class C,class=void> struct Probe_T0121{static void run(){std::printf("T0121\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0121<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0121");}};

template<class C,class=void> struct Probe_T0122{static void run(){std::printf("T0122\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0122<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0122");}};

template<class C,class=void> struct Probe_T0124{static void run(){std::printf("T0124\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0124<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0124");}};

template<class C,class=void> struct Probe_T0125{static void run(){std::printf("T0125\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0125<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0125");}};

template<class C,class=void> struct Probe_T0126{static void run(){std::printf("T0126\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0126<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0126");}};

template<class C,class=void> struct Probe_T0127{static void run(){std::printf("T0127\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0127<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0127");}};

template<class C,class=void> struct Probe_T0128{static void run(){std::printf("T0128\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0128<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0128");}};

template<class C,class=void> struct Probe_T0129{static void run(){std::printf("T0129\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0129<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0129");}};

template<class C,class=void> struct Probe_T0130{static void run(){std::printf("T0130\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0130<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0130");}};

template<class C,class=void> struct Probe_T0131{static void run(){std::printf("T0131\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0131<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0131");}};

template<class C,class=void> struct Probe_T0132{static void run(){std::printf("T0132\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0132<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0132");}};

template<class C,class=void> struct Probe_T0133{static void run(){std::printf("T0133\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0133<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0133");}};

template<class C,class=void> struct Probe_T0134{static void run(){std::printf("T0134\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0134<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0134");}};

template<class C,class=void> struct Probe_T0136{static void run(){std::printf("T0136\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0136<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0136");}};

template<class C,class=void> struct Probe_T0137{static void run(){std::printf("T0137\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0137<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0137");}};

template<class C,class=void> struct Probe_T0138{static void run(){std::printf("T0138\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0138<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0138");}};

template<class C,class=void> struct Probe_T0139{static void run(){std::printf("T0139\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0139<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0139");}};

template<class C,class=void> struct Probe_T0140{static void run(){std::printf("T0140\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0140<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0140");}};

template<class C,class=void> struct Probe_T0141{static void run(){std::printf("T0141\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0141<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0141");}};

template<class C,class=void> struct Probe_T0142{static void run(){std::printf("T0142\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0142<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0142");}};

template<class C,class=void> struct Probe_T0143{static void run(){std::printf("T0143\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0143<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0143");}};

template<class C,class=void> struct Probe_T0144{static void run(){std::printf("T0144\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0144<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0144");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0097<std::u16string>::run();
Probe_T0098<std::u16string>::run();
emit<decltype(std::u16string::npos)>("T0099");
Probe_T0100<std::u32string>::run();
Probe_T0101<std::u32string>::run();
Probe_T0102<std::u32string>::run();
Probe_T0103<std::u32string>::run();
Probe_T0104<std::u32string>::run();
Probe_T0105<std::u32string>::run();
Probe_T0106<std::u32string>::run();
Probe_T0107<std::u32string>::run();
Probe_T0108<std::u32string>::run();
Probe_T0109<std::u32string>::run();
Probe_T0110<std::u32string>::run();
emit<decltype(std::u32string::npos)>("T0111");
Probe_T0112<std::string_view>::run();
Probe_T0113<std::string_view>::run();
Probe_T0114<std::string_view>::run();
Probe_T0115<std::string_view>::run();
Probe_T0116<std::string_view>::run();
Probe_T0117<std::string_view>::run();
Probe_T0118<std::string_view>::run();
Probe_T0119<std::string_view>::run();
Probe_T0120<std::string_view>::run();
Probe_T0121<std::string_view>::run();
Probe_T0122<std::string_view>::run();
emit<decltype(std::string_view::npos)>("T0123");
Probe_T0124<std::wstring_view>::run();
Probe_T0125<std::wstring_view>::run();
Probe_T0126<std::wstring_view>::run();
Probe_T0127<std::wstring_view>::run();
Probe_T0128<std::wstring_view>::run();
Probe_T0129<std::wstring_view>::run();
Probe_T0130<std::wstring_view>::run();
Probe_T0131<std::wstring_view>::run();
Probe_T0132<std::wstring_view>::run();
Probe_T0133<std::wstring_view>::run();
Probe_T0134<std::wstring_view>::run();
emit<decltype(std::wstring_view::npos)>("T0135");
Probe_T0136<std::vector<int>>::run();
Probe_T0137<std::vector<int>>::run();
Probe_T0138<std::vector<int>>::run();
Probe_T0139<std::vector<int>>::run();
Probe_T0140<std::vector<int>>::run();
Probe_T0141<std::vector<int>>::run();
Probe_T0142<std::vector<int>>::run();
Probe_T0143<std::vector<int>>::run();
Probe_T0144<std::vector<int>>::run();
}
