# 标准库对象大小与对齐差异的成员级说明

这份配套说明只分析原表中大小或对齐不同的 10 个类型。差异都有可见的成员表示原因，但**把大小改成相同并不能让两套标准库对象安全互换**。下文逐项列出实际成员、源码原文、设计依据与改动后果；结论中的“能改但不值得”是有条件的工程判断，不是技术不可能性或上游裁决。

## 基线、范围与证据读法

- **既有实测**：x86_64，GNU 探针 GCC 13.3.0；LLVM 探针 Clang 21.1.1，使用平台 libc++/libc++abi；libc++ 头宏 220108（22.1.8），不是编译器版本。大小／对齐来自[原表原样副本](baseline/abi_layout_comparison.tsv)，两侧[GNU 原始输出](baseline/layout_libstdcxx.tsv)、[LLVM 原始输出](baseline/layout_libcxx.tsv)和[探针源码](baseline/layout_probe.cpp)一并保留。本次没有构建、运行新探针或使用开发板。
- **名称差异**：本地能找到的总部附表名为 `abi_layout_comparison.tsv`，与已归档的 `d11_layout_comparison.tsv` 逐字一致；任务所称 `abi_size_align_comparison.tsv` 未定位到同名文件（`NOT_AVAILABLE`）。没有改名覆盖原件。两份已定位原表 SHA256 都是 `08a99bb4ac7d314ba20c05befd92f40301dc4bcd3572fb126c8cf68c249509a7`。
- **源码核查**：平台 GCC 14.2.0 revision `49c5060508ce8a6ed6beca375233688a6bdd9140`；平台 LLVM revision `5ed6c77278dfa7a470667cf1a137723d3c96fe60`，与原报告列出的 revision 相同。所摘平台文件逐个与其 Git blob 核对一致。LLVM 已比较的布局头与原探针缓存头逐字相同。GNU 同时保存现存宿主 `libstdc++-13-dev 13.3.0-6ubuntu2~24.04.1` 头；平台 14.2 不是该次 GNU 测量版本。对应声明逐段核对见 [GNU 版本对照](GNU_REVISION_CROSSCHECK.md)。历史探针时的宿主头文件哈希未存档，历史逐字同一性为 `NOT_AVAILABLE`，不能以包版本相同冒充哈希证明。
- **成员字节账目**：类型、声明顺序、联合体重叠、继承和分支来自源码；成员字节数／对齐填充是按本基线 ABI 规则对源码作的静态核算，不是新增 `sizeof`、`offsetof` 或记录布局转储实测。采用 LP64（指针／long 为 8，int 为 4）：普通数据／函数指针 8，size_t 8，float 4，bool 1；来源为 [AMD64 ABI 0.99.6 §3.1.2](https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf) 与 [Itanium C++ ABI §2](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#data-layout)。没有把不明的节点／控制块尺寸补成猜测值。
- **术语**：ABI 是已编译模块之间约定的对象表示、符号和调用等二进制契约；`sizeof` 是对象占用字节数，`alignof` 是对象地址必须满足的对齐。填充是为对齐插入的空隙；`bit` 表示二进制位，8 位为 1 字节；联合体成员重叠，大小不能相加。空基类优化／`no_unique_address` 允许无状态的分配器、比较器等不额外占空间，**不是说这些空类型作为独立完整对象的大小为 0**。分配器负责申请／释放存储；哈希器和比较器负责键的分组和比较。以下均限默认分配器及默认比较／哈希器。LLVM 按缓存的 ABI version 1、Linux x86_64 分支分析；其宏原文在文末。历史成功构建日志仅保存命令摘要，完整命令行和全部宏转储 `NOT_AVAILABLE`，所以不把这里的静态分支核查冒充历史成员偏移实测。
- **标记**：实测仅指既有输出；源码核查包括上述静态字节核算；设计说明只归因于所引上游文字。`NOT_OBSERVED` 表示本轮或既有可用证据没有观测，`NOT_AVAILABLE` 表示所需资料不可得／未找到。没有无依据的设计动机推断。

## 各类型说明

### `std::any`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 16 | 32 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 依次：管理函数指针 `_M_manager` 8；联合体 `_M_storage` 8（`void*` 8 与内联缓冲 8 共用空间）。[std/any:83](sources/gnu14/std/any.txt#L83) | 依次：处理函数指针 `__h_` 8；联合体 `__s_` 24（`void*` 8 与 `char[24]` 共用空间，对齐 8）。[any:150](sources/llvm/any.txt#L150) |

**源码（实际摘录）**：

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

**差异的实质**：

两侧都保存一个管理函数指针；多出的 16 字节全部来自 libc++ 较大的内联缓冲区。内联是把小对象直接放在 `any` 自身里面，避免为它另分配内存；存不下时，两侧都改存指向外部对象的指针。

**设计原因**：

[C++20 草案 N4861，any.class 第 3 段](https://timsong-cpp.github.io/cppwp/n4861/any.class#3)建议小值避免动态分配，并限制这种优化只能用于移动构造不抛异常的类型。两侧源码条件与此对应。为什么具体选择 1 个指针宽度与 3 个指针宽度，未找到公开依据（`NOT_AVAILABLE`）；不把更大缓冲区的效果写成作者动机。

**能否改成一致**：

**能改但不值得（工程判断）**：改变联合体容量并重写相关路径，技术上可以另造一种布局；缩小会改变哪些类型需要分配，扩大也会改变对象步长、嵌入它的类及旧程序的成员地址，破坏被修改库自身的 ABI。单改容量仍不能互用：GNU 回调返回 `void`、参数为操作码／源对象／结果联合体；LLVM 返回 `void*`，另带类型及后备类型信息，协议不同。小对象阈值改变还会改变分配失败的机会；不能因此声称标准要求的 `any_cast` 行为不同。动态异常、跨库类型识别和复制析构的全部组合为 `NOT_OBSERVED`。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::function<void()>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 32 | 48 |
| 对齐（既有实测，字节） | 8 | 16 |
| 成员构成（声明顺序；字节为静态核算） | 空接口基类不另占空间；`_Function_base` 内依次 `_Any_data _M_functor` 16、管理函数指针 `_M_manager` 8；派生类再有调用函数指针 `_M_invoker` 8。[bits/std_function.h:62](sources/gnu14/bits/std_function.h.txt#L62) | 空接口基类不另占空间；唯一成员 `__f_` 为 `__value_func`：对齐存储 `__buf_` 实占 32（请求 24，对齐 16 后取整），再有 `__base<void()>* __f_` 8，尾部填充 8；合计 48。[__functional/function.h:131](sources/llvm/__functional/function.h.txt#L131) [__type_traits/aligned_storage.h:34](sources/llvm/__type_traits/aligned_storage.h.txt#L34) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 用缓冲区加两个普通函数指针；LLVM 用更大、对齐更严格的缓冲区加一个指向可调用包装对象的指针。LLVM 的包装对象通过虚函数分派操作；缓冲区对齐取整和对象尾部填充一起解释了 48 字节，不能误写成简单的 24＋8。GNU `_Nocopy_types` 中成员函数指针按 Itanium ABI §2.3.2 由两个机器字组成（16 字节），所以不能把它当成普通 8 字节函数指针。

**设计原因**：

[Jonathan Wakely，2016-07-30 上游邮件](https://gcc.gnu.org/pipermail/libstdc++/2016-July/044437.html)明确因破坏库 ABI 拒绝把 GNU 缓冲区从 16 扩大为 24 字节。这说明维持现状的原因，不证明最初为什么选 16。LLVM [ABI 文档](https://libcxx.llvm.org/ABIGuarantees.html)说明优化版 `function` 会重构表示且尚非稳定 ABI；当前平台未启用该宏。LLVM 最初选择本缓冲大小和虚函数方案的公开动机未找到（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：连同缓冲区、对齐、分派及复制析构一起重写可实现另一种表示；只修改数字不行，且破坏既有 8／16 字节对齐契约与含该类型的二进制布局。内部虚表（按对象实际类型选择操作的函数表）与 GNU 管理／调用函数协议仍不同。GNU 的就地条件要求 location-invariant（地址改变不影响所存值；默认用可平凡复制判定），LLVM 检查整个包装对象大小及目标的无异常复制条件；分配与移动路径因此不同。源码中 LLVM 移动内联目标会克隆，源包装仍持有值；这是允许的移动后状态，不是标准违约。完整异常和调用行为等价性 `NOT_OBSERVED`。上游对扩大 GNU 缓冲区已有明确拒绝记录；不能假定接受跨库布局改造。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::deque<int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 80 | 48 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 空分配器基类；依次 `int** _M_map` 8、`size_t _M_map_size` 8、起点迭代器 `_M_start` 32、终点 `_M_finish` 32。每个迭代器依次为 `int* _M_cur/_M_first/_M_last` 与 `int** _M_node`，各 8。[bits/stl_deque.h:83](sources/gnu14/bits/stl_deque.h.txt#L83) | 依次 `__map_` 32（`int** __front_cap_/__begin_/__end_/__back_cap_` 各 8，后跟空指针分配器）、`size_type __start_` 8、`size_type __size_` 8，后跟压缩的空元素分配器。对外迭代器另为目录指针＋元素指针，各 8，并未嵌入对象。[deque:266](sources/llvm/deque.txt#L266) [__split_buffer:193](sources/llvm/__split_buffer.txt#L193) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 把两份较大的完整迭代器放在容器里；LLVM 把起点和元素数记成整数，并用四个指针管理块目录。这里的目录是“指向各段元素数组的指针数组”，与 `std::map` 无关。

**设计原因**：

GNU 源码设计说明指出：使块大小较小且相对固定以改善分配器效率，迭代器必须维护块首／块尾等不变式（操作前后必须成立的内部关系）；同文件还警告更改块大小会破坏二进制兼容。LLVM `__split_buffer` 注释说明两端保留空余空间可在空间用尽前向两侧增长而不搬移已有目录内容。它们是源码自带的设计说明，实际原文见上方；为什么分别选 512 和 4096 字节、为什么最初选择这套对象成员数量，未找到更完整公开依据（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：更换目录及起止表示可行，但既有内联操作、迭代器和容器对象都会失配。即使外壳统一，`int` 的块容量源码仍为 GNU 512/4＝128、LLVM 4096/4＝1024，跨块递增／寻址规则不能混用；GNU 明确保持空 deque 也有一个块，LLVM 不能套用这个不变式。标准的双端操作复杂度与迭代器失效要求仍须满足，不能把上述实现不同写成标准失效规则不同；见 [N4861 deque.modifiers](https://timsong-cpp.github.io/cppwp/n4861/deque.modifiers)。本轮未做运行时失效／异常穷举（`NOT_OBSERVED`）。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::map<int,int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 48 | 24 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 唯一树成员 `_M_t`：空节点分配器基类；比较器包装（`less<int>` 普通成员 1＋对齐填充 7）；树头内依次颜色枚举 4＋填充 4、父／左／右指针各 8；元素计数 8。合计 8＋32＋8＝48。[bits/stl_tree.h:95](sources/gnu14/bits/stl_tree.h.txt#L95) [bits/stl_map.h:149](sources/gnu14/bits/stl_map.h.txt#L149) | 唯一树成员 `__tree_`：起始节点指针 `__begin_node_` 8；结束哨兵 `__end_node_` 8（仅一个根指针 `__left_`）及空节点分配器；元素计数 `__size_` 8 及压缩的空比较器。合计 24。[__tree:557](sources/llvm/__tree.txt#L557) [map:639](sources/llvm/map.txt#L639) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 容器中放了一份完整的树头，还为普通成员形式的空比较器付出一个对齐单元；LLVM 用只有根指针的结束哨兵和压缩比较器。哨兵是不存业务值、用于表示边界的辅助对象；业务元素节点都在对象外，本实例的节点值为`pair<const int,int>`（两个 int，共 8 字节）。

**设计原因**：

GNU 树头注释明确解释：缓存最左节点使 `begin()` 为常数时间，缓存最右节点服务通用集合算法；删除双子节点时重连后继而不复制，避免使其他节点迭代器失效。LLVM 树算法说明根的父节点只需提供指回根的 `__left_`，其他字段不访问，这是最小结束哨兵所满足的算法契约。以上是上游源码说明；为什么最初选这两种树头而非同一种，未找到额外公开依据（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：可以重做树头／比较器存储并改写平衡、插入和删除算法，但会破坏该库已发布容器、内联成员及迭代器代码的 ABI。节点也不同：GNU 是颜色枚举在前三个链接之前；LLVM 为继承的左链接、右链接、父链接、`bool` 颜色，再是值。指针各 8，GNU 颜色 4、LLVM 颜色 1；完整节点大小及尾部填充复用未实测（`NOT_OBSERVED`），不能从容器 48／24 推成节点同样比例。统一外壳无法修复这些节点协议。两侧都须遵守 [N4861 associative.reqmts](https://timsong-cpp.github.io/cppwp/n4861/associative.reqmts) 的有序访问、复杂度和迭代器保证；本核查未证明普通异常或标准失效规则有不同，完整运行验证 `NOT_OBSERVED`。 `map` 的键为 const、映射值可修改；改布局必须继续保留这一语义。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::set<int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 48 | 24 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 唯一树成员 `_M_t`：空节点分配器基类；比较器包装（`less<int>` 普通成员 1＋对齐填充 7）；树头内依次颜色枚举 4＋填充 4、父／左／右指针各 8；元素计数 8。合计 8＋32＋8＝48。[bits/stl_tree.h:95](sources/gnu14/bits/stl_tree.h.txt#L95) [bits/stl_set.h:129](sources/gnu14/bits/stl_set.h.txt#L129) | 唯一树成员 `__tree_`：起始节点指针 `__begin_node_` 8；结束哨兵 `__end_node_` 8（仅一个根指针 `__left_`）及空节点分配器；元素计数 `__size_` 8 及压缩的空比较器。合计 24。[__tree:557](sources/llvm/__tree.txt#L557) [set:590](sources/llvm/set.txt#L590) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 容器中放了一份完整的树头，还为普通成员形式的空比较器付出一个对齐单元；LLVM 用只有根指针的结束哨兵和压缩比较器。哨兵是不存业务值、用于表示边界的辅助对象；业务元素节点都在对象外，本实例的节点值为`int`（4 字节）。

**设计原因**：

GNU 树头注释明确解释：缓存最左节点使 `begin()` 为常数时间，缓存最右节点服务通用集合算法；删除双子节点时重连后继而不复制，避免使其他节点迭代器失效。LLVM 树算法说明根的父节点只需提供指回根的 `__left_`，其他字段不访问，这是最小结束哨兵所满足的算法契约。以上是上游源码说明；为什么最初选这两种树头而非同一种，未找到额外公开依据（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：可以重做树头／比较器存储并改写平衡、插入和删除算法，但会破坏该库已发布容器、内联成员及迭代器代码的 ABI。节点也不同：GNU 是颜色枚举在前三个链接之前；LLVM 为继承的左链接、右链接、父链接、`bool` 颜色，再是值。指针各 8，GNU 颜色 4、LLVM 颜色 1；完整节点大小及尾部填充复用未实测（`NOT_OBSERVED`），不能从容器 48／24 推成节点同样比例。统一外壳无法修复这些节点协议。两侧都须遵守 [N4861 associative.reqmts](https://timsong-cpp.github.io/cppwp/n4861/associative.reqmts) 的有序访问、复杂度和迭代器保证；本核查未证明普通异常或标准失效规则有不同，完整运行验证 `NOT_OBSERVED`。 `set` 的迭代器不允许修改键，改布局必须继续保留这一语义（GNU typedef 原文见上方）。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::unordered_map<int,int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 56 | 40 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 唯一 `_M_h`：按序桶数组指针 8、桶数 8、前置哨兵（单个 next 指针）8、元素数 8、`_Prime_rehash_policy` 16（最大装载因子 float 4＋填充 4＋下次扩容阈值 size_t 8）、内置单桶指针 8。默认哈希器／相等比较器／分配器为空基类，不额外计存储。[bits/hashtable.h:47](sources/gnu14/bits/hashtable.h.txt#L47) [bits/hashtable_policy.h:315](sources/gnu14/bits/hashtable_policy.h.txt#L315) [bits/unordered_map.h:43](sources/gnu14/bits/unordered_map.h.txt#L43) | 唯一 `__table_`：桶数组所有者 `__bucket_list_` 16（数组指针 8；删除器内桶数 8＋压缩空分配器；无状态边界检查器不另占空间）；前置哨兵 next 指针 8＋空节点分配器；元素数 8＋空哈希器；最大装载因子 float 4＋空相等比较器；尾填充 4。合计 40。[__hash_table:79](sources/llvm/__hash_table.txt#L79) [__memory/unique_ptr.h:340](sources/llvm/__memory/unique_ptr.h.txt#L340) [unordered_map:899](sources/llvm/unordered_map.txt#L899) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 多保存一个“下次扩容阈值”和一个可在对象内部使用的单桶指针，各 8 字节；其余管理状态虽包装和顺序不同，字节账目一致。这 16 字节解释外壳差值；哈希桶是将键按哈希值分组后查找的目录，不等于业务元素本身。

**设计原因**：

GNU `_M_single_bucket` 注释给出明确原因：移动后留下一个无需分配的桶，使移动操作可不抛异常；若零桶会要求大量防止模零检查。`_Prime_rehash_policy` 源码说明按装载因子选择通常为素数的桶数；节点缓存注释把缓存哈希值称为时间／空间取舍。LLVM 桶数在删除器中供 `deallocate` 使用，属于源码事实；为什么选择这套字段组织及总是缓存本实例节点哈希值，未找到公开设计动机（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：取消单桶／阈值或重排字段须同步改写重哈希、移动、释放和空表逻辑，破坏自身已发布 ABI。布局相同也仍有节点协议差异：GNU 默认 `hash<int>` 快且不抛异常，故 `__cache_default` 为 false，节点仅 next 指针 8＋值（`pair<const int,int>`，8 字节）；LLVM 节点按序 next 8＋`size_t __hash_` 8＋值。完整节点 `sizeof` 为 `NOT_OBSERVED`。桶索引算法 GNU `%`，LLVM 对二次幂桶数可用位掩码，见源码；不能按对方规则继续查找或删除。标准只要求相应复杂度、重哈希及失效保证，见 [N4861 unord.req](https://timsong-cpp.github.io/cppwp/n4861/unord.req)；这里没有证明两侧标准保证不同，也没有运行时枚举所有异常／失效路径（`NOT_OBSERVED`）。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::unordered_set<int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 56 | 40 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 唯一 `_M_h`：按序桶数组指针 8、桶数 8、前置哨兵（单个 next 指针）8、元素数 8、`_Prime_rehash_policy` 16（最大装载因子 float 4＋填充 4＋下次扩容阈值 size_t 8）、内置单桶指针 8。默认哈希器／相等比较器／分配器为空基类，不额外计存储。[bits/hashtable.h:47](sources/gnu14/bits/hashtable.h.txt#L47) [bits/hashtable_policy.h:315](sources/gnu14/bits/hashtable_policy.h.txt#L315) [bits/unordered_set.h:43](sources/gnu14/bits/unordered_set.h.txt#L43) | 唯一 `__table_`：桶数组所有者 `__bucket_list_` 16（数组指针 8；删除器内桶数 8＋压缩空分配器；无状态边界检查器不另占空间）；前置哨兵 next 指针 8＋空节点分配器；元素数 8＋空哈希器；最大装载因子 float 4＋空相等比较器；尾填充 4。合计 40。[__hash_table:79](sources/llvm/__hash_table.txt#L79) [__memory/unique_ptr.h:340](sources/llvm/__memory/unique_ptr.h.txt#L340) [unordered_set:608](sources/llvm/unordered_set.txt#L608) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 多保存一个“下次扩容阈值”和一个可在对象内部使用的单桶指针，各 8 字节；其余管理状态虽包装和顺序不同，字节账目一致。这 16 字节解释外壳差值；哈希桶是将键按哈希值分组后查找的目录，不等于业务元素本身。

**设计原因**：

GNU `_M_single_bucket` 注释给出明确原因：移动后留下一个无需分配的桶，使移动操作可不抛异常；若零桶会要求大量防止模零检查。`_Prime_rehash_policy` 源码说明按装载因子选择通常为素数的桶数；节点缓存注释把缓存哈希值称为时间／空间取舍。LLVM 桶数在删除器中供 `deallocate` 使用，属于源码事实；为什么选择这套字段组织及总是缓存本实例节点哈希值，未找到公开设计动机（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：取消单桶／阈值或重排字段须同步改写重哈希、移动、释放和空表逻辑，破坏自身已发布 ABI。布局相同也仍有节点协议差异：GNU 默认 `hash<int>` 快且不抛异常，故 `__cache_default` 为 false，节点仅 next 指针 8＋值（`int`，4 字节）；LLVM 节点按序 next 8＋`size_t __hash_` 8＋值。完整节点 `sizeof` 为 `NOT_OBSERVED`。桶索引算法 GNU `%`，LLVM 对二次幂桶数可用位掩码，见源码；不能按对方规则继续查找或删除。标准只要求相应复杂度、重哈希及失效保证，见 [N4861 unord.req](https://timsong-cpp.github.io/cppwp/n4861/unord.req)；这里没有证明两侧标准保证不同，也没有运行时枚举所有异常／失效路径（`NOT_OBSERVED`）。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::string`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 32 | 24 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 依次 `_M_dataplus` 8（空 allocator 基类＋`char* _M_p`）、长度 `size_type _M_string_length` 8、联合体 16（`char _M_local_buf[16]` 与 `size_type _M_allocated_capacity` 8 重叠）。[bits/basic_string.h:185](sources/gnu14/bits/basic_string.h.txt#L185) | `__rep_` 联合体 24，后跟压缩的空 allocator。长态按序为同一 size_type 单元的长态标记 1 bit＋容量 63 bits（合计 8 字节）、长度 8、`char*` 8；短态为标记 1 bit＋长度 7 bits（合计 1 字节），再有 `char[23]`（含终止零）。`char` 的 `__padding<0>` 不另占存储。[string:719](sources/llvm/string.txt#L719) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 始终保留指针和长度，另留 16 字节可兼作短字符串内容；LLVM 把整个 24 字节表示复用为长态或短态，用一个位区分。故 LLVM 对象小 8 字节，却能就地保存至多 22 个字符；GNU 至多 15 个，均不计结尾的零字符。

**设计原因**：

[GNU Dual ABI 文档](https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html)说明新字符串实现为符合 C++11、不再使用写时复制（共享内容直至写入再复制），并用双 ABI 保留旧二进制兼容；这不是 32／24 的直接历史原因。LLVM 源码说明位标记与容量编码关系；[D128285](https://reviews.llvm.org/D128285)说明 `packed` 是为修复 AIX 上改用位域后的 ABI 变化。为什么最初选 15／22 的短字符串容量及完整布局，未找到公开依据（`NOT_AVAILABLE`）。不能借 AIX 修复解释 x86_64 最初选择。

**能否改成一致**：

**能改但不值得（工程判断）**：可以重写长短态表示，但这会改变短态容量、指针解释、长度／容量编码、析构分支及包含字符串的所有既有二进制布局。GNU 用数据指针是否指向自身缓冲区判短态，LLVM 读位标记；即使塞填充做成同样大小也不能相互读写。GNU 的 `std::__cxx11` 与 LLVM 的 `std::__1` 符号身份还不同，双 ABI 开关并非跨库转换器。分配时点和容量变化会影响实现层行为；本轮未测完整增长策略、异常路径或指针／迭代器失效组合（`NOT_OBSERVED`），不声称标准字符串语义不同。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::future<int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 16 | 8 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | `future` 的基类 `__basic_future` 唯一成员 `_M_state` 为 `shared_ptr<_State_base>` 16：按序状态指针 `_M_ptr` 8、引用计数句柄 `_M_refcount` 8；后者展开为控制块指针 `_M_pi`。[std/future:700](sources/gnu14/std/future.txt#L700) [bits/shared_ptr_base.h:1135](sources/gnu14/bits/shared_ptr_base.h.txt#L1135) | 唯一数据成员 `__assoc_state<int>* __state_` 8；引用计数在指向的状态对象中，通过继承的 `__shared_count` 管理，并非没有引用计数。[future:918](sources/llvm/future.txt#L918) [__memory/shared_count.h:44](sources/llvm/__memory/shared_count.h.txt#L44) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 的 future 随身带“状态地址”和“引用计数控制块地址”两个指针；LLVM 只带状态地址，把引用计数放进状态中。减少的是句柄大小，不等于整个异步操作总共节省 8 字节。

对象外还要追到共享状态：GNU `shared_ptr` 内的第一个指针指向 `_State_baseV2`，第二个指向引用计数控制块（管理存活期的辅助对象）；状态本身另有结果所有者、就绪状态、已取 future 标记和一次性设置标记，结果对象持异常指针、4 字节 int 存储和 1 字节初始化标记。LLVM 将 `long __shared_owners_`（8 字节）、虚函数分派、异常指针（原表 8）、mutex（原表 40）、condition_variable（原表 48）、unsigned 状态（4）及 int 存储（4）组织在同一继承链。虚表指针按本机 ABI 为 8；两侧完整控制块／状态大小、GNU 同步辅助类型大小与偏移本轮均为 `NOT_OBSERVED`，不虚填堆内存总量。

**设计原因**：

[N4861 futures.state](https://timsong-cpp.github.io/cppwp/n4861/futures.state)规定共享状态承载结果和同步，并管理释放时机；不规定用几根指针。GNU 源码说明状态在提供者与 future 间共享；LLVM 的引用计数增减可直接核查。为什么 GNU 使用通用 shared_ptr 而 LLVM 使用嵌入式计数，未找到公开依据（`NOT_AVAILABLE`），不推测性能动机。

**能否改成一致**：

**能改但不值得（工程判断）**：可以重新设计句柄和共享状态，但改变句柄大小本身已破坏旧调用方、返回／传参及嵌入布局。只改句柄不改变“引用计数在哪里、到零时如何释放、结果如何取得”就不能互通。源码中 GNU 用 futex 辅助类型（Linux 的按地址等待／唤醒机制，宏可选其他后端）和 `call_once`；LLVM 状态用互斥锁与条件变量。就绪／设置不变式、等待、析构、`exception_ptr` 再抛及错误类别仍须整体匹配；本核查没有证明标准规定的 get/wait 语义不一致，取消／异常组合运行结果 `NOT_OBSERVED`。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。

### `std::promise<int>`

| 项 | libstdc++ | libc++ |
|---|---|---|
| 大小（既有实测，字节） | 24 | 8 |
| 对齐（既有实测，字节） | 8 | 8 |
| 成员构成（声明顺序；字节为静态核算） | 依次 `shared_ptr<_State> _M_future` 16（状态指针 8＋控制块指针 8）、`unique_ptr<_Result<int>, _Deleter> _M_storage` 8（结果指针 8；空删除器不另占空间）。[std/future:1097](sources/gnu14/std/future.txt#L1097) [bits/shared_ptr_base.h:1135](sources/gnu14/bits/shared_ptr_base.h.txt#L1135) [bits/unique_ptr.h:224](sources/gnu14/bits/unique_ptr.h.txt#L224) | 唯一成员 `__assoc_state<int>* __state_` 8；计数、结果存储及同步状态均在外部状态继承链内。[future:1114](sources/llvm/future.txt#L1114) [future:523](sources/llvm/future.txt#L523) |

**源码（实际摘录）**：

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

**差异的实质**：

GNU 除了共享状态的两个指针，还单独持有尚未交付的结果对象指针；LLVM 直接指向同时承载结果的状态对象。GNU 的设置路径把结果所有权移入共享状态，LLVM 在状态内构造结果；因此分别为三根与一根指针。

对象外还要追到共享状态：GNU `shared_ptr` 内的第一个指针指向 `_State_baseV2`，第二个指向引用计数控制块（管理存活期的辅助对象）；状态本身另有结果所有者、就绪状态、已取 future 标记和一次性设置标记，结果对象持异常指针、4 字节 int 存储和 1 字节初始化标记。LLVM 将 `long __shared_owners_`（8 字节）、虚函数分派、异常指针（原表 8）、mutex（原表 40）、condition_variable（原表 48）、unsigned 状态（4）及 int 存储（4）组织在同一继承链。虚表指针按本机 ABI 为 8；两侧完整控制块／状态大小、GNU 同步辅助类型大小与偏移本轮均为 `NOT_OBSERVED`，不虚填堆内存总量。

**设计原因**：

标准对提供者放弃未完成共享状态、报告 `broken_promise`（结果未交付即失去提供者）有要求，见 [N4861 futures.state](https://timsong-cpp.github.io/cppwp/n4861/futures.state)。GNU 构造器可见独立 `make_shared` 和 `new _Res_type`，LLVM 为 `new __assoc_state`，设置路径也在上方；这是事实，不证明哪种更快。为什么最初分拆或合并结果与状态，未找到公开设计依据（`NOT_AVAILABLE`）。

**能否改成一致**：

**能改但不值得（工程判断）**：可把所有权及结果存储合并或拆分，但需同时改 promise、future、状态基类、分配器构造、设置和释放路径，破坏被改库自身 ABI。只改外壳不能兼容结果交付协议；析构还要正确通知失去提供者的 future。普通错误与共享状态同步义务并不会因为对象同样大而自动满足；分配次数、异常状态承载、等待后端和到零析构均有源码层差异。标准规定的错误语义不应宣称不同；完整运行行为及自定义分配器验证 `NOT_OBSERVED`。

上述改造涉及上游代码；我们可维护下游分叉，但不再能沿用该库现有二进制兼容承诺。除本节明确列出的上游记录，针对这项改造的接受度为 `NOT_AVAILABLE`。共同政策和替代路径见总结。
## 共同模式、标准约束与实施选择

| 共同模式（源码归纳） | 类型 | 实际变化 |
|---|---|---|
| 对象内部保存小值的方式 | any、function、string | 内联缓冲区容量、长短态复用及分派表示不同；function 还涉及更强对齐 |
| 容器管理元数据 | deque、map、set、unordered_map、unordered_set | 完整迭代器或整数索引、完整树头或最小哨兵、扩容阈值和单桶状态；业务元素本身不计入容器 sizeof |
| 所有权和共享状态 | future、promise | 两指针通用共享所有者、独立结果所有者，或一指针指向含计数／结果的状态 |

这是对前述声明的分类，不是“全部为了性能”或“全部为了节省内存”的动机判断。`any`／`function` 在 LLVM 更大，其他列举对象更小；完整节点、堆分配与业务负载不同，不能据此评出某库整体内存或性能更优。

**标准为何允许不同表示。** 固定引用 C++20 公开工作草案 N4861，避免把后续草案段号混入测量基线：

- §16.4.2.4 [objects.within.classes] 第 1、3 段明确类表示不由这些库条款指定，可定义实现语义所需的成员，并允许产生等价可观察行为的实现技术。原文短引：**“do not specify the representation of classes”**。这直接支持本表私有表示不必一致；见 [条文](https://timsong-cpp.github.io/cppwp/n4861/objects.within.classes)。
- §16.5.5.12 [derivation] 第 1 段允许从实现保留名称的类派生；§16.5.5.5 [member.functions] 第 1、2 段对内联及保持调用行为的成员声明形式留有余地。见 [继承条文](https://timsong-cpp.github.io/cppwp/n4861/derivation)、[成员函数条文](https://timsong-cpp.github.io/cppwp/n4861/member.functions)。
- “标准只规定接口”应准确理解为也规定效果、异常、安全前提、同步和复杂度；不是任意行为都被允许。§16.4.1.4 [structure.specifications] 第 1、3、7 段列出这些约束及复杂度上界，见 [条文](https://timsong-cpp.github.io/cppwp/n4861/structure.specifications)。不能据标准不统一这些私有布局，推广为标准对任何库类型都绝无表示约束。

**两套库为什么不承诺互相布局一致。** 上述标准没有施加本表类型的共同私有布局；两套实现各有已发布的 ABI。GNU 的 [ABI 政策](https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html)和 LLVM 的 [ABI 稳定性说明](https://libcxx.llvm.org/DesignDocs/ABIVersioning.html)描述各自的兼容承诺；LLVM 还通过 ABI 版本支持有意的破坏性改进。由此作出的工程判断是：让一侧追随另一侧会引入本库旧用户的迁移问题，不是修改十个 sizeof 数字即可完成。除 `function` 缓冲区历史邮件外，本次未找到上游对“匹配另一套库全部布局”方案的逐型接受／拒绝裁决，接受度为 `NOT_AVAILABLE`，不能替上游承诺。

**我们能怎么做。** 对上游代码维护下游布局分叉在技术上可做，但须承担新的 ABI 版本、所有相关使用方重编、头文件内联代码／运行库同步发布和后续合并验证；不能作为保留全部旧二进制的原位修复。这是基于上述源码依赖及 ABI 政策的工程判断，并非本次做过改造或测过成本。要同时保持已经不同的旧对象布局并让它们原地互换，单改一侧布局做不到；建立另一套兼容实现／转换边界属于更大范围的独立工程。

有两类可评审路径：可重编的互相传递 C++ 对象的模块，统一到同一运行库、同一受支持 ABI 配置后共同重编；必须保留旧二进制的模块，通过值复制／序列化或受控 C 接口、不可解引用的句柄衔接，并让创建方负责销毁，异常在所属边界转换为约定错误结果。对于有状态的 future/promise 不能简单复制内存，须转接“提交结果／等待／取消”等服务协议。**这些是建议，不是已验证任意混用安全的结论**；边界方案仍需针对实际接口验证所有权、同步、错误和复杂度。具体 16／32 等数值不是标准必然值；固定两套已发布实现之间的这些不一致则是已观测事实。

## 资料缺口、自行判断与待审问题

- 本次未找到的具体设计动机已逐型标为 `NOT_AVAILABLE`；查找范围为上游源码说明、GNU 官方文档与邮件列表、LLVM 官方文档与评审归档、标准草案。记录见 [公开来源与检索范围](PUBLIC_REFERENCES.md)。未找到不等于不存在。
- 成员偏移、整节点／整控制块大小、虚表实际槽位转储、完整分配次数与性能数据，以及全部操作／异常／失效／取消等价性均为 `NOT_OBSERVED`。代码中虚函数声明顺序已展示，但没有声称虚表已经完成二进制核验。GNU 同步辅助成员只展开到足以区分协议；未获尺寸不补填。
- 自行判断：只针对默认实例；以源码＋既定 ABI 静态解释原测量，不构建复测；将源码注释视为作者对局部结构用途的说明，不扩大成未记录的历史动机；将改布局的成本／收益结论和第三条路明确列为工程判断。使用同名原表的原样副本便于审阅，没有改动 33 行机器数据。源码摘录保留原行号；本次所选 GNU 13/14 片段逐段完全相同，未据此声称全头完全相同。
- 待人工评审：是否需要下一步对指定实际接口做偏移和节点控制块探针；哪些旧二进制必须保留；哪类边界能转为值／句柄协议；是否愿意承担新 ABI 分叉及全量重编。尚未获取这些产品决策，不能在本说明中替评审方决定。

## 公共辅助源码

以下宏解释表中空成员压缩与当前 ABI 分支；它们没有新增业务成员。“人工填充”是上游为保持旧 compressed_pair 布局而保留的机制，不能任意删除。
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
