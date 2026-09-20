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
template<class C,class=void> struct Probe_T0145{static void run(){std::printf("T0145\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0145<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0145");}};

template<class C,class=void> struct Probe_T0146{static void run(){std::printf("T0146\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0146<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0146");}};

template<class C,class=void> struct Probe_T0147{static void run(){std::printf("T0147\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0147<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0147");}};

template<class C,class=void> struct Probe_T0148{static void run(){std::printf("T0148\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0148<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0148");}};

template<class C,class=void> struct Probe_T0149{static void run(){std::printf("T0149\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0149<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0149");}};

template<class C,class=void> struct Probe_T0150{static void run(){std::printf("T0150\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0150<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0150");}};

template<class C,class=void> struct Probe_T0151{static void run(){std::printf("T0151\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0151<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0151");}};

template<class C,class=void> struct Probe_T0152{static void run(){std::printf("T0152\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0152<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0152");}};

template<class C,class=void> struct Probe_T0153{static void run(){std::printf("T0153\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0153<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0153");}};

template<class C,class=void> struct Probe_T0154{static void run(){std::printf("T0154\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0154<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0154");}};

template<class C,class=void> struct Probe_T0155{static void run(){std::printf("T0155\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0155<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0155");}};

template<class C,class=void> struct Probe_T0156{static void run(){std::printf("T0156\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0156<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0156");}};

template<class C,class=void> struct Probe_T0157{static void run(){std::printf("T0157\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0157<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0157");}};

template<class C,class=void> struct Probe_T0158{static void run(){std::printf("T0158\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0158<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0158");}};

template<class C,class=void> struct Probe_T0159{static void run(){std::printf("T0159\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0159<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0159");}};

template<class C,class=void> struct Probe_T0160{static void run(){std::printf("T0160\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0160<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0160");}};

template<class C,class=void> struct Probe_T0161{static void run(){std::printf("T0161\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0161<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0161");}};

template<class C,class=void> struct Probe_T0162{static void run(){std::printf("T0162\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0162<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0162");}};

template<class C,class=void> struct Probe_T0163{static void run(){std::printf("T0163\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0163<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0163");}};

template<class C,class=void> struct Probe_T0164{static void run(){std::printf("T0164\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0164<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0164");}};

template<class C,class=void> struct Probe_T0165{static void run(){std::printf("T0165\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0165<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0165");}};

template<class C,class=void> struct Probe_T0166{static void run(){std::printf("T0166\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0166<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0166");}};

template<class C,class=void> struct Probe_T0167{static void run(){std::printf("T0167\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0167<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0167");}};

template<class C,class=void> struct Probe_T0168{static void run(){std::printf("T0168\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0168<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0168");}};

template<class C,class=void> struct Probe_T0169{static void run(){std::printf("T0169\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0169<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0169");}};

template<class C,class=void> struct Probe_T0170{static void run(){std::printf("T0170\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0170<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0170");}};

template<class C,class=void> struct Probe_T0171{static void run(){std::printf("T0171\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0171<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0171");}};

template<class C,class=void> struct Probe_T0172{static void run(){std::printf("T0172\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0172<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0172");}};

template<class C,class=void> struct Probe_T0173{static void run(){std::printf("T0173\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0173<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0173");}};

template<class C,class=void> struct Probe_T0174{static void run(){std::printf("T0174\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0174<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0174");}};

template<class C,class=void> struct Probe_T0175{static void run(){std::printf("T0175\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0175<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0175");}};

template<class C,class=void> struct Probe_T0176{static void run(){std::printf("T0176\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0176<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0176");}};

template<class C,class=void> struct Probe_T0177{static void run(){std::printf("T0177\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0177<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0177");}};

template<class C,class=void> struct Probe_T0178{static void run(){std::printf("T0178\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0178<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0178");}};

template<class C,class=void> struct Probe_T0179{static void run(){std::printf("T0179\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0179<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0179");}};

template<class C,class=void> struct Probe_T0180{static void run(){std::printf("T0180\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0180<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0180");}};

template<class C,class=void> struct Probe_T0181{static void run(){std::printf("T0181\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0181<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0181");}};

template<class C,class=void> struct Probe_T0182{static void run(){std::printf("T0182\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0182<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0182");}};

template<class C,class=void> struct Probe_T0183{static void run(){std::printf("T0183\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0183<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0183");}};

template<class C,class=void> struct Probe_T0184{static void run(){std::printf("T0184\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0184<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0184");}};

template<class C,class=void> struct Probe_T0185{static void run(){std::printf("T0185\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0185<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0185");}};

template<class C,class=void> struct Probe_T0186{static void run(){std::printf("T0186\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0186<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0186");}};

template<class C,class=void> struct Probe_T0187{static void run(){std::printf("T0187\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0187<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0187");}};

template<class C,class=void> struct Probe_T0188{static void run(){std::printf("T0188\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0188<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0188");}};

template<class C,class=void> struct Probe_T0189{static void run(){std::printf("T0189\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0189<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0189");}};

template<class C,class=void> struct Probe_T0190{static void run(){std::printf("T0190\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0190<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0190");}};

template<class C,class=void> struct Probe_T0191{static void run(){std::printf("T0191\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0191<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0191");}};

template<class C,class=void> struct Probe_T0192{static void run(){std::printf("T0192\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0192<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0192");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0145<std::vector<int>>::run();
Probe_T0146<std::vector<int>>::run();
Probe_T0147<std::vector<bool>>::run();
Probe_T0148<std::vector<bool>>::run();
Probe_T0149<std::vector<bool>>::run();
Probe_T0150<std::vector<bool>>::run();
Probe_T0151<std::vector<bool>>::run();
Probe_T0152<std::vector<bool>>::run();
Probe_T0153<std::vector<bool>>::run();
Probe_T0154<std::vector<bool>>::run();
Probe_T0155<std::vector<bool>>::run();
Probe_T0156<std::vector<bool>>::run();
Probe_T0157<std::vector<bool>>::run();
Probe_T0158<std::deque<int>>::run();
Probe_T0159<std::deque<int>>::run();
Probe_T0160<std::deque<int>>::run();
Probe_T0161<std::deque<int>>::run();
Probe_T0162<std::deque<int>>::run();
Probe_T0163<std::deque<int>>::run();
Probe_T0164<std::deque<int>>::run();
Probe_T0165<std::deque<int>>::run();
Probe_T0166<std::deque<int>>::run();
Probe_T0167<std::deque<int>>::run();
Probe_T0168<std::deque<int>>::run();
Probe_T0169<std::list<int>>::run();
Probe_T0170<std::list<int>>::run();
Probe_T0171<std::list<int>>::run();
Probe_T0172<std::list<int>>::run();
Probe_T0173<std::list<int>>::run();
Probe_T0174<std::list<int>>::run();
Probe_T0175<std::list<int>>::run();
Probe_T0176<std::list<int>>::run();
Probe_T0177<std::list<int>>::run();
Probe_T0178<std::list<int>>::run();
Probe_T0179<std::list<int>>::run();
Probe_T0180<std::forward_list<int>>::run();
Probe_T0181<std::forward_list<int>>::run();
Probe_T0182<std::forward_list<int>>::run();
Probe_T0183<std::forward_list<int>>::run();
Probe_T0184<std::forward_list<int>>::run();
Probe_T0185<std::forward_list<int>>::run();
Probe_T0186<std::forward_list<int>>::run();
Probe_T0187<std::forward_list<int>>::run();
Probe_T0188<std::forward_list<int>>::run();
Probe_T0189<std::forward_list<int>>::run();
Probe_T0190<std::forward_list<int>>::run();
Probe_T0191<std::array<int,3>>::run();
Probe_T0192<std::array<int,3>>::run();
}
