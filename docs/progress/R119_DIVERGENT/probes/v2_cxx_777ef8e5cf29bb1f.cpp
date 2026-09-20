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
template<class C,class=void> struct Probe_T0529{static void run(){std::printf("T0529\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0529<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0529");}};

template<class C,class=void> struct Probe_T0530{static void run(){std::printf("T0530\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0530<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0530");}};

template<class C,class=void> struct Probe_T0531{static void run(){std::printf("T0531\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0531<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0531");}};

template<class C,class=void> struct Probe_T0532{static void run(){std::printf("T0532\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0532<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0532");}};

template<class C,class=void> struct Probe_T0533{static void run(){std::printf("T0533\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0533<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0533");}};

template<class C,class=void> struct Probe_T0534{static void run(){std::printf("T0534\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0534<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0534");}};

template<class C,class=void> struct Probe_T0535{static void run(){std::printf("T0535\tUNAVAILABLE=member flag_type\n");}};
template<class C> struct Probe_T0535<C,std::void_t<typename C::flag_type>>{static void run(){emit<typename C::flag_type>("T0535");}};

template<class C,class=void> struct Probe_T0536{static void run(){std::printf("T0536\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0536<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0536");}};

template<class C,class=void> struct Probe_T0537{static void run(){std::printf("T0537\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0537<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0537");}};

template<class C,class=void> struct Probe_T0538{static void run(){std::printf("T0538\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0538<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0538");}};

template<class C,class=void> struct Probe_T0539{static void run(){std::printf("T0539\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0539<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0539");}};

template<class C,class=void> struct Probe_T0540{static void run(){std::printf("T0540\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0540<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0540");}};

template<class C,class=void> struct Probe_T0541{static void run(){std::printf("T0541\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0541<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0541");}};

template<class C,class=void> struct Probe_T0542{static void run(){std::printf("T0542\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0542<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0542");}};

template<class C,class=void> struct Probe_T0543{static void run(){std::printf("T0543\tUNAVAILABLE=member flag_type\n");}};
template<class C> struct Probe_T0543<C,std::void_t<typename C::flag_type>>{static void run(){emit<typename C::flag_type>("T0543");}};

template<class C,class=void> struct Probe_T0544{static void run(){std::printf("T0544\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0544<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0544");}};

template<class C,class=void> struct Probe_T0545{static void run(){std::printf("T0545\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0545<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0545");}};

template<class C,class=void> struct Probe_T0546{static void run(){std::printf("T0546\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0546<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0546");}};

template<class C,class=void> struct Probe_T0547{static void run(){std::printf("T0547\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0547<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0547");}};

template<class C,class=void> struct Probe_T0548{static void run(){std::printf("T0548\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0548<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0548");}};

template<class C,class=void> struct Probe_T0549{static void run(){std::printf("T0549\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0549<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0549");}};

template<class C,class=void> struct Probe_T0550{static void run(){std::printf("T0550\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0550<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0550");}};

template<class C,class=void> struct Probe_T0551{static void run(){std::printf("T0551\tUNAVAILABLE=member flag_type\n");}};
template<class C> struct Probe_T0551<C,std::void_t<typename C::flag_type>>{static void run(){emit<typename C::flag_type>("T0551");}};

template<class C,class=void> struct Probe_T0552{static void run(){std::printf("T0552\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0552<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0552");}};

template<class C,class=void> struct Probe_T0553{static void run(){std::printf("T0553\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0553<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0553");}};

template<class C,class=void> struct Probe_T0554{static void run(){std::printf("T0554\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0554<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0554");}};

template<class C,class=void> struct Probe_T0555{static void run(){std::printf("T0555\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0555<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0555");}};

template<class C,class=void> struct Probe_T0556{static void run(){std::printf("T0556\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0556<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0556");}};

template<class C,class=void> struct Probe_T0557{static void run(){std::printf("T0557\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0557<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0557");}};

template<class C,class=void> struct Probe_T0558{static void run(){std::printf("T0558\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0558<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0558");}};

template<class C,class=void> struct Probe_T0559{static void run(){std::printf("T0559\tUNAVAILABLE=member flag_type\n");}};
template<class C> struct Probe_T0559<C,std::void_t<typename C::flag_type>>{static void run(){emit<typename C::flag_type>("T0559");}};

template<class C,class=void> struct Probe_T0560{static void run(){std::printf("T0560\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0560<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0560");}};

template<class C,class=void> struct Probe_T0561{static void run(){std::printf("T0561\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0561<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0561");}};

template<class C,class=void> struct Probe_T0562{static void run(){std::printf("T0562\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0562<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0562");}};

template<class C,class=void> struct Probe_T0563{static void run(){std::printf("T0563\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0563<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0563");}};

template<class C,class=void> struct Probe_T0564{static void run(){std::printf("T0564\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0564<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0564");}};

template<class C,class=void> struct Probe_T0565{static void run(){std::printf("T0565\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0565<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0565");}};

template<class C,class=void> struct Probe_T0566{static void run(){std::printf("T0566\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0566<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0566");}};

template<class C,class=void> struct Probe_T0567{static void run(){std::printf("T0567\tUNAVAILABLE=member flag_type\n");}};
template<class C> struct Probe_T0567<C,std::void_t<typename C::flag_type>>{static void run(){emit<typename C::flag_type>("T0567");}};

template<class C,class=void> struct Probe_T0568{static void run(){std::printf("T0568\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0568<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0568");}};

template<class C,class=void> struct Probe_T0569{static void run(){std::printf("T0569\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0569<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0569");}};

template<class C,class=void> struct Probe_T0570{static void run(){std::printf("T0570\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0570<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0570");}};

template<class C,class=void> struct Probe_T0571{static void run(){std::printf("T0571\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0571<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0571");}};

template<class C,class=void> struct Probe_T0572{static void run(){std::printf("T0572\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0572<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0572");}};

template<class C,class=void> struct Probe_T0573{static void run(){std::printf("T0573\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0573<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0573");}};

template<class C,class=void> struct Probe_T0574{static void run(){std::printf("T0574\tUNAVAILABLE=member char_type\n");}};
template<class C> struct Probe_T0574<C,std::void_t<typename C::char_type>>{static void run(){emit<typename C::char_type>("T0574");}};

template<class C,class=void> struct Probe_T0575{static void run(){std::printf("T0575\tUNAVAILABLE=member flag_type\n");}};
template<class C> struct Probe_T0575<C,std::void_t<typename C::flag_type>>{static void run(){emit<typename C::flag_type>("T0575");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0529<std::regex>::run();
Probe_T0530<std::regex>::run();
Probe_T0531<std::regex>::run();
Probe_T0532<std::regex>::run();
Probe_T0533<std::regex>::run();
Probe_T0534<std::regex>::run();
Probe_T0535<std::regex>::run();
Probe_T0536<std::wregex>::run();
Probe_T0537<std::wregex>::run();
Probe_T0538<std::wregex>::run();
Probe_T0539<std::wregex>::run();
Probe_T0540<std::wregex>::run();
Probe_T0541<std::wregex>::run();
Probe_T0542<std::wregex>::run();
Probe_T0543<std::wregex>::run();
Probe_T0544<std::cmatch>::run();
Probe_T0545<std::cmatch>::run();
Probe_T0546<std::cmatch>::run();
Probe_T0547<std::cmatch>::run();
Probe_T0548<std::cmatch>::run();
Probe_T0549<std::cmatch>::run();
Probe_T0550<std::cmatch>::run();
Probe_T0551<std::cmatch>::run();
Probe_T0552<std::smatch>::run();
Probe_T0553<std::smatch>::run();
Probe_T0554<std::smatch>::run();
Probe_T0555<std::smatch>::run();
Probe_T0556<std::smatch>::run();
Probe_T0557<std::smatch>::run();
Probe_T0558<std::smatch>::run();
Probe_T0559<std::smatch>::run();
Probe_T0560<std::csub_match>::run();
Probe_T0561<std::csub_match>::run();
Probe_T0562<std::csub_match>::run();
Probe_T0563<std::csub_match>::run();
Probe_T0564<std::csub_match>::run();
Probe_T0565<std::csub_match>::run();
Probe_T0566<std::csub_match>::run();
Probe_T0567<std::csub_match>::run();
Probe_T0568<std::ssub_match>::run();
Probe_T0569<std::ssub_match>::run();
Probe_T0570<std::ssub_match>::run();
Probe_T0571<std::ssub_match>::run();
Probe_T0572<std::ssub_match>::run();
Probe_T0573<std::ssub_match>::run();
Probe_T0574<std::ssub_match>::run();
Probe_T0575<std::ssub_match>::run();
emit<std::exception_ptr>("T0576");
}
