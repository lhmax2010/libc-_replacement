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
 emit<// NOLINT
                              int>("ee1f301ecd1f4bb404e8a3ae");
 emit<// NOLINT
                                    int>("015125270165dc59ef2366b3");
}
