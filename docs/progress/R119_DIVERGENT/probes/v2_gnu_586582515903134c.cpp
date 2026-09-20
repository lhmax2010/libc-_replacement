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
template<class C,class=void> struct Probe_T0289{static void run(){std::printf("T0289\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0289<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0289");}};

template<class C,class=void> struct Probe_T0290{static void run(){std::printf("T0290\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0290<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0290");}};

template<class C,class=void> struct Probe_T0291{static void run(){std::printf("T0291\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0291<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0291");}};

template<class C,class=void> struct Probe_T0292{static void run(){std::printf("T0292\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0292<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0292");}};

template<class C,class=void> struct Probe_T0293{static void run(){std::printf("T0293\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0293<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0293");}};

template<class C,class=void> struct Probe_T0294{static void run(){std::printf("T0294\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0294<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0294");}};

template<class C,class=void> struct Probe_T0295{static void run(){std::printf("T0295\tUNAVAILABLE=member local_iterator\n");}};
template<class C> struct Probe_T0295<C,std::void_t<typename C::local_iterator>>{static void run(){emit<typename C::local_iterator>("T0295");}};

template<class C,class=void> struct Probe_T0296{static void run(){std::printf("T0296\tUNAVAILABLE=member const_local_iterator\n");}};
template<class C> struct Probe_T0296<C,std::void_t<typename C::const_local_iterator>>{static void run(){emit<typename C::const_local_iterator>("T0296");}};

template<class C,class=void> struct Probe_T0297{static void run(){std::printf("T0297\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0297<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0297");}};

template<class C,class=void> struct Probe_T0298{static void run(){std::printf("T0298\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0298<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0298");}};

template<class C,class=void> struct Probe_T0299{static void run(){std::printf("T0299\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0299<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0299");}};

template<class C,class=void> struct Probe_T0300{static void run(){std::printf("T0300\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0300<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0300");}};

template<class C,class=void> struct Probe_T0301{static void run(){std::printf("T0301\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0301<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0301");}};

template<class C,class=void> struct Probe_T0302{static void run(){std::printf("T0302\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0302<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0302");}};

template<class C,class=void> struct Probe_T0303{static void run(){std::printf("T0303\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0303<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0303");}};

template<class C,class=void> struct Probe_T0304{static void run(){std::printf("T0304\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0304<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0304");}};

template<class C,class=void> struct Probe_T0305{static void run(){std::printf("T0305\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0305<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0305");}};

template<class C,class=void> struct Probe_T0306{static void run(){std::printf("T0306\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0306<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0306");}};

template<class C,class=void> struct Probe_T0307{static void run(){std::printf("T0307\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0307<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0307");}};

template<class C,class=void> struct Probe_T0308{static void run(){std::printf("T0308\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0308<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0308");}};

template<class C,class=void> struct Probe_T0309{static void run(){std::printf("T0309\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0309<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0309");}};

template<class C,class=void> struct Probe_T0310{static void run(){std::printf("T0310\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0310<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0310");}};

template<class C,class=void> struct Probe_T0311{static void run(){std::printf("T0311\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0311<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0311");}};

template<class C,class=void> struct Probe_T0312{static void run(){std::printf("T0312\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0312<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0312");}};

template<class C,class=void> struct Probe_T0313{static void run(){std::printf("T0313\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0313<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0313");}};

template<class C,class=void> struct Probe_T0314{static void run(){std::printf("T0314\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0314<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0314");}};

template<class C,class=void> struct Probe_T0315{static void run(){std::printf("T0315\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0315<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0315");}};

template<class C,class=void> struct Probe_T0316{static void run(){std::printf("T0316\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0316<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0316");}};

template<class C,class=void> struct Probe_T0317{static void run(){std::printf("T0317\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0317<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0317");}};

template<class C,class=void> struct Probe_T0318{static void run(){std::printf("T0318\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0318<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0318");}};

template<class C,class=void> struct Probe_T0319{static void run(){std::printf("T0319\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0319<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0319");}};

template<class C,class=void> struct Probe_T0320{static void run(){std::printf("T0320\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0320<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0320");}};

template<class C,class=void> struct Probe_T0321{static void run(){std::printf("T0321\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0321<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0321");}};

template<class C,class=void> struct Probe_T0322{static void run(){std::printf("T0322\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0322<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0322");}};

template<class C,class=void> struct Probe_T0323{static void run(){std::printf("T0323\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0323<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0323");}};

template<class C,class=void> struct Probe_T0324{static void run(){std::printf("T0324\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0324<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0324");}};

template<class C,class=void> struct Probe_T0325{static void run(){std::printf("T0325\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0325<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0325");}};

template<class C,class=void> struct Probe_T0326{static void run(){std::printf("T0326\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0326<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0326");}};

template<class C,class=void> struct Probe_T0327{static void run(){std::printf("T0327\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0327<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0327");}};

template<class C,class=void> struct Probe_T0328{static void run(){std::printf("T0328\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0328<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0328");}};

template<class C,class=void> struct Probe_T0329{static void run(){std::printf("T0329\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0329<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0329");}};

template<class C,class=void> struct Probe_T0330{static void run(){std::printf("T0330\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0330<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0330");}};

template<class C,class=void> struct Probe_T0331{static void run(){std::printf("T0331\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0331<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0331");}};

template<class C,class=void> struct Probe_T0332{static void run(){std::printf("T0332\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0332<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0332");}};

template<class C,class=void> struct Probe_T0333{static void run(){std::printf("T0333\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0333<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0333");}};

template<class C,class=void> struct Probe_T0334{static void run(){std::printf("T0334\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0334<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0334");}};

template<class C,class=void> struct Probe_T0335{static void run(){std::printf("T0335\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0335<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0335");}};

template<class C,class=void> struct Probe_T0336{static void run(){std::printf("T0336\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0336<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0336");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0289<std::unordered_multiset<int>>::run();
Probe_T0290<std::unordered_multiset<int>>::run();
Probe_T0291<std::unordered_multiset<int>>::run();
Probe_T0292<std::unordered_multiset<int>>::run();
Probe_T0293<std::unordered_multiset<int>>::run();
Probe_T0294<std::unordered_multiset<int>>::run();
Probe_T0295<std::unordered_multiset<int>>::run();
Probe_T0296<std::unordered_multiset<int>>::run();
Probe_T0297<std::unordered_multiset<int>>::run();
Probe_T0298<std::stack<int>>::run();
Probe_T0299<std::stack<int>>::run();
Probe_T0300<std::stack<int>>::run();
Probe_T0301<std::stack<int>>::run();
Probe_T0302<std::queue<int>>::run();
Probe_T0303<std::queue<int>>::run();
Probe_T0304<std::queue<int>>::run();
Probe_T0305<std::queue<int>>::run();
Probe_T0306<std::priority_queue<int>>::run();
Probe_T0307<std::priority_queue<int>>::run();
Probe_T0308<std::priority_queue<int>>::run();
Probe_T0309<std::priority_queue<int>>::run();
Probe_T0310<std::iterator_traits<int*>>::run();
Probe_T0311<std::iterator_traits<int*>>::run();
Probe_T0312<std::iterator_traits<int*>>::run();
Probe_T0313<std::iterator_traits<int*>>::run();
Probe_T0314<std::iterator_traits<int*>>::run();
Probe_T0315<std::iterator_traits<std::vector<int>::iterator>>::run();
Probe_T0316<std::iterator_traits<std::vector<int>::iterator>>::run();
Probe_T0317<std::iterator_traits<std::vector<int>::iterator>>::run();
Probe_T0318<std::iterator_traits<std::vector<int>::iterator>>::run();
Probe_T0319<std::iterator_traits<std::vector<int>::iterator>>::run();
Probe_T0320<std::reverse_iterator<int*>>::run();
Probe_T0321<std::reverse_iterator<int*>>::run();
Probe_T0322<std::reverse_iterator<int*>>::run();
Probe_T0323<std::reverse_iterator<int*>>::run();
Probe_T0324<std::reverse_iterator<int*>>::run();
Probe_T0325<std::move_iterator<int*>>::run();
Probe_T0326<std::move_iterator<int*>>::run();
Probe_T0327<std::move_iterator<int*>>::run();
Probe_T0328<std::move_iterator<int*>>::run();
Probe_T0329<std::move_iterator<int*>>::run();
Probe_T0330<std::back_insert_iterator<std::vector<int>>>::run();
Probe_T0331<std::back_insert_iterator<std::vector<int>>>::run();
Probe_T0332<std::back_insert_iterator<std::vector<int>>>::run();
Probe_T0333<std::back_insert_iterator<std::vector<int>>>::run();
Probe_T0334<std::back_insert_iterator<std::vector<int>>>::run();
Probe_T0335<std::front_insert_iterator<std::list<int>>>::run();
Probe_T0336<std::front_insert_iterator<std::list<int>>>::run();
}
