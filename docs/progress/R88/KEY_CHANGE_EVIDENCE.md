# R88 关键改动代码证据

以下均直接取自 Gerrit patch set commit
`e79f1c3dde12c356142b77231e304c0da67adaf1`，行号由 `nl -ba` 生成。

## A-1：外来异常续跑重抛

`libcxxabi/src/cxa_exception.cpp:642-657`：

```cpp
else  // this is a foreign exception
{
    globals->caughtExceptions = 0;
}
#ifdef __USING_SJLJ_EXCEPTIONS__
_Unwind_SjLj_RaiseException(&exception_header->unwindHeader);
#else
if (native_exception)
    _Unwind_RaiseException(&exception_header->unwindHeader);
else
    _Unwind_Resume_or_Rethrow(&exception_header->unwindHeader);
#endif
```

## A-2：ARM EHABI catching handler gate

`libcxxabi/src/cxa_personality.cpp:1272-1283`：

```cpp
if (results.reason == _URC_HANDLER_FOUND)
{
    // A non-zero ttypeIndex identifies a catching handler. ARM EHABI
    // requires __cxa_begin_cleanup only for a true cleanup entry
    // (ttypeIndex == 0), matching libstdc++'s found_cleanup gate.
    if (is_force_unwinding && !native_exception &&
        results.ttypeIndex != 0)
    {
        save_results_to_barrier_cache(unwind_exception, results);
        set_registers(unwind_exception, context, results);
        return _URC_INSTALL_CONTEXT;
    }
```

## B-1：识别类型定义

`libcxxabi/include/cxxabi.h:40-46`：

```cpp
// An out-of-line key function makes the runtime library own the unique
// exported typeinfo definition. Keep the libstdc++ abstract class shape so
// this marker can only be caught by reference.
class _LIBCXXABI_TYPE_VIS __forced_unwind {
  virtual ~__forced_unwind();
  virtual void __pure_dummy() = 0;
};
```

`libcxxabi/src/cxa_exception.cpp:39` 另有：

```cpp
__forced_unwind::~__forced_unwind() = default;
```

## B-2：强制展开映射到识别类型

`libcxxabi/src/cxa_personality.cpp:822-836`：

```cpp
else if (!native_exception &&
         (actions & _UA_FORCE_UNWIND))
{
    void* adjustedPtr =
        get_thrown_object_ptr(unwind_exception);
    const __shim_type_info* forcedType =
        static_cast<const __shim_type_info*>(
            &typeid(__cxxabiv1::__forced_unwind));
    if (catchType->can_catch(forcedType, adjustedPtr))
    {
        results.ttypeIndex = ttypeIndex;
        results.actionRecord = actionRecord;
        results.adjustedPtr = adjustedPtr;
        results.reason = _URC_HANDLER_FOUND;
        return;
    }
}
```

## F1：掩码重抛在内层 catch，尾部无裸重抛

`libcxx/src/ios.cpp:363-388` 的两个函数结构相同：

```cpp
void ios_base::__set_badbit_and_consider_rethrow() {
  __rdstate_ |= badbit;
#if _LIBCPP_HAS_EXCEPTIONS
  try {
    throw;
  } catch (const __cxxabiv1::__forced_unwind&) {
    throw;
  } catch (...) {
    if (__exceptions_ & badbit)
      throw;
  }
#endif // _LIBCPP_HAS_EXCEPTIONS
}

void ios_base::__set_failbit_and_consider_rethrow() {
  __rdstate_ |= failbit;
#if _LIBCPP_HAS_EXCEPTIONS
  try {
    throw;
  } catch (const __cxxabiv1::__forced_unwind&) {
    throw;
  } catch (...) {
    if (__exceptions_ & failbit)
      throw;
  }
#endif // _LIBCPP_HAS_EXCEPTIONS
}
```

这里入口 `try` 内的 `throw;` 用来取得当前异常；forced 分支立即重抛；普通
异常只在 mask 命中时重抛。两个函数在 catch 结构结束后直接结束，没有
函数尾部的无条件裸 `throw;`。

## F2：istream typed 分支使用不抛状态提交

`libcxx/include/istream:661-675`：

```cpp
} catch (const __cxxabiv1::__forced_unwind&) {
  __state |= ios_base::badbit;
  if (__gc_ == 0)
    __state |= ios_base::failbit;
  this->__setstate_nothrow(__state);
  throw;
} catch (...) {
  __state |= ios_base::badbit;
  if (__gc_ == 0)
    __state |= ios_base::failbit;

  this->__setstate_nothrow(__state);
  if (this->exceptions() & ios_base::failbit || this->exceptions() & ios_base::badbit) {
    throw;
  }
}
```

## N1：算术输出没有额外裸重抛 handler

`libcxx/include/__ostream/basic_ostream.h:95-112`：

```cpp
template <class _Tp>
_LIBCPP_HIDE_FROM_ABI basic_ostream& __put_num(_Tp __value) {
#  if _LIBCPP_HAS_EXCEPTIONS
  try {
#  endif // _LIBCPP_HAS_EXCEPTIONS
    sentry __s(*this);
    if (__s) {
      using _Fp = num_put<char_type, ostreambuf_iterator<char_type, traits_type> >;
      const _Fp& __facet = std::use_facet<_Fp>(this->getloc());
      if (__facet.put(*this, *this, this->fill(), __value).failed())
        this->setstate(ios_base::badbit | ios_base::failbit);
    }
#  if _LIBCPP_HAS_EXCEPTIONS
  } catch (...) {
    this->__set_badbit_and_consider_rethrow();
  }
#  endif // _LIBCPP_HAS_EXCEPTIONS
  return *this;
}
```

文件内 `throw;` 仅出现在 `sentry::~sentry()` 的 forced catch（第 219 行），
没有针对 `__put_num` / `__put_num_integer_promote` 新增直接裸重抛 handler。

## N2：四处 `<cxxabi.h>`

```text
libcxx/include/__ostream/basic_ostream.h:16:#  include <cxxabi.h>
libcxx/include/future:370:#    include <cxxabi.h>
libcxx/include/istream:169:#    include <cxxabi.h>
libcxx/include/string:652:#  include <cxxabi.h>
```

## M1：deferred 恢复与 wait 循环

`libcxx/include/future:891-903`：

```cpp
void __deferred_assoc_state<_Rp, _Fp>::__execute() {
#    if _LIBCPP_HAS_EXCEPTIONS
  try {
#    endif // _LIBCPP_HAS_EXCEPTIONS
    this->set_value(__func_());
#    if _LIBCPP_HAS_EXCEPTIONS
  } catch (const __cxxabiv1::__forced_unwind&) {
    this->__handle_forced_unwind(base::__forced_unwind_action::__restore_deferred);
    throw;
  } catch (...) {
    this->set_exception(current_exception());
  }
#    endif // _LIBCPP_HAS_EXCEPTIONS
}
```

`libcxx/src/future.cpp:113-122`：

```cpp
void __assoc_sub_state::__sub_wait(unique_lock<mutex>& __lk) {
  while (!__is_ready()) {
    if (__state_ & static_cast<unsigned>(deferred)) {
      __state_ &= ~static_cast<unsigned>(deferred);
      __lk.unlock();
      __execute();
      if (!__lk.owns_lock())
        __lk.lock();
    } else {
      __cv_.wait(__lk);
```

## M2：用户析构在锁外

`libcxx/include/future:662-689`：

```cpp
void __assoc_state<_Rp>::__complete_forced_unwind(exception_ptr __error) _NOEXCEPT {
  bool __destroy_value = false;
  try {
    {
      unique_lock<mutex> __lk(this->__mut_);
      if (!(this->__state_ & base::ready)) {
        __destroy_value = (this->__state_ & base::__constructed) != 0;
        this->__state_ &= ~base::__constructed;
        this->__exception_ = std::move(__error);
      }
    }
    if (__destroy_value) {
      try {
        reinterpret_cast<_Rp*>(std::addressof(__value_))->~_Rp();
      } catch (...) {
      }
    }
    unique_lock<mutex> __lk(this->__mut_);
    if (!(this->__state_ & base::ready) && this->__exception_ != nullptr) {
      this->__state_ |= base::ready;
      this->__cv_.notify_all();
    }
```

第一段 `unique_lock` 的块在析构前已经结束；析构后才重新取得锁完成状态。

## 引用特化的两个成员

`libcxx/include/future:733-787` 同时声明并定义：

```cpp
_LIBCPP_HIDE_FROM_ABI void __construct_value_unready(_Rp& __arg);
_LIBCPP_HIDE_FROM_ABI void __complete_forced_unwind(exception_ptr __error) _NOEXCEPT;
```

定义分别在第 762 和 771 行，后者清除 `__constructed`、将 `__value_` 置空、
保存异常并提交 ready/notify。
