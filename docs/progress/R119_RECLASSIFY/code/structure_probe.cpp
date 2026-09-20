#include <cstdio>
#include <cstddef>
#include <ios>
#include <limits>
#include <type_traits>
#include <iterator>
#include <tuple>
#include <memory>
#include <chrono>
#include <functional>
#include <utility>
#include <variant>
#include <string>
struct Base {}; struct Derived:Base {}; enum class Enum:unsigned {zero}; struct Fn { int operator()() const; };
template<class T> const char* actual_type() {return __PRETTY_FUNCTION__;}
template<class T> void type_result(int rank, const char* name, const char* kind) {
  if constexpr (std::is_void<T>::value) std::printf("%d\t%s\t%s\t%s\tsize=NA\tempty=NA\n",rank,name,kind,actual_type<T>());
  else std::printf("%d\t%s\t%s\t%s\tsize=%zu\tempty=%d\n",rank,name,kind,actual_type<T>(),sizeof(T),int(std::is_empty<T>::value));
}
int main() {
  static_assert(!std::is_empty<int>::value, "非空对照");
  static_assert(std::is_empty<Base>::value, "空类对照");
  std::printf("CONTROL empty_Base=%d nonempty_int=%d unequal_builtin=%d\n",int(std::is_empty<Base>::value),int(!std::is_empty<int>::value),int(!std::is_same<int,long>::value));
  std::printf("VERSION libcxx=%d gcc=%d\n",
#ifdef _LIBCPP_VERSION
  _LIBCPP_VERSION,
#else
  0,
#endif
#ifdef _GLIBCXX_RELEASE
  _GLIBCXX_RELEASE
#else
  0
#endif
  );
  type_result<std::size_t>(12,"std::size_t","BUILTIN_ALIAS");
  type_result<std::streamsize>(38,"std::streamsize","BUILTIN_ALIAS");
  type_result<std::nullptr_t>(48,"std::nullptr_t","BUILTIN_ALIAS");
  type_result<std::ptrdiff_t>(49,"std::ptrdiff_t","BUILTIN_ALIAS");
  type_result<std::true_type>(43,"std::true_type","EMPTY_TYPE");
  static_assert(std::is_empty<std::true_type>::value, "rank 43 实测空类条件");
  type_result<std::false_type>(45,"std::false_type","EMPTY_TYPE");
  static_assert(std::is_empty<std::false_type>::value, "rank 45 实测空类条件");
  type_result<std::chrono::steady_clock>(52,"std::chrono::steady_clock","EMPTY_TYPE");
  static_assert(std::is_empty<std::chrono::steady_clock>::value, "rank 52 实测空类条件");
  type_result<std::chrono::system_clock>(58,"std::chrono::system_clock","EMPTY_TYPE");
  static_assert(std::is_empty<std::chrono::system_clock>::value, "rank 58 实测空类条件");
  type_result<std::char_traits<char>>(66,"std::char_traits","EMPTY_TYPE");
  static_assert(std::is_empty<std::char_traits<char>>::value, "rank 66 实测空类条件");
  type_result<std::forward_iterator_tag>(67,"std::forward_iterator_tag","EMPTY_TYPE");
  static_assert(std::is_empty<std::forward_iterator_tag>::value, "rank 67 实测空类条件");
  type_result<std::integral_constant<int, 3>>(74,"std::integral_constant","EMPTY_TYPE");
  static_assert(std::is_empty<std::integral_constant<int, 3>>::value, "rank 74 实测空类条件");
  type_result<std::random_access_iterator_tag>(98,"std::random_access_iterator_tag","EMPTY_TYPE");
  static_assert(std::is_empty<std::random_access_iterator_tag>::value, "rank 98 实测空类条件");
  type_result<std::bidirectional_iterator_tag>(100,"std::bidirectional_iterator_tag","EMPTY_TYPE");
  static_assert(std::is_empty<std::bidirectional_iterator_tag>::value, "rank 100 实测空类条件");
  type_result<std::chrono::high_resolution_clock>(121,"std::chrono::high_resolution_clock","EMPTY_TYPE");
  static_assert(std::is_empty<std::chrono::high_resolution_clock>::value, "rank 121 实测空类条件");
  type_result<std::index_sequence<0, 1>>(134,"std::index_sequence","EMPTY_TYPE");
  static_assert(std::is_empty<std::index_sequence<0, 1>>::value, "rank 134 实测空类条件");
  type_result<std::greater<double>>(136,"std::greater","EMPTY_TYPE");
  static_assert(std::is_empty<std::greater<double>>::value, "rank 136 实测空类条件");
  type_result<std::make_index_sequence<2>>(138,"std::make_index_sequence","EMPTY_TYPE");
  static_assert(std::is_empty<std::make_index_sequence<2>>::value, "rank 138 实测空类条件");
  type_result<std::input_iterator_tag>(148,"std::input_iterator_tag","EMPTY_TYPE");
  static_assert(std::is_empty<std::input_iterator_tag>::value, "rank 148 实测空类条件");
  type_result<std::milli>(160,"std::milli","EMPTY_TYPE");
  static_assert(std::is_empty<std::milli>::value, "rank 160 实测空类条件");
  type_result<std::monostate>(170,"std::monostate","EMPTY_TYPE");
  static_assert(std::is_empty<std::monostate>::value, "rank 170 实测空类条件");
  type_result<std::ratio<1, 7>>(171,"std::ratio","EMPTY_TYPE");
  static_assert(std::is_empty<std::ratio<1, 7>>::value, "rank 171 实测空类条件");
  type_result<std::iterator<std::input_iterator_tag, int>>(197,"std::iterator","EMPTY_TYPE");
  static_assert(std::is_empty<std::iterator<std::input_iterator_tag, int>>::value, "rank 197 实测空类条件");
  type_result<std::nano>(198,"std::nano","EMPTY_TYPE");
  static_assert(std::is_empty<std::nano>::value, "rank 198 实测空类条件");
  type_result<std::not_equal_to<int>>(199,"std::not_equal_to","EMPTY_TYPE");
  static_assert(std::is_empty<std::not_equal_to<int>>::value, "rank 199 实测空类条件");
  constexpr auto value_19 = std::numeric_limits<int>::max();
  std::printf("19\tstd::numeric_limits\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_19);
  constexpr auto value_37 = std::is_same<int, int>::value;
  std::printf("37\tstd::is_same\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_37);
  type_result<typename std::enable_if<true, int>::type>(39,"std::enable_if","COMPILE_TIME_ONLY");
  constexpr auto value_46 = std::is_base_of<Base, Derived>::value;
  std::printf("46\tstd::is_base_of\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_46);
  type_result<typename std::remove_pointer<int*>::type>(50,"std::remove_pointer","COMPILE_TIME_ONLY");
  type_result<typename std::conditional<true, int, double>::type>(53,"std::conditional","COMPILE_TIME_ONLY");
  type_result<typename std::remove_reference<int&>::type>(55,"std::remove_reference","COMPILE_TIME_ONLY");
  type_result<typename std::decay<int&>::type>(57,"std::decay","COMPILE_TIME_ONLY");
  constexpr auto value_59 = std::is_integral<int>::value;
  std::printf("59\tstd::is_integral\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_59);
  type_result<typename std::iterator_traits<int*>::value_type>(64,"std::iterator_traits","COMPILE_TIME_ONLY");
  constexpr auto value_75 = std::is_convertible<int, double>::value;
  std::printf("75\tstd::is_convertible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_75);
  type_result<typename std::underlying_type<Enum>::type>(78,"std::underlying_type","COMPILE_TIME_ONLY");
  type_result<std::enable_if_t<true, int>>(80,"std::enable_if_t","COMPILE_TIME_ONLY");
  constexpr auto value_81 = std::tuple_size<std::tuple<int>>::value;
  std::printf("81\tstd::tuple_size\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_81);
  type_result<typename std::remove_const<const int>::type>(84,"std::remove_const","COMPILE_TIME_ONLY");
  constexpr auto value_85 = std::is_constructible<int, int>::value;
  std::printf("85\tstd::is_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_85);
  constexpr auto value_91 = std::is_pointer<int*>::value;
  std::printf("91\tstd::is_pointer\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_91);
  constexpr auto value_92 = std::is_signed<int>::value;
  std::printf("92\tstd::is_signed\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_92);
  constexpr auto value_93 = std::is_unsigned<unsigned>::value;
  std::printf("93\tstd::is_unsigned\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_93);
  constexpr auto value_95 = std::is_enum<Enum>::value;
  std::printf("95\tstd::is_enum\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_95);
  constexpr auto value_96 = std::is_reference<int&>::value;
  std::printf("96\tstd::is_reference\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_96);
  type_result<typename std::remove_cv<const volatile int>::type>(99,"std::remove_cv","COMPILE_TIME_ONLY");
  constexpr auto value_106 = std::is_floating_point<double>::value;
  std::printf("106\tstd::is_floating_point\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_106);
  constexpr auto value_107 = std::is_trivially_destructible<int>::value;
  std::printf("107\tstd::is_trivially_destructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_107);
  type_result<typename std::tuple_element<0, std::tuple<int>>::type>(109,"std::tuple_element","COMPILE_TIME_ONLY");
  type_result<typename std::allocator_traits<std::allocator<int>>::value_type>(111,"std::allocator_traits","COMPILE_TIME_ONLY");
  constexpr auto value_114 = std::is_arithmetic<int>::value;
  std::printf("114\tstd::is_arithmetic\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_114);
  constexpr auto value_116 = std::is_const<const int>::value;
  std::printf("116\tstd::is_const\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_116);
  constexpr auto value_117 = std::is_scalar<int>::value;
  std::printf("117\tstd::is_scalar\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_117);
  constexpr auto value_118 = std::is_standard_layout<int>::value;
  std::printf("118\tstd::is_standard_layout\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_118);
  constexpr auto value_119 = std::is_trivial<int>::value;
  std::printf("119\tstd::is_trivial\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_119);
  type_result<typename std::make_unsigned<int>::type>(120,"std::make_unsigned","COMPILE_TIME_ONLY");
  constexpr auto value_127 = std::is_void<void>::value;
  std::printf("127\tstd::is_void\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_127);
  constexpr auto value_128 = std::is_nothrow_move_constructible<int>::value;
  std::printf("128\tstd::is_nothrow_move_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_128);
  constexpr auto value_129 = std::is_trivially_copyable<int>::value;
  std::printf("129\tstd::is_trivially_copyable\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_129);
  constexpr auto value_135 = std::is_array<int[2]>::value;
  std::printf("135\tstd::is_array\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_135);
  constexpr auto value_137 = std::is_default_constructible<int>::value;
  std::printf("137\tstd::is_default_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_137);
  type_result<std::conditional_t<true, int, double>>(140,"std::conditional_t","COMPILE_TIME_ONLY");
  type_result<std::remove_pointer_t<int*>>(142,"std::remove_pointer_t","COMPILE_TIME_ONLY");
  type_result<std::remove_reference_t<int&>>(143,"std::remove_reference_t","COMPILE_TIME_ONLY");
  constexpr auto value_149 = std::is_copy_constructible<int>::value;
  std::printf("149\tstd::is_copy_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_149);
  constexpr auto value_150 = std::is_move_assignable<int>::value;
  std::printf("150\tstd::is_move_assignable\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_150);
  constexpr auto value_151 = std::is_move_constructible<int>::value;
  std::printf("151\tstd::is_move_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_151);
  constexpr auto value_153 = std::is_copy_assignable<int>::value;
  std::printf("153\tstd::is_copy_assignable\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_153);
  constexpr auto value_156 = std::is_function<void()>::value;
  std::printf("156\tstd::is_function\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_156);
  constexpr auto value_157 = std::is_lvalue_reference<int&>::value;
  std::printf("157\tstd::is_lvalue_reference\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_157);
  type_result<std::decay_t<int&>>(161,"std::decay_t","COMPILE_TIME_ONLY");
  constexpr auto value_164 = std::is_trivially_copy_constructible<int>::value;
  std::printf("164\tstd::is_trivially_copy_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_164);
  type_result<typename std::result_of<Fn()>::type>(165,"std::result_of","COMPILE_TIME_ONLY");
  type_result<std::underlying_type_t<Enum>>(166,"std::underlying_type_t","COMPILE_TIME_ONLY");
  type_result<typename std::add_lvalue_reference<int>::type>(167,"std::add_lvalue_reference","COMPILE_TIME_ONLY");
  type_result<typename std::add_const<int>::type>(173,"std::add_const","COMPILE_TIME_ONLY");
  type_result<typename std::add_pointer<int>::type>(174,"std::add_pointer","COMPILE_TIME_ONLY");
  type_result<typename std::aligned_storage<8, 8>::type>(175,"std::aligned_storage","COMPILE_TIME_ONLY");
  type_result<typename std::common_type<int, double>::type>(180,"std::common_type","COMPILE_TIME_ONLY");
  constexpr auto value_182 = std::is_assignable<int&, int>::value;
  std::printf("182\tstd::is_assignable\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_182);
  constexpr auto value_183 = std::is_pod<int>::value;
  std::printf("183\tstd::is_pod\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_183);
  type_result<std::remove_cv_t<const int>>(184,"std::remove_cv_t","COMPILE_TIME_ONLY");
  type_result<typename std::invoke_result<Fn>::type>(188,"std::invoke_result","COMPILE_TIME_ONLY");
  constexpr auto value_189 = std::is_empty<Base>::value;
  std::printf("189\tstd::is_empty\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_189);
  constexpr auto value_190 = std::is_nothrow_default_constructible<int>::value;
  std::printf("190\tstd::is_nothrow_default_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_190);
  constexpr auto value_191 = std::is_nothrow_move_assignable<int>::value;
  std::printf("191\tstd::is_nothrow_move_assignable\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_191);
  type_result<std::void_t<int>>(192,"std::void_t","COMPILE_TIME_ONLY");
  constexpr auto value_195 = std::is_member_pointer<int Base::*>::value;
  std::printf("195\tstd::is_member_pointer\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_195);
  constexpr auto value_196 = std::is_nothrow_copy_constructible<int>::value;
  std::printf("196\tstd::is_nothrow_copy_constructible\tCOMPILE_TIME_ONLY\tvalue=%lld\n",(long long)value_196);
}
