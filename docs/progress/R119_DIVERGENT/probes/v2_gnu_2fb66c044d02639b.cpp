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
template<class C,class=void> struct Probe_T0386{static void run(){std::printf("T0386\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0386<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0386");}};

template<class C,class=void> struct Probe_T0388{static void run(){std::printf("T0388\tUNAVAILABLE=member rep\n");}};
template<class C> struct Probe_T0388<C,std::void_t<typename C::rep>>{static void run(){emit<typename C::rep>("T0388");}};

template<class C,class=void> struct Probe_T0408{static void run(){std::printf("T0408\tUNAVAILABLE=member result_type\n");}};
template<class C> struct Probe_T0408<C,std::void_t<typename C::result_type>>{static void run(){emit<typename C::result_type>("T0408");}};

template<class C,class=void> struct Probe_T0409{static void run(){std::printf("T0409\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0409<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0409");}};

template<class C,class=void> struct Probe_T0410{static void run(){std::printf("T0410\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0410<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0410");}};

template<class C,class=void> struct Probe_T0411{static void run(){std::printf("T0411\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0411<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0411");}};

template<class C,class=void> struct Probe_T0412{static void run(){std::printf("T0412\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0412<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0412");}};

template<class C,class=void> struct Probe_T0413{static void run(){std::printf("T0413\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0413<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0413");}};

template<class C,class=void> struct Probe_T0414{static void run(){std::printf("T0414\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0414<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0414");}};

template<class C,class=void> struct Probe_T0415{static void run(){std::printf("T0415\tUNAVAILABLE=member native_handle_type\n");}};
template<class C> struct Probe_T0415<C,std::void_t<typename C::native_handle_type>>{static void run(){emit<typename C::native_handle_type>("T0415");}};

template<class C,class=void> struct Probe_T0416{static void run(){std::printf("T0416\tUNAVAILABLE=member id\n");}};
template<class C> struct Probe_T0416<C,std::void_t<typename C::id>>{static void run(){emit<typename C::id>("T0416");}};

template<class C,class=void> struct Probe_T0417{static void run(){std::printf("T0417\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0417<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0417");}};

template<class C,class=void> struct Probe_T0418{static void run(){std::printf("T0418\tUNAVAILABLE=member string_type\n");}};
template<class C> struct Probe_T0418<C,std::void_t<typename C::string_type>>{static void run(){emit<typename C::string_type>("T0418");}};

template<class C,class=void> struct Probe_T0419{static void run(){std::printf("T0419\tUNAVAILABLE=member iterator\n");}};
template<class C> struct Probe_T0419<C,std::void_t<typename C::iterator>>{static void run(){emit<typename C::iterator>("T0419");}};

template<class C,class=void> struct Probe_T0420{static void run(){std::printf("T0420\tUNAVAILABLE=member const_iterator\n");}};
template<class C> struct Probe_T0420<C,std::void_t<typename C::const_iterator>>{static void run(){emit<typename C::const_iterator>("T0420");}};

template<class C,class=void> struct Probe_T0432{static void run(){std::printf("T0432\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0432<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0432");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
emit<std::chrono::minutes>("T0385");
Probe_T0386<std::chrono::minutes>::run();
emit<std::chrono::hours>("T0387");
Probe_T0388<std::chrono::hours>::run();
emit<std::minstd_rand0>("T0389");
emit<std::minstd_rand0::result_type>("T0390");
emit<std::minstd_rand>("T0391");
emit<std::minstd_rand::result_type>("T0392");
emit<std::mt19937>("T0393");
emit<std::mt19937::result_type>("T0394");
emit<std::mt19937_64::result_type>("T0395");
emit<std::ranlux24_base>("T0396");
emit<std::ranlux24_base::result_type>("T0397");
emit<std::ranlux48_base>("T0398");
emit<std::ranlux48_base::result_type>("T0399");
emit<std::ranlux24>("T0400");
emit<std::ranlux24::result_type>("T0401");
emit<std::ranlux48>("T0402");
emit<std::ranlux48::result_type>("T0403");
emit<std::knuth_b>("T0404");
emit<std::knuth_b::result_type>("T0405");
emit<std::default_random_engine>("T0406");
emit<std::default_random_engine::result_type>("T0407");
Probe_T0408<std::random_device>::run();
Probe_T0409<std::thread>::run();
Probe_T0410<std::mutex>::run();
Probe_T0411<std::recursive_mutex>::run();
Probe_T0412<std::timed_mutex>::run();
Probe_T0413<std::recursive_timed_mutex>::run();
Probe_T0414<std::shared_mutex>::run();
Probe_T0415<std::condition_variable>::run();
Probe_T0416<std::thread>::run();
Probe_T0417<std::filesystem::path>::run();
Probe_T0418<std::filesystem::path>::run();
Probe_T0419<std::filesystem::path>::run();
Probe_T0420<std::filesystem::path>::run();
emit<std::filesystem::file_time_type>("T0421");
emit<typename std::decay<decltype(std::placeholders::_1)>::type>("T0422");
emit<typename std::decay<decltype(std::placeholders::_2)>::type>("T0423");
emit<typename std::decay<decltype(std::placeholders::_3)>::type>("T0424");
emit<typename std::decay<decltype(std::placeholders::_4)>::type>("T0425");
emit<typename std::decay<decltype(std::placeholders::_5)>::type>("T0426");
emit<typename std::decay<decltype(std::placeholders::_6)>::type>("T0427");
emit<typename std::decay<decltype(std::placeholders::_7)>::type>("T0428");
emit<typename std::decay<decltype(std::placeholders::_8)>::type>("T0429");
emit<typename std::decay<decltype(std::placeholders::_9)>::type>("T0430");
emit<typename std::decay<decltype(std::placeholders::_10)>::type>("T0431");
Probe_T0432<std::unique_ptr<int>>::run();
}
