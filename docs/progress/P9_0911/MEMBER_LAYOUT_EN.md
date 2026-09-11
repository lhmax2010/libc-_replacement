# Member-level explanation of standard-library size and alignment differences

This companion covers only the ten differing types in the original table. Their size differences have source-visible representation causes, but **equal sizes would not make the two libraries’ objects interchangeable**. Each section includes actual members, source excerpts, available rationale and the consequences of matching layouts. “Technically possible, but not worthwhile” is a conditional engineering judgment, not a claim of impossibility or an upstream ruling.

## Baseline, scope and evidence

- **Existing measurements:** x86_64; GNU probe GCC 13.3.0; LLVM probe Clang 21.1.1 with platform libc++/libc++abi. Header macro 220108 means libc++ 22.1.8, not the compiler version. Sizes/alignments come from the [unchanged table copy](baseline/abi_layout_comparison.tsv), with [GNU output](baseline/layout_libstdcxx.tsv), [LLVM output](baseline/layout_libcxx.tsv) and [probe source](baseline/layout_probe.cpp). No build, new probe execution or development board was used.
- **Filename discrepancy:** the located headquarters table is `abi_layout_comparison.tsv`, byte-identical to archived `d11_layout_comparison.tsv`. The requested filename `abi_size_align_comparison.tsv` was not located (`NOT_AVAILABLE`); neither original was renamed or overwritten. Both located originals have SHA256 `08a99bb4ac7d314ba20c05befd92f40301dc4bcd3572fb126c8cf68c249509a7`.
- **Source audit:** platform GCC 14.2.0 revision `49c5060508ce8a6ed6beca375233688a6bdd9140`; platform LLVM revision `5ed6c77278dfa7a470667cf1a137723d3c96fe60`, matching revisions cited by the original report. Extracted platform files match their Git blobs. Compared LLVM layout headers match the original probe’s cached headers byte for byte. Current host headers from `libstdc++-13-dev 13.3.0-6ubuntu2~24.04.1` are also archived; platform GCC 14.2 is not the GNU measurement version. See the [GNU declaration crosscheck](GNU_REVISION_CROSSCHECK.md). Historical host-header hashes were not preserved, so historical byte identity is `NOT_AVAILABLE`; matching package versions do not prove it.
- **Member byte accounting:** types, declaration order, unions, inheritance and branches come from source. Member sizes and padding are static accounting under the baseline ABI, not new sizeof/offsetof measurements or compiler record-layout dumps. LP64 has 8-byte pointers/long and 4-byte int. Ordinary data/function pointers and size_t are 8, float 4, bool 1; see [AMD64 ABI 0.99.6 §3.1.2](https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf) and [Itanium C++ ABI §2](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#data-layout). Unknown complete node/control-block sizes are not invented.
- **Terminology:** ABI is the binary contract for representation, symbols and calls between compiled modules. sizeof is object size; alignof is the required address alignment. Padding fills alignment gaps. A bit is a binary digit; eight bits make one byte. Union members overlap and must not be added together. Empty-base optimization/no_unique_address may make a stateless allocator/comparator add no storage; this does **not** mean an independent complete empty object has size zero. Allocators manage storage; hashers and comparators group or compare keys. All instances use default allocator/comparison/hash types. LLVM analysis follows cached ABI version 1 and Linux x86_64 branches; the macros are reproduced below. Historical successful build logs contain command summaries rather than complete command lines; full flags/macro dumps are `NOT_AVAILABLE`. This static branch audit is not a historical member-offset measurement.
- **Labels:** measurements mean existing probe output; source audit includes the stated static accounting. Rationale is attributed only to cited upstream text. `NOT_OBSERVED` means no available observation; `NOT_AVAILABLE` means needed information was not found or obtainable. No unsupported design motive is supplied.

## Individual types

### `std::any`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 16 | 32 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | In order: manager function pointer `_M_manager` 8; storage union `_M_storage` 8 (an 8-byte pointer or an 8-byte inline buffer). [std/any:83](sources/gnu14/std/any.txt#L83) | In order: handler function pointer `__h_` 8; storage union `__s_` 24 (an 8-byte pointer or `char[24]`, aligned to 8). [any:150](sources/llvm/any.txt#L150) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/std/any:83–99](sources/gnu14/std/any.txt#L83)

```cpp
    // Holds either pointer to a heap object or the contained object itself.
    union _Storage
    {
      constexpr _Storage() : _M_ptr{nullptr} {}

      // Prevent trivial copies of this type, buffer might hold a non-POD.
      _Storage(const _Storage&) = delete;
      _Storage& operator=(const _Storage&) = delete;

      void* _M_ptr;
      aligned_storage<sizeof(_M_ptr), alignof(void*)>::type _M_buffer;
    };

    template<typename _Tp, typename _Safe = is_nothrow_move_constructible<_Tp>,
	     bool _Fits = (sizeof(_Tp) <= sizeof(_Storage))
			  && (alignof(_Tp) <= alignof(_Storage))>
      using _Internal = std::integral_constant<bool, _Safe::value && _Fits>;
```

[codes/gcc/libstdc++-v3/include/std/any:361–362](sources/gnu14/std/any.txt#L361)

```cpp
    void (*_M_manager)(_Op, const any*, _Arg*);
    _Storage _M_storage;
```

[codes/gcc/libstdc++-v3/include/std/any:401–424](sources/gnu14/std/any.txt#L401)

```cpp
    // Manage external contained object.
    template<typename _Tp>
      struct _Manager_external
      {
	static void
	_S_manage(_Op __which, const any* __anyp, _Arg* __arg);

	template<typename _Up>
	  static void
	  _S_create(_Storage& __storage, _Up&& __value)
	  {
	    __storage._M_ptr = new _Tp(std::forward<_Up>(__value));
	  }
	template<typename... _Args>
	  static void
	  _S_create(_Storage& __storage, _Args&&... __args)
	  {
	    __storage._M_ptr = new _Tp(std::forward<_Args>(__args)...);
	  }
	static _Tp*
	_S_access(const _Storage& __storage)
	{
	  // The contained object is in *__storage._M_ptr
	  return static_cast<_Tp*>(__storage._M_ptr);
```

[codes/llvm/libcxx/include/any:150–158](sources/llvm/any.txt#L150)

```cpp
namespace __any_imp {
inline constexpr size_t __small_buffer_size      = 3 * sizeof(void*);
inline constexpr size_t __small_buffer_alignment = alignof(void*);

template <class _Tp>
using _IsSmallObject _LIBCPP_NODEBUG =
    integral_constant<bool,
                      sizeof(_Tp) <= __small_buffer_size && alignof(_Tp) <= __small_buffer_alignment &&
                          is_nothrow_move_constructible_v<_Tp>>;
```

[codes/llvm/libcxx/include/any:308–317](sources/llvm/any.txt#L308)

```cpp
private:
  using _Action _LIBCPP_NODEBUG = __any_imp::_Action;
  using _HandleFuncPtr
      _LIBCPP_NODEBUG = void* (*)(_Action, any const*, any*, const type_info*, const void* __fallback_info);

  union _Storage {
    _LIBCPP_HIDE_FROM_ABI constexpr _Storage() : __ptr(nullptr) {}
    void* __ptr;
    alignas(__any_imp::__small_buffer_alignment) char __buf[__any_imp::__small_buffer_size];
  };
```

[codes/llvm/libcxx/include/any:341–342](sources/llvm/any.txt#L341)

```cpp
  _HandleFuncPtr __h_ = nullptr;
  _Storage __s_;
```

[codes/llvm/libcxx/include/any:427–443](sources/llvm/any.txt#L427)

```cpp
  template <class... _Args>
  _LIBCPP_HIDE_FROM_ABI static _Tp& __create(any& __dest, _Args&&... __args) {
    _Tp* __ptr = static_cast<_Tp*>(std::__libcpp_allocate<_Tp>(__element_count(1)));
    std::__exception_guard __guard([&] { std::__libcpp_deallocate<_Tp>(__ptr, __element_count(1)); });
    std::__construct_at(__ptr, std::forward<_Args>(__args)...);
    __guard.__complete();
    __dest.__s_.__ptr = __ptr;
    __dest.__h_       = &_LargeHandler::__handle;
    return *__ptr;
  }

private:
  _LIBCPP_HIDE_FROM_ABI static void __destroy(any& __this) {
    _Tp* __p = static_cast<_Tp*>(__this.__s_.__ptr);
    std::__destroy_at(__p);
    std::__libcpp_deallocate<_Tp>(__p, __element_count(1));
    __this.__h_ = nullptr;
```

**What causes the difference**:

Both keep one management-function pointer. The entire 16-byte difference comes from the larger libc++ inline buffer. Inline storage places a small value inside the wrapper; larger values live in separately allocated memory.

**Design rationale**:

[C++20 draft N4861, any.class paragraph 3](https://timsong-cpp.github.io/cppwp/n4861/any.class#3) recommends avoiding dynamic allocation for small values and limits this optimization to nothrow-move-constructible types. Both predicates reflect that rule. No public rationale was found for choosing exactly one versus three pointer widths (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** A new layout can be implemented by changing the union capacity and its operations. This breaks the modified library’s object stride and embedded-object ABI; shrinking also changes which values allocate. Matching capacity leaves incompatible handler protocols: GNU returns `void` and uses an operation/source/result union, while LLVM returns `void*` and accepts type and fallback-type information. Allocation failure exposure can change. This does not establish different standard `any_cast` semantics; exhaustive exception, type-identification and lifecycle interoperability is `NOT_OBSERVED`.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::function<void()>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 32 | 48 |
| Alignment (existing measurement, bytes) | 8 | 16 |
| Members (declaration order; static byte accounting) | Empty interface bases add no storage. `_Function_base` contains `_Any_data _M_functor` 16 then manager pointer 8; the derived wrapper adds invoker pointer 8. [bits/std_function.h:62](sources/gnu14/bits/std_function.h.txt#L62) | One `__value_func` member: aligned buffer 32 (requested length 24 rounded to alignment 16), then `__base<void()>*` 8 and trailing padding 8, totaling 48. Empty interface bases add no storage. [__functional/function.h:131](sources/llvm/__functional/function.h.txt#L131) [__type_traits/aligned_storage.h:34](sources/llvm/__type_traits/aligned_storage.h.txt#L34) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/std_function.h:62–100](sources/gnu14/bits/std_function.h.txt#L62)

```cpp
  /**
   *  Trait identifying "location-invariant" types, meaning that the
   *  address of the object (or any of its members) will not escape.
   *  Trivially copyable types are location-invariant and users can
   *  specialize this trait for other types.
   */
  template<typename _Tp>
    struct __is_location_invariant
    : is_trivially_copyable<_Tp>::type
    { };

  class _Undefined_class;

  union _Nocopy_types
  {
    void*       _M_object;
    const void* _M_const_object;
    void (*_M_function_pointer)();
    void (_Undefined_class::*_M_member_pointer)();
  };

  union [[gnu::may_alias]] _Any_data
  {
    void*       _M_access()       noexcept { return &_M_pod_data[0]; }
    const void* _M_access() const noexcept { return &_M_pod_data[0]; }

    template<typename _Tp>
      _Tp&
      _M_access() noexcept
      { return *static_cast<_Tp*>(_M_access()); }

    template<typename _Tp>
      const _Tp&
      _M_access() const noexcept
      { return *static_cast<const _Tp*>(_M_access()); }

    _Nocopy_types _M_unused;
    char _M_pod_data[sizeof(_Nocopy_types)];
  };
```

[codes/gcc/libstdc++-v3/include/bits/std_function.h:117–128](sources/gnu14/bits/std_function.h.txt#L117)

```cpp
    static const size_t _M_max_size = sizeof(_Nocopy_types);
    static const size_t _M_max_align = __alignof__(_Nocopy_types);

    template<typename _Functor>
      class _Base_manager
      {
      protected:
	static const bool __stored_locally =
	(__is_location_invariant<_Functor>::value
	 && sizeof(_Functor) <= _M_max_size
	 && __alignof__(_Functor) <= _M_max_align
	 && (_M_max_align % __alignof__(_Functor) == 0));
```

[codes/gcc/libstdc++-v3/include/bits/std_function.h:249–254](sources/gnu14/bits/std_function.h.txt#L249)

```cpp
    using _Manager_type
      = bool (*)(_Any_data&, const _Any_data&, _Manager_operation);

    _Any_data     _M_functor{};
    _Manager_type _M_manager{};
  };
```

[codes/gcc/libstdc++-v3/include/bits/std_function.h:333–336](sources/gnu14/bits/std_function.h.txt#L333)

```cpp
  template<typename _Res, typename... _ArgTypes>
    class function<_Res(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
      private _Function_base
```

[codes/gcc/libstdc++-v3/include/bits/std_function.h:666–668](sources/gnu14/bits/std_function.h.txt#L666)

```cpp
    private:
      using _Invoker_type = _Res (*)(const _Any_data&, _ArgTypes&&...);
      _Invoker_type _M_invoker = nullptr;
```

[codes/llvm/libcxx/include/__functional/function.h:131–148](sources/llvm/__functional/function.h.txt#L131)

```cpp
template <class _Rp, class... _ArgTypes>
class __base<_Rp(_ArgTypes...)> {
public:
  __base(const __base&)            = delete;
  __base& operator=(const __base&) = delete;

  _LIBCPP_HIDE_FROM_ABI __base() {}
  _LIBCPP_HIDE_FROM_ABI_VIRTUAL virtual ~__base() {}
  virtual __base* __clone() const             = 0;
  virtual void __clone(__base*) const         = 0;
  virtual void destroy() _NOEXCEPT            = 0;
  virtual void destroy_deallocate() _NOEXCEPT = 0;
  virtual _Rp operator()(_ArgTypes&&...)      = 0;
#  if _LIBCPP_HAS_RTTI
  virtual const void* target(const type_info&) const _NOEXCEPT = 0;
  virtual const std::type_info& target_type() const _NOEXCEPT  = 0;
#  endif // _LIBCPP_HAS_RTTI
};
```

[codes/llvm/libcxx/include/__functional/function.h:155–157](sources/llvm/__functional/function.h.txt#L155)

```cpp
template <class _Fp, class _Rp, class... _ArgTypes>
class __func<_Fp, _Rp(_ArgTypes...)> : public __base<_Rp(_ArgTypes...)> {
  _Fp __func_;
```

[codes/llvm/libcxx/include/__functional/function.h:189–196](sources/llvm/__functional/function.h.txt#L189)

```cpp
template <class _Rp, class... _ArgTypes>
class __value_func<_Rp(_ArgTypes...)> {
  _LIBCPP_SUPPRESS_DEPRECATED_PUSH
  typename aligned_storage<3 * sizeof(void*)>::type __buf_;
  _LIBCPP_SUPPRESS_DEPRECATED_POP

  typedef __base<_Rp(_ArgTypes...)> __func;
  __func* __f_;
```

[codes/llvm/libcxx/include/__functional/function.h:203–217](sources/llvm/__functional/function.h.txt#L203)

```cpp
  template <class _Fp, __enable_if_t<!is_same<__decay_t<_Fp>, __value_func>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI explicit __value_func(_Fp&& __f) : __f_(nullptr) {
    typedef __function::__func<_Fp, _Rp(_ArgTypes...)> _Fun;

    if (__function::__is_null(__f))
      return;

    if (sizeof(_Fun) <= sizeof(__buf_) && is_nothrow_copy_constructible<_Fp>::value) {
      __f_ = ::new (std::addressof(__buf_)) _Fun(std::move(__f));
    } else {
      __f_ = new _Fun(std::move(__f));
    }
  }

  _LIBCPP_HIDE_FROM_ABI __value_func(const __value_func& __f) {
```

[codes/llvm/libcxx/include/__functional/function.h:227–243](sources/llvm/__functional/function.h.txt#L227)

```cpp
  _LIBCPP_HIDE_FROM_ABI __value_func(__value_func&& __f) _NOEXCEPT {
    if (__f.__f_ == nullptr)
      __f_ = nullptr;
    else if ((void*)__f.__f_ == &__f.__buf_) {
      __f_ = __as_base(&__buf_);
      __f.__f_->__clone(__f_);
    } else {
      __f_     = __f.__f_;
      __f.__f_ = nullptr;
    }
  }

  _LIBCPP_HIDE_FROM_ABI ~__value_func() {
    if ((void*)__f_ == &__buf_)
      __f_->destroy();
    else if (__f_)
      __f_->destroy_deallocate();
```

[codes/llvm/libcxx/include/__functional/function.h:600–610](sources/llvm/__functional/function.h.txt#L600)

```cpp
template <class _Rp, class... _ArgTypes>
class function<_Rp(_ArgTypes...)>
    : public __function::__maybe_derive_from_unary_function<_Rp(_ArgTypes...)>,
      public __function::__maybe_derive_from_binary_function<_Rp(_ArgTypes...)> {
#  ifndef _LIBCPP_ABI_OPTIMIZED_FUNCTION
  typedef __function::__value_func<_Rp(_ArgTypes...)> __func;
#  else
  typedef __function::__policy_func<_Rp(_ArgTypes...)> __func;
#  endif

  __func __f_;
```

[codes/llvm/libcxx/include/__type_traits/aligned_storage.h:34–47](sources/llvm/__type_traits/aligned_storage.h.txt#L34)

```cpp
inline const size_t __aligned_storage_max_align =
    _LIBCPP_ALIGNOF(__max_align_impl<unsigned long long, double, long double, __struct_double, __struct_double4, int*>);

template <size_t _Len>
inline const size_t __aligned_storage_alignment =
    _Len > __aligned_storage_max_align
        ? __aligned_storage_max_align
        : size_t(1) << ((sizeof(size_t) * __CHAR_BIT__) - __builtin_clzg(_Len) - 1);

template <size_t _Len, size_t _Align = __aligned_storage_alignment<_Len> >
struct _LIBCPP_DEPRECATED_IN_CXX23 _LIBCPP_NO_SPECIALIZATIONS aligned_storage {
  union _ALIGNAS(_Align) type {
    unsigned char __data[(_Len + _Align - 1) / _Align * _Align];
  };
```

**What causes the difference**:

GNU uses a buffer and two ordinary function pointers. LLVM uses a larger, more strictly aligned buffer and a pointer to a wrapper with virtual operations. Buffer rounding and trailing padding explain 48 bytes; 24 + 8 would be incorrect. GNU’s union includes a member-function pointer, represented by two machine words (16 bytes) under Itanium ABI §2.3.2; it is not an ordinary 8-byte function pointer.

**Design rationale**:

[Jonathan Wakely’s upstream reply, 2016-07-30](https://gcc.gnu.org/pipermail/libstdc++/2016-July/044437.html) rejects increasing GNU’s buffer from 16 to 24 bytes because it breaks library ABI. This explains retaining the size, not the original choice. LLVM’s [ABI documentation](https://libcxx.llvm.org/ABIGuarantees.html) describes an unstable optimized representation; this platform does not enable it. The original LLVM size/dispatch rationale was not found (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Replacing storage, alignment, dispatch and lifecycle together can create another representation, while breaking the existing library ABI. Changing sizes alone leaves LLVM’s virtual dispatch incompatible with GNU’s manager/invoker protocol. GNU’s local-storage predicate requires location invariance (defaulting to trivial copyability); LLVM checks wrapper size and nothrow copy construction of the target. Their allocation and move paths differ: LLVM clones an inline target during move and can leave the source populated, an allowed moved-from state. Complete exception/call equivalence is `NOT_OBSERVED`. GNU already has an explicit upstream rejection of buffer enlargement.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::deque<int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 80 | 48 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | Empty allocator base, then `int** _M_map` 8, `size_t _M_map_size` 8, start iterator 32, finish iterator 32. Each iterator stores three `int*` fields (`_M_cur/_M_first/_M_last`) and one `int** _M_node`, 8 each. [bits/stl_deque.h:83](sources/gnu14/bits/stl_deque.h.txt#L83) | `__map_` 32: four `int**` fields (`__front_cap_/__begin_/__end_/__back_cap_`) plus compressed empty pointer allocator; then start index 8 and element count 8, with compressed empty element allocator. Public iterators separately contain a directory pointer and an element pointer; they are not embedded in the deque. [deque:266](sources/llvm/deque.txt#L266) [__split_buffer:193](sources/llvm/__split_buffer.txt#L193) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/stl_deque.h:83–98](sources/gnu14/bits/stl_deque.h.txt#L83)

```cpp
   *  seems to be a useful wrapper around a repeated constant
   *  expression.  The @b 512 is tunable (and no other code needs to
   *  change), but no investigation has been done since inheriting the
   *  SGI code.  Touch _GLIBCXX_DEQUE_BUF_SIZE only if you know what
   *  you are doing, however: changing it breaks the binary
   *  compatibility!!
  */

#ifndef _GLIBCXX_DEQUE_BUF_SIZE
#define _GLIBCXX_DEQUE_BUF_SIZE 512
#endif

  _GLIBCXX_CONSTEXPR inline size_t
  __deque_buf_size(size_t __size)
  { return (__size < _GLIBCXX_DEQUE_BUF_SIZE
	    ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1)); }
```

[codes/gcc/libstdc++-v3/include/bits/stl_deque.h:142–145](sources/gnu14/bits/stl_deque.h.txt#L142)

```cpp
      _Elt_pointer _M_cur;
      _Elt_pointer _M_first;
      _Elt_pointer _M_last;
      _Map_pointer _M_node;
```

[codes/gcc/libstdc++-v3/include/bits/stl_deque.h:509–515](sources/gnu14/bits/stl_deque.h.txt#L509)

```cpp
      struct _Deque_impl_data
      {
	_Map_pointer _M_map;
	size_t _M_map_size;
	iterator _M_start;
	iterator _M_finish;
```

[codes/gcc/libstdc++-v3/include/bits/stl_deque.h:539–544](sources/gnu14/bits/stl_deque.h.txt#L539)

```cpp
      // This struct encapsulates the implementation of the std::deque
      // standard container and at the same time makes use of the EBO
      // for empty allocators.
      struct _Deque_impl
      : public _Tp_alloc_type, public _Deque_impl_data
      {
```

[codes/gcc/libstdc++-v3/include/bits/stl_deque.h:722–740](sources/gnu14/bits/stl_deque.h.txt#L722)

```cpp
   *  Here's how a deque<Tp> manages memory.  Each deque has 4 members:
   *
   *  - Tp**        _M_map
   *  - size_t      _M_map_size
   *  - iterator    _M_start, _M_finish
   *
   *  map_size is at least 8.  %map is an array of map_size
   *  pointers-to-@a nodes.  (The name %map has nothing to do with the
   *  std::map class, and @b nodes should not be confused with
   *  std::list's usage of @a node.)
   *
   *  A @a node has no specific type name as such, but it is referred
   *  to as @a node in this file.  It is a simple array-of-Tp.  If Tp
   *  is very large, there will be one Tp element per node (i.e., an
   *  @a array of one).  For non-huge Tp's, node size is inversely
   *  related to Tp size: the larger the Tp, the fewer Tp's will fit
   *  in a node.  The goal here is to keep the total size of a node
   *  relatively small and constant over different Tp's, to improve
   *  allocator efficiency.
```

[codes/gcc/libstdc++-v3/include/bits/stl_deque.h:750–763](sources/gnu14/bits/stl_deque.h.txt#L750)

```cpp
   *  Class invariants:
   * - For any nonsingular iterator i:
   *    - i.node points to a member of the %map array.  (Yes, you read that
   *      correctly:  i.node does not actually point to a node.)  The member of
   *      the %map array is what actually points to the node.
   *    - i.first == *(i.node)    (This points to the node (first Tp element).)
   *    - i.last  == i.first + node_size
   *    - i.cur is a pointer in the range [i.first, i.last).  NOTE:
   *      the implication of this is that i.cur is always a dereferenceable
   *      pointer, even if i is a past-the-end iterator.
   * - Start and Finish are always nonsingular iterators.  NOTE: this
   * means that an empty deque must have one node, a deque with <N
   * elements (where N is the node buffer size) must have one node, a
   * deque with N through (2N-1) elements must have two nodes, etc.
```

[codes/llvm/libcxx/include/deque:266–269](sources/llvm/deque.txt#L266)

```cpp
template <class _ValueType, class _DiffType>
struct __deque_block_size {
  static const _DiffType value = sizeof(_ValueType) < 256 ? 4096 / sizeof(_ValueType) : 16;
};
```

[codes/llvm/libcxx/include/deque:285–296](sources/llvm/deque.txt#L285)

```cpp
class __deque_iterator {
  typedef _MapPointer __map_iterator;

public:
  typedef _Pointer pointer;
  typedef _DiffType difference_type;

private:
  __map_iterator __m_iter_;
  pointer __ptr_;

  static const difference_type __block_size;
```

[codes/llvm/libcxx/include/deque:486–489](sources/llvm/deque.txt#L486)

```cpp
template <class _Tp, class _Allocator /*= allocator<_Tp>*/>
class deque {
  template <class _Up, class _Alloc>
  using __split_buffer _LIBCPP_NODEBUG = std::__split_buffer<_Up, _Alloc, __split_buffer_pointer_layout>;
```

[codes/llvm/libcxx/include/deque:508–510](sources/llvm/deque.txt#L508)

```cpp
  using __pointer_allocator _LIBCPP_NODEBUG       = __rebind_alloc<__alloc_traits, pointer>;
  using __const_pointer_allocator _LIBCPP_NODEBUG = __rebind_alloc<__alloc_traits, const_pointer>;
  using __map _LIBCPP_NODEBUG                     = __split_buffer<pointer, __pointer_allocator>;
```

[codes/llvm/libcxx/include/deque:602–612](sources/llvm/deque.txt#L602)

```cpp

  __map __map_;
  size_type __start_;
  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, allocator_type, __alloc_);

public:
  // construct/copy/destroy:
  _LIBCPP_HIDE_FROM_ABI deque() _NOEXCEPT_(is_nothrow_default_constructible<allocator_type>::value)
      : __start_(0), __size_(0) {
    __annotate_new(0);
  }
```

[codes/llvm/libcxx/include/__split_buffer:193–201](sources/llvm/__split_buffer.txt#L193)

```cpp
private:
  pointer __front_cap_ = nullptr;
  pointer __begin_     = nullptr;
  pointer __end_       = nullptr;
  _LIBCPP_COMPRESSED_PAIR(pointer, __back_cap_, allocator_type, __alloc_);

  template <class, class, class>
  friend class __split_buffer_pointer_layout;
};
```

[codes/llvm/libcxx/include/__split_buffer:360–375](sources/llvm/__split_buffer.txt#L360)

```cpp
// `__split_buffer` is a contiguous array data structure. It may hold spare capacity at both ends of
// the sequence. This allows for a `__split_buffer` to grow from both the front and the back without
// relocating its contents until it runs out of room. This characteristic sets it apart from
// `std::vector`, which only holds spare capacity at its end. As such, `__split_buffer` is useful
// for implementing both `std::vector` and `std::deque`.
//
// The sequence is stored as a contiguous chunk of memory delimited by the following "pointers" (`o` denotes
// uninitialized memory and `x` denotes a valid object):
//
//     |oooooooooooooooooooxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxoooooooooooooooooooooooo|
//      ^                  ^                                    ^                       ^
//  __front_cap_        __begin_                              __end_               __back_cap_
//
// The range [__front_cap_, __begin_) contains uninitialized memory. It is referred to as the "front spare capacity".
// The range [__begin_, __end_) contains valid objects. It is referred to as the "valid range".
// The range [__end_, __back_cap_) contains uninitialized memory. It is referred to as the "back spare capacity".
```

[codes/llvm/libcxx/include/__split_buffer:443–445](sources/llvm/__split_buffer.txt#L443)

```cpp
template <class _Tp, class _Allocator, template <class, class, class> class _Layout>
class __split_buffer : _Layout<__split_buffer<_Tp, _Allocator, _Layout>, _Tp, _Allocator> {
  using __base_type _LIBCPP_NODEBUG = _Layout<__split_buffer<_Tp, _Allocator, _Layout>, _Tp, _Allocator>;
```

**What causes the difference**:

GNU embeds two full, larger iterators. LLVM records a start index and an element count, and uses four pointers for its block directory. That directory is an array of pointers to element blocks, unrelated to `std::map`.

**Design rationale**:

GNU’s source rationale says relatively small, consistent blocks improve allocator efficiency, and documents iterator invariants (internal relationships operations must preserve). It also warns that changing block size breaks binary compatibility. LLVM’s `__split_buffer` comments explain spare capacity at both ends permits growth without relocating existing directory entries until space runs out. These are source-authored explanations, reproduced above. The exact 512/4096-byte constants and original member-count choices lack a fuller public rationale (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Replacing the directory and endpoint representation breaks existing inline operations, iterator representation and object ABI. Even matching the wrapper leaves different block capacities for `int`: GNU 512/4 = 128; LLVM 4096/4 = 1024. Cross-block arithmetic is incompatible. GNU explicitly requires a block even for an empty deque; that invariant cannot be imposed on LLVM objects. Both must satisfy the standard complexity and invalidation contract in [N4861 deque.modifiers](https://timsong-cpp.github.io/cppwp/n4861/deque.modifiers); representation differences do not establish different standard rules. Exhaustive runtime invalidation/exception checks are `NOT_OBSERVED`.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::map<int,int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 48 | 24 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | One tree member `_M_t`: empty node-allocator base; comparator wrapper (ordinary `less<int>` member 1 plus alignment padding 7); header with color enum 4, padding 4 and parent/left/right pointers 8 each; node count 8. Total 8 + 32 + 8 = 48. [bits/stl_tree.h:95](sources/gnu14/bits/stl_tree.h.txt#L95) [bits/stl_map.h:149](sources/gnu14/bits/stl_map.h.txt#L149) | One tree member `__tree_`: begin-node pointer 8; end sentinel 8 (only the root pointer `__left_`) with compressed empty node allocator; size 8 with compressed empty comparator. Total 24. [__tree:557](sources/llvm/__tree.txt#L557) [map:639](sources/llvm/map.txt#L639) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/stl_map.h:149–158](sources/gnu14/bits/stl_map.h.txt#L149)

```cpp
    private:
      /// This turns a red-black tree into a [multi]map.
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<value_type>::other _Pair_alloc_type;

      typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
		       key_compare, _Pair_alloc_type> _Rep_type;

      /// The actual tree structure.
      _Rep_type _M_t;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:95–105](sources/gnu14/bits/stl_tree.h.txt#L95)

```cpp
  enum _Rb_tree_color { _S_red = false, _S_black = true };

  struct _Rb_tree_node_base
  {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color	_M_color;
    _Base_ptr		_M_parent;
    _Base_ptr		_M_left;
    _Base_ptr		_M_right;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:136–140](sources/gnu14/bits/stl_tree.h.txt#L136)

```cpp
  // Helper type offering value initialization guarantee on the compare functor.
  template<typename _Key_compare>
    struct _Rb_tree_key_compare
    {
      _Key_compare		_M_key_compare;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:164–167](sources/gnu14/bits/stl_tree.h.txt#L164)

```cpp
  struct _Rb_tree_header
  {
    _Rb_tree_node_base	_M_header;
    size_t		_M_node_count; // Keeps track of size of tree.
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:658–661](sources/gnu14/bits/stl_tree.h.txt#L658)

```cpp
	struct _Rb_tree_impl
	: public _Node_allocator
	, public _Rb_tree_key_compare<_Key_compare>
	, public _Rb_tree_header
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:704–704](sources/gnu14/bits/stl_tree.h.txt#L704)

```cpp
      _Rb_tree_impl<_Compare> _M_impl;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:211–214](sources/gnu14/bits/stl_tree.h.txt#L211)

```cpp
  template<typename _Val>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
      typedef _Rb_tree_node<_Val>* _Link_type;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:226–227](sources/gnu14/bits/stl_tree.h.txt#L226)

```cpp
#else
      __gnu_cxx::__aligned_membuf<_Val> _M_storage;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:79–93](sources/gnu14/bits/stl_tree.h.txt#L79)

```cpp
  // Red-black tree class, designed for use in implementing STL
  // associative containers (set, multiset, map, and multimap). The
  // insertion and deletion algorithms are based on those in Cormen,
  // Leiserson, and Rivest, Introduction to Algorithms (MIT Press,
  // 1990), except that
  //
  // (1) the header cell is maintained with links not only to the root
  // but also to the leftmost node of the tree, to enable constant
  // time begin(), and to the rightmost node of the tree, to enable
  // linear time performance when used with the generic set algorithms
  // (set_union, etc.)
  //
  // (2) when a node being deleted has two children its successor node
  // is relinked into its place, rather than copied, so that the only
  // iterators invalidated are those referring to the deleted node.
```

[codes/llvm/libcxx/include/map:639–648](sources/llvm/map.txt#L639)

```cpp
template <class _Key, class _CP, class _Compare>
class __map_value_compare {
  _LIBCPP_COMPRESSED_ELEMENT(_Compare, __comp_);

public:
  _LIBCPP_HIDE_FROM_ABI __map_value_compare() _NOEXCEPT_(is_nothrow_default_constructible<_Compare>::value)
      : __comp_() {}
  _LIBCPP_HIDE_FROM_ABI __map_value_compare(_Compare __c) _NOEXCEPT_(is_nothrow_copy_constructible<_Compare>::value)
      : __comp_(__c) {}
  _LIBCPP_HIDE_FROM_ABI const _Compare& key_comp() const _NOEXCEPT { return __comp_; }
```

[codes/llvm/libcxx/include/map:951–960](sources/llvm/map.txt#L951)

```cpp
private:
  typedef std::__value_type<key_type, mapped_type> __value_type;
  typedef __map_value_compare<key_type, value_type, key_compare> __vc;
  typedef __tree<__value_type, __vc, allocator_type> __base;
  typedef typename __base::__node_traits __node_traits;
  typedef allocator_traits<allocator_type> __alloc_traits;

  static_assert(__check_valid_allocator<allocator_type>::value, "");

  __base __tree_;
```

[codes/llvm/libcxx/include/__tree:557–574](sources/llvm/__tree.txt#L557)

```cpp
template <class _Pointer>
class __tree_end_node {
public:
  using pointer = _Pointer;
  pointer __left_;

  _LIBCPP_HIDE_FROM_ABI __tree_end_node() _NOEXCEPT : __left_() {}
};

template <class _VoidPtr>
class __tree_node_base : public __tree_end_node<__rebind_pointer_t<_VoidPtr, __tree_node_base<_VoidPtr> > > {
public:
  using pointer                            = __rebind_pointer_t<_VoidPtr, __tree_node_base>;
  using __end_node_pointer _LIBCPP_NODEBUG = __rebind_pointer_t<_VoidPtr, __tree_end_node<pointer> >;

  pointer __right_;
  __end_node_pointer __parent_;
  bool __is_black_;
```

[codes/llvm/libcxx/include/__tree:585–598](sources/llvm/__tree.txt#L585)

```cpp
template <class _Tp, class _VoidPtr>
class __tree_node : public __tree_node_base<_VoidPtr> {
public:
  using __node_value_type _LIBCPP_NODEBUG = __get_node_value_type_t<_Tp>;

// We use a union to avoid initialization during member initialization, which allows us
// to use the allocator from the container to construct the `__node_value_type` in the
// memory provided by the union member
#ifndef _LIBCPP_CXX03_LANG

private:
  union {
    __node_value_type __value_;
  };
```

[codes/llvm/libcxx/include/__tree:924–927](sources/llvm/__tree.txt#L924)

```cpp
private:
  __end_node_pointer __begin_node_;
  _LIBCPP_COMPRESSED_PAIR(__end_node_t, __end_node_, __node_allocator, __node_alloc_);
  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, value_compare, __value_comp_);
```

[codes/llvm/libcxx/include/__tree:96–109](sources/llvm/__tree.txt#L96)

```cpp
The algorithms taking _NodePtr are red black tree algorithms.  Those
algorithms taking a parameter named __root should assume that __root
points to a proper red black tree (unless otherwise specified).

Each algorithm herein assumes that __root->__parent_ points to a non-null
structure which has a member __left_ which points back to __root.  No other
member is read or written to at __root->__parent_.

__root->__parent_ will be referred to below (in comments only) as end_node.
end_node->__left_ is an externably accessible lvalue for __root, and can be
changed by node insertion and removal (without explicit reference to end_node).

All nodes (with the exception of end_node), even the node referred to as
__root, have a non-null __parent_ field.
```

**What causes the difference**:

GNU embeds a full tree header and spends an alignment unit on a wrapper containing an ordinary empty comparator member. LLVM uses a root-pointer-only end sentinel and a compressed comparator. A sentinel represents a boundary and holds no business value. Element nodes are outside the container object; this instantiation stores `pair<const int,int>` (two ints, 8 bytes).

**Design rationale**:

GNU’s header comments explain cached leftmost/rightmost links and relinking the successor during two-child deletion so that other node iterators remain valid. LLVM’s algorithm contract says the root’s parent needs only a `__left_` link back to the root; no other field is accessed there. This explains what its minimal sentinel supports. These are upstream source explanations; a further historical rationale for choosing different headers was not found (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Reworking the header/comparator and balancing, insertion and deletion algorithms breaks published object and inline-operation ABI. Node protocols also differ: GNU puts a color enum before its three links; LLVM uses inherited left link, right link, parent link, a boolean color, then the value. Pointers are 8 bytes; GNU color is 4, LLVM color is 1. Complete node sizes and tail-padding reuse are `NOT_OBSERVED`; the container size ratio is not a node-size ratio. A matching wrapper cannot fix this. Both must preserve ordering, complexity and iterator requirements in [N4861 associative.reqmts](https://timsong-cpp.github.io/cppwp/n4861/associative.reqmts). Different ordinary-exception or standard invalidation rules have not been established; exhaustive runtime validation is `NOT_OBSERVED`. Map keys remain const while mapped values remain mutable.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::set<int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 48 | 24 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | One tree member `_M_t`: empty node-allocator base; comparator wrapper (ordinary `less<int>` member 1 plus alignment padding 7); header with color enum 4, padding 4 and parent/left/right pointers 8 each; node count 8. Total 8 + 32 + 8 = 48. [bits/stl_tree.h:95](sources/gnu14/bits/stl_tree.h.txt#L95) [bits/stl_set.h:129](sources/gnu14/bits/stl_set.h.txt#L129) | One tree member `__tree_`: begin-node pointer 8; end sentinel 8 (only the root pointer `__left_`) with compressed empty node allocator; size 8 with compressed empty comparator. Total 24. [__tree:557](sources/llvm/__tree.txt#L557) [set:590](sources/llvm/set.txt#L590) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/stl_set.h:129–135](sources/gnu14/bits/stl_set.h.txt#L129)

```cpp
    private:
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<_Key>::other _Key_alloc_type;

      typedef _Rb_tree<key_type, value_type, _Identity<value_type>,
		       key_compare, _Key_alloc_type> _Rep_type;
      _Rep_type _M_t;  // Red-black tree representing set.
```

[codes/gcc/libstdc++-v3/include/bits/stl_set.h:145–150](sources/gnu14/bits/stl_set.h.txt#L145)

```cpp
      typedef typename _Alloc_traits::const_reference	 const_reference;
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 103. set::iterator is required to be modifiable,
      // but this allows modification of keys.
      typedef typename _Rep_type::const_iterator	 iterator;
      typedef typename _Rep_type::const_iterator	 const_iterator;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:95–105](sources/gnu14/bits/stl_tree.h.txt#L95)

```cpp
  enum _Rb_tree_color { _S_red = false, _S_black = true };

  struct _Rb_tree_node_base
  {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color	_M_color;
    _Base_ptr		_M_parent;
    _Base_ptr		_M_left;
    _Base_ptr		_M_right;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:136–140](sources/gnu14/bits/stl_tree.h.txt#L136)

```cpp
  // Helper type offering value initialization guarantee on the compare functor.
  template<typename _Key_compare>
    struct _Rb_tree_key_compare
    {
      _Key_compare		_M_key_compare;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:164–167](sources/gnu14/bits/stl_tree.h.txt#L164)

```cpp
  struct _Rb_tree_header
  {
    _Rb_tree_node_base	_M_header;
    size_t		_M_node_count; // Keeps track of size of tree.
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:658–661](sources/gnu14/bits/stl_tree.h.txt#L658)

```cpp
	struct _Rb_tree_impl
	: public _Node_allocator
	, public _Rb_tree_key_compare<_Key_compare>
	, public _Rb_tree_header
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:704–704](sources/gnu14/bits/stl_tree.h.txt#L704)

```cpp
      _Rb_tree_impl<_Compare> _M_impl;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:211–214](sources/gnu14/bits/stl_tree.h.txt#L211)

```cpp
  template<typename _Val>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
      typedef _Rb_tree_node<_Val>* _Link_type;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:226–227](sources/gnu14/bits/stl_tree.h.txt#L226)

```cpp
#else
      __gnu_cxx::__aligned_membuf<_Val> _M_storage;
```

[codes/gcc/libstdc++-v3/include/bits/stl_tree.h:79–93](sources/gnu14/bits/stl_tree.h.txt#L79)

```cpp
  // Red-black tree class, designed for use in implementing STL
  // associative containers (set, multiset, map, and multimap). The
  // insertion and deletion algorithms are based on those in Cormen,
  // Leiserson, and Rivest, Introduction to Algorithms (MIT Press,
  // 1990), except that
  //
  // (1) the header cell is maintained with links not only to the root
  // but also to the leftmost node of the tree, to enable constant
  // time begin(), and to the rightmost node of the tree, to enable
  // linear time performance when used with the generic set algorithms
  // (set_union, etc.)
  //
  // (2) when a node being deleted has two children its successor node
  // is relinked into its place, rather than copied, so that the only
  // iterators invalidated are those referring to the deleted node.
```

[codes/llvm/libcxx/include/set:590–596](sources/llvm/set.txt#L590)

```cpp
private:
  typedef __tree<value_type, value_compare, allocator_type> __base;
  typedef allocator_traits<allocator_type> __alloc_traits;

  static_assert(__check_valid_allocator<allocator_type>::value, "");

  __base __tree_;
```

[codes/llvm/libcxx/include/__tree:557–574](sources/llvm/__tree.txt#L557)

```cpp
template <class _Pointer>
class __tree_end_node {
public:
  using pointer = _Pointer;
  pointer __left_;

  _LIBCPP_HIDE_FROM_ABI __tree_end_node() _NOEXCEPT : __left_() {}
};

template <class _VoidPtr>
class __tree_node_base : public __tree_end_node<__rebind_pointer_t<_VoidPtr, __tree_node_base<_VoidPtr> > > {
public:
  using pointer                            = __rebind_pointer_t<_VoidPtr, __tree_node_base>;
  using __end_node_pointer _LIBCPP_NODEBUG = __rebind_pointer_t<_VoidPtr, __tree_end_node<pointer> >;

  pointer __right_;
  __end_node_pointer __parent_;
  bool __is_black_;
```

[codes/llvm/libcxx/include/__tree:585–598](sources/llvm/__tree.txt#L585)

```cpp
template <class _Tp, class _VoidPtr>
class __tree_node : public __tree_node_base<_VoidPtr> {
public:
  using __node_value_type _LIBCPP_NODEBUG = __get_node_value_type_t<_Tp>;

// We use a union to avoid initialization during member initialization, which allows us
// to use the allocator from the container to construct the `__node_value_type` in the
// memory provided by the union member
#ifndef _LIBCPP_CXX03_LANG

private:
  union {
    __node_value_type __value_;
  };
```

[codes/llvm/libcxx/include/__tree:924–927](sources/llvm/__tree.txt#L924)

```cpp
private:
  __end_node_pointer __begin_node_;
  _LIBCPP_COMPRESSED_PAIR(__end_node_t, __end_node_, __node_allocator, __node_alloc_);
  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, value_compare, __value_comp_);
```

[codes/llvm/libcxx/include/__tree:96–109](sources/llvm/__tree.txt#L96)

```cpp
The algorithms taking _NodePtr are red black tree algorithms.  Those
algorithms taking a parameter named __root should assume that __root
points to a proper red black tree (unless otherwise specified).

Each algorithm herein assumes that __root->__parent_ points to a non-null
structure which has a member __left_ which points back to __root.  No other
member is read or written to at __root->__parent_.

__root->__parent_ will be referred to below (in comments only) as end_node.
end_node->__left_ is an externably accessible lvalue for __root, and can be
changed by node insertion and removal (without explicit reference to end_node).

All nodes (with the exception of end_node), even the node referred to as
__root, have a non-null __parent_ field.
```

**What causes the difference**:

GNU embeds a full tree header and spends an alignment unit on a wrapper containing an ordinary empty comparator member. LLVM uses a root-pointer-only end sentinel and a compressed comparator. A sentinel represents a boundary and holds no business value. Element nodes are outside the container object; this instantiation stores `int` (4 bytes).

**Design rationale**:

GNU’s header comments explain cached leftmost/rightmost links and relinking the successor during two-child deletion so that other node iterators remain valid. LLVM’s algorithm contract says the root’s parent needs only a `__left_` link back to the root; no other field is accessed there. This explains what its minimal sentinel supports. These are upstream source explanations; a further historical rationale for choosing different headers was not found (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Reworking the header/comparator and balancing, insertion and deletion algorithms breaks published object and inline-operation ABI. Node protocols also differ: GNU puts a color enum before its three links; LLVM uses inherited left link, right link, parent link, a boolean color, then the value. Pointers are 8 bytes; GNU color is 4, LLVM color is 1. Complete node sizes and tail-padding reuse are `NOT_OBSERVED`; the container size ratio is not a node-size ratio. A matching wrapper cannot fix this. Both must preserve ordering, complexity and iterator requirements in [N4861 associative.reqmts](https://timsong-cpp.github.io/cppwp/n4861/associative.reqmts). Different ordinary-exception or standard invalidation rules have not been established; exhaustive runtime validation is `NOT_OBSERVED`. Set iterators must continue to prevent key modification (see the GNU typedef above).

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::unordered_map<int,int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 56 | 40 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | One `_M_h`: bucket-array pointer 8, bucket count 8, before-begin sentinel/next pointer 8, element count 8, prime-rehash policy 16 (float load factor 4 + padding 4 + size_t next-resize threshold 8), embedded single-bucket pointer 8. Default hash/equality/allocator bases add no storage. [bits/hashtable.h:47](sources/gnu14/bits/hashtable.h.txt#L47) [bits/hashtable_policy.h:315](sources/gnu14/bits/hashtable_policy.h.txt#L315) [bits/unordered_map.h:43](sources/gnu14/bits/unordered_map.h.txt#L43) | One `__table_`: bucket-array owner 16 (array pointer 8, deleter’s bucket count 8 with compressed empty allocator; stateless bounds checker adds no storage); sentinel/next pointer 8 with empty node allocator; size 8 with empty hasher; float load factor 4 with empty equality object; trailing padding 4. Total 40. [__hash_table:79](sources/llvm/__hash_table.txt#L79) [__memory/unique_ptr.h:340](sources/llvm/__memory/unique_ptr.h.txt#L340) [unordered_map:899](sources/llvm/unordered_map.txt#L899) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/unordered_map.h:43–58](sources/gnu14/bits/unordered_map.h.txt#L43)

```cpp
  /// Base types for unordered_map.
  template<bool _Cache>
    using __umap_traits = __detail::_Hashtable_traits<_Cache, false, true>;

  template<typename _Key,
	   typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = std::equal_to<_Key>,
	   typename _Alloc = std::allocator<std::pair<const _Key, _Tp> >,
	   typename _Tr = __umap_traits<__cache_default<_Key, _Hash>::value>>
    using __umap_hashtable = _Hashtable<_Key, std::pair<const _Key, _Tp>,
                                        _Alloc, __detail::_Select1st,
				        _Pred, _Hash,
				        __detail::_Mod_range_hashing,
				        __detail::_Default_ranged_hash,
				        __detail::_Prime_rehash_policy, _Tr>;
```

[codes/gcc/libstdc++-v3/include/bits/unordered_map.h:105–112](sources/gnu14/bits/unordered_map.h.txt#L105)

```cpp
  template<typename _Key, typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = equal_to<_Key>,
	   typename _Alloc = allocator<std::pair<const _Key, _Tp>>>
    class unordered_map
    {
      typedef __umap_hashtable<_Key, _Tp, _Hash, _Pred, _Alloc>  _Hashtable;
      _Hashtable _M_h;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable.h:47–52](sources/gnu14/bits/hashtable.h.txt#L47)

```cpp
  template<typename _Tp, typename _Hash>
    using __cache_default
      =  __not_<__and_<// Do not cache for fast hasher.
		       __is_fast_hash<_Hash>,
		       // Mandatory to have erase not throwing.
		       __is_nothrow_invocable<const _Hash&, const _Tp&>>>;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable.h:391–404](sources/gnu14/bits/hashtable.h.txt#L391)

```cpp
    private:
      __buckets_ptr		_M_buckets		= &_M_single_bucket;
      size_type			_M_bucket_count		= 1;
      __node_base		_M_before_begin;
      size_type			_M_element_count	= 0;
      _RehashPolicy		_M_rehash_policy;

      // A single bucket used when only need for 1 bucket. Especially
      // interesting in move semantic to leave hashtable with only 1 bucket
      // which is not allocated so that we can have those operations noexcept
      // qualified.
      // Note that we can't leave hashtable with 0 bucket without adding
      // numerous checks in the code to avoid 0 modulus.
      __node_base_ptr		_M_single_bucket	= nullptr;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:315–318](sources/gnu14/bits/hashtable_policy.h.txt#L315)

```cpp
  struct _Hash_node_base
  {
    _Hash_node_base* _M_nxt;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:329–334](sources/gnu14/bits/hashtable_policy.h.txt#L329)

```cpp
  template<typename _Value>
    struct _Hash_node_value_base
    {
      typedef _Value value_type;

      __gnu_cxx::__aligned_buffer<_Value> _M_storage;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:360–384](sources/gnu14/bits/hashtable_policy.h.txt#L360)

```cpp
  template<bool _Cache_hash_code>
    struct _Hash_node_code_cache
    { };

  /**
   *  Specialization for node with cache, struct _Hash_node_code_cache.
   */
  template<>
    struct _Hash_node_code_cache<true>
    { std::size_t  _M_hash_code; };

  template<typename _Value, bool _Cache_hash_code>
    struct _Hash_node_value
    : _Hash_node_value_base<_Value>
    , _Hash_node_code_cache<_Cache_hash_code>
    { };

  /**
   *  Primary template struct _Hash_node.
   */
  template<typename _Value, bool _Cache_hash_code>
    struct _Hash_node
    : _Hash_node_base
    , _Hash_node_value<_Value, _Cache_hash_code>
    {
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:544–551](sources/gnu14/bits/hashtable_policy.h.txt#L544)

```cpp
  /// Default value for rehash policy.  Bucket size is (usually) the
  /// smallest prime that keeps the load factor small enough.
  struct _Prime_rehash_policy
  {
    using __has_load_factor = true_type;

    _Prime_rehash_policy(float __z = 1.0) noexcept
    : _M_max_load_factor(__z), _M_next_resize(0) { }
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:588–592](sources/gnu14/bits/hashtable_policy.h.txt#L588)

```cpp
    static const std::size_t _S_growth_factor = 2;

    float		_M_max_load_factor;
    mutable std::size_t	_M_next_resize;
  };
```

[codes/gcc/libstdc++-v3/include/bits/functional_hash.h:114–122](sources/gnu14/bits/functional_hash.h.txt#L114)

```cpp
  // Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp) 	\
  template<>						\
    struct hash<_Tp> : public __hash_base<size_t, _Tp>  \
    {                                                   \
      size_t                                            \
      operator()(_Tp __val) const noexcept              \
      { return static_cast<size_t>(__val); }            \
    };
```

[codes/gcc/libstdc++-v3/include/bits/functional_hash.h:153–154](sources/gnu14/bits/functional_hash.h.txt#L153)

```cpp
  /// Explicit specialization for int.
  _Cxx_hashtable_define_trivial_hash(int)
```

[codes/gcc/libstdc++-v3/include/bits/functional_hash.h:286–299](sources/gnu14/bits/functional_hash.h.txt#L286)

```cpp
   * will cache the hash code.
   * The default behavior is to consider that hashers are fast unless specified
   * otherwise.
   *
   * Users can specialize this for their own hash functions in order to force
   * caching of hash codes in unordered containers. Specializing this trait
   * affects the ABI of the unordered containers, so use it carefully.
   */
  template<typename _Hash>
    struct __is_fast_hash : public std::true_type
    { };

  template<>
    struct __is_fast_hash<hash<long double>> : public std::false_type
```

[codes/gcc/libstdc++-v3/include/bits/hashtable.h:128–146](sources/gnu14/bits/hashtable.h.txt#L128)

```cpp
   *  The non-empty buckets contain the node before the first node in the
   *  bucket. This design makes it possible to implement something like a
   *  std::forward_list::insert_after on container insertion and
   *  std::forward_list::erase_after on container erase
   *  calls. _M_before_begin is equivalent to
   *  std::forward_list::before_begin. Empty buckets contain
   *  nullptr.  Note that one of the non-empty buckets contains
   *  &_M_before_begin which is not a dereferenceable node so the
   *  node pointer in a bucket shall never be dereferenced, only its
   *  next node can be.
   *
   *  Walking through a bucket's nodes requires a check on the hash code to
   *  see if each node is still in the bucket. Such a design assumes a
   *  quite efficient hash functor and is one of the reasons it is
   *  highly advisable to set __cache_hash_code to true.
   *
   *  The container iterators are simply built from nodes. This way
   *  incrementing the iterator is perfectly efficient independent of
   *  how many empty buckets there are in the container.
```

[codes/llvm/libcxx/include/unordered_map:899–905](sources/llvm/unordered_map.txt#L899)

```cpp
  typedef __hash_value_type<key_type, mapped_type> __value_type;
  typedef __unordered_map_hasher<key_type, value_type, hasher, key_equal> __hasher;
  typedef __unordered_map_equal<key_type, value_type, key_equal, hasher> __key_equal;

  typedef __hash_table<__value_type, __hasher, __key_equal, allocator_type> __table;

  __table __table_;
```

[codes/llvm/libcxx/include/__hash_table:79–86](sources/llvm/__hash_table.txt#L79)

```cpp
struct __hash_node_base {
  typedef typename pointer_traits<_NodePtr>::element_type __node_type;
  typedef __hash_node_base __first_node;
  typedef __rebind_pointer_t<_NodePtr, __first_node> __node_base_pointer;
  typedef _NodePtr __node_pointer;
  typedef __node_base_pointer __next_pointer;

  __next_pointer __next_;
```

[codes/llvm/libcxx/include/__hash_table:129–143](sources/llvm/__hash_table.txt#L129)

```cpp
struct __hash_node : public __hash_node_base< __rebind_pointer_t<_VoidPtr, __hash_node<_Tp, _VoidPtr> > > {
  using __node_value_type _LIBCPP_NODEBUG = __get_hash_node_value_type_t<_Tp>;
  using _Base _LIBCPP_NODEBUG          = __hash_node_base<__rebind_pointer_t<_VoidPtr, __hash_node<_Tp, _VoidPtr> > >;
  using __next_pointer _LIBCPP_NODEBUG = typename _Base::__next_pointer;

  size_t __hash_;

  // We allow starting the lifetime of nodes without initializing the value held by the node,
  // since that is handled by the hash table itself in order to be allocator-aware.
#ifndef _LIBCPP_CXX03_LANG

private:
  union {
    __node_value_type __value_;
  };
```

[codes/llvm/libcxx/include/__hash_table:507–519](sources/llvm/__hash_table.txt#L507)

```cpp
template <class _Alloc>
class __bucket_list_deallocator {
  typedef _Alloc allocator_type;
  typedef allocator_traits<allocator_type> __alloc_traits;
  typedef typename __alloc_traits::size_type size_type;

  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, allocator_type, __alloc_);

public:
  typedef typename __alloc_traits::pointer pointer;

  _LIBCPP_HIDE_FROM_ABI __bucket_list_deallocator() _NOEXCEPT_(is_nothrow_default_constructible<allocator_type>::value)
      : __size_(0) {}
```

[codes/llvm/libcxx/include/__hash_table:663–675](sources/llvm/__hash_table.txt#L663)

```cpp
private:
  typedef __rebind_alloc<__node_traits, __next_pointer> __pointer_allocator;
  typedef __bucket_list_deallocator<__pointer_allocator> __bucket_list_deleter;
  typedef unique_ptr<__next_pointer[], __bucket_list_deleter> __bucket_list;
  typedef allocator_traits<__pointer_allocator> __pointer_alloc_traits;
  typedef typename __bucket_list_deleter::pointer __node_pointer_pointer;

  // --- Member data begin ---
  __bucket_list __bucket_list_;
  _LIBCPP_COMPRESSED_PAIR(__first_node, __first_node_, __node_allocator, __node_alloc_);
  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, hasher, __hasher_);
  _LIBCPP_COMPRESSED_PAIR(float, __max_load_factor_, key_equal, __key_eq_);
  // --- Member data end ---
```

[codes/llvm/libcxx/include/__memory/unique_ptr.h:340–362](sources/llvm/__memory/unique_ptr.h.txt#L340)

```cpp
struct __unique_ptr_array_bounds_stateless {
  __unique_ptr_array_bounds_stateless() = default;
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR explicit __unique_ptr_array_bounds_stateless(size_t) {}

  template <class _Deleter,
            class _Tp,
            __enable_if_t<__is_default_deleter_v<_Deleter> && __has_array_cookie<_Tp>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR bool __in_bounds(_Tp* __ptr, size_t __index) const {
    // In constant expressions, we can't check the array cookie so we just pretend that the index
    // is in-bounds. The compiler catches invalid accesses anyway.
    if (__libcpp_is_constant_evaluated())
      return true;
    size_t __cookie = std::__get_array_cookie(__ptr);
    return __index < __cookie;
  }

  template <class _Deleter,
            class _Tp,
            __enable_if_t<!__is_default_deleter_v<_Deleter> || !__has_array_cookie<_Tp>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR bool __in_bounds(_Tp*, size_t) const {
    return true; // If we don't have an array cookie, we assume the access is in-bounds
  }
};
```

[codes/llvm/libcxx/include/__memory/unique_ptr.h:419–425](sources/llvm/__memory/unique_ptr.h.txt#L419)

```cpp
  _LIBCPP_COMPRESSED_PAIR(pointer, __ptr_, deleter_type, __deleter_);
#ifdef _LIBCPP_ABI_BOUNDED_UNIQUE_PTR
  using _BoundsChecker _LIBCPP_NODEBUG = __unique_ptr_array_bounds_stored;
#else
  using _BoundsChecker _LIBCPP_NODEBUG = __unique_ptr_array_bounds_stateless;
#endif
  _LIBCPP_NO_UNIQUE_ADDRESS _BoundsChecker __checker_;
```

[codes/llvm/libcxx/include/__hash_table:167–170](sources/llvm/__hash_table.txt#L167)

```cpp
inline _LIBCPP_HIDE_FROM_ABI bool __is_hash_power2(size_t __bc) { return __bc > 2 && !(__bc & (__bc - 1)); }

inline _LIBCPP_HIDE_FROM_ABI size_t __constrain_hash(size_t __h, size_t __bc) {
  return !(__bc & (__bc - 1)) ? __h & (__bc - 1) : (__h < __bc ? __h : __h % __bc);
```

**What causes the difference**:

GNU additionally stores a next-resize threshold and an embedded single-bucket pointer, 8 bytes each. Other management state is packaged differently but has the same byte contribution. These 16 bytes explain the wrapper difference; buckets index groups of hashed keys and are not the element values.

**Design rationale**:

GNU explicitly explains the embedded bucket: it leaves a non-allocating one-bucket moved-from state, supporting nonthrowing moves without pervasive modulus-by-zero checks. Its policy comments describe usually-prime bucket counts and the time/space tradeoff of cached hashes. LLVM’s deleter stores the bucket count for deallocation, a source-visible use; no public rationale was found for this field organization or always caching node hashes in these instantiations (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Removing/reordering state requires coordinated changes to rehash, move, empty-table and deallocation logic, breaking published ABI. Node protocols still differ: GNU’s fast, nonthrowing default `hash<int>` disables cached hashes, giving next pointer 8 plus value (`pair<const int,int>`, 8 bytes); LLVM stores next pointer 8, hash 8, then value. Complete node `sizeof` is `NOT_OBSERVED`. GNU range hashing uses modulus; LLVM can mask a power-of-two bucket count. Matching the wrapper cannot make lookup/erase use the other node protocol. Both remain subject to complexity, rehash and invalidation requirements in [N4861 unord.req](https://timsong-cpp.github.io/cppwp/n4861/unord.req); different standard guarantees are not established. Exhaustive exception/invalidation execution is `NOT_OBSERVED`.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::unordered_set<int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 56 | 40 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | One `_M_h`: bucket-array pointer 8, bucket count 8, before-begin sentinel/next pointer 8, element count 8, prime-rehash policy 16 (float load factor 4 + padding 4 + size_t next-resize threshold 8), embedded single-bucket pointer 8. Default hash/equality/allocator bases add no storage. [bits/hashtable.h:47](sources/gnu14/bits/hashtable.h.txt#L47) [bits/hashtable_policy.h:315](sources/gnu14/bits/hashtable_policy.h.txt#L315) [bits/unordered_set.h:43](sources/gnu14/bits/unordered_set.h.txt#L43) | One `__table_`: bucket-array owner 16 (array pointer 8, deleter’s bucket count 8 with compressed empty allocator; stateless bounds checker adds no storage); sentinel/next pointer 8 with empty node allocator; size 8 with empty hasher; float load factor 4 with empty equality object; trailing padding 4. Total 40. [__hash_table:79](sources/llvm/__hash_table.txt#L79) [__memory/unique_ptr.h:340](sources/llvm/__memory/unique_ptr.h.txt#L340) [unordered_set:608](sources/llvm/unordered_set.txt#L608) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/unordered_set.h:43–56](sources/gnu14/bits/unordered_set.h.txt#L43)

```cpp
  /// Base types for unordered_set.
  template<bool _Cache>
    using __uset_traits = __detail::_Hashtable_traits<_Cache, true, true>;

  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = std::equal_to<_Value>,
  	   typename _Alloc = std::allocator<_Value>,
	   typename _Tr = __uset_traits<__cache_default<_Value, _Hash>::value>>
    using __uset_hashtable = _Hashtable<_Value, _Value, _Alloc,
					__detail::_Identity, _Pred, _Hash,
					__detail::_Mod_range_hashing,
					__detail::_Default_ranged_hash,
					__detail::_Prime_rehash_policy, _Tr>;
```

[codes/gcc/libstdc++-v3/include/bits/unordered_set.h:100–107](sources/gnu14/bits/unordered_set.h.txt#L100)

```cpp
  template<typename _Value,
	   typename _Hash = hash<_Value>,
	   typename _Pred = equal_to<_Value>,
	   typename _Alloc = allocator<_Value>>
    class unordered_set
    {
      typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
      _Hashtable _M_h;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable.h:47–52](sources/gnu14/bits/hashtable.h.txt#L47)

```cpp
  template<typename _Tp, typename _Hash>
    using __cache_default
      =  __not_<__and_<// Do not cache for fast hasher.
		       __is_fast_hash<_Hash>,
		       // Mandatory to have erase not throwing.
		       __is_nothrow_invocable<const _Hash&, const _Tp&>>>;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable.h:391–404](sources/gnu14/bits/hashtable.h.txt#L391)

```cpp
    private:
      __buckets_ptr		_M_buckets		= &_M_single_bucket;
      size_type			_M_bucket_count		= 1;
      __node_base		_M_before_begin;
      size_type			_M_element_count	= 0;
      _RehashPolicy		_M_rehash_policy;

      // A single bucket used when only need for 1 bucket. Especially
      // interesting in move semantic to leave hashtable with only 1 bucket
      // which is not allocated so that we can have those operations noexcept
      // qualified.
      // Note that we can't leave hashtable with 0 bucket without adding
      // numerous checks in the code to avoid 0 modulus.
      __node_base_ptr		_M_single_bucket	= nullptr;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:315–318](sources/gnu14/bits/hashtable_policy.h.txt#L315)

```cpp
  struct _Hash_node_base
  {
    _Hash_node_base* _M_nxt;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:329–334](sources/gnu14/bits/hashtable_policy.h.txt#L329)

```cpp
  template<typename _Value>
    struct _Hash_node_value_base
    {
      typedef _Value value_type;

      __gnu_cxx::__aligned_buffer<_Value> _M_storage;
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:360–384](sources/gnu14/bits/hashtable_policy.h.txt#L360)

```cpp
  template<bool _Cache_hash_code>
    struct _Hash_node_code_cache
    { };

  /**
   *  Specialization for node with cache, struct _Hash_node_code_cache.
   */
  template<>
    struct _Hash_node_code_cache<true>
    { std::size_t  _M_hash_code; };

  template<typename _Value, bool _Cache_hash_code>
    struct _Hash_node_value
    : _Hash_node_value_base<_Value>
    , _Hash_node_code_cache<_Cache_hash_code>
    { };

  /**
   *  Primary template struct _Hash_node.
   */
  template<typename _Value, bool _Cache_hash_code>
    struct _Hash_node
    : _Hash_node_base
    , _Hash_node_value<_Value, _Cache_hash_code>
    {
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:544–551](sources/gnu14/bits/hashtable_policy.h.txt#L544)

```cpp
  /// Default value for rehash policy.  Bucket size is (usually) the
  /// smallest prime that keeps the load factor small enough.
  struct _Prime_rehash_policy
  {
    using __has_load_factor = true_type;

    _Prime_rehash_policy(float __z = 1.0) noexcept
    : _M_max_load_factor(__z), _M_next_resize(0) { }
```

[codes/gcc/libstdc++-v3/include/bits/hashtable_policy.h:588–592](sources/gnu14/bits/hashtable_policy.h.txt#L588)

```cpp
    static const std::size_t _S_growth_factor = 2;

    float		_M_max_load_factor;
    mutable std::size_t	_M_next_resize;
  };
```

[codes/gcc/libstdc++-v3/include/bits/functional_hash.h:114–122](sources/gnu14/bits/functional_hash.h.txt#L114)

```cpp
  // Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp) 	\
  template<>						\
    struct hash<_Tp> : public __hash_base<size_t, _Tp>  \
    {                                                   \
      size_t                                            \
      operator()(_Tp __val) const noexcept              \
      { return static_cast<size_t>(__val); }            \
    };
```

[codes/gcc/libstdc++-v3/include/bits/functional_hash.h:153–154](sources/gnu14/bits/functional_hash.h.txt#L153)

```cpp
  /// Explicit specialization for int.
  _Cxx_hashtable_define_trivial_hash(int)
```

[codes/gcc/libstdc++-v3/include/bits/functional_hash.h:286–299](sources/gnu14/bits/functional_hash.h.txt#L286)

```cpp
   * will cache the hash code.
   * The default behavior is to consider that hashers are fast unless specified
   * otherwise.
   *
   * Users can specialize this for their own hash functions in order to force
   * caching of hash codes in unordered containers. Specializing this trait
   * affects the ABI of the unordered containers, so use it carefully.
   */
  template<typename _Hash>
    struct __is_fast_hash : public std::true_type
    { };

  template<>
    struct __is_fast_hash<hash<long double>> : public std::false_type
```

[codes/gcc/libstdc++-v3/include/bits/hashtable.h:128–146](sources/gnu14/bits/hashtable.h.txt#L128)

```cpp
   *  The non-empty buckets contain the node before the first node in the
   *  bucket. This design makes it possible to implement something like a
   *  std::forward_list::insert_after on container insertion and
   *  std::forward_list::erase_after on container erase
   *  calls. _M_before_begin is equivalent to
   *  std::forward_list::before_begin. Empty buckets contain
   *  nullptr.  Note that one of the non-empty buckets contains
   *  &_M_before_begin which is not a dereferenceable node so the
   *  node pointer in a bucket shall never be dereferenced, only its
   *  next node can be.
   *
   *  Walking through a bucket's nodes requires a check on the hash code to
   *  see if each node is still in the bucket. Such a design assumes a
   *  quite efficient hash functor and is one of the reasons it is
   *  highly advisable to set __cache_hash_code to true.
   *
   *  The container iterators are simply built from nodes. This way
   *  incrementing the iterator is perfectly efficient independent of
   *  how many empty buckets there are in the container.
```

[codes/llvm/libcxx/include/unordered_set:608–611](sources/llvm/unordered_set.txt#L608)

```cpp
private:
  typedef __hash_table<value_type, hasher, key_equal, allocator_type> __table;

  __table __table_;
```

[codes/llvm/libcxx/include/__hash_table:79–86](sources/llvm/__hash_table.txt#L79)

```cpp
struct __hash_node_base {
  typedef typename pointer_traits<_NodePtr>::element_type __node_type;
  typedef __hash_node_base __first_node;
  typedef __rebind_pointer_t<_NodePtr, __first_node> __node_base_pointer;
  typedef _NodePtr __node_pointer;
  typedef __node_base_pointer __next_pointer;

  __next_pointer __next_;
```

[codes/llvm/libcxx/include/__hash_table:129–143](sources/llvm/__hash_table.txt#L129)

```cpp
struct __hash_node : public __hash_node_base< __rebind_pointer_t<_VoidPtr, __hash_node<_Tp, _VoidPtr> > > {
  using __node_value_type _LIBCPP_NODEBUG = __get_hash_node_value_type_t<_Tp>;
  using _Base _LIBCPP_NODEBUG          = __hash_node_base<__rebind_pointer_t<_VoidPtr, __hash_node<_Tp, _VoidPtr> > >;
  using __next_pointer _LIBCPP_NODEBUG = typename _Base::__next_pointer;

  size_t __hash_;

  // We allow starting the lifetime of nodes without initializing the value held by the node,
  // since that is handled by the hash table itself in order to be allocator-aware.
#ifndef _LIBCPP_CXX03_LANG

private:
  union {
    __node_value_type __value_;
  };
```

[codes/llvm/libcxx/include/__hash_table:507–519](sources/llvm/__hash_table.txt#L507)

```cpp
template <class _Alloc>
class __bucket_list_deallocator {
  typedef _Alloc allocator_type;
  typedef allocator_traits<allocator_type> __alloc_traits;
  typedef typename __alloc_traits::size_type size_type;

  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, allocator_type, __alloc_);

public:
  typedef typename __alloc_traits::pointer pointer;

  _LIBCPP_HIDE_FROM_ABI __bucket_list_deallocator() _NOEXCEPT_(is_nothrow_default_constructible<allocator_type>::value)
      : __size_(0) {}
```

[codes/llvm/libcxx/include/__hash_table:663–675](sources/llvm/__hash_table.txt#L663)

```cpp
private:
  typedef __rebind_alloc<__node_traits, __next_pointer> __pointer_allocator;
  typedef __bucket_list_deallocator<__pointer_allocator> __bucket_list_deleter;
  typedef unique_ptr<__next_pointer[], __bucket_list_deleter> __bucket_list;
  typedef allocator_traits<__pointer_allocator> __pointer_alloc_traits;
  typedef typename __bucket_list_deleter::pointer __node_pointer_pointer;

  // --- Member data begin ---
  __bucket_list __bucket_list_;
  _LIBCPP_COMPRESSED_PAIR(__first_node, __first_node_, __node_allocator, __node_alloc_);
  _LIBCPP_COMPRESSED_PAIR(size_type, __size_, hasher, __hasher_);
  _LIBCPP_COMPRESSED_PAIR(float, __max_load_factor_, key_equal, __key_eq_);
  // --- Member data end ---
```

[codes/llvm/libcxx/include/__memory/unique_ptr.h:340–362](sources/llvm/__memory/unique_ptr.h.txt#L340)

```cpp
struct __unique_ptr_array_bounds_stateless {
  __unique_ptr_array_bounds_stateless() = default;
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR explicit __unique_ptr_array_bounds_stateless(size_t) {}

  template <class _Deleter,
            class _Tp,
            __enable_if_t<__is_default_deleter_v<_Deleter> && __has_array_cookie<_Tp>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR bool __in_bounds(_Tp* __ptr, size_t __index) const {
    // In constant expressions, we can't check the array cookie so we just pretend that the index
    // is in-bounds. The compiler catches invalid accesses anyway.
    if (__libcpp_is_constant_evaluated())
      return true;
    size_t __cookie = std::__get_array_cookie(__ptr);
    return __index < __cookie;
  }

  template <class _Deleter,
            class _Tp,
            __enable_if_t<!__is_default_deleter_v<_Deleter> || !__has_array_cookie<_Tp>::value, int> = 0>
  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR bool __in_bounds(_Tp*, size_t) const {
    return true; // If we don't have an array cookie, we assume the access is in-bounds
  }
};
```

[codes/llvm/libcxx/include/__memory/unique_ptr.h:419–425](sources/llvm/__memory/unique_ptr.h.txt#L419)

```cpp
  _LIBCPP_COMPRESSED_PAIR(pointer, __ptr_, deleter_type, __deleter_);
#ifdef _LIBCPP_ABI_BOUNDED_UNIQUE_PTR
  using _BoundsChecker _LIBCPP_NODEBUG = __unique_ptr_array_bounds_stored;
#else
  using _BoundsChecker _LIBCPP_NODEBUG = __unique_ptr_array_bounds_stateless;
#endif
  _LIBCPP_NO_UNIQUE_ADDRESS _BoundsChecker __checker_;
```

[codes/llvm/libcxx/include/__hash_table:167–170](sources/llvm/__hash_table.txt#L167)

```cpp
inline _LIBCPP_HIDE_FROM_ABI bool __is_hash_power2(size_t __bc) { return __bc > 2 && !(__bc & (__bc - 1)); }

inline _LIBCPP_HIDE_FROM_ABI size_t __constrain_hash(size_t __h, size_t __bc) {
  return !(__bc & (__bc - 1)) ? __h & (__bc - 1) : (__h < __bc ? __h : __h % __bc);
```

**What causes the difference**:

GNU additionally stores a next-resize threshold and an embedded single-bucket pointer, 8 bytes each. Other management state is packaged differently but has the same byte contribution. These 16 bytes explain the wrapper difference; buckets index groups of hashed keys and are not the element values.

**Design rationale**:

GNU explicitly explains the embedded bucket: it leaves a non-allocating one-bucket moved-from state, supporting nonthrowing moves without pervasive modulus-by-zero checks. Its policy comments describe usually-prime bucket counts and the time/space tradeoff of cached hashes. LLVM’s deleter stores the bucket count for deallocation, a source-visible use; no public rationale was found for this field organization or always caching node hashes in these instantiations (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Removing/reordering state requires coordinated changes to rehash, move, empty-table and deallocation logic, breaking published ABI. Node protocols still differ: GNU’s fast, nonthrowing default `hash<int>` disables cached hashes, giving next pointer 8 plus value (`int`, 4 bytes); LLVM stores next pointer 8, hash 8, then value. Complete node `sizeof` is `NOT_OBSERVED`. GNU range hashing uses modulus; LLVM can mask a power-of-two bucket count. Matching the wrapper cannot make lookup/erase use the other node protocol. Both remain subject to complexity, rehash and invalidation requirements in [N4861 unord.req](https://timsong-cpp.github.io/cppwp/n4861/unord.req); different standard guarantees are not established. Exhaustive exception/invalidation execution is `NOT_OBSERVED`.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::string`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 32 | 24 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | Pointer/allocator wrapper `_M_dataplus` 8 (empty allocator base plus `char*`), length 8, then union 16 (`char[16]` overlapping an 8-byte allocated capacity). [bits/basic_string.h:185](sources/gnu14/bits/basic_string.h.txt#L185) | Representation union 24 plus compressed empty allocator. Long mode: one size_type unit with 1-bit long flag and 63-bit encoded capacity (8 bytes total), length 8, `char*` 8. Short mode: 1-bit flag plus 7-bit length (1 byte total), then `char[23]` including terminator. `__padding<0>` adds no storage for char. [string:719](sources/llvm/string.txt#L719) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/bits/basic_string.h:185–186](sources/gnu14/bits/basic_string.h.txt#L185)

```cpp
      // Use empty-base optimization: http://www.cantrip.org/emptyopt.html
      struct _Alloc_hider : allocator_type // TODO check __is_final
```

[codes/gcc/libstdc++-v3/include/bits/basic_string.h:201–213](sources/gnu14/bits/basic_string.h.txt#L201)

```cpp
	pointer _M_p; // The actual data.
      };

      _Alloc_hider	_M_dataplus;
      size_type		_M_string_length;

      enum { _S_local_capacity = 15 / sizeof(_CharT) };

      union
      {
	_CharT           _M_local_buf[_S_local_capacity + 1];
	size_type        _M_allocated_capacity;
      };
```

[codes/gcc/libstdc++-v3/include/bits/basic_string.h:265–276](sources/gnu14/bits/basic_string.h.txt#L265)

```cpp
      _GLIBCXX20_CONSTEXPR
      bool
      _M_is_local() const
      {
	if (_M_data() == _M_local_data())
	  {
	    if (_M_string_length > _S_local_capacity)
	      __builtin_unreachable();
	    return true;
	  }
	return false;
      }
```

[codes/llvm/libcxx/include/string:719–725](sources/llvm/string.txt#L719)

```cpp
template <size_t _PaddingSize>
struct __padding {
  char __padding_[_PaddingSize];
};

template <>
struct __padding<0> {};
```

[codes/llvm/libcxx/include/string:864–875](sources/llvm/string.txt#L864)

```cpp
#  else // _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT

#    ifdef _LIBCPP_BIG_ENDIAN
  static const size_type __endian_factor = 1;
#    else
  static const size_type __endian_factor = 2;
#    endif

  // Attribute 'packed' is used to keep the layout compatible with the
  // previous definition that did not use bit fields. This is because on
  // some platforms bit fields have a default size rather than the actual
  // size used, e.g., it is 4 bytes on AIX. See D128285 for details.
```

[codes/llvm/libcxx/include/string:884–909](sources/llvm/string.txt#L884)

```cpp
    struct _LIBCPP_PACKED {
      size_type __is_long_ : 1;
      size_type __cap_ : sizeof(size_type) * CHAR_BIT - 1;
    };
    size_type __size_;
    pointer __data_;
  };

  enum { __min_cap = (sizeof(__long) - 1) / sizeof(value_type) > 2 ? (sizeof(__long) - 1) / sizeof(value_type) : 2 };

  struct __short {
    struct _LIBCPP_PACKED {
      unsigned char __is_long_ : 1;
      unsigned char __size_    : 7;
    };
    _LIBCPP_NO_UNIQUE_ADDRESS __padding<sizeof(value_type) - 1> __padding_;
    value_type __data_[__min_cap];
  };

#  endif // _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT

  static_assert(sizeof(__short) == (sizeof(value_type) * (__min_cap + 1)), "__short has an unexpected size.");

  union __rep {
    __short __s;
    __long __l;
```

[codes/llvm/libcxx/include/string:917–917](sources/llvm/string.txt#L917)

```cpp
  _LIBCPP_COMPRESSED_PAIR(__rep, __rep_, allocator_type, __alloc_);
```

[codes/llvm/libcxx/include/string:845–856](sources/llvm/string.txt#L845)

```cpp
  // The __endian_factor is required because the field we use to store the size
  // has one fewer bit than it would if it were not a bitfield.
  //
  // If the LSB is used to store the short-flag in the short string representation,
  // we have to multiply the size by two when it is stored and divide it by two when
  // it is loaded to make sure that we always store an even number. In the long string
  // representation, we can ignore this because we can assume that we always allocate
  // an even amount of value_types.
  //
  // If the MSB is used for the short-flag, the max_size() is numeric_limits<size_type>::max() / 2.
  // This does not impact the short string representation, since we never need the MSB
  // for representing the size of a short string anyway.
```

**What causes the difference**:

GNU always keeps a pointer and length, plus 16 bytes usable for short text. LLVM overlays the entire 24-byte representation between long and short modes, distinguished by one bit. Its object is 8 bytes smaller yet holds up to 22 short characters versus GNU’s 15, excluding the terminator.

**Design rationale**:

[GNU’s Dual ABI documentation](https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html) explains replacing copy-on-write strings for C++11 conformance while retaining old binary support; this does not explain the exact 32/24-byte choice. LLVM’s comments explain the flag/capacity encoding. [D128285](https://reviews.llvm.org/D128285) explains packing as an AIX ABI repair after adopting bit-fields, not the original x86_64 layout. No public rationale was found for the original 15/22-character capacities and complete representations (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Rewriting both representations changes short capacity, pointer interpretation, encoded capacity, destruction and every published embedding layout. GNU identifies short mode by a self-buffer pointer; LLVM uses a flag. Padding to equal size cannot make either read the other’s object. GNU `std::__cxx11` and LLVM `std::__1` also have different symbol identities; GNU’s dual-ABI switch is not a cross-library converter. Allocation timing/capacity can differ. Full growth, exception and invalidation behavior is `NOT_OBSERVED`; different standard string semantics are not asserted.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::future<int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 16 | 8 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | The base `__basic_future` contains only `_M_state`, a 16-byte shared_ptr: state pointer `_M_ptr` 8 then ownership handle `_M_refcount` 8, which contains control pointer `_M_pi`. [std/future:700](sources/gnu14/std/future.txt#L700) [bits/shared_ptr_base.h:1135](sources/gnu14/bits/shared_ptr_base.h.txt#L1135) | Only `__assoc_state<int>* __state_`, 8 bytes. The pointee carries reference counting through `__shared_count`; a smaller handle does not mean no ownership accounting. [future:918](sources/llvm/future.txt#L918) [__memory/shared_count.h:44](sources/llvm/__memory/shared_count.h.txt#L44) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/std/future:700–709](sources/gnu14/std/future.txt#L700)

```cpp
  /// Common implementation for future and shared_future.
  template<typename _Res>
    class __basic_future : public __future_base
    {
    protected:
      typedef shared_ptr<_State_base>		__state_type;
      typedef __future_base::_Result<_Res>&	__result_type;

    private:
      __state_type 		_M_state;
```

[codes/gcc/libstdc++-v3/include/std/future:790–808](sources/gnu14/std/future.txt#L790)

```cpp
  /// Primary template for future.
  template<typename _Res>
    class future : public __basic_future<_Res>
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 3458. Is shared_future intended to work with arrays or function types?
      static_assert(!is_array<_Res>{}, "result type must not be an array");
      static_assert(!is_function<_Res>{}, "result type must not be a function");
      static_assert(is_destructible<_Res>{},
		    "result type must be destructible");

      friend class promise<_Res>;
      template<typename> friend class packaged_task;
      template<typename _Fn, typename... _Args>
        friend future<__async_result_of<_Fn, _Args...>>
        async(launch, _Fn&&, _Args&&...);

      typedef __basic_future<_Res> _Base_type;
      typedef typename _Base_type::__state_type __state_type;
```

[codes/gcc/libstdc++-v3/include/bits/shared_ptr_base.h:1135–1136](sources/gnu14/bits/shared_ptr_base.h.txt#L1135)

```cpp

      _Sp_counted_base<_Lp>*  _M_pi;
```

[codes/gcc/libstdc++-v3/include/bits/shared_ptr_base.h:1789–1790](sources/gnu14/bits/shared_ptr_base.h.txt#L1789)

```cpp
      element_type*	   _M_ptr;         // Contained pointer.
      __shared_count<_Lp>  _M_refcount;    // Reference counter.
```

[codes/gcc/libstdc++-v3/include/bits/shared_ptr_base.h:232–238](sources/gnu14/bits/shared_ptr_base.h.txt#L232)

```cpp

    private:
      _Sp_counted_base(_Sp_counted_base const&) = delete;
      _Sp_counted_base& operator=(_Sp_counted_base const&) = delete;

      _Atomic_word  _M_use_count;     // #shared
      _Atomic_word  _M_weak_count;    // #weak + (#shared != 0)
```

[codes/gcc/libstdc++-v3/include/std/future:224–246](sources/gnu14/std/future.txt#L224)

```cpp
    struct _Result_base
    {
      exception_ptr		_M_error;

      _Result_base(const _Result_base&) = delete;
      _Result_base& operator=(const _Result_base&) = delete;

      // _M_destroy() allows derived classes to control deallocation
      virtual void _M_destroy() = 0;

      struct _Deleter
      {
	void operator()(_Result_base* __fr) const { __fr->_M_destroy(); }
      };

    protected:
      _Result_base();
      virtual ~_Result_base();
    };

    /// A unique_ptr for result objects.
    template<typename _Res>
      using _Ptr = unique_ptr<_Res, _Result_base::_Deleter>;
```

[codes/gcc/libstdc++-v3/include/std/future:248–254](sources/gnu14/std/future.txt#L248)

```cpp
    /// A result object that has storage for an object of type _Res.
    template<typename _Res>
      struct _Result : _Result_base
      {
      private:
	__gnu_cxx::__aligned_buffer<_Res>	_M_storage;
	bool 					_M_initialized;
```

[codes/gcc/libstdc++-v3/include/std/future:329–351](sources/gnu14/std/future.txt#L329)

```cpp
    // Base class for various types of shared state created by an
    // asynchronous provider (such as a std::promise) and shared with one
    // or more associated futures.
    class _State_baseV2
    {
      typedef _Ptr<_Result_base> _Ptr_type;

      enum _Status : unsigned {
	__not_ready,
	__ready
      };

      _Ptr_type			_M_result;
      __atomic_futex_unsigned<>	_M_status;
      atomic_flag         	_M_retrieved = ATOMIC_FLAG_INIT;
      once_flag			_M_once;

    public:
      _State_baseV2() noexcept : _M_result(), _M_status(_Status::__not_ready)
	{ }
      _State_baseV2(const _State_baseV2&) = delete;
      _State_baseV2& operator=(const _State_baseV2&) = delete;
      virtual ~_State_baseV2() = default;
```

[codes/gcc/libstdc++-v3/include/bits/atomic_futex.h:50–52](sources/gnu14/bits/atomic_futex.h.txt#L50)

```cpp
#ifdef _GLIBCXX_HAS_GTHREADS
#if defined(_GLIBCXX_HAVE_LINUX_FUTEX) && ATOMIC_INT_LOCK_FREE > 1
  struct __atomic_futex_unsigned_base
```

[codes/gcc/libstdc++-v3/include/bits/atomic_futex.h:70–76](sources/gnu14/bits/atomic_futex.h.txt#L70)

```cpp
  template <unsigned _Waiter_bit = 0x80000000>
  class __atomic_futex_unsigned : __atomic_futex_unsigned_base
  {
    typedef chrono::steady_clock __clock_t;

    // This must be lock-free and at offset 0.
    atomic<unsigned> _M_data;
```

[codes/llvm/libcxx/include/future:918–920](sources/llvm/future.txt#L918)

```cpp
template <class _Rp>
class future {
  __assoc_state<_Rp>* __state_;
```

[codes/llvm/libcxx/include/future:965–969](sources/llvm/future.txt#L965)

```cpp

template <class _Rp>
future<_Rp>::future(__assoc_state<_Rp>* __state) : __state_(__state) {
  __state_->__attach_future();
}
```

[codes/llvm/libcxx/include/future:975–988](sources/llvm/future.txt#L975)

```cpp
template <class _Rp>
future<_Rp>::~future() {
  if (__state_)
    __state_->__release_shared();
}

template <class _Rp>
_Rp future<_Rp>::get() {
  unique_ptr<__shared_count, __release_shared_count> __guard(__state_);
  __assoc_state<_Rp>* __s = __state_;
  __state_                = nullptr;
  return __s->move();
}
```

[codes/llvm/libcxx/include/__memory/shared_count.h:44–56](sources/llvm/__memory/shared_count.h.txt#L44)

```cpp
class _LIBCPP_EXPORTED_FROM_ABI __shared_count {
  __shared_count(const __shared_count&);
  __shared_count& operator=(const __shared_count&);

protected:
  long __shared_owners_;
  virtual ~__shared_count();

private:
  virtual void __on_zero_shared() _NOEXCEPT = 0;

public:
  _LIBCPP_HIDE_FROM_ABI explicit __shared_count(long __refs = 0) _NOEXCEPT : __shared_owners_(__refs) {}
```

[codes/llvm/libcxx/include/__memory/shared_count.h:62–76](sources/llvm/__memory/shared_count.h.txt#L62)

```cpp
  _LIBCPP_HIDE_FROM_ABI void __add_shared() _NOEXCEPT { __libcpp_atomic_refcount_increment(__shared_owners_); }
  _LIBCPP_HIDE_FROM_ABI bool __release_shared() _NOEXCEPT {
    if (__libcpp_atomic_refcount_decrement(__shared_owners_) == -1) {
      __on_zero_shared();
      return true;
    }
    return false;
  }
#endif
  _LIBCPP_HIDE_FROM_ABI long use_count() const _NOEXCEPT {
#if _LIBCPP_HAS_THREADS
    return __atomic_load_n(&__shared_owners_, __ATOMIC_RELAXED) + 1;
#else
    return __shared_owners_ + 1;
#endif
```

[codes/llvm/libcxx/include/future:523–547](sources/llvm/future.txt#L523)

```cpp
class _LIBCPP_EXPORTED_FROM_ABI __assoc_sub_state : public __shared_count {
protected:
  exception_ptr __exception_;
  mutable mutex __mut_;
  mutable condition_variable __cv_;
  unsigned __state_;

  void __on_zero_shared() _NOEXCEPT override;
  void __sub_wait(unique_lock<mutex>& __lk);

public:
  enum { __constructed = 1, __future_attached = 2, ready = 4, deferred = 8 };

  _LIBCPP_HIDE_FROM_ABI __assoc_sub_state() : __state_(0) {}

  _LIBCPP_HIDE_FROM_ABI bool __has_value() const { return (__state_ & __constructed) || (__exception_ != nullptr); }

  _LIBCPP_HIDE_FROM_ABI void __attach_future() {
    lock_guard<mutex> __lk(__mut_);
    bool __has_future_attached = (__state_ & __future_attached) != 0;
    if (__has_future_attached)
      std::__throw_future_error(future_errc::future_already_retrieved);
    this->__add_shared();
    __state_ |= __future_attached;
  }
```

[codes/llvm/libcxx/include/future:585–603](sources/llvm/future.txt#L585)

```cpp
template <class _Rp>
class _LIBCPP_HIDDEN __assoc_state : public __assoc_sub_state {
  typedef __assoc_sub_state base;

protected:
  _ALIGNAS_TYPE(_Rp) char __value_[sizeof(_Rp)];

  _LIBCPP_HIDE_FROM_ABI_VIRTUAL void __on_zero_shared() _NOEXCEPT override;

public:
  template <class _Arg>
  _LIBCPP_HIDE_FROM_ABI void set_value(_Arg&& __arg);

  template <class _Arg>
  _LIBCPP_HIDE_FROM_ABI void set_value_at_thread_exit(_Arg&& __arg);

  _LIBCPP_HIDE_FROM_ABI _Rp move();
  _LIBCPP_HIDE_FROM_ABI _Rp& copy();
};
```

[codes/llvm/libcxx/include/future:605–609](sources/llvm/future.txt#L605)

```cpp
template <class _Rp>
void __assoc_state<_Rp>::__on_zero_shared() _NOEXCEPT {
  if (this->__state_ & base::__constructed)
    reinterpret_cast<_Rp*>(std::addressof(__value_))->~_Rp();
  delete this;
```

[codes/llvm/libcxx/include/future:612–620](sources/llvm/future.txt#L612)

```cpp
template <class _Rp>
template <class _Arg>
void __assoc_state<_Rp>::set_value(_Arg&& __arg) {
  unique_lock<mutex> __lk(this->__mut_);
  if (this->__has_value())
    std::__throw_future_error(future_errc::promise_already_satisfied);
  ::new ((void*)std::addressof(__value_)) _Rp(std::forward<_Arg>(__arg));
  this->__state_ |= base::__constructed | base::ready;
  __cv_.notify_all();
```

[codes/llvm/libcxx/include/future:634–640](sources/llvm/future.txt#L634)

```cpp
template <class _Rp>
_Rp __assoc_state<_Rp>::move() {
  unique_lock<mutex> __lk(this->__mut_);
  this->__sub_wait(__lk);
  if (this->__exception_ != nullptr)
    std::rethrow_exception(this->__exception_);
  return std::move(*reinterpret_cast<_Rp*>(std::addressof(__value_)));
```

[codes/gcc/libstdc++-v3/include/std/future:427–440](sources/gnu14/std/future.txt#L427)

```cpp
      // Provide a result to the shared state and make it ready.
      // Calls at most once: _M_result = __res();
      void
      _M_set_result(function<_Ptr_type()> __res, bool __ignore_failure = false)
      {
	bool __did_set = false;
        // all calls to this function are serialized,
        // side-effects of invoking __res only happen once
	call_once(_M_once, &_State_baseV2::_M_do_set, this,
		  std::__addressof(__res), std::__addressof(__did_set));
	if (__did_set)
	  // Use release MO to synchronize with observers of the ready state.
	  _M_status._M_store_notify_all(_Status::__ready,
					memory_order_release);
```

[codes/gcc/libstdc++-v3/include/std/future:484–490](sources/gnu14/std/future.txt#L484)

```cpp
      // Called when this object is first passed to a future.
      void
      _M_set_retrieved_flag()
      {
	if (_M_retrieved.test_and_set())
	  __throw_future_error(int(future_errc::future_already_retrieved));
      }
```

[codes/gcc/libstdc++-v3/include/std/future:505–510](sources/gnu14/std/future.txt#L505)

```cpp
	  // Used by std::promise to copy construct the result.
          typename promise<_Res>::_Ptr_type operator()() const
          {
            _M_promise->_M_storage->_M_set(*_M_arg);
            return std::move(_M_promise->_M_storage);
          }
```

**What causes the difference**:

GNU’s future carries both a state address and an ownership-control address. LLVM carries only the state address and puts ownership accounting in that state. This reduces handle size; it does not prove an 8-byte reduction in the whole asynchronous operation.

Follow the external state as well: GNU shared_ptr contains a state pointer and a separate ownership-control pointer. Its state has a result owner, readiness state, future-retrieved flag and once flag; its result has an exception pointer, 4-byte int storage and 1-byte initialization flag. LLVM combines an 8-byte long ownership count, virtual dispatch, exception pointer (baseline 8), mutex (baseline 40), condition_variable (baseline 48), unsigned flags (4) and int storage (4) in one inheritance chain. A virtual-table pointer is 8 under this ABI. Complete control-block/state sizes, offsets and GNU synchronization-helper sizes are `NOT_OBSERVED`; no total heap footprint is invented.

**Design rationale**:

[N4861 futures.state](https://timsong-cpp.github.io/cppwp/n4861/futures.state) specifies result storage, synchronization and lifetime, without prescribing pointer counts. GNU documents provider/future sharing; LLVM’s reference-count updates are visible in source. No public rationale was found for GNU’s generic shared_ptr versus LLVM’s embedded ownership count (`NOT_AVAILABLE`); no performance motive is inferred.

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Redesigning the handle and shared state breaks published handle, parameter/return and embedding ABI. A matching handle cannot relocate ownership counts, release logic or result extraction. GNU’s source uses a futex helper (Linux address-based wait/wake, with a configurable fallback) and call_once; LLVM uses a mutex and condition variable. Readiness, setting, waiting, destruction, exception_ptr rethrow and error categories require a coherent protocol. Different standard get/wait semantics have not been established; runtime cancellation/exception combinations are `NOT_OBSERVED`.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.

### `std::promise<int>`

| Item | libstdc++ | libc++ |
|---|---|---|
| Size (existing measurement, bytes) | 24 | 8 |
| Alignment (existing measurement, bytes) | 8 | 8 |
| Members (declaration order; static byte accounting) | `shared_ptr<_State> _M_future` 16 (state/control pointers 8 each), then `unique_ptr<_Result<int>, _Deleter> _M_storage` 8 (result pointer; empty deleter adds no storage). [std/future:1097](sources/gnu14/std/future.txt#L1097) [bits/shared_ptr_base.h:1135](sources/gnu14/bits/shared_ptr_base.h.txt#L1135) [bits/unique_ptr.h:224](sources/gnu14/bits/unique_ptr.h.txt#L224) | Only `__assoc_state<int>* __state_`, 8 bytes. Ownership count, result storage and synchronization live in the external state hierarchy. [future:1114](sources/llvm/future.txt#L1114) [future:523](sources/llvm/future.txt#L523) |

**Source (verbatim excerpts):**

[codes/gcc/libstdc++-v3/include/std/future:1097–1110](sources/gnu14/std/future.txt#L1097)

```cpp
      typedef __future_base::_State_base 	_State;
      typedef __future_base::_Result<_Res>	_Res_type;
      typedef __future_base::_Ptr<_Res_type>	_Ptr_type;
      template<typename, typename> friend struct _State::_Setter;
      friend _State;

      shared_ptr<_State>                        _M_future;
      _Ptr_type                                 _M_storage;

    public:
      promise()
      : _M_future(std::make_shared<_State>()),
	_M_storage(new _Res_type())
      { }
```

[codes/gcc/libstdc++-v3/include/std/future:1131–1135](sources/gnu14/std/future.txt#L1131)

```cpp
      ~promise()
      {
        if (static_cast<bool>(_M_future) && !_M_future.unique())
          _M_future->_M_break_promise(std::move(_M_storage));
      }
```

[codes/gcc/libstdc++-v3/include/bits/shared_ptr_base.h:1135–1136](sources/gnu14/bits/shared_ptr_base.h.txt#L1135)

```cpp

      _Sp_counted_base<_Lp>*  _M_pi;
```

[codes/gcc/libstdc++-v3/include/bits/shared_ptr_base.h:1789–1790](sources/gnu14/bits/shared_ptr_base.h.txt#L1789)

```cpp
      element_type*	   _M_ptr;         // Contained pointer.
      __shared_count<_Lp>  _M_refcount;    // Reference counter.
```

[codes/gcc/libstdc++-v3/include/bits/shared_ptr_base.h:232–238](sources/gnu14/bits/shared_ptr_base.h.txt#L232)

```cpp

    private:
      _Sp_counted_base(_Sp_counted_base const&) = delete;
      _Sp_counted_base& operator=(_Sp_counted_base const&) = delete;

      _Atomic_word  _M_use_count;     // #shared
      _Atomic_word  _M_weak_count;    // #weak + (#shared != 0)
```

[codes/gcc/libstdc++-v3/include/bits/unique_ptr.h:224–227](sources/gnu14/bits/unique_ptr.h.txt#L224)

```cpp

    private:
      tuple<pointer, _Dp> _M_t;
    };
```

[codes/gcc/libstdc++-v3/include/bits/unique_ptr.h:272–277](sources/gnu14/bits/unique_ptr.h.txt#L272)

```cpp
    {
      template <typename _Up>
	using _DeleterConstraint =
	  typename __uniq_ptr_impl<_Tp, _Up>::_DeleterConstraint::type;

      __uniq_ptr_data<_Tp, _Dp> _M_t;
```

[codes/gcc/libstdc++-v3/include/std/future:224–246](sources/gnu14/std/future.txt#L224)

```cpp
    struct _Result_base
    {
      exception_ptr		_M_error;

      _Result_base(const _Result_base&) = delete;
      _Result_base& operator=(const _Result_base&) = delete;

      // _M_destroy() allows derived classes to control deallocation
      virtual void _M_destroy() = 0;

      struct _Deleter
      {
	void operator()(_Result_base* __fr) const { __fr->_M_destroy(); }
      };

    protected:
      _Result_base();
      virtual ~_Result_base();
    };

    /// A unique_ptr for result objects.
    template<typename _Res>
      using _Ptr = unique_ptr<_Res, _Result_base::_Deleter>;
```

[codes/gcc/libstdc++-v3/include/std/future:248–254](sources/gnu14/std/future.txt#L248)

```cpp
    /// A result object that has storage for an object of type _Res.
    template<typename _Res>
      struct _Result : _Result_base
      {
      private:
	__gnu_cxx::__aligned_buffer<_Res>	_M_storage;
	bool 					_M_initialized;
```

[codes/gcc/libstdc++-v3/include/std/future:329–351](sources/gnu14/std/future.txt#L329)

```cpp
    // Base class for various types of shared state created by an
    // asynchronous provider (such as a std::promise) and shared with one
    // or more associated futures.
    class _State_baseV2
    {
      typedef _Ptr<_Result_base> _Ptr_type;

      enum _Status : unsigned {
	__not_ready,
	__ready
      };

      _Ptr_type			_M_result;
      __atomic_futex_unsigned<>	_M_status;
      atomic_flag         	_M_retrieved = ATOMIC_FLAG_INIT;
      once_flag			_M_once;

    public:
      _State_baseV2() noexcept : _M_result(), _M_status(_Status::__not_ready)
	{ }
      _State_baseV2(const _State_baseV2&) = delete;
      _State_baseV2& operator=(const _State_baseV2&) = delete;
      virtual ~_State_baseV2() = default;
```

[codes/gcc/libstdc++-v3/include/std/future:427–440](sources/gnu14/std/future.txt#L427)

```cpp
      // Provide a result to the shared state and make it ready.
      // Calls at most once: _M_result = __res();
      void
      _M_set_result(function<_Ptr_type()> __res, bool __ignore_failure = false)
      {
	bool __did_set = false;
        // all calls to this function are serialized,
        // side-effects of invoking __res only happen once
	call_once(_M_once, &_State_baseV2::_M_do_set, this,
		  std::__addressof(__res), std::__addressof(__did_set));
	if (__did_set)
	  // Use release MO to synchronize with observers of the ready state.
	  _M_status._M_store_notify_all(_Status::__ready,
					memory_order_release);
```

[codes/gcc/libstdc++-v3/include/std/future:484–490](sources/gnu14/std/future.txt#L484)

```cpp
      // Called when this object is first passed to a future.
      void
      _M_set_retrieved_flag()
      {
	if (_M_retrieved.test_and_set())
	  __throw_future_error(int(future_errc::future_already_retrieved));
      }
```

[codes/gcc/libstdc++-v3/include/std/future:505–510](sources/gnu14/std/future.txt#L505)

```cpp
	  // Used by std::promise to copy construct the result.
          typename promise<_Res>::_Ptr_type operator()() const
          {
            _M_promise->_M_storage->_M_set(*_M_arg);
            return std::move(_M_promise->_M_storage);
          }
```

[codes/llvm/libcxx/include/future:1114–1127](sources/llvm/future.txt#L1114)

```cpp
template <class _Rp>
class promise {
  __assoc_state<_Rp>* __state_;

  _LIBCPP_HIDE_FROM_ABI explicit promise(nullptr_t) _NOEXCEPT : __state_(nullptr) {}

  template <class>
  friend class packaged_task;

public:
  _LIBCPP_HIDE_FROM_ABI promise();
  template <class _Alloc>
  _LIBCPP_HIDE_FROM_ABI promise(allocator_arg_t, const _Alloc& __a);
  _LIBCPP_HIDE_FROM_ABI promise(promise&& __rhs) _NOEXCEPT : __state_(__rhs.__state_) { __rhs.__state_ = nullptr; }
```

[codes/llvm/libcxx/include/future:1154–1155](sources/llvm/future.txt#L1154)

```cpp
template <class _Rp>
promise<_Rp>::promise() : __state_(new __assoc_state<_Rp>) {}
```

[codes/llvm/libcxx/include/future:1169–1175](sources/llvm/future.txt#L1169)

```cpp
template <class _Rp>
promise<_Rp>::~promise() {
  if (__state_) {
    if (!__state_->__has_value() && __state_->use_count() > 1)
      __state_->set_exception(make_exception_ptr(future_error(make_error_code(future_errc::broken_promise))));
    __state_->__release_shared();
  }
```

[codes/llvm/libcxx/include/__memory/shared_count.h:44–56](sources/llvm/__memory/shared_count.h.txt#L44)

```cpp
class _LIBCPP_EXPORTED_FROM_ABI __shared_count {
  __shared_count(const __shared_count&);
  __shared_count& operator=(const __shared_count&);

protected:
  long __shared_owners_;
  virtual ~__shared_count();

private:
  virtual void __on_zero_shared() _NOEXCEPT = 0;

public:
  _LIBCPP_HIDE_FROM_ABI explicit __shared_count(long __refs = 0) _NOEXCEPT : __shared_owners_(__refs) {}
```

[codes/llvm/libcxx/include/__memory/shared_count.h:62–76](sources/llvm/__memory/shared_count.h.txt#L62)

```cpp
  _LIBCPP_HIDE_FROM_ABI void __add_shared() _NOEXCEPT { __libcpp_atomic_refcount_increment(__shared_owners_); }
  _LIBCPP_HIDE_FROM_ABI bool __release_shared() _NOEXCEPT {
    if (__libcpp_atomic_refcount_decrement(__shared_owners_) == -1) {
      __on_zero_shared();
      return true;
    }
    return false;
  }
#endif
  _LIBCPP_HIDE_FROM_ABI long use_count() const _NOEXCEPT {
#if _LIBCPP_HAS_THREADS
    return __atomic_load_n(&__shared_owners_, __ATOMIC_RELAXED) + 1;
#else
    return __shared_owners_ + 1;
#endif
```

[codes/llvm/libcxx/include/future:523–547](sources/llvm/future.txt#L523)

```cpp
class _LIBCPP_EXPORTED_FROM_ABI __assoc_sub_state : public __shared_count {
protected:
  exception_ptr __exception_;
  mutable mutex __mut_;
  mutable condition_variable __cv_;
  unsigned __state_;

  void __on_zero_shared() _NOEXCEPT override;
  void __sub_wait(unique_lock<mutex>& __lk);

public:
  enum { __constructed = 1, __future_attached = 2, ready = 4, deferred = 8 };

  _LIBCPP_HIDE_FROM_ABI __assoc_sub_state() : __state_(0) {}

  _LIBCPP_HIDE_FROM_ABI bool __has_value() const { return (__state_ & __constructed) || (__exception_ != nullptr); }

  _LIBCPP_HIDE_FROM_ABI void __attach_future() {
    lock_guard<mutex> __lk(__mut_);
    bool __has_future_attached = (__state_ & __future_attached) != 0;
    if (__has_future_attached)
      std::__throw_future_error(future_errc::future_already_retrieved);
    this->__add_shared();
    __state_ |= __future_attached;
  }
```

[codes/llvm/libcxx/include/future:585–603](sources/llvm/future.txt#L585)

```cpp
template <class _Rp>
class _LIBCPP_HIDDEN __assoc_state : public __assoc_sub_state {
  typedef __assoc_sub_state base;

protected:
  _ALIGNAS_TYPE(_Rp) char __value_[sizeof(_Rp)];

  _LIBCPP_HIDE_FROM_ABI_VIRTUAL void __on_zero_shared() _NOEXCEPT override;

public:
  template <class _Arg>
  _LIBCPP_HIDE_FROM_ABI void set_value(_Arg&& __arg);

  template <class _Arg>
  _LIBCPP_HIDE_FROM_ABI void set_value_at_thread_exit(_Arg&& __arg);

  _LIBCPP_HIDE_FROM_ABI _Rp move();
  _LIBCPP_HIDE_FROM_ABI _Rp& copy();
};
```

[codes/llvm/libcxx/include/future:605–609](sources/llvm/future.txt#L605)

```cpp
template <class _Rp>
void __assoc_state<_Rp>::__on_zero_shared() _NOEXCEPT {
  if (this->__state_ & base::__constructed)
    reinterpret_cast<_Rp*>(std::addressof(__value_))->~_Rp();
  delete this;
```

[codes/llvm/libcxx/include/future:612–620](sources/llvm/future.txt#L612)

```cpp
template <class _Rp>
template <class _Arg>
void __assoc_state<_Rp>::set_value(_Arg&& __arg) {
  unique_lock<mutex> __lk(this->__mut_);
  if (this->__has_value())
    std::__throw_future_error(future_errc::promise_already_satisfied);
  ::new ((void*)std::addressof(__value_)) _Rp(std::forward<_Arg>(__arg));
  this->__state_ |= base::__constructed | base::ready;
  __cv_.notify_all();
```

[codes/llvm/libcxx/include/future:634–640](sources/llvm/future.txt#L634)

```cpp
template <class _Rp>
_Rp __assoc_state<_Rp>::move() {
  unique_lock<mutex> __lk(this->__mut_);
  this->__sub_wait(__lk);
  if (this->__exception_ != nullptr)
    std::rethrow_exception(this->__exception_);
  return std::move(*reinterpret_cast<_Rp*>(std::addressof(__value_)));
```

**What causes the difference**:

GNU holds two shared-state pointers plus a separately owned result pointer. LLVM points to a state that also contains the result. GNU transfers result ownership into the shared state when setting it; LLVM constructs the result within the state, yielding three pointers versus one.

Follow the external state as well: GNU shared_ptr contains a state pointer and a separate ownership-control pointer. Its state has a result owner, readiness state, future-retrieved flag and once flag; its result has an exception pointer, 4-byte int storage and 1-byte initialization flag. LLVM combines an 8-byte long ownership count, virtual dispatch, exception pointer (baseline 8), mutex (baseline 40), condition_variable (baseline 48), unsigned flags (4) and int storage (4) in one inheritance chain. A virtual-table pointer is 8 under this ABI. Complete control-block/state sizes, offsets and GNU synchronization-helper sizes are `NOT_OBSERVED`; no total heap footprint is invented.

**Design rationale**:

The standard requires handling abandonment of unfinished shared state, including broken_promise, in [N4861 futures.state](https://timsong-cpp.github.io/cppwp/n4861/futures.state). GNU’s constructor separately calls make_shared and new for its result; LLVM constructs one associated-state object. Setting paths are reproduced above. This is an implementation fact, not a performance result. No public rationale for the original split/combined representation was found (`NOT_AVAILABLE`).

**Can the layouts be made identical?**:

**Technically possible, but not worthwhile (engineering judgment).** Combining or splitting ownership/result storage requires coordinated changes to promise, future, state bases, allocator construction, setting and release, breaking the modified library’s ABI. A matching wrapper cannot reconcile result delivery or broken-promise notification. Allocation structure, exception storage, wait backends and final destruction still differ in source. Equal object size does not discharge error/synchronization obligations. Different required standard error semantics are not asserted; complete runtime and custom-allocator validation is `NOT_OBSERVED`.

This is upstream code: a downstream fork is possible, but its existing binary compatibility promise cannot simply be retained. Except for specific records cited above, upstream acceptance of this redesign is `NOT_AVAILABLE`. Common policies and alternatives are discussed below.
## Common patterns, standard requirements and implementation choices

| Source-based pattern | Types | Representation change |
|---|---|---|
| Small values stored inside the object | any, function, string | Inline capacity, short/long overlap and dispatch; function also has stronger alignment |
| Container metadata | deque, map, set, unordered_map, unordered_set | Full iterators versus indices; full header versus minimal sentinel; resize threshold and single-bucket state. Element allocations are outside container sizeof |
| Ownership and shared state | future, promise | Two-pointer generic shared ownership and separate result ownership versus a one-pointer handle to counted result state |

This classifies the declarations; it does not attribute every choice to speed or memory savings. LLVM any/function are larger, while the other listed objects are smaller. Complete nodes, heap allocations and workloads differ; the table cannot rank total memory use or performance.

**Why the standard permits different representations.** References use the fixed public C++20 working draft N4861:

- §16.4.2.4 [objects.within.classes], paragraphs 1 and 3, leaves these class representations unspecified and permits members/techniques implementing the required observable semantics. Short quotation: **“do not specify the representation of classes”**. This directly supports different private representations here; see the [wording](https://timsong-cpp.github.io/cppwp/n4861/objects.within.classes).
- §16.5.5.12 [derivation] paragraph 1 permits implementation-reserved base classes; §16.5.5.5 [member.functions] paragraphs 1–2 allows implementation latitude for inline members and declarations preserving call behavior. See [derivation](https://timsong-cpp.github.io/cppwp/n4861/derivation) and [member functions](https://timsong-cpp.github.io/cppwp/n4861/member.functions).
- “The standard specifies interfaces” also includes effects, exceptions, preconditions, synchronization and complexity. §16.4.1.4 [structure.specifications] paragraphs 1, 3 and 7 describes those constraints and complexity upper bounds. See the [wording](https://timsong-cpp.github.io/cppwp/n4861/structure.specifications). It would be incorrect to generalize this to no representation constraints on any library type.

**Why the libraries do not promise matching layouts.** The standard does not impose a common private layout for these types, while each implementation has an installed ABI. GNU’s [ABI policy](https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html) and LLVM’s [ABI stability design](https://libcxx.llvm.org/DesignDocs/ABIVersioning.html) describe their own compatibility commitments, including LLVM ABI versions for intentional breaking changes. The resulting engineering judgment is that making one follow the other creates migration obligations for its existing users. Except for the historical function-buffer reply, a per-type upstream ruling on matching the other library’s whole representation was not found; acceptance is `NOT_AVAILABLE` and cannot be promised.

**What we can do.** Maintaining a downstream layout fork is technically possible, but requires a new ABI identity, rebuilding affected consumers, coordinated inline-header/runtime delivery and continuing merge/validation work. It is not an in-place repair retaining every old binary. This is an engineering judgment from the documented dependencies/policies; no redesign or cost benchmark was performed. Retaining two already-different old representations and making them directly interchangeable cannot be achieved by changing only one layout. A compatibility implementation or conversion boundary would be a separate, larger undertaking.

Reviewable paths include rebuilding mutually dependent C++ object interfaces against one runtime and supported ABI configuration; or preserving required old binaries behind copied/serialized values, controlled C interfaces or opaque handles, with creator-side destruction and boundary conversion of exceptions to agreed errors. Stateful futures/promises require a result/wait/cancellation service protocol, not byte copying. **These are proposals, not evidence that arbitrary mixing is safe.** Actual boundaries still need ownership, synchronization, error and complexity validation. The exact size numbers are not mandated by the standard; their mismatch in the fixed released implementations is an observed fact.

## Evidence gaps, independent judgments and review questions

- Missing specific design rationales are marked `NOT_AVAILABLE` per type. Searches covered upstream source explanations, GNU official documentation/mailing lists, LLVM official documentation/review archives and standard wording. See [public references and search scope](PUBLIC_REFERENCES.md). Not found does not mean nonexistent.
- Member offsets, complete node/control-block sizes, binary vtable slots, complete allocation/performance data and exhaustive operation/exception/invalidation/cancellation equivalence are `NOT_OBSERVED`. Virtual declarations are reproduced without claiming a binary vtable audit. GNU synchronization helpers are expanded only far enough to distinguish protocols; missing sizes are not filled in.
- Independent judgments: restrict analysis to default instances; explain prior measurements statically using source and the stated ABI; treat source comments as local explanations without inventing historical motives; classify layout-change cost/benefit and boundary proposals as engineering judgments. An unchanged copy of the located table aids review; all 33 machine-readable data rows remain unchanged. Excerpts preserve original line numbers. All selected GNU 13/14 fragments match exactly; this does not imply whole-header equality.
- Questions for human review: whether specified real interfaces need a later offset/node/control-block probe; which old binaries must remain; which boundaries can use values/handles; and whether a new ABI fork and coordinated rebuild are acceptable. Those product decisions have not been supplied and are not decided by this document.

## Common supporting source

These macros explain empty-member compression and the current ABI branches. Artificial padding is an upstream mechanism preserving legacy compressed_pair layout, not disposable unused space.
[codes/gcc/libstdc++-v3/include/bits/stl_function.h:130–141](sources/gnu14/bits/stl_function.h.txt#L130)

```cpp
  template<typename _Arg1, typename _Arg2, typename _Result>
    struct binary_function
    {
      /// @c first_argument_type is the type of the first argument
      typedef _Arg1 	first_argument_type; 

      /// @c second_argument_type is the type of the second argument
      typedef _Arg2 	second_argument_type;

      /// @c result_type is the return type
      typedef _Result 	result_type;
    } _GLIBCXX11_DEPRECATED;
```

[codes/gcc/libstdc++-v3/include/bits/stl_function.h:398–406](sources/gnu14/bits/stl_function.h.txt#L398)

```cpp
  /// One of the @link comparison_functors comparison functors@endlink.
  template<typename _Tp>
    struct less : public binary_function<_Tp, _Tp, bool>
    {
      _GLIBCXX14_CONSTEXPR
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x < __y; }
    };
```

[codes/llvm/libcxx/include/__memory/compressed_pair.h:25–29](sources/llvm/__memory/compressed_pair.h.txt#L25)

```cpp
// ================================================================================================================== //
// The utilites here are for staying ABI compatible with the legacy `__compressed_pair`. They should not be used      //
// for new data structures. Use `_LIBCPP_NO_UNIQUE_ADDRESS` for new data structures instead (but make sure you        //
// understand how it works).                                                                                          //
// ================================================================================================================== //
```

[codes/llvm/libcxx/include/__memory/compressed_pair.h:68–88](sources/llvm/__memory/compressed_pair.h.txt#L68)

```cpp
template <class _ToPad>
inline const bool __is_reference_or_unpadded_object =
    (is_empty<_ToPad>::value && !__is_final_v<_ToPad>) || sizeof(_ToPad) == __datasizeof_v<_ToPad>;

template <class _Tp>
inline const bool __is_reference_or_unpadded_object<_Tp&> = true;

template <class _Tp>
inline const bool __is_reference_or_unpadded_object<_Tp&&> = true;

template <class _ToPad, bool _Empty = __is_reference_or_unpadded_object<_ToPad> >
class __compressed_pair_padding {
  char __padding_[sizeof(_ToPad) - __datasizeof_v<_ToPad>] = {};
};

template <class _ToPad>
class __compressed_pair_padding<_ToPad, true> {};

#  define _LIBCPP_COMPRESSED_ELEMENT(T1, Initializer1)                                                                 \
    _LIBCPP_NO_UNIQUE_ADDRESS T1 Initializer1;                                                                         \
    _LIBCPP_NO_UNIQUE_ADDRESS ::std::__compressed_pair_padding<T1> _LIBCPP_CONCAT3(__padding_, __LINE__, _)
```

[codes/llvm/libcxx/include/__memory/compressed_pair.h:108–114](sources/llvm/__memory/compressed_pair.h.txt#L108)

```cpp
#    define _LIBCPP_COMPRESSED_PAIR(T1, Initializer1, T2, Initializer2)                                                \
      struct {                                                                                                         \
        _LIBCPP_NO_UNIQUE_ADDRESS T1 Initializer1;                                                                     \
        _LIBCPP_NO_UNIQUE_ADDRESS ::std::__compressed_pair_padding<T1> _LIBCPP_CONCAT3(__padding1_, __LINE__, _);      \
        _LIBCPP_NO_UNIQUE_ADDRESS T2 Initializer2;                                                                     \
        _LIBCPP_NO_UNIQUE_ADDRESS ::std::__compressed_pair_padding<T2> _LIBCPP_CONCAT3(__padding2_, __LINE__, _);      \
      }
```

[/home/toolchain/development/libc++_replacement/progress/T4/x86_64/static/extracted/usr/include/c++/v1/__config_site:10–15](sources/cache/__config_site.txt#L10)

```cpp
#define _LIBCPP___CONFIG_SITE

#define _LIBCPP_ABI_VERSION 1
#define _LIBCPP_ABI_NAMESPACE __1
#define _LIBCPP_ABI_FORCE_ITANIUM 0
#define _LIBCPP_ABI_FORCE_MICROSOFT 0
```

[codes/llvm/libcxx/include/__configuration/abi.h:52–86](sources/llvm/__configuration/abi.h.txt#L52)

```cpp
#if _LIBCPP_ABI_VERSION >= 2
// TODO: Move the description of the remaining ABI flags to ABIGuarantees.rst or remove them.

// Override the default return value of exception::what() for bad_function_call::what()
// with a string that is specific to bad_function_call (see http://wg21.link/LWG2233).
// This is an ABI break on platforms that sign and authenticate vtable function pointers
// because it changes the mangling of the virtual function located in the vtable, which
// changes how it gets signed.
#  define _LIBCPP_ABI_BAD_FUNCTION_CALL_GOOD_WHAT_MESSAGE
// According to the Standard, `bitset::operator[] const` returns bool
#  define _LIBCPP_ABI_BITSET_VECTOR_BOOL_CONST_SUBSCRIPT_RETURN_BOOL

// These flags are documented in ABIGuarantees.rst
#  define _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT
#  define _LIBCPP_ABI_ATOMIC_WAIT_NATIVE_BY_SIZE
#  define _LIBCPP_ABI_ENABLE_SHARED_PTR_TRIVIAL_ABI
#  define _LIBCPP_ABI_ENABLE_UNIQUE_PTR_TRIVIAL_ABI
#  define _LIBCPP_ABI_FIX_CITYHASH_IMPLEMENTATION
#  define _LIBCPP_ABI_FIX_UNORDERED_CONTAINER_SIZE_TYPE
#  define _LIBCPP_ABI_INCOMPLETE_TYPES_IN_DEQUE
#  define _LIBCPP_ABI_IOS_ALLOW_ARBITRARY_FILL_VALUE
#  define _LIBCPP_ABI_NO_COMPRESSED_PAIR_PADDING
#  define _LIBCPP_ABI_NO_FILESYSTEM_INLINE_NAMESPACE
#  define _LIBCPP_ABI_NO_ITERATOR_BASES
#  define _LIBCPP_ABI_NO_RANDOM_DEVICE_COMPATIBILITY_LAYOUT
#  define _LIBCPP_ABI_NO_REVERSE_ITERATOR_SECOND_MEMBER
#  define _LIBCPP_ABI_OPTIMIZED_FUNCTION
#  define _LIBCPP_ABI_REGEX_CONSTANTS_NONZERO
#  define _LIBCPP_ABI_STRING_OPTIMIZED_EXTERNAL_INSTANTIATION
#  define _LIBCPP_ABI_USE_WRAP_ITER_IN_STD_ARRAY
#  define _LIBCPP_ABI_USE_WRAP_ITER_IN_STD_STRING_VIEW
#  define _LIBCPP_ABI_VARIANT_INDEX_TYPE_OPTIMIZATION
#  define _LIBCPP_ABI_TRIVIALLY_COPYABLE_BIT_ITERATOR

#elif _LIBCPP_ABI_VERSION == 1
```

[codes/llvm/libcxx/include/__configuration/abi.h:125–141](sources/llvm/__configuration/abi.h.txt#L125)

```cpp
// #define _LIBCPP_ABI_BOUNDED_UNIQUE_PTR

#if defined(_LIBCPP_COMPILER_CLANG_BASED)
#  if defined(__APPLE__)
#    if defined(__i386__) || defined(__x86_64__)
// use old string layout on x86_64 and i386
#    elif defined(__arm__)
// use old string layout on arm (which does not include aarch64/arm64), except on watch ABIs
#      if defined(__ARM_ARCH_7K__) && __ARM_ARCH_7K__ >= 2
#        define _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT
#      endif
#    else
#      define _LIBCPP_ABI_ALTERNATE_STRING_LAYOUT
#    endif
#  endif
#endif
```
