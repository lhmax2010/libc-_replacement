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
 static_assert(std::is_empty<std::true_type>::value,"control empty");
 static_assert(!std::is_empty<std::string>::value,"control object");
 emit<void>("CONTROL_VOID");emit<int>("CONTROL_BUILTIN");emit<std::true_type>("CONTROL_EMPTY");emit<std::string>("CONTROL_OBJECT");
 emit<decltype(std::numeric_limits<unsigned long>::digits10)>("a9df17932c61e95c2328a8b2");
 emit<decltype(std::numeric_limits<std::clock_t>::max())>("1a08a6bc2c76c694bd7f3b55");
 emit<decltype(std::numeric_limits<unsigned long>::min())>("f2dad05beed03824ec656018");
 emit<decltype(std::numeric_limits<wchar_t>::is_signed)>("d72a64d028ccf7424a08b834");
 emit<decltype(std::numeric_limits<std::int64_t>::max())>("91f66d168b242d6a51ce6cfe");
 emit<decltype(std::numeric_limits<float>::round_style)>("0b8a1bad0bc4416f0f178ebd");
 emit<decltype(std::numeric_limits<float>::radix)>("5f5b62cea149fad3c75ae13e");
 emit<decltype(std::numeric_limits<float>::min_exponent)>("13232cc4617796cba158f947");
 emit<decltype(std::numeric_limits<float>::min_exponent10)>("7b3455318b6dd2dfa25a24f9");
 emit<decltype(std::numeric_limits<float>::max_exponent)>("f748ece76ab074011a869b4b");
 emit<decltype(std::numeric_limits<float>::max_exponent10)>("bb010e8229e9638e964eb467");
 emit<decltype(std::numeric_limits<float>::traps)>("de1e6d2cb856a734ee81d8c4");
 emit<decltype(std::numeric_limits<float>::tinyness_before)>("2a60905d2533a7164c0147c6");
 emit<decltype(std::numeric_limits<std::ptrdiff_t>::min())>("2ed9a7581fae52fd67190605");
 emit<decltype(std::numeric_limits<std::uint8_t>::max())>("60a7d6bc78c13570a23c59b6");
 emit<decltype(std::numeric_limits<std::uint16_t>::max())>("6d261873879cbaa79c70c310");
 emit<decltype(std::numeric_limits<std::int8_t>::min())>("dca237f1232d4b58cb2a6555");
 emit<decltype(std::numeric_limits<std::int8_t>::max())>("8f4470ecc2eb688174d77280");
 emit<decltype(std::numeric_limits<std::int16_t>::min())>("b7b80eed2b0b3e1d9efe6220");
 emit<decltype(std::numeric_limits<std::int16_t>::max())>("5f412417edd960016c47a0b1");
 emit<decltype(std::numeric_limits<std::int32_t>::min())>("1d16d01fa0308722a43f6a60");
 emit<decltype(std::numeric_limits<std::int64_t>::min())>("59b7e494966d7b7ffb530b33");
 emit<decltype(std::numeric_limits<std::uint8_t>::min())>("06b83241807df63a8bfb6334");
 emit<decltype(std::numeric_limits<float>::quiet_NaN())>("18396ef4d4ebd5010cf72016");
 emit<decltype(std::numeric_limits<char>::is_signed)>("2ffb0062bde7df843b7fdaf0");
 emit<decltype(std::numeric_limits<int>::lowest())>("edb9de1f9595b94ee86dd849");
 emit<decltype(std::numeric_limits<double>::is_iec559)>("bf8934eb180770ccf7be22d4");
 emit<decltype(std::numeric_limits< double >::quiet_NaN())>("a9d3f012b0b7b87048d0112e");
 emit<decltype(std::numeric_limits<float>::epsilon())>("b56d3a0c96ea271b32a96301");
 emit<decltype(std::is_same<std::intmax_t, long>::value)>("a4f082d8c98ddae08d6381fb");
 emit<typename std::conditional<std::is_signed<char>::value,
                    signed long long, unsigned long long
                >::type>("9f4248bdb88ea94c42ee2984");
 emit<typename std::conditional<std::is_same<std::intmax_t, long>::value, long long, std::intmax_t>::type>("9f2a3a6e0e6ae170675e3ad7");
 emit<typename std::conditional<(std::numeric_limits<long double>::digits == 64), long double, double>::type>("4f5909341377820b0cd412c3");
 emit<typename std::conditional<sizeof(unsigned long) == sizeof(unsigned int),
                                unsigned int, unsigned long long>::type>("2c0f032c8fc41db35bd98bce");
 emit<typename std::decay<decltype(std::placeholders::_1)>::type>("bab3973381b01da4166f0f84");
 emit<typename std::decay<decltype(std::placeholders::_2)>::type>("33676f637be2460f1f4a7b3f");
 emit<typename std::decay<decltype(std::placeholders::_3)>::type>("f563e476a3ef23907544dbe9");
 emit<typename std::decay<decltype(std::placeholders::_4)>::type>("662f55bc0a199544aa3a8d31");
 emit<typename std::decay<decltype(std::placeholders::_5)>::type>("80125c4c785eea907a570893");
 emit<typename std::decay<decltype(std::placeholders::_6)>::type>("de609906e031c453356eb821");
 emit<typename std::decay<decltype(std::placeholders::_7)>::type>("27ec07ef2038b0c25ea8e6e0");
 emit<typename std::decay<decltype(std::placeholders::_8)>::type>("87aa91e55e4b5691f60809cf");
 emit<typename std::decay<decltype(std::placeholders::_9)>::type>("d02678d2c8b0c83f2ff04262");
}
