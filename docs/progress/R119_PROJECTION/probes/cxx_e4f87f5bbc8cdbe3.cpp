#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <iterator>
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <array>
#include <utility>
#include <chrono>
#include <ratio>
#include <complex>
struct EmptyControl {};
template<class T> const char* actual() { return __PRETTY_FUNCTION__; }
template<class T> struct base {using type=typename std::remove_cv<T>::type;};
template<class T> struct base<T*> :base<T>{};
template<class T> struct base<T&> :base<T>{};
template<class T> struct base<T&&> :base<T>{};
template<class T, std::size_t N> struct base<T[N]> :base<T>{};
template<class T> void emit(const char* id) {
 using B=typename base<T>::type;
 std::printf("%s\tTYPE=%s\tBASE=%s\t",id,actual<T>(),actual<B>());
 if constexpr(std::is_void<B>::value) std::printf("kind=void\tsize=NA\tempty=NA\n");
 else if constexpr(std::is_function<B>::value) std::printf("kind=function\tsize=NA\tempty=NA\n");
 else std::printf("kind=%s\tsize=%zu\tempty=%d\tclass=%d\tunion=%d\tenum=%d\n",
 std::is_arithmetic<B>::value||std::is_null_pointer<B>::value?"builtin":"other",sizeof(B),int(std::is_empty<B>::value),int(std::is_class<B>::value),int(std::is_union<B>::value),int(std::is_enum<B>::value));
}
int main(){
 // 正向/负向测法对照，不来自候选，不计入真实投影结果。
 static_assert(std::is_empty<EmptyControl>::value,"control empty");
 static_assert(!std::is_empty<std::string>::value,"control object");
 emit<void>("CONTROL_VOID");emit<int>("CONTROL_BUILTIN");emit<EmptyControl>("CONTROL_EMPTY");emit<std::string>("CONTROL_OBJECT");
 emit<void>("445be54d48a2e6294369c84c");
 emit<int>("6da88c34ba124c41f977db66");
 emit<bool>("b760f44fa5965c2474a3b471");
 emit<char*>("ef1b6fc9219b08873b43bddf");
 emit<std::size_t>("8744d54be68a82f12298aac6");
 emit<void*>("3e430d23a71b88b46f56fc32");
 emit<long>("fc66f021c67d064c1490a12b");
 emit<long long>("61bc86149eb27c2b04e40a4e");
 emit<unsigned>("ceffe727ab2fa2c7c3322ee4");
 emit<void *>("bbccd59c2eb9b737fa8dc524");
 emit<std::string>("71fc1fac5cd5e6f90a49fc00");
 emit<double>("2ce06a9e7f8c3e6bf85a6067");
 emit<std::ostream &>("62ef9f4c47e26107233cd707");
 emit<std::nullptr_t>("8dac8f174340550b3f29ea49");
 emit<int*>("6844ce0b3b32f5d602732f68");
 emit<char>("411b0496da5eeced5314a3b5");
}
