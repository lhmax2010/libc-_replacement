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
template<class C,class=void> struct Probe_T0241{static void run(){std::printf("T0241\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0241<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0241");}};

template<class C,class=void> struct Probe_T0242{static void run(){std::printf("T0242\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0242<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0242");}};

template<class C,class=void> struct Probe_T0243{static void run(){std::printf("T0243\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0243<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0243");}};

template<class C,class=void> struct Probe_T0244{static void run(){std::printf("T0244\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0244<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0244");}};

template<class C,class=void> struct Probe_T0245{static void run(){std::printf("T0245\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0245<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0245");}};

template<class C,class=void> struct Probe_T0246{static void run(){std::printf("T0246\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0246<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0246");}};

template<class C,class=void> struct Probe_T0247{static void run(){std::printf("T0247\tUNAVAILABLE=member reverse_iterator\n");}};
template<class C> struct Probe_T0247<C,std::void_t<typename C::reverse_iterator>>{static void run(){emit<typename C::reverse_iterator>("T0247");}};

template<class C,class=void> struct Probe_T0248{static void run(){std::printf("T0248\tUNAVAILABLE=member const_reverse_iterator\n");}};
template<class C> struct Probe_T0248<C,std::void_t<typename C::const_reverse_iterator>>{static void run(){emit<typename C::const_reverse_iterator>("T0248");}};

template<class C,class=void> struct Probe_T0249{static void run(){std::printf("T0249\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0249<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0249");}};

template<class C,class=void> struct Probe_T0250{static void run(){std::printf("T0250\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0250<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0250");}};

template<class C,class=void> struct Probe_T0251{static void run(){std::printf("T0251\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0251<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0251");}};

template<class C,class=void> struct Probe_T0252{static void run(){std::printf("T0252\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0252<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0252");}};

template<class C,class=void> struct Probe_T0253{static void run(){std::printf("T0253\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0253<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0253");}};

template<class C,class=void> struct Probe_T0254{static void run(){std::printf("T0254\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0254<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0254");}};

template<class C,class=void> struct Probe_T0255{static void run(){std::printf("T0255\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0255<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0255");}};

template<class C,class=void> struct Probe_T0256{static void run(){std::printf("T0256\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0256<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0256");}};

template<class C,class=void> struct Probe_T0257{static void run(){std::printf("T0257\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0257<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0257");}};

template<class C,class=void> struct Probe_T0258{static void run(){std::printf("T0258\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0258<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0258");}};

template<class C,class=void> struct Probe_T0259{static void run(){std::printf("T0259\tUNAVAILABLE=member local_iterator\n");}};
template<class C> struct Probe_T0259<C,std::void_t<typename C::local_iterator>>{static void run(){emit<typename C::local_iterator>("T0259");}};

template<class C,class=void> struct Probe_T0260{static void run(){std::printf("T0260\tUNAVAILABLE=member const_local_iterator\n");}};
template<class C> struct Probe_T0260<C,std::void_t<typename C::const_local_iterator>>{static void run(){emit<typename C::const_local_iterator>("T0260");}};

template<class C,class=void> struct Probe_T0261{static void run(){std::printf("T0261\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0261<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0261");}};

template<class C,class=void> struct Probe_T0262{static void run(){std::printf("T0262\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0262<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0262");}};

template<class C,class=void> struct Probe_T0263{static void run(){std::printf("T0263\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0263<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0263");}};

template<class C,class=void> struct Probe_T0264{static void run(){std::printf("T0264\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0264<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0264");}};

template<class C,class=void> struct Probe_T0265{static void run(){std::printf("T0265\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0265<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0265");}};

template<class C,class=void> struct Probe_T0266{static void run(){std::printf("T0266\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0266<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0266");}};

template<class C,class=void> struct Probe_T0267{static void run(){std::printf("T0267\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0267<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0267");}};

template<class C,class=void> struct Probe_T0268{static void run(){std::printf("T0268\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0268<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0268");}};

template<class C,class=void> struct Probe_T0269{static void run(){std::printf("T0269\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0269<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0269");}};

template<class C,class=void> struct Probe_T0270{static void run(){std::printf("T0270\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0270<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0270");}};

template<class C,class=void> struct Probe_T0271{static void run(){std::printf("T0271\tUNAVAILABLE=member local_iterator\n");}};
template<class C> struct Probe_T0271<C,std::void_t<typename C::local_iterator>>{static void run(){emit<typename C::local_iterator>("T0271");}};

template<class C,class=void> struct Probe_T0272{static void run(){std::printf("T0272\tUNAVAILABLE=member const_local_iterator\n");}};
template<class C> struct Probe_T0272<C,std::void_t<typename C::const_local_iterator>>{static void run(){emit<typename C::const_local_iterator>("T0272");}};

template<class C,class=void> struct Probe_T0273{static void run(){std::printf("T0273\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0273<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0273");}};

template<class C,class=void> struct Probe_T0274{static void run(){std::printf("T0274\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0274<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0274");}};

template<class C,class=void> struct Probe_T0275{static void run(){std::printf("T0275\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0275<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0275");}};

template<class C,class=void> struct Probe_T0276{static void run(){std::printf("T0276\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0276<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0276");}};

template<class C,class=void> struct Probe_T0277{static void run(){std::printf("T0277\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0277<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0277");}};

template<class C,class=void> struct Probe_T0278{static void run(){std::printf("T0278\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0278<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0278");}};

template<class C,class=void> struct Probe_T0279{static void run(){std::printf("T0279\tUNAVAILABLE=member const_reference\n");}};
template<class C> struct Probe_T0279<C,std::void_t<typename C::const_reference>>{static void run(){emit<typename C::const_reference>("T0279");}};

template<class C,class=void> struct Probe_T0280{static void run(){std::printf("T0280\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0280<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0280");}};

template<class C,class=void> struct Probe_T0281{static void run(){std::printf("T0281\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0281<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0281");}};

template<class C,class=void> struct Probe_T0282{static void run(){std::printf("T0282\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0282<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0282");}};

template<class C,class=void> struct Probe_T0283{static void run(){std::printf("T0283\tUNAVAILABLE=member local_iterator\n");}};
template<class C> struct Probe_T0283<C,std::void_t<typename C::local_iterator>>{static void run(){emit<typename C::local_iterator>("T0283");}};

template<class C,class=void> struct Probe_T0284{static void run(){std::printf("T0284\tUNAVAILABLE=member const_local_iterator\n");}};
template<class C> struct Probe_T0284<C,std::void_t<typename C::const_local_iterator>>{static void run(){emit<typename C::const_local_iterator>("T0284");}};

template<class C,class=void> struct Probe_T0285{static void run(){std::printf("T0285\tUNAVAILABLE=member node_type\n");}};
template<class C> struct Probe_T0285<C,std::void_t<typename C::node_type>>{static void run(){emit<typename C::node_type>("T0285");}};

template<class C,class=void> struct Probe_T0286{static void run(){std::printf("T0286\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0286<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0286");}};

template<class C,class=void> struct Probe_T0287{static void run(){std::printf("T0287\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0287<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0287");}};

template<class C,class=void> struct Probe_T0288{static void run(){std::printf("T0288\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0288<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0288");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0241<std::multiset<int>>::run();
Probe_T0242<std::multiset<int>>::run();
Probe_T0243<std::multiset<int>>::run();
Probe_T0244<std::multiset<int>>::run();
Probe_T0245<std::multiset<int>>::run();
Probe_T0246<std::multiset<int>>::run();
Probe_T0247<std::multiset<int>>::run();
Probe_T0248<std::multiset<int>>::run();
Probe_T0249<std::multiset<int>>::run();
Probe_T0250<std::unordered_map<int,int>>::run();
Probe_T0251<std::unordered_map<int,int>>::run();
Probe_T0252<std::unordered_map<int,int>>::run();
Probe_T0253<std::unordered_map<int,int>>::run();
Probe_T0254<std::unordered_map<int,int>>::run();
Probe_T0255<std::unordered_map<int,int>>::run();
Probe_T0256<std::unordered_map<int,int>>::run();
Probe_T0257<std::unordered_map<int,int>>::run();
Probe_T0258<std::unordered_map<int,int>>::run();
Probe_T0259<std::unordered_map<int,int>>::run();
Probe_T0260<std::unordered_map<int,int>>::run();
Probe_T0261<std::unordered_map<int,int>>::run();
Probe_T0262<std::unordered_multimap<int,int>>::run();
Probe_T0263<std::unordered_multimap<int,int>>::run();
Probe_T0264<std::unordered_multimap<int,int>>::run();
Probe_T0265<std::unordered_multimap<int,int>>::run();
Probe_T0266<std::unordered_multimap<int,int>>::run();
Probe_T0267<std::unordered_multimap<int,int>>::run();
Probe_T0268<std::unordered_multimap<int,int>>::run();
Probe_T0269<std::unordered_multimap<int,int>>::run();
Probe_T0270<std::unordered_multimap<int,int>>::run();
Probe_T0271<std::unordered_multimap<int,int>>::run();
Probe_T0272<std::unordered_multimap<int,int>>::run();
Probe_T0273<std::unordered_multimap<int,int>>::run();
Probe_T0274<std::unordered_set<int>>::run();
Probe_T0275<std::unordered_set<int>>::run();
Probe_T0276<std::unordered_set<int>>::run();
Probe_T0277<std::unordered_set<int>>::run();
Probe_T0278<std::unordered_set<int>>::run();
Probe_T0279<std::unordered_set<int>>::run();
Probe_T0280<std::unordered_set<int>>::run();
Probe_T0281<std::unordered_set<int>>::run();
Probe_T0282<std::unordered_set<int>>::run();
Probe_T0283<std::unordered_set<int>>::run();
Probe_T0284<std::unordered_set<int>>::run();
Probe_T0285<std::unordered_set<int>>::run();
Probe_T0286<std::unordered_multiset<int>>::run();
Probe_T0287<std::unordered_multiset<int>>::run();
Probe_T0288<std::unordered_multiset<int>>::run();
}
