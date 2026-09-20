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
template<class C,class=void> struct Probe_T0337{static void run(){std::printf("T0337\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0337<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0337");}};

template<class C,class=void> struct Probe_T0338{static void run(){std::printf("T0338\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0338<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0338");}};

template<class C,class=void> struct Probe_T0339{static void run(){std::printf("T0339\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0339<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0339");}};

template<class C,class=void> struct Probe_T0340{static void run(){std::printf("T0340\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0340<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0340");}};

template<class C,class=void> struct Probe_T0341{static void run(){std::printf("T0341\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0341<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0341");}};

template<class C,class=void> struct Probe_T0342{static void run(){std::printf("T0342\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0342<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0342");}};

template<class C,class=void> struct Probe_T0343{static void run(){std::printf("T0343\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0343<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0343");}};

template<class C,class=void> struct Probe_T0344{static void run(){std::printf("T0344\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0344<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0344");}};

template<class C,class=void> struct Probe_T0345{static void run(){std::printf("T0345\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0345<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0345");}};

template<class C,class=void> struct Probe_T0346{static void run(){std::printf("T0346\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0346<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0346");}};

template<class C,class=void> struct Probe_T0347{static void run(){std::printf("T0347\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0347<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0347");}};

template<class C,class=void> struct Probe_T0348{static void run(){std::printf("T0348\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0348<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0348");}};

template<class C,class=void> struct Probe_T0349{static void run(){std::printf("T0349\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0349<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0349");}};

template<class C,class=void> struct Probe_T0350{static void run(){std::printf("T0350\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0350<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0350");}};

template<class C,class=void> struct Probe_T0351{static void run(){std::printf("T0351\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0351<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0351");}};

template<class C,class=void> struct Probe_T0352{static void run(){std::printf("T0352\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0352<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0352");}};

template<class C,class=void> struct Probe_T0353{static void run(){std::printf("T0353\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0353<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0353");}};

template<class C,class=void> struct Probe_T0354{static void run(){std::printf("T0354\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0354<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0354");}};

template<class C,class=void> struct Probe_T0355{static void run(){std::printf("T0355\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0355<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0355");}};

template<class C,class=void> struct Probe_T0356{static void run(){std::printf("T0356\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0356<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0356");}};

template<class C,class=void> struct Probe_T0357{static void run(){std::printf("T0357\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0357<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0357");}};

template<class C,class=void> struct Probe_T0358{static void run(){std::printf("T0358\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0358<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0358");}};

template<class C,class=void> struct Probe_T0359{static void run(){std::printf("T0359\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0359<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0359");}};

template<class C,class=void> struct Probe_T0360{static void run(){std::printf("T0360\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0360<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0360");}};

template<class C,class=void> struct Probe_T0361{static void run(){std::printf("T0361\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0361<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0361");}};

template<class C,class=void> struct Probe_T0362{static void run(){std::printf("T0362\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0362<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0362");}};

template<class C,class=void> struct Probe_T0363{static void run(){std::printf("T0363\tUNAVAILABLE=member reference\n");}};
template<class C> struct Probe_T0363<C,std::void_t<typename C::reference>>{static void run(){emit<typename C::reference>("T0363");}};

template<class C,class=void> struct Probe_T0364{static void run(){std::printf("T0364\tUNAVAILABLE=member iterator_category\n");}};
template<class C> struct Probe_T0364<C,std::void_t<typename C::iterator_category>>{static void run(){emit<typename C::iterator_category>("T0364");}};

template<class C,class=void> struct Probe_T0365{static void run(){std::printf("T0365\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0365<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0365");}};

template<class C,class=void> struct Probe_T0366{static void run(){std::printf("T0366\tUNAVAILABLE=member period\n");}};
template<class C> struct Probe_T0366<C,std::void_t<typename C::period>>{static void run(){emit<typename C::period>("T0366");}};

template<class C,class=void> struct Probe_T0367{static void run(){std::printf("T0367\tUNAVAILABLE=member duration\n");}};
template<class C> struct Probe_T0367<C,std::void_t<typename C::duration>>{static void run(){emit<typename C::duration>("T0367");}};

template<class C,class=void> struct Probe_T0368{static void run(){std::printf("T0368\tUNAVAILABLE=member time_point\n");}};
template<class C> struct Probe_T0368<C,std::void_t<typename C::time_point>>{static void run(){emit<typename C::time_point>("T0368");}};

template<class C,class=void> struct Probe_T0369{static void run(){std::printf("T0369\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0369<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0369");}};

template<class C,class=void> struct Probe_T0370{static void run(){std::printf("T0370\tUNAVAILABLE=member period\n");}};
template<class C> struct Probe_T0370<C,std::void_t<typename C::period>>{static void run(){emit<typename C::period>("T0370");}};

template<class C,class=void> struct Probe_T0371{static void run(){std::printf("T0371\tUNAVAILABLE=member duration\n");}};
template<class C> struct Probe_T0371<C,std::void_t<typename C::duration>>{static void run(){emit<typename C::duration>("T0371");}};

template<class C,class=void> struct Probe_T0372{static void run(){std::printf("T0372\tUNAVAILABLE=member time_point\n");}};
template<class C> struct Probe_T0372<C,std::void_t<typename C::time_point>>{static void run(){emit<typename C::time_point>("T0372");}};

template<class C,class=void> struct Probe_T0373{static void run(){std::printf("T0373\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0373<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0373");}};

template<class C,class=void> struct Probe_T0374{static void run(){std::printf("T0374\tUNAVAILABLE=member period\n");}};
template<class C> struct Probe_T0374<C,std::void_t<typename C::period>>{static void run(){emit<typename C::period>("T0374");}};

template<class C,class=void> struct Probe_T0375{static void run(){std::printf("T0375\tUNAVAILABLE=member duration\n");}};
template<class C> struct Probe_T0375<C,std::void_t<typename C::duration>>{static void run(){emit<typename C::duration>("T0375");}};

template<class C,class=void> struct Probe_T0376{static void run(){std::printf("T0376\tUNAVAILABLE=member time_point\n");}};
template<class C> struct Probe_T0376<C,std::void_t<typename C::time_point>>{static void run(){emit<typename C::time_point>("T0376");}};

template<class C,class=void> struct Probe_T0378{static void run(){std::printf("T0378\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0378<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0378");}};

template<class C,class=void> struct Probe_T0380{static void run(){std::printf("T0380\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0380<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0380");}};

template<class C,class=void> struct Probe_T0382{static void run(){std::printf("T0382\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0382<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0382");}};

template<class C,class=void> struct Probe_T0384{static void run(){std::printf("T0384\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0384<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0384");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0337<std::front_insert_iterator<std::list<int>>>::run();
Probe_T0338<std::front_insert_iterator<std::list<int>>>::run();
Probe_T0339<std::front_insert_iterator<std::list<int>>>::run();
Probe_T0340<std::insert_iterator<std::vector<int>>>::run();
Probe_T0341<std::insert_iterator<std::vector<int>>>::run();
Probe_T0342<std::insert_iterator<std::vector<int>>>::run();
Probe_T0343<std::insert_iterator<std::vector<int>>>::run();
Probe_T0344<std::insert_iterator<std::vector<int>>>::run();
Probe_T0345<std::istream_iterator<int>>::run();
Probe_T0346<std::istream_iterator<int>>::run();
Probe_T0347<std::istream_iterator<int>>::run();
Probe_T0348<std::istream_iterator<int>>::run();
Probe_T0349<std::istream_iterator<int>>::run();
Probe_T0350<std::ostream_iterator<int>>::run();
Probe_T0351<std::ostream_iterator<int>>::run();
Probe_T0352<std::ostream_iterator<int>>::run();
Probe_T0353<std::ostream_iterator<int>>::run();
Probe_T0354<std::ostream_iterator<int>>::run();
Probe_T0355<std::istreambuf_iterator<char>>::run();
Probe_T0356<std::istreambuf_iterator<char>>::run();
Probe_T0357<std::istreambuf_iterator<char>>::run();
Probe_T0358<std::istreambuf_iterator<char>>::run();
Probe_T0359<std::istreambuf_iterator<char>>::run();
Probe_T0360<std::ostreambuf_iterator<char>>::run();
Probe_T0361<std::ostreambuf_iterator<char>>::run();
Probe_T0362<std::ostreambuf_iterator<char>>::run();
Probe_T0363<std::ostreambuf_iterator<char>>::run();
Probe_T0364<std::ostreambuf_iterator<char>>::run();
Probe_T0365<std::chrono::system_clock>::run();
Probe_T0366<std::chrono::system_clock>::run();
Probe_T0367<std::chrono::system_clock>::run();
Probe_T0368<std::chrono::system_clock>::run();
Probe_T0369<std::chrono::steady_clock>::run();
Probe_T0370<std::chrono::steady_clock>::run();
Probe_T0371<std::chrono::steady_clock>::run();
Probe_T0372<std::chrono::steady_clock>::run();
Probe_T0373<std::chrono::high_resolution_clock>::run();
Probe_T0374<std::chrono::high_resolution_clock>::run();
Probe_T0375<std::chrono::high_resolution_clock>::run();
Probe_T0376<std::chrono::high_resolution_clock>::run();
emit<std::chrono::nanoseconds>("T0377");
Probe_T0378<std::chrono::nanoseconds>::run();
emit<std::chrono::microseconds>("T0379");
Probe_T0380<std::chrono::microseconds>::run();
emit<std::chrono::milliseconds>("T0381");
Probe_T0382<std::chrono::milliseconds>::run();
emit<std::chrono::seconds>("T0383");
Probe_T0384<std::chrono::seconds>::run();
}
