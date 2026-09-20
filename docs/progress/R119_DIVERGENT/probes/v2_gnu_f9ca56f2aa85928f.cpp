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
template<class C,class=void> struct Probe_T0193{static void run(){std::printf("T0193\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0193<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0193");}};

template<class C,class=void> struct Probe_T0194{static void run(){std::printf("T0194\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0194<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0194");}};

template<class C,class=void> struct Probe_T0195{static void run(){std::printf("T0195\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0195<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0195");}};

template<class C,class=void> struct Probe_T0196{static void run(){std::printf("T0196\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0196<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0196");}};

template<class C,class=void> struct Probe_T0197{static void run(){std::printf("T0197\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0197<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0197");}};

template<class C,class=void> struct Probe_T0198{static void run(){std::printf("T0198\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0198<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0198");}};

template<class C,class=void> struct Probe_T0199{static void run(){std::printf("T0199\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0199<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0199");}};

template<class C,class=void> struct Probe_T0200{static void run(){std::printf("T0200\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0200<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0200");}};

template<class C,class=void> struct Probe_T0201{static void run(){std::printf("T0201\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0201<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0201");}};

template<class C,class=void> struct Probe_T0202{static void run(){std::printf("T0202\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0202<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0202");}};

template<class C,class=void> struct Probe_T0203{static void run(){std::printf("T0203\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0203<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0203");}};

template<class C,class=void> struct Probe_T0204{static void run(){std::printf("T0204\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0204<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0204");}};

template<class C,class=void> struct Probe_T0205{static void run(){std::printf("T0205\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0205<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0205");}};

template<class C,class=void> struct Probe_T0206{static void run(){std::printf("T0206\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0206<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0206");}};

template<class C,class=void> struct Probe_T0207{static void run(){std::printf("T0207\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0207<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0207");}};

template<class C,class=void> struct Probe_T0208{static void run(){std::printf("T0208\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0208<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0208");}};

template<class C,class=void> struct Probe_T0209{static void run(){std::printf("T0209\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0209<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0209");}};

template<class C,class=void> struct Probe_T0210{static void run(){std::printf("T0210\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0210<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0210");}};

template<class C,class=void> struct Probe_T0211{static void run(){std::printf("T0211\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0211<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0211");}};

template<class C,class=void> struct Probe_T0212{static void run(){std::printf("T0212\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0212<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0212");}};

template<class C,class=void> struct Probe_T0213{static void run(){std::printf("T0213\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0213<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0213");}};

template<class C,class=void> struct Probe_T0214{static void run(){std::printf("T0214\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0214<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0214");}};

template<class C,class=void> struct Probe_T0215{static void run(){std::printf("T0215\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0215<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0215");}};

template<class C,class=void> struct Probe_T0216{static void run(){std::printf("T0216\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0216<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0216");}};

template<class C,class=void> struct Probe_T0217{static void run(){std::printf("T0217\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0217<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0217");}};

template<class C,class=void> struct Probe_T0218{static void run(){std::printf("T0218\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0218<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0218");}};

template<class C,class=void> struct Probe_T0219{static void run(){std::printf("T0219\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0219<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0219");}};

template<class C,class=void> struct Probe_T0220{static void run(){std::printf("T0220\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0220<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0220");}};

template<class C,class=void> struct Probe_T0221{static void run(){std::printf("T0221\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0221<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0221");}};

template<class C,class=void> struct Probe_T0222{static void run(){std::printf("T0222\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0222<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0222");}};

template<class C,class=void> struct Probe_T0223{static void run(){std::printf("T0223\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0223<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0223");}};

template<class C,class=void> struct Probe_T0224{static void run(){std::printf("T0224\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0224<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0224");}};

template<class C,class=void> struct Probe_T0225{static void run(){std::printf("T0225\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0225<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0225");}};

template<class C,class=void> struct Probe_T0226{static void run(){std::printf("T0226\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0226<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0226");}};

template<class C,class=void> struct Probe_T0227{static void run(){std::printf("T0227\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0227<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0227");}};

template<class C,class=void> struct Probe_T0228{static void run(){std::printf("T0228\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0228<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0228");}};

template<class C,class=void> struct Probe_T0229{static void run(){std::printf("T0229\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0229<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0229");}};

template<class C,class=void> struct Probe_T0230{static void run(){std::printf("T0230\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0230<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0230");}};

template<class C,class=void> struct Probe_T0231{static void run(){std::printf("T0231\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0231<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0231");}};

template<class C,class=void> struct Probe_T0232{static void run(){std::printf("T0232\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0232<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0232");}};

template<class C,class=void> struct Probe_T0233{static void run(){std::printf("T0233\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0233<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0233");}};

template<class C,class=void> struct Probe_T0234{static void run(){std::printf("T0234\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0234<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0234");}};

template<class C,class=void> struct Probe_T0235{static void run(){std::printf("T0235\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0235<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0235");}};

template<class C,class=void> struct Probe_T0236{static void run(){std::printf("T0236\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0236<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0236");}};

template<class C,class=void> struct Probe_T0237{static void run(){std::printf("T0237\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0237<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0237");}};

template<class C,class=void> struct Probe_T0238{static void run(){std::printf("T0238\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0238<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0238");}};

template<class C,class=void> struct Probe_T0239{static void run(){std::printf("T0239\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0239<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0239");}};

template<class C,class=void> struct Probe_T0240{static void run(){std::printf("T0240\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0240<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0240");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0193<std::array<int,3>>::run();
Probe_T0194<std::array<int,3>>::run();
Probe_T0195<std::array<int,3>>::run();
Probe_T0196<std::array<int,3>>::run();
Probe_T0197<std::array<int,3>>::run();
Probe_T0198<std::array<int,3>>::run();
Probe_T0199<std::array<int,3>>::run();
Probe_T0200<std::array<int,3>>::run();
Probe_T0201<std::array<int,3>>::run();
Probe_T0202<std::map<int,int>>::run();
Probe_T0203<std::map<int,int>>::run();
Probe_T0204<std::map<int,int>>::run();
Probe_T0205<std::map<int,int>>::run();
Probe_T0206<std::map<int,int>>::run();
Probe_T0207<std::map<int,int>>::run();
Probe_T0208<std::map<int,int>>::run();
Probe_T0209<std::map<int,int>>::run();
Probe_T0210<std::map<int,int>>::run();
Probe_T0211<std::map<int,int>>::run();
Probe_T0212<std::map<int,int>>::run();
Probe_T0213<std::map<int,int>>::run();
Probe_T0214<std::multimap<int,int>>::run();
Probe_T0215<std::multimap<int,int>>::run();
Probe_T0216<std::multimap<int,int>>::run();
Probe_T0217<std::multimap<int,int>>::run();
Probe_T0218<std::multimap<int,int>>::run();
Probe_T0219<std::multimap<int,int>>::run();
Probe_T0220<std::multimap<int,int>>::run();
Probe_T0221<std::multimap<int,int>>::run();
Probe_T0222<std::multimap<int,int>>::run();
Probe_T0223<std::multimap<int,int>>::run();
Probe_T0224<std::multimap<int,int>>::run();
Probe_T0225<std::multimap<int,int>>::run();
Probe_T0226<std::set<int>>::run();
Probe_T0227<std::set<int>>::run();
Probe_T0228<std::set<int>>::run();
Probe_T0229<std::set<int>>::run();
Probe_T0230<std::set<int>>::run();
Probe_T0231<std::set<int>>::run();
Probe_T0232<std::set<int>>::run();
Probe_T0233<std::set<int>>::run();
Probe_T0234<std::set<int>>::run();
Probe_T0235<std::set<int>>::run();
Probe_T0236<std::set<int>>::run();
Probe_T0237<std::set<int>>::run();
Probe_T0238<std::multiset<int>>::run();
Probe_T0239<std::multiset<int>>::run();
Probe_T0240<std::multiset<int>>::run();
}
