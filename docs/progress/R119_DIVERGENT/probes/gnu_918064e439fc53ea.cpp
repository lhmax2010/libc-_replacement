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
template<class C,class=void> struct Probe_T0433{static void run(){std::printf("T0433\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0433<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0433");}};

template<class C,class=void> struct Probe_T0434{static void run(){std::printf("T0434\tUNAVAILABLE=member void_pointer\n");}};
template<class C> struct Probe_T0434<C,std::void_t<typename C::void_pointer>>{static void run(){emit<typename C::void_pointer>("T0434");}};

template<class C,class=void> struct Probe_T0435{static void run(){std::printf("T0435\tUNAVAILABLE=member const_void_pointer\n");}};
template<class C> struct Probe_T0435<C,std::void_t<typename C::const_void_pointer>>{static void run(){emit<typename C::const_void_pointer>("T0435");}};

template<class C,class=void> struct Probe_T0436{static void run(){std::printf("T0436\tUNAVAILABLE=member element_type\n");}};
template<class C> struct Probe_T0436<C,std::void_t<typename C::element_type>>{static void run(){emit<typename C::element_type>("T0436");}};

template<class C,class=void> struct Probe_T0437{static void run(){std::printf("T0437\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0437<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0437");}};

template<class C,class=void> struct Probe_T0438{static void run(){std::printf("T0438\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0438<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0438");}};

template<class C,class=void> struct Probe_T0439{static void run(){std::printf("T0439\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0439<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0439");}};

template<class C,class=void> struct Probe_T0440{static void run(){std::printf("T0440\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0440<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0440");}};

template<class C,class=void> struct Probe_T0441{static void run(){std::printf("T0441\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0441<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0441");}};

template<class C,class=void> struct Probe_T0442{static void run(){std::printf("T0442\tUNAVAILABLE=member void_pointer\n");}};
template<class C> struct Probe_T0442<C,std::void_t<typename C::void_pointer>>{static void run(){emit<typename C::void_pointer>("T0442");}};

template<class C,class=void> struct Probe_T0443{static void run(){std::printf("T0443\tUNAVAILABLE=member const_void_pointer\n");}};
template<class C> struct Probe_T0443<C,std::void_t<typename C::const_void_pointer>>{static void run(){emit<typename C::const_void_pointer>("T0443");}};

template<class C,class=void> struct Probe_T0444{static void run(){std::printf("T0444\tUNAVAILABLE=member element_type\n");}};
template<class C> struct Probe_T0444<C,std::void_t<typename C::element_type>>{static void run(){emit<typename C::element_type>("T0444");}};

template<class C,class=void> struct Probe_T0445{static void run(){std::printf("T0445\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0445<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0445");}};

template<class C,class=void> struct Probe_T0446{static void run(){std::printf("T0446\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0446<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0446");}};

template<class C,class=void> struct Probe_T0447{static void run(){std::printf("T0447\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0447<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0447");}};

template<class C,class=void> struct Probe_T0448{static void run(){std::printf("T0448\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0448<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0448");}};

template<class C,class=void> struct Probe_T0449{static void run(){std::printf("T0449\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0449<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0449");}};

template<class C,class=void> struct Probe_T0450{static void run(){std::printf("T0450\tUNAVAILABLE=member void_pointer\n");}};
template<class C> struct Probe_T0450<C,std::void_t<typename C::void_pointer>>{static void run(){emit<typename C::void_pointer>("T0450");}};

template<class C,class=void> struct Probe_T0451{static void run(){std::printf("T0451\tUNAVAILABLE=member const_void_pointer\n");}};
template<class C> struct Probe_T0451<C,std::void_t<typename C::const_void_pointer>>{static void run(){emit<typename C::const_void_pointer>("T0451");}};

template<class C,class=void> struct Probe_T0452{static void run(){std::printf("T0452\tUNAVAILABLE=member element_type\n");}};
template<class C> struct Probe_T0452<C,std::void_t<typename C::element_type>>{static void run(){emit<typename C::element_type>("T0452");}};

template<class C,class=void> struct Probe_T0453{static void run(){std::printf("T0453\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0453<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0453");}};

template<class C,class=void> struct Probe_T0454{static void run(){std::printf("T0454\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0454<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0454");}};

template<class C,class=void> struct Probe_T0455{static void run(){std::printf("T0455\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0455<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0455");}};

template<class C,class=void> struct Probe_T0456{static void run(){std::printf("T0456\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0456<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0456");}};

template<class C,class=void> struct Probe_T0457{static void run(){std::printf("T0457\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0457<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0457");}};

template<class C,class=void> struct Probe_T0458{static void run(){std::printf("T0458\tUNAVAILABLE=member void_pointer\n");}};
template<class C> struct Probe_T0458<C,std::void_t<typename C::void_pointer>>{static void run(){emit<typename C::void_pointer>("T0458");}};

template<class C,class=void> struct Probe_T0459{static void run(){std::printf("T0459\tUNAVAILABLE=member const_void_pointer\n");}};
template<class C> struct Probe_T0459<C,std::void_t<typename C::const_void_pointer>>{static void run(){emit<typename C::const_void_pointer>("T0459");}};

template<class C,class=void> struct Probe_T0460{static void run(){std::printf("T0460\tUNAVAILABLE=member element_type\n");}};
template<class C> struct Probe_T0460<C,std::void_t<typename C::element_type>>{static void run(){emit<typename C::element_type>("T0460");}};

template<class C,class=void> struct Probe_T0461{static void run(){std::printf("T0461\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0461<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0461");}};

template<class C,class=void> struct Probe_T0462{static void run(){std::printf("T0462\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0462<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0462");}};

template<class C,class=void> struct Probe_T0463{static void run(){std::printf("T0463\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0463<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0463");}};

template<class C,class=void> struct Probe_T0464{static void run(){std::printf("T0464\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0464<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0464");}};

template<class C,class=void> struct Probe_T0465{static void run(){std::printf("T0465\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0465<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0465");}};

template<class C,class=void> struct Probe_T0466{static void run(){std::printf("T0466\tUNAVAILABLE=member void_pointer\n");}};
template<class C> struct Probe_T0466<C,std::void_t<typename C::void_pointer>>{static void run(){emit<typename C::void_pointer>("T0466");}};

template<class C,class=void> struct Probe_T0467{static void run(){std::printf("T0467\tUNAVAILABLE=member const_void_pointer\n");}};
template<class C> struct Probe_T0467<C,std::void_t<typename C::const_void_pointer>>{static void run(){emit<typename C::const_void_pointer>("T0467");}};

template<class C,class=void> struct Probe_T0468{static void run(){std::printf("T0468\tUNAVAILABLE=member element_type\n");}};
template<class C> struct Probe_T0468<C,std::void_t<typename C::element_type>>{static void run(){emit<typename C::element_type>("T0468");}};

template<class C,class=void> struct Probe_T0469{static void run(){std::printf("T0469\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0469<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0469");}};

template<class C,class=void> struct Probe_T0470{static void run(){std::printf("T0470\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0470<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0470");}};

template<class C,class=void> struct Probe_T0471{static void run(){std::printf("T0471\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0471<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0471");}};

template<class C,class=void> struct Probe_T0472{static void run(){std::printf("T0472\tUNAVAILABLE=member pointer\n");}};
template<class C> struct Probe_T0472<C,std::void_t<typename C::pointer>>{static void run(){emit<typename C::pointer>("T0472");}};

template<class C,class=void> struct Probe_T0473{static void run(){std::printf("T0473\tUNAVAILABLE=member const_pointer\n");}};
template<class C> struct Probe_T0473<C,std::void_t<typename C::const_pointer>>{static void run(){emit<typename C::const_pointer>("T0473");}};

template<class C,class=void> struct Probe_T0474{static void run(){std::printf("T0474\tUNAVAILABLE=member void_pointer\n");}};
template<class C> struct Probe_T0474<C,std::void_t<typename C::void_pointer>>{static void run(){emit<typename C::void_pointer>("T0474");}};

template<class C,class=void> struct Probe_T0475{static void run(){std::printf("T0475\tUNAVAILABLE=member const_void_pointer\n");}};
template<class C> struct Probe_T0475<C,std::void_t<typename C::const_void_pointer>>{static void run(){emit<typename C::const_void_pointer>("T0475");}};

template<class C,class=void> struct Probe_T0476{static void run(){std::printf("T0476\tUNAVAILABLE=member element_type\n");}};
template<class C> struct Probe_T0476<C,std::void_t<typename C::element_type>>{static void run(){emit<typename C::element_type>("T0476");}};

template<class C,class=void> struct Probe_T0477{static void run(){std::printf("T0477\tUNAVAILABLE=member value_type\n");}};
template<class C> struct Probe_T0477<C,std::void_t<typename C::value_type>>{static void run(){emit<typename C::value_type>("T0477");}};

template<class C,class=void> struct Probe_T0478{static void run(){std::printf("T0478\tUNAVAILABLE=member size_type\n");}};
template<class C> struct Probe_T0478<C,std::void_t<typename C::size_type>>{static void run(){emit<typename C::size_type>("T0478");}};

template<class C,class=void> struct Probe_T0479{static void run(){std::printf("T0479\tUNAVAILABLE=member difference_type\n");}};
template<class C> struct Probe_T0479<C,std::void_t<typename C::difference_type>>{static void run(){emit<typename C::difference_type>("T0479");}};

int main(){emit<int>("CONTROL_INT");emit<long>("CONTROL_LONG");emit<long long>("CONTROL_LONG_LONG");emit<const long&>("CONTROL_CONST_REF");
Probe_T0433<std::unique_ptr<int>>::run();
Probe_T0434<std::unique_ptr<int>>::run();
Probe_T0435<std::unique_ptr<int>>::run();
Probe_T0436<std::unique_ptr<int>>::run();
Probe_T0437<std::unique_ptr<int>>::run();
Probe_T0438<std::unique_ptr<int>>::run();
Probe_T0439<std::unique_ptr<int>>::run();
Probe_T0440<std::unique_ptr<int[]>>::run();
Probe_T0441<std::unique_ptr<int[]>>::run();
Probe_T0442<std::unique_ptr<int[]>>::run();
Probe_T0443<std::unique_ptr<int[]>>::run();
Probe_T0444<std::unique_ptr<int[]>>::run();
Probe_T0445<std::unique_ptr<int[]>>::run();
Probe_T0446<std::unique_ptr<int[]>>::run();
Probe_T0447<std::unique_ptr<int[]>>::run();
Probe_T0448<std::shared_ptr<int>>::run();
Probe_T0449<std::shared_ptr<int>>::run();
Probe_T0450<std::shared_ptr<int>>::run();
Probe_T0451<std::shared_ptr<int>>::run();
Probe_T0452<std::shared_ptr<int>>::run();
Probe_T0453<std::shared_ptr<int>>::run();
Probe_T0454<std::shared_ptr<int>>::run();
Probe_T0455<std::shared_ptr<int>>::run();
Probe_T0456<std::weak_ptr<int>>::run();
Probe_T0457<std::weak_ptr<int>>::run();
Probe_T0458<std::weak_ptr<int>>::run();
Probe_T0459<std::weak_ptr<int>>::run();
Probe_T0460<std::weak_ptr<int>>::run();
Probe_T0461<std::weak_ptr<int>>::run();
Probe_T0462<std::weak_ptr<int>>::run();
Probe_T0463<std::weak_ptr<int>>::run();
Probe_T0464<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0465<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0466<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0467<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0468<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0469<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0470<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0471<std::allocator_traits<std::allocator<int>>>::run();
Probe_T0472<std::pointer_traits<int*>>::run();
Probe_T0473<std::pointer_traits<int*>>::run();
Probe_T0474<std::pointer_traits<int*>>::run();
Probe_T0475<std::pointer_traits<int*>>::run();
Probe_T0476<std::pointer_traits<int*>>::run();
Probe_T0477<std::pointer_traits<int*>>::run();
Probe_T0478<std::pointer_traits<int*>>::run();
Probe_T0479<std::pointer_traits<int*>>::run();
emit<std::atomic_bool>("T0480");
}
