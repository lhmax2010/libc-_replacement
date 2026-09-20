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
 emit<decltype(std::numeric_limits<int>::max())>("e734415c2946a263a2da03cc");
 emit<decltype(std::numeric_limits<unsigned>::digits)>("f0aee244a1ee5a6f11e27ff0");
 emit<decltype(std::numeric_limits<long double>::digits)>("46ed708d190f5ad9ec395123");
 emit<decltype(std::numeric_limits<double>::digits)>("cf9e342a181cedb16427973d");
 emit<decltype(std::numeric_limits<int>::min())>("9271a093d164d572e0138b09");
 emit<decltype(std::numeric_limits<int>::digits10)>("ddc7bd335e26af45d46475b2");
 emit<decltype(std::numeric_limits<std::int_least64_t>::max())>("296311c5ee81985ab21f8b68");
 emit<decltype(std::numeric_limits<std::int_least64_t>::min())>("6326d6265566b10017734f51");
 emit<decltype(std::numeric_limits<std::streamsize>::max())>("9aeee231b30768aeaa844087");
 emit<decltype(std::numeric_limits<std::size_t>::max())>("ed2b9664c2bcc4f4806c74a4");
 emit<decltype(std::numeric_limits<long>::max())>("c8933cff0290ed47fc7e7936");
 emit<decltype(std::numeric_limits<std::uint64_t>::max())>("65a115ff52fae95080d857c7");
 emit<decltype(std::numeric_limits<std::uint32_t>::max())>("5f7888788af783070e600ad3");
 emit<decltype(std::numeric_limits<
                std::uint64_t>::max())>("416ff073fd9b6f2f723dff7e");
 emit<decltype(std::numeric_limits<float>::max())>("74802e559c35b5b1d3561033");
 emit<decltype(std::numeric_limits<double>::max())>("146633dd735514926613d41c");
 emit<decltype(std::numeric_limits<long double>::max())>("a5a4c36fb50b0a1977c40f7e");
 emit<decltype(std::numeric_limits<unsigned long long>::max())>("f575a09281b0ca447fbed9e0");
 emit<decltype(std::numeric_limits<int>::digits)>("cad41bb57a142001b87d6058");
 emit<decltype(std::numeric_limits<std::size_t>::digits)>("867297b8b52e9eb31128b67a");
 emit<decltype(std::numeric_limits<unsigned int>::digits)>("c4d9973a9d1c6ee203416950");
 emit<decltype(std::numeric_limits<unsigned char>::digits)>("9e334997b62038223f5e1821");
 emit<decltype(std::numeric_limits<unsigned short>::digits)>("94a786102d66696ba4331b7c");
 emit<decltype(std::numeric_limits<unsigned long>::digits)>("3e2ad77a908d58189d60cd60");
 emit<decltype(std::numeric_limits<wchar_t>::max())>("5e47bd22f210339d2d2196a5");
 emit<decltype(std::numeric_limits< std::streamsize >::max())>("cd17d657407b85fb43d97449");
 emit<decltype(std::numeric_limits<double>::epsilon())>("765a781b28d8b9d27a479b2f");
 emit<decltype(std::numeric_limits< unsigned char >::digits)>("8e352690019467734e4b3360");
 emit<decltype(std::numeric_limits< std::size_t >::max())>("807d13e66dac1e80a60d841b");
 emit<decltype(std::numeric_limits<double>::infinity())>("8e8ea314ded6b8b4029e3463");
 emit<decltype(std::numeric_limits<float>::infinity())>("bda1b38f8558347ab9c5dd50");
 emit<decltype(std::numeric_limits<long>::digits)>("46a92a3bd701ea79b63f68be");
 emit<decltype(std::numeric_limits<short>::digits)>("c11773d8c67d276a098d4f1e");
 emit<decltype(std::numeric_limits<signed char>::digits)>("a59c57c8404c049c677d93ca");
 emit<decltype(std::numeric_limits<double>::quiet_NaN())>("33717e621192c587bbfb984a");
 emit<decltype(std::numeric_limits<float>::min())>("b58328443c6c73a41e8f7401");
 emit<decltype(std::numeric_limits<char>::min())>("595705504d2fea576d375113");
 emit<decltype(std::numeric_limits<double>::min())>("bd8a30bf60227d2ef0739c9a");
 emit<decltype(std::numeric_limits<long long>::max())>("e29a5f58af77693363ed163b");
 emit<decltype(std::numeric_limits<std::uintmax_t>::digits)>("5d49a649475aa3a5c7176a7c");
 emit<decltype(std::numeric_limits<double>::max_exponent)>("f294926d9ceadb4f2eafdc66");
 emit<decltype(std::numeric_limits<float>::digits)>("63ff7bd1c84c575a1b2c4080");
 emit<decltype(std::numeric_limits<unsigned long long>::digits)>("46c9a6e4a9a97208126f830a");
 emit<decltype(std::numeric_limits<long double>::is_specialized)>("1dac8a56abe3ef09b035b831");
 emit<decltype(std::numeric_limits<float>::max_digits10)>("d5693f46ee02db890e64a6bb");
 emit<decltype(std::numeric_limits<double>::max_digits10)>("e5e12bf2a2ee0ce5e7656849");
 emit<decltype(std::numeric_limits<long double>::max_digits10)>("6779ae1cb88b276deb0e77b9");
 emit<decltype(std::numeric_limits<long long>::digits)>("33544d8bdbecafba975b8b5a");
 emit<decltype(std::numeric_limits<long double>::denorm_min())>("bae1ade5ad55174d303d0ddf");
 emit<decltype(std::numeric_limits<std::uintmax_t>::max())>("ae3d7a6bb934831b8f4e51df");
 emit<decltype(std::numeric_limits<long double>::digits10)>("57f9799152398ac0b3983a4a");
 emit<decltype(std::numeric_limits<std::int32_t>::max())>("ad3f3882c42150bfc75412c4");
 emit<decltype(std::numeric_limits<long double>::min())>("f51873a6f3078e8843e350de");
 emit<decltype(std::numeric_limits<long long>::min())>("e91b76b248b4961e05a4df8e");
 emit<decltype(std::numeric_limits<double>::digits10)>("03c6eccb50dcb78d664ae4f2");
 emit<decltype(std::numeric_limits<long double>::quiet_NaN())>("745e95e246ec6ba794338563");
 emit<decltype(std::numeric_limits<long double>::infinity())>("f22aabe4d6dc2532b1796f4a");
 emit<decltype(std::numeric_limits<unsigned long long>::digits10)>("a4a32c1c3cca47a4b08038d3");
 emit<decltype(std::numeric_limits<unsigned>::max())>("d449f4d552f23a3ec05bcbec");
 emit<decltype(std::numeric_limits<long>::min())>("50e765398c4794d9a6a83c20");
 emit<decltype(std::numeric_limits<std::intmax_t>::max())>("e09a031045aeb57ce6971fca");
 emit<decltype(std::numeric_limits<std::intmax_t>::digits)>("26d6953dfc97f1c0e31f23a8");
 emit<decltype(std::numeric_limits<unsigned long>::max())>("b49343fdce1af67bd74e703f");
 emit<decltype(std::numeric_limits<std::int64_t>::digits)>("6dfa77f72e646ab2e28d36c3");
 emit<decltype(std::numeric_limits<std::streamoff>::min())>("7d084ac1506588a8169db8f5");
 emit<decltype(std::numeric_limits<char>::max())>("25897e0cb775dd1abae994fe");
 emit<decltype(std::numeric_limits<signed char>::max())>("331d794f8c4a8a92db23ab18");
 emit<decltype(std::numeric_limits<signed char>::min())>("c78e93470de4c1f9d63db25e");
 emit<decltype(std::numeric_limits<unsigned char>::max())>("d61c8d3e8e83fcc0adfa89e5");
 emit<decltype(std::numeric_limits<std::ptrdiff_t>::max())>("cb09b68df8154c979dabc224");
 emit<decltype(std::numeric_limits<std::intmax_t>::is_specialized)>("bc130a1e4da8cb7c4d6a2c3a");
 emit<decltype(std::numeric_limits<float>::lowest())>("033bc01544e216e355a2d2dd");
 emit<decltype(std::numeric_limits<double>::lowest())>("842c057f0724e5d3ad11a902");
 emit<decltype(std::numeric_limits<unsigned short>::max())>("cf783b872157ac4d7f1127bf");
 emit<decltype(std::numeric_limits<unsigned int>::max())>("782988c84f3280d7609f01c9");
 emit<decltype(std::numeric_limits<double>::max_exponent10)>("4c9abbd83c9ed40d489de4b4");
 emit<decltype(std::numeric_limits<long double>::max_exponent10)>("11d9dd14763b74c3deabfc28");
 emit<decltype(std::numeric_limits<long>::digits10)>("a02072de04829035269cf272");
 emit<decltype(std::numeric_limits<long long>::digits10)>("5f7801c35aa2c75e9e4364c8");
 emit<decltype(std::numeric_limits<unsigned int>::digits10)>("cd6725a3df510302997cbb88");
}
