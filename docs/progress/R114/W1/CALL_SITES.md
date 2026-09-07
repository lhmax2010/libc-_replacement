# 调用点代码及分类

## include/__condition_variable/condition_variable.h:112

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 108 
 109   template <class _Predicate>
 110   _LIBCPP_HIDE_FROM_ABI void wait(unique_lock<mutex>& __lk, _Predicate __pred) {
 111     while (!__pred())
 112       wait(__lk);
 113   }
 114 
 115   template <class _Clock, class _Duration>
 116   _LIBCPP_HIDE_FROM_ABI cv_status
 117   wait_until(unique_lock<mutex>& __lk, const chrono::time_point<_Clock, _Duration>& __t) {
```

## include/__condition_variable/condition_variable.h:127

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 123       return cv_status::timeout;
 124 
 125     __clock_tp_ns __t_ns = __clock_tp_ns(std::__safe_nanosecond_cast(__t.time_since_epoch()));
 126 
 127     __do_timed_wait(__lk, __t_ns);
 128     return _Clock::now() < __t ? cv_status::no_timeout : cv_status::timeout;
 129   }
 130 
 131   template <class _Clock, class _Duration, class _Predicate>
 132   _LIBCPP_HIDE_FROM_ABI bool
```

## include/__condition_variable/condition_variable.h:135

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 131   template <class _Clock, class _Duration, class _Predicate>
 132   _LIBCPP_HIDE_FROM_ABI bool
 133   wait_until(unique_lock<mutex>& __lk, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred) {
 134     while (!__pred()) {
 135       if (wait_until(__lk, __t) == cv_status::timeout)
 136         return __pred();
 137     }
 138     return true;
 139   }
 140 
```

## include/__condition_variable/condition_variable.h:160

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 156 
 157     __ns_rep __d_ns_count = std::__safe_nanosecond_cast(__d).count();
 158 
 159     if (__now_count_ns > numeric_limits<__ns_rep>::max() - __d_ns_count) {
 160       __do_timed_wait(__lk, __clock_tp_ns::max());
 161     } else {
 162       __do_timed_wait(__lk, __clock_tp_ns(nanoseconds(__now_count_ns + __d_ns_count)));
 163     }
 164 
 165     return steady_clock::now() - __c_now < __d ? cv_status::no_timeout : cv_status::timeout;
```

## include/__condition_variable/condition_variable.h:162

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 158 
 159     if (__now_count_ns > numeric_limits<__ns_rep>::max() - __d_ns_count) {
 160       __do_timed_wait(__lk, __clock_tp_ns::max());
 161     } else {
 162       __do_timed_wait(__lk, __clock_tp_ns(nanoseconds(__now_count_ns + __d_ns_count)));
 163     }
 164 
 165     return steady_clock::now() - __c_now < __d ? cv_status::no_timeout : cv_status::timeout;
 166   }
 167 
```

## include/__condition_variable/condition_variable.h:193

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 189 
 190 template <class _Rep, class _Period, class _Predicate>
 191 inline bool
 192 condition_variable::wait_for(unique_lock<mutex>& __lk, const chrono::duration<_Rep, _Period>& __d, _Predicate __pred) {
 193   return wait_until(__lk, chrono::steady_clock::now() + __d, std::move(__pred));
 194 }
 195 
 196 #  if _LIBCPP_HAS_COND_CLOCKWAIT
 197 inline void condition_variable::__do_timed_wait(
 198     unique_lock<mutex>& __lk, chrono::time_point<chrono::steady_clock, chrono::nanoseconds> __tp) _NOEXCEPT {
```

## include/__condition_variable/condition_variable.h:223

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 219 
 220 template <class _Clock>
 221 inline void condition_variable::__do_timed_wait(unique_lock<mutex>& __lk,
 222                                                 chrono::time_point<_Clock, chrono::nanoseconds> __tp) _NOEXCEPT {
 223   wait_for(__lk, __tp - _Clock::now());
 224 }
 225 
 226 #endif // _LIBCPP_HAS_THREADS
 227 
 228 _LIBCPP_END_NAMESPACE_STD
```

## include/__cxx03/__condition_variable/condition_variable.h:147

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 143 #ifndef _LIBCPP_HAS_NO_THREADS
 144 template <class _Predicate>
 145 void condition_variable::wait(unique_lock<mutex>& __lk, _Predicate __pred) {
 146   while (!__pred())
 147     wait(__lk);
 148 }
 149 
 150 template <class _Clock, class _Duration>
 151 cv_status condition_variable::wait_until(unique_lock<mutex>& __lk, const chrono::time_point<_Clock, _Duration>& __t) {
 152   using namespace chrono;
```

## include/__cxx03/__condition_variable/condition_variable.h:161

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 157     return cv_status::timeout;
 158 
 159   __clock_tp_ns __t_ns = __clock_tp_ns(std::__safe_nanosecond_cast(__t.time_since_epoch()));
 160 
 161   __do_timed_wait(__lk, __t_ns);
 162   return _Clock::now() < __t ? cv_status::no_timeout : cv_status::timeout;
 163 }
 164 
 165 template <class _Clock, class _Duration, class _Predicate>
 166 bool condition_variable::wait_until(
```

## include/__cxx03/__condition_variable/condition_variable.h:169

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 165 template <class _Clock, class _Duration, class _Predicate>
 166 bool condition_variable::wait_until(
 167     unique_lock<mutex>& __lk, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred) {
 168   while (!__pred()) {
 169     if (wait_until(__lk, __t) == cv_status::timeout)
 170       return __pred();
 171   }
 172   return true;
 173 }
 174 
```

## include/__cxx03/__condition_variable/condition_variable.h:194

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 190 
 191   __ns_rep __d_ns_count = std::__safe_nanosecond_cast(__d).count();
 192 
 193   if (__now_count_ns > numeric_limits<__ns_rep>::max() - __d_ns_count) {
 194     __do_timed_wait(__lk, __clock_tp_ns::max());
 195   } else {
 196     __do_timed_wait(__lk, __clock_tp_ns(nanoseconds(__now_count_ns + __d_ns_count)));
 197   }
 198 
 199   return steady_clock::now() - __c_now < __d ? cv_status::no_timeout : cv_status::timeout;
```

## include/__cxx03/__condition_variable/condition_variable.h:196

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 192 
 193   if (__now_count_ns > numeric_limits<__ns_rep>::max() - __d_ns_count) {
 194     __do_timed_wait(__lk, __clock_tp_ns::max());
 195   } else {
 196     __do_timed_wait(__lk, __clock_tp_ns(nanoseconds(__now_count_ns + __d_ns_count)));
 197   }
 198 
 199   return steady_clock::now() - __c_now < __d ? cv_status::no_timeout : cv_status::timeout;
 200 }
 201 
```

## include/__cxx03/__condition_variable/condition_variable.h:205

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 201 
 202 template <class _Rep, class _Period, class _Predicate>
 203 inline bool
 204 condition_variable::wait_for(unique_lock<mutex>& __lk, const chrono::duration<_Rep, _Period>& __d, _Predicate __pred) {
 205   return wait_until(__lk, chrono::steady_clock::now() + __d, std::move(__pred));
 206 }
 207 
 208 #  if defined(_LIBCPP_HAS_COND_CLOCKWAIT)
 209 inline void condition_variable::__do_timed_wait(
 210     unique_lock<mutex>& __lk, chrono::time_point<chrono::steady_clock, chrono::nanoseconds> __tp) _NOEXCEPT {
```

## include/__cxx03/__condition_variable/condition_variable.h:235

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
 231 
 232 template <class _Clock>
 233 inline void condition_variable::__do_timed_wait(unique_lock<mutex>& __lk,
 234                                                 chrono::time_point<_Clock, chrono::nanoseconds> __tp) _NOEXCEPT {
 235   wait_for(__lk, __tp - _Clock::now());
 236 }
 237 
 238 #endif // _LIBCPP_HAS_NO_THREADS
 239 
 240 _LIBCPP_END_NAMESPACE_STD
```

## include/__cxx03/__mutex/unique_lock.h:57

分类：无法判定（模板实参）。守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。

```cpp
  53       : __m_(std::addressof(__m)), __owns_(true) {}
  54 
  55   template <class _Clock, class _Duration>
  56   _LIBCPP_NODISCARD _LIBCPP_HIDE_FROM_ABI unique_lock(mutex_type& __m, const chrono::time_point<_Clock, _Duration>& __t)
  57       : __m_(std::addressof(__m)), __owns_(__m.try_lock_until(__t)) {}
  58 
  59   template <class _Rep, class _Period>
  60   _LIBCPP_NODISCARD _LIBCPP_HIDE_FROM_ABI unique_lock(mutex_type& __m, const chrono::duration<_Rep, _Period>& __d)
  61       : __m_(std::addressof(__m)), __owns_(__m.try_lock_for(__d)) {}
  62 
```

## include/__cxx03/__mutex/unique_lock.h:156

分类：无法判定（模板实参）。守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。

```cpp
 152   if (__m_ == nullptr)
 153     __throw_system_error(EPERM, "unique_lock::try_lock_until: references null mutex");
 154   if (__owns_)
 155     __throw_system_error(EDEADLK, "unique_lock::try_lock_until: already locked");
 156   __owns_ = __m_->try_lock_until(__t);
 157   return __owns_;
 158 }
 159 
 160 template <class _Mutex>
 161 void unique_lock<_Mutex>::unlock() {
```

## include/__cxx03/__thread/this_thread.h:58

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
  54   mutex __mut;
  55   condition_variable __cv;
  56   unique_lock<mutex> __lk(__mut);
  57   while (_Clock::now() < __t)
  58     __cv.wait_until(__lk, __t);
  59 }
  60 
  61 template <class _Duration>
  62 inline _LIBCPP_HIDE_FROM_ABI void sleep_until(const chrono::time_point<chrono::steady_clock, _Duration>& __t) {
  63   this_thread::sleep_for(__t - chrono::steady_clock::now());
```

## include/__cxx03/condition_variable:208

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 204   shared_ptr<mutex> __mut = __mut_;
 205   unique_lock<mutex> __lk(*__mut);
 206   __unlock_guard<_Lock> __unlock(__lock);
 207   lock_guard<unique_lock<mutex> > __lx(__lk, adopt_lock_t());
 208   __cv_.wait(__lk);
 209 } // __mut_.unlock(), __lock.lock()
 210 
 211 template <class _Lock, class _Predicate>
 212 inline void condition_variable_any::wait(_Lock& __lock, _Predicate __pred) {
 213   while (!__pred())
```

## include/__cxx03/condition_variable:214

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 210 
 211 template <class _Lock, class _Predicate>
 212 inline void condition_variable_any::wait(_Lock& __lock, _Predicate __pred) {
 213   while (!__pred())
 214     wait(__lock);
 215 }
 216 
 217 template <class _Lock, class _Clock, class _Duration>
 218 cv_status condition_variable_any::wait_until(_Lock& __lock, const chrono::time_point<_Clock, _Duration>& __t) {
 219   shared_ptr<mutex> __mut = __mut_;
```

## include/__cxx03/condition_variable:223

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 219   shared_ptr<mutex> __mut = __mut_;
 220   unique_lock<mutex> __lk(*__mut);
 221   __unlock_guard<_Lock> __unlock(__lock);
 222   lock_guard<unique_lock<mutex> > __lx(__lk, adopt_lock_t());
 223   return __cv_.wait_until(__lk, __t);
 224 } // __mut_.unlock(), __lock.lock()
 225 
 226 template <class _Lock, class _Clock, class _Duration, class _Predicate>
 227 inline bool
 228 condition_variable_any::wait_until(_Lock& __lock, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred) {
```

## include/__cxx03/condition_variable:230

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 226 template <class _Lock, class _Clock, class _Duration, class _Predicate>
 227 inline bool
 228 condition_variable_any::wait_until(_Lock& __lock, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred) {
 229   while (!__pred())
 230     if (wait_until(__lock, __t) == cv_status::timeout)
 231       return __pred();
 232   return true;
 233 }
 234 
 235 template <class _Lock, class _Rep, class _Period>
```

## include/__cxx03/condition_variable:237

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 233 }
 234 
 235 template <class _Lock, class _Rep, class _Period>
 236 inline cv_status condition_variable_any::wait_for(_Lock& __lock, const chrono::duration<_Rep, _Period>& __d) {
 237   return wait_until(__lock, chrono::steady_clock::now() + __d);
 238 }
 239 
 240 template <class _Lock, class _Rep, class _Period, class _Predicate>
 241 inline bool
 242 condition_variable_any::wait_for(_Lock& __lock, const chrono::duration<_Rep, _Period>& __d, _Predicate __pred) {
```

## include/__cxx03/condition_variable:243

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 239 
 240 template <class _Lock, class _Rep, class _Period, class _Predicate>
 241 inline bool
 242 condition_variable_any::wait_for(_Lock& __lock, const chrono::duration<_Rep, _Period>& __d, _Predicate __pred) {
 243   return wait_until(__lock, chrono::steady_clock::now() + __d, std::move(__pred));
 244 }
 245 
 246 _LIBCPP_EXPORTED_FROM_ABI void notify_all_at_thread_exit(condition_variable&, unique_lock<mutex>);
 247 
 248 _LIBCPP_END_NAMESPACE_STD
```

## include/__cxx03/future:517

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 513   unique_lock<mutex> __lk(__mut_);
 514   if (__state_ & deferred)
 515     return future_status::deferred;
 516   while (!(__state_ & ready) && _Clock::now() < __abs_time)
 517     __cv_.wait_until(__lk, __abs_time);
 518   if (__state_ & ready)
 519     return future_status::ready;
 520   return future_status::timeout;
 521 }
 522 
```

## include/__cxx03/future:525

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 521 }
 522 
 523 template <class _Rep, class _Period>
 524 inline future_status __assoc_sub_state::wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 525   return wait_until(chrono::steady_clock::now() + __rel_time);
 526 }
 527 
 528 template <class _Rp>
 529 class _LIBCPP_HIDDEN __assoc_state : public __assoc_sub_state {
 530   typedef __assoc_sub_state base;
```

## include/__cxx03/future:583

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 579 
 580 template <class _Rp>
 581 _Rp __assoc_state<_Rp>::move() {
 582   unique_lock<mutex> __lk(this->__mut_);
 583   this->__sub_wait(__lk);
 584   if (this->__exception_ != nullptr)
 585     std::rethrow_exception(this->__exception_);
 586   return std::move(*reinterpret_cast<_Rp*>(&__value_));
 587 }
 588 
```

## include/__cxx03/future:592

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 588 
 589 template <class _Rp>
 590 __add_lvalue_reference_t<_Rp> __assoc_state<_Rp>::copy() {
 591   unique_lock<mutex> __lk(this->__mut_);
 592   this->__sub_wait(__lk);
 593   if (this->__exception_ != nullptr)
 594     std::rethrow_exception(this->__exception_);
 595   return *reinterpret_cast<_Rp*>(&__value_);
 596 }
 597 
```

## include/__cxx03/future:643

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 639 
 640 template <class _Rp>
 641 _Rp& __assoc_state<_Rp&>::copy() {
 642   unique_lock<mutex> __lk(this->__mut_);
 643   this->__sub_wait(__lk);
 644   if (this->__exception_ != nullptr)
 645     std::rethrow_exception(this->__exception_);
 646   return *__value_;
 647 }
 648 
```

## include/__cxx03/future:807

分类：无法判定（展开不能穿过此规格）。async 最后引用释放的 __on_zero_shared 为 noexcept，调用 wait 后才销毁；本次四入口解除不解除这一边界，不能归为可回滚的已退出残留。

```cpp
 803 }
 804 
 805 template <class _Rp, class _Fp>
 806 void __async_assoc_state<_Rp, _Fp>::__on_zero_shared() _NOEXCEPT {
 807   this->wait();
 808   base::__on_zero_shared();
 809 }
 810 
 811 template <class _Fp>
 812 class __async_assoc_state<void, _Fp> : public __assoc_sub_state {
```

## include/__cxx03/future:844

分类：无法判定（展开不能穿过此规格）。async 最后引用释放的 __on_zero_shared 为 noexcept，调用 wait 后才销毁；本次四入口解除不解除这一边界，不能归为可回滚的已退出残留。

```cpp
 840 }
 841 
 842 template <class _Fp>
 843 void __async_assoc_state<void, _Fp>::__on_zero_shared() _NOEXCEPT {
 844   this->wait();
 845   base::__on_zero_shared();
 846 }
 847 
 848 template <class _Rp>
 849 class _LIBCPP_TEMPLATE_VIS promise;
```

## include/__cxx03/future:901

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 897 
 898   // functions to check state
 899   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
 900 
 901   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
 902   template <class _Rep, class _Period>
 903   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 904     return __state_->wait_for(__rel_time);
 905   }
 906   template <class _Clock, class _Duration>
```

## include/__cxx03/future:904

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 900 
 901   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
 902   template <class _Rep, class _Period>
 903   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 904     return __state_->wait_for(__rel_time);
 905   }
 906   template <class _Clock, class _Duration>
 907   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
 908     return __state_->wait_until(__abs_time);
 909   }
```

## include/__cxx03/future:908

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 904     return __state_->wait_for(__rel_time);
 905   }
 906   template <class _Clock, class _Duration>
 907   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
 908     return __state_->wait_until(__abs_time);
 909   }
 910 };
 911 
 912 template <class _Rp>
 913 future<_Rp>::future(__assoc_state<_Rp>* __state) : __state_(__state) {
```

## include/__cxx03/future:932

分类：有残留但无害（限定普通共享状态）。get 先置空 future 指针，unique_ptr/release_shared_count 负责释放原引用；消费 future 的状态保留但无孤立锁。async 最后引用另列 N。

```cpp
 928 _Rp future<_Rp>::get() {
 929   unique_ptr<__shared_count, __release_shared_count> __guard(__state_);
 930   __assoc_state<_Rp>* __s = __state_;
 931   __state_                = nullptr;
 932   return __s->move();
 933 }
 934 
 935 template <class _Rp>
 936 class _LIBCPP_TEMPLATE_VIS future<_Rp&> {
 937   __assoc_state<_Rp&>* __state_;
```

## include/__cxx03/future:972

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 968 
 969   // functions to check state
 970   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
 971 
 972   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
 973   template <class _Rep, class _Period>
 974   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 975     return __state_->wait_for(__rel_time);
 976   }
 977   template <class _Clock, class _Duration>
```

## include/__cxx03/future:975

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 971 
 972   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
 973   template <class _Rep, class _Period>
 974   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 975     return __state_->wait_for(__rel_time);
 976   }
 977   template <class _Clock, class _Duration>
 978   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
 979     return __state_->wait_until(__abs_time);
 980   }
```

## include/__cxx03/future:979

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 975     return __state_->wait_for(__rel_time);
 976   }
 977   template <class _Clock, class _Duration>
 978   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
 979     return __state_->wait_until(__abs_time);
 980   }
 981 };
 982 
 983 template <class _Rp>
 984 future<_Rp&>::future(__assoc_state<_Rp&>* __state) : __state_(__state) {
```

## include/__cxx03/future:999

分类：有残留但无害（限定普通共享状态）。get 先置空 future 指针，unique_ptr/release_shared_count 负责释放原引用；消费 future 的状态保留但无孤立锁。async 最后引用另列 N。

```cpp
 995 _Rp& future<_Rp&>::get() {
 996   unique_ptr<__shared_count, __release_shared_count> __guard(__state_);
 997   __assoc_state<_Rp&>* __s = __state_;
 998   __state_                 = nullptr;
 999   return __s->copy();
1000 }
1001 
1002 template <>
1003 class _LIBCPP_EXPORTED_FROM_ABI future<void> {
1004   __assoc_sub_state* __state_;
```

## include/__cxx03/future:1039

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1035 
1036   // functions to check state
1037   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1038 
1039   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1040   template <class _Rep, class _Period>
1041   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1042     return __state_->wait_for(__rel_time);
1043   }
1044   template <class _Clock, class _Duration>
```

## include/__cxx03/future:1042

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1038 
1039   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1040   template <class _Rep, class _Period>
1041   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1042     return __state_->wait_for(__rel_time);
1043   }
1044   template <class _Clock, class _Duration>
1045   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1046     return __state_->wait_until(__abs_time);
1047   }
```

## include/__cxx03/future:1046

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1042     return __state_->wait_for(__rel_time);
1043   }
1044   template <class _Clock, class _Duration>
1045   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1046     return __state_->wait_until(__abs_time);
1047   }
1048 };
1049 
1050 template <class _Rp>
1051 inline _LIBCPP_HIDE_FROM_ABI void swap(future<_Rp>& __x, future<_Rp>& __y) _NOEXCEPT {
```

## include/__cxx03/future:1781

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1777     return *this;
1778   }
1779 
1780   // retrieving the value
1781   _LIBCPP_HIDE_FROM_ABI const _Rp& get() const { return __state_->copy(); }
1782 
1783   _LIBCPP_HIDE_FROM_ABI void swap(shared_future& __rhs) _NOEXCEPT { std::swap(__state_, __rhs.__state_); }
1784 
1785   // functions to check state
1786   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
```

## include/__cxx03/future:1788

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1784 
1785   // functions to check state
1786   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1787 
1788   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1789   template <class _Rep, class _Period>
1790   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1791     return __state_->wait_for(__rel_time);
1792   }
1793   template <class _Clock, class _Duration>
```

## include/__cxx03/future:1791

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1787 
1788   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1789   template <class _Rep, class _Period>
1790   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1791     return __state_->wait_for(__rel_time);
1792   }
1793   template <class _Clock, class _Duration>
1794   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1795     return __state_->wait_until(__abs_time);
1796   }
```

## include/__cxx03/future:1795

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1791     return __state_->wait_for(__rel_time);
1792   }
1793   template <class _Clock, class _Duration>
1794   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1795     return __state_->wait_until(__abs_time);
1796   }
1797 };
1798 
1799 template <class _Rp>
1800 shared_future<_Rp>::~shared_future() {
```

## include/__cxx03/future:1837

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1833     return *this;
1834   }
1835 
1836   // retrieving the value
1837   _LIBCPP_HIDE_FROM_ABI _Rp& get() const { return __state_->copy(); }
1838 
1839   _LIBCPP_HIDE_FROM_ABI void swap(shared_future& __rhs) _NOEXCEPT { std::swap(__state_, __rhs.__state_); }
1840 
1841   // functions to check state
1842   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
```

## include/__cxx03/future:1844

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1840 
1841   // functions to check state
1842   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1843 
1844   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1845   template <class _Rep, class _Period>
1846   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1847     return __state_->wait_for(__rel_time);
1848   }
1849   template <class _Clock, class _Duration>
```

## include/__cxx03/future:1847

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1843 
1844   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1845   template <class _Rep, class _Period>
1846   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1847     return __state_->wait_for(__rel_time);
1848   }
1849   template <class _Clock, class _Duration>
1850   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1851     return __state_->wait_until(__abs_time);
1852   }
```

## include/__cxx03/future:1851

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1847     return __state_->wait_for(__rel_time);
1848   }
1849   template <class _Clock, class _Duration>
1850   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1851     return __state_->wait_until(__abs_time);
1852   }
1853 };
1854 
1855 template <class _Rp>
1856 shared_future<_Rp&>::~shared_future() {
```

## include/__cxx03/future:1893

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1889     return *this;
1890   }
1891 
1892   // retrieving the value
1893   _LIBCPP_HIDE_FROM_ABI void get() const { __state_->copy(); }
1894 
1895   _LIBCPP_HIDE_FROM_ABI void swap(shared_future& __rhs) _NOEXCEPT { std::swap(__state_, __rhs.__state_); }
1896 
1897   // functions to check state
1898   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
```

## include/__cxx03/future:1900

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1896 
1897   // functions to check state
1898   _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1899 
1900   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1901   template <class _Rep, class _Period>
1902   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1903     return __state_->wait_for(__rel_time);
1904   }
1905   template <class _Clock, class _Duration>
```

## include/__cxx03/future:1903

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1899 
1900   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1901   template <class _Rep, class _Period>
1902   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1903     return __state_->wait_for(__rel_time);
1904   }
1905   template <class _Clock, class _Duration>
1906   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1907     return __state_->wait_until(__abs_time);
1908   }
```

## include/__cxx03/future:1907

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1903     return __state_->wait_for(__rel_time);
1904   }
1905   template <class _Clock, class _Duration>
1906   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1907     return __state_->wait_until(__abs_time);
1908   }
1909 };
1910 
1911 template <class _Rp>
1912 inline _LIBCPP_HIDE_FROM_ABI void swap(shared_future<_Rp>& __x, shared_future<_Rp>& __y) _NOEXCEPT {
```

## include/__cxx03/mutex:253

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 249   void lock();
 250   bool try_lock() _NOEXCEPT;
 251   template <class _Rep, class _Period>
 252   _LIBCPP_HIDE_FROM_ABI bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) {
 253     return try_lock_until(chrono::steady_clock::now() + __d);
 254   }
 255   template <class _Clock, class _Duration>
 256   _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS bool
 257   try_lock_until(const chrono::time_point<_Clock, _Duration>& __t);
 258   void unlock() _NOEXCEPT;
```

## include/__cxx03/mutex:267

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 263   using namespace chrono;
 264   unique_lock<mutex> __lk(__m_);
 265   bool __no_timeout = _Clock::now() < __t;
 266   while (__no_timeout && __locked_)
 267     __no_timeout = __cv_.wait_until(__lk, __t) == cv_status::no_timeout;
 268   if (!__locked_) {
 269     __locked_ = true;
 270     return true;
 271   }
 272   return false;
```

## include/__cxx03/mutex:292

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 288   void lock();
 289   bool try_lock() _NOEXCEPT;
 290   template <class _Rep, class _Period>
 291   _LIBCPP_HIDE_FROM_ABI bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) {
 292     return try_lock_until(chrono::steady_clock::now() + __d);
 293   }
 294   template <class _Clock, class _Duration>
 295   _LIBCPP_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS bool
 296   try_lock_until(const chrono::time_point<_Clock, _Duration>& __t);
 297   void unlock() _NOEXCEPT;
```

## include/__cxx03/mutex:313

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 309     return true;
 310   }
 311   bool __no_timeout = _Clock::now() < __t;
 312   while (__no_timeout && __count_ != 0)
 313     __no_timeout = __cv_.wait_until(__lk, __t) == cv_status::no_timeout;
 314   if (__count_ == 0) {
 315     __count_ = 1;
 316     __id_    = __id;
 317     return true;
 318   }
```

## include/__mutex/unique_lock.h:59

分类：无法判定（模板实参）。守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。

```cpp
  55       : __m_(std::addressof(__m)), __owns_(true) {}
  56 
  57   template <class _Clock, class _Duration>
  58   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI unique_lock(mutex_type& __m, const chrono::time_point<_Clock, _Duration>& __t)
  59       : __m_(std::addressof(__m)), __owns_(__m.try_lock_until(__t)) {}
  60 
  61   template <class _Rep, class _Period>
  62   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI unique_lock(mutex_type& __m, const chrono::duration<_Rep, _Period>& __d)
  63       : __m_(std::addressof(__m)), __owns_(__m.try_lock_for(__d)) {}
  64 
```

## include/__mutex/unique_lock.h:153

分类：无法判定（模板实参）。守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。

```cpp
 149   if (__m_ == nullptr)
 150     std::__throw_system_error(EPERM, "unique_lock::try_lock_until: references null mutex");
 151   if (__owns_)
 152     std::__throw_system_error(EDEADLK, "unique_lock::try_lock_until: already locked");
 153   __owns_ = __m_->try_lock_until(__t);
 154   return __owns_;
 155 }
 156 
 157 template <class _Mutex>
 158 _LIBCPP_HIDE_FROM_ABI void unique_lock<_Mutex>::unlock() {
```

## include/__thread/this_thread.h:61

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
  57   mutex __mut;
  58   condition_variable __cv;
  59   unique_lock<mutex> __lk(__mut);
  60   while (_Clock::now() < __t)
  61     __cv.wait_until(__lk, __t);
  62 }
  63 
  64 template <class _Duration>
  65 inline _LIBCPP_HIDE_FROM_ABI void sleep_until(const chrono::time_point<chrono::steady_clock, _Duration>& __t) {
  66   this_thread::sleep_for(__t - chrono::steady_clock::now());
```

## include/condition_variable:181

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 177     shared_ptr<mutex> __mut = __mut_;
 178     unique_lock<mutex> __lk(*__mut);
 179     __unlock_guard<_Lock> __unlock(__lock);
 180     lock_guard<unique_lock<mutex> > __lx(__lk, adopt_lock_t());
 181     __cv_.wait(__lk);
 182   } // __mut_.unlock(), __lock.lock()
 183 
 184   template <class _Lock, class _Predicate>
 185   _LIBCPP_HIDE_FROM_ABI void wait(_Lock& __lock, _Predicate __pred);
 186 
```

## include/condition_variable:193

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 189     shared_ptr<mutex> __mut = __mut_;
 190     unique_lock<mutex> __lk(*__mut);
 191     __unlock_guard<_Lock> __unlock(__lock);
 192     lock_guard<unique_lock<mutex> > __lx(__lk, adopt_lock_t());
 193     return __cv_.wait_until(__lk, __t);
 194   } // __mut_.unlock(), __lock.lock()
 195 
 196   template <class _Lock, class _Clock, class _Duration, class _Predicate>
 197   bool _LIBCPP_HIDE_FROM_ABI
 198   wait_until(_Lock& __lock, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred);
```

## include/condition_variable:237

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 233 
 234 template <class _Lock, class _Predicate>
 235 inline void condition_variable_any::wait(_Lock& __lock, _Predicate __pred) {
 236   while (!__pred())
 237     wait(__lock);
 238 }
 239 
 240 template <class _Lock, class _Clock, class _Duration, class _Predicate>
 241 inline bool
 242 condition_variable_any::wait_until(_Lock& __lock, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred) {
```

## include/condition_variable:244

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 240 template <class _Lock, class _Clock, class _Duration, class _Predicate>
 241 inline bool
 242 condition_variable_any::wait_until(_Lock& __lock, const chrono::time_point<_Clock, _Duration>& __t, _Predicate __pred) {
 243   while (!__pred())
 244     if (wait_until(__lock, __t) == cv_status::timeout)
 245       return __pred();
 246   return true;
 247 }
 248 
 249 template <class _Lock, class _Rep, class _Period>
```

## include/condition_variable:251

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 247 }
 248 
 249 template <class _Lock, class _Rep, class _Period>
 250 inline cv_status condition_variable_any::wait_for(_Lock& __lock, const chrono::duration<_Rep, _Period>& __d) {
 251   return wait_until(__lock, chrono::steady_clock::now() + __d);
 252 }
 253 
 254 template <class _Lock, class _Rep, class _Period, class _Predicate>
 255 inline bool
 256 condition_variable_any::wait_for(_Lock& __lock, const chrono::duration<_Rep, _Period>& __d, _Predicate __pred) {
```

## include/condition_variable:257

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 253 
 254 template <class _Lock, class _Rep, class _Period, class _Predicate>
 255 inline bool
 256 condition_variable_any::wait_for(_Lock& __lock, const chrono::duration<_Rep, _Period>& __d, _Predicate __pred) {
 257   return wait_until(__lock, chrono::steady_clock::now() + __d, std::move(__pred));
 258 }
 259 
 260 #    if _LIBCPP_STD_VER >= 20
 261 
 262 template <class _Lock, class _Predicate>
```

## include/condition_variable:300

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 296 
 297     __unlock_guard<_Lock> __unlock(__user_lock);
 298     unique_lock<mutex> __internal_lock2(
 299         std::move(__internal_lock)); // switch unlock order between __internal_lock and __user_lock
 300     __cv_.wait(__internal_lock2);
 301   } // __internal_lock2.unlock(), __user_lock.lock()
 302   return __pred();
 303 }
 304 
 305 template <class _Lock, class _Clock, class _Duration, class _Predicate>
```

## include/condition_variable:329

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 325     __unlock_guard<_Lock> __unlock(__user_lock);
 326     unique_lock<mutex> __internal_lock2(
 327         std::move(__internal_lock)); // switch unlock order between __internal_lock and __user_lock
 328 
 329     if (__cv_.wait_until(__internal_lock2, __abs_time) == cv_status::timeout)
 330       break;
 331   } // __internal_lock2.unlock(), __user_lock.lock()
 332   return __pred();
 333 }
 334 
```

## include/condition_variable:338

分类：安全（限定成功重锁）。复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。

```cpp
 334 
 335 template <class _Lock, class _Rep, class _Period, class _Predicate>
 336 bool condition_variable_any::wait_for(
 337     _Lock& __lock, stop_token __stoken, const chrono::duration<_Rep, _Period>& __rel_time, _Predicate __pred) {
 338   return wait_until(__lock, std::move(__stoken), chrono::steady_clock::now() + __rel_time, std::move(__pred));
 339 }
 340 
 341 #    endif // _LIBCPP_STD_VER >= 20
 342 
 343 _LIBCPP_EXPORTED_FROM_ABI void notify_all_at_thread_exit(condition_variable&, unique_lock<mutex>);
```

## include/future:571

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 567     unique_lock<mutex> __lk(__mut_);
 568     if (__state_ & deferred)
 569       return future_status::deferred;
 570     while (!(__state_ & ready) && _Clock::now() < __abs_time)
 571       __cv_.wait_until(__lk, __abs_time);
 572     if (__state_ & ready)
 573       return future_status::ready;
 574     return future_status::timeout;
 575   }
 576 
```

## include/future:582

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 578 };
 579 
 580 template <class _Rep, class _Period>
 581 inline future_status __assoc_sub_state::wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 582   return wait_until(chrono::steady_clock::now() + __rel_time);
 583 }
 584 
 585 template <class _Rp>
 586 class _LIBCPP_HIDDEN __assoc_state : public __assoc_sub_state {
 587   typedef __assoc_sub_state base;
```

## include/future:637

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 633 
 634 template <class _Rp>
 635 _Rp __assoc_state<_Rp>::move() {
 636   unique_lock<mutex> __lk(this->__mut_);
 637   this->__sub_wait(__lk);
 638   if (this->__exception_ != nullptr)
 639     std::rethrow_exception(this->__exception_);
 640   return std::move(*reinterpret_cast<_Rp*>(std::addressof(__value_)));
 641 }
 642 
```

## include/future:646

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 642 
 643 template <class _Rp>
 644 _Rp& __assoc_state<_Rp>::copy() {
 645   unique_lock<mutex> __lk(this->__mut_);
 646   this->__sub_wait(__lk);
 647   if (this->__exception_ != nullptr)
 648     std::rethrow_exception(this->__exception_);
 649   return *reinterpret_cast<_Rp*>(std::addressof(__value_));
 650 }
 651 
```

## include/future:697

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 693 
 694 template <class _Rp>
 695 _Rp& __assoc_state<_Rp&>::copy() {
 696   unique_lock<mutex> __lk(this->__mut_);
 697   this->__sub_wait(__lk);
 698   if (this->__exception_ != nullptr)
 699     std::rethrow_exception(this->__exception_);
 700   return *__value_;
 701 }
 702 
```

## include/future:861

分类：无法判定（展开不能穿过此规格）。async 最后引用释放的 __on_zero_shared 为 noexcept，调用 wait 后才销毁；本次四入口解除不解除这一边界，不能归为可回滚的已退出残留。

```cpp
 857 }
 858 
 859 template <class _Rp, class _Fp>
 860 void __async_assoc_state<_Rp, _Fp>::__on_zero_shared() _NOEXCEPT {
 861   this->wait();
 862   base::__on_zero_shared();
 863 }
 864 
 865 template <class _Fp>
 866 class __async_assoc_state<void, _Fp> : public __assoc_sub_state {
```

## include/future:898

分类：无法判定（展开不能穿过此规格）。async 最后引用释放的 __on_zero_shared 为 noexcept，调用 wait 后才销毁；本次四入口解除不解除这一边界，不能归为可回滚的已退出残留。

```cpp
 894 }
 895 
 896 template <class _Fp>
 897 void __async_assoc_state<void, _Fp>::__on_zero_shared() _NOEXCEPT {
 898   this->wait();
 899   base::__on_zero_shared();
 900 }
 901 
 902 template <class _Rp>
 903 class promise;
```

## include/future:955

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 951 
 952   // functions to check state
 953   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
 954 
 955   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
 956   template <class _Rep, class _Period>
 957   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 958     return __state_->wait_for(__rel_time);
 959   }
 960   template <class _Clock, class _Duration>
```

## include/future:958

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 954 
 955   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
 956   template <class _Rep, class _Period>
 957   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
 958     return __state_->wait_for(__rel_time);
 959   }
 960   template <class _Clock, class _Duration>
 961   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
 962     return __state_->wait_until(__abs_time);
 963   }
```

## include/future:962

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 958     return __state_->wait_for(__rel_time);
 959   }
 960   template <class _Clock, class _Duration>
 961   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
 962     return __state_->wait_until(__abs_time);
 963   }
 964 };
 965 
 966 template <class _Rp>
 967 future<_Rp>::future(__assoc_state<_Rp>* __state) : __state_(__state) {
```

## include/future:986

分类：有残留但无害（限定普通共享状态）。get 先置空 future 指针，unique_ptr/release_shared_count 负责释放原引用；消费 future 的状态保留但无孤立锁。async 最后引用另列 N。

```cpp
 982 _Rp future<_Rp>::get() {
 983   unique_ptr<__shared_count, __release_shared_count> __guard(__state_);
 984   __assoc_state<_Rp>* __s = __state_;
 985   __state_                = nullptr;
 986   return __s->move();
 987 }
 988 
 989 template <class _Rp>
 990 class future<_Rp&> {
 991   __assoc_state<_Rp&>* __state_;
```

## include/future:1026

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1022 
1023   // functions to check state
1024   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1025 
1026   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1027   template <class _Rep, class _Period>
1028   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1029     return __state_->wait_for(__rel_time);
1030   }
1031   template <class _Clock, class _Duration>
```

## include/future:1029

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1025 
1026   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1027   template <class _Rep, class _Period>
1028   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1029     return __state_->wait_for(__rel_time);
1030   }
1031   template <class _Clock, class _Duration>
1032   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1033     return __state_->wait_until(__abs_time);
1034   }
```

## include/future:1033

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1029     return __state_->wait_for(__rel_time);
1030   }
1031   template <class _Clock, class _Duration>
1032   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1033     return __state_->wait_until(__abs_time);
1034   }
1035 };
1036 
1037 template <class _Rp>
1038 future<_Rp&>::future(__assoc_state<_Rp&>* __state) : __state_(__state) {
```

## include/future:1053

分类：有残留但无害（限定普通共享状态）。get 先置空 future 指针，unique_ptr/release_shared_count 负责释放原引用；消费 future 的状态保留但无孤立锁。async 最后引用另列 N。

```cpp
1049 _Rp& future<_Rp&>::get() {
1050   unique_ptr<__shared_count, __release_shared_count> __guard(__state_);
1051   __assoc_state<_Rp&>* __s = __state_;
1052   __state_                 = nullptr;
1053   return __s->copy();
1054 }
1055 
1056 template <>
1057 class _LIBCPP_EXPORTED_FROM_ABI future<void> {
1058   __assoc_sub_state* __state_;
```

## include/future:1093

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1089 
1090   // functions to check state
1091   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1092 
1093   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1094   template <class _Rep, class _Period>
1095   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1096     return __state_->wait_for(__rel_time);
1097   }
1098   template <class _Clock, class _Duration>
```

## include/future:1096

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1092 
1093   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1094   template <class _Rep, class _Period>
1095   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1096     return __state_->wait_for(__rel_time);
1097   }
1098   template <class _Clock, class _Duration>
1099   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1100     return __state_->wait_until(__abs_time);
1101   }
```

## include/future:1100

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1096     return __state_->wait_for(__rel_time);
1097   }
1098   template <class _Clock, class _Duration>
1099   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1100     return __state_->wait_until(__abs_time);
1101   }
1102 };
1103 
1104 template <class _Rp>
1105 inline _LIBCPP_HIDE_FROM_ABI void swap(future<_Rp>& __x, future<_Rp>& __y) _NOEXCEPT {
```

## include/future:1908

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1904     return *this;
1905   }
1906 
1907   // retrieving the value
1908   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI const _Rp& get() const { return __state_->copy(); }
1909 
1910   _LIBCPP_HIDE_FROM_ABI void swap(shared_future& __rhs) _NOEXCEPT { std::swap(__state_, __rhs.__state_); }
1911 
1912   // functions to check state
1913   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
```

## include/future:1915

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1911 
1912   // functions to check state
1913   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1914 
1915   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1916   template <class _Rep, class _Period>
1917   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1918     return __state_->wait_for(__rel_time);
1919   }
1920   template <class _Clock, class _Duration>
```

## include/future:1918

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1914 
1915   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1916   template <class _Rep, class _Period>
1917   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1918     return __state_->wait_for(__rel_time);
1919   }
1920   template <class _Clock, class _Duration>
1921   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1922     return __state_->wait_until(__abs_time);
1923   }
```

## include/future:1922

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1918     return __state_->wait_for(__rel_time);
1919   }
1920   template <class _Clock, class _Duration>
1921   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1922     return __state_->wait_until(__abs_time);
1923   }
1924 };
1925 
1926 template <class _Rp>
1927 shared_future<_Rp>::~shared_future() {
```

## include/future:1964

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1960     return *this;
1961   }
1962 
1963   // retrieving the value
1964   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI _Rp& get() const { return __state_->copy(); }
1965 
1966   _LIBCPP_HIDE_FROM_ABI void swap(shared_future& __rhs) _NOEXCEPT { std::swap(__state_, __rhs.__state_); }
1967 
1968   // functions to check state
1969   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
```

## include/future:1971

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1967 
1968   // functions to check state
1969   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
1970 
1971   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1972   template <class _Rep, class _Period>
1973   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1974     return __state_->wait_for(__rel_time);
1975   }
1976   template <class _Clock, class _Duration>
```

## include/future:1974

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1970 
1971   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
1972   template <class _Rep, class _Period>
1973   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
1974     return __state_->wait_for(__rel_time);
1975   }
1976   template <class _Clock, class _Duration>
1977   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1978     return __state_->wait_until(__abs_time);
1979   }
```

## include/future:1978

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
1974     return __state_->wait_for(__rel_time);
1975   }
1976   template <class _Clock, class _Duration>
1977   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
1978     return __state_->wait_until(__abs_time);
1979   }
1980 };
1981 
1982 template <class _Rp>
1983 shared_future<_Rp&>::~shared_future() {
```

## include/future:2020

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
2016     return *this;
2017   }
2018 
2019   // retrieving the value
2020   _LIBCPP_HIDE_FROM_ABI void get() const { __state_->copy(); }
2021 
2022   _LIBCPP_HIDE_FROM_ABI void swap(shared_future& __rhs) _NOEXCEPT { std::swap(__state_, __rhs.__state_); }
2023 
2024   // functions to check state
2025   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
```

## include/future:2027

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
2023 
2024   // functions to check state
2025   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool valid() const _NOEXCEPT { return __state_ != nullptr; }
2026 
2027   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
2028   template <class _Rep, class _Period>
2029   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
2030     return __state_->wait_for(__rel_time);
2031   }
2032   template <class _Clock, class _Duration>
```

## include/future:2030

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
2026 
2027   _LIBCPP_HIDE_FROM_ABI void wait() const { __state_->wait(); }
2028   template <class _Rep, class _Period>
2029   _LIBCPP_HIDE_FROM_ABI future_status wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const {
2030     return __state_->wait_for(__rel_time);
2031   }
2032   template <class _Clock, class _Duration>
2033   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
2034     return __state_->wait_until(__abs_time);
2035   }
```

## include/future:2034

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
2030     return __state_->wait_for(__rel_time);
2031   }
2032   template <class _Clock, class _Duration>
2033   _LIBCPP_HIDE_FROM_ABI future_status wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const {
2034     return __state_->wait_until(__abs_time);
2035   }
2036 };
2037 
2038 template <class _Rp>
2039 inline _LIBCPP_HIDE_FROM_ABI void swap(shared_future<_Rp>& __x, shared_future<_Rp>& __y) _NOEXCEPT {
```

## include/mutex:257

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 253   void lock();
 254   [[__nodiscard__]] bool try_lock() _NOEXCEPT;
 255   template <class _Rep, class _Period>
 256   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) {
 257     return try_lock_until(chrono::steady_clock::now() + __d);
 258   }
 259 
 260   template <class _Clock, class _Duration>
 261   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __t) {
 262     using namespace chrono;
```

## include/mutex:266

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 262     using namespace chrono;
 263     unique_lock<mutex> __lk(__m_);
 264     bool __no_timeout = _Clock::now() < __t;
 265     while (__no_timeout && __locked_)
 266       __no_timeout = __cv_.wait_until(__lk, __t) == cv_status::no_timeout;
 267     if (!__locked_) {
 268       __locked_ = true;
 269       return true;
 270     }
 271     return false;
```

## include/mutex:294

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 290   void lock();
 291   [[__nodiscard__]] bool try_lock() _NOEXCEPT;
 292   template <class _Rep, class _Period>
 293   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) {
 294     return try_lock_until(chrono::steady_clock::now() + __d);
 295   }
 296 
 297   template <class _Clock, class _Duration>
 298   [[__nodiscard__]] _LIBCPP_HIDE_FROM_ABI bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __t) {
 299     using namespace chrono;
```

## include/mutex:310

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 306       return true;
 307     }
 308     bool __no_timeout = _Clock::now() < __t;
 309     while (__no_timeout && __count_ != 0)
 310       __no_timeout = __cv_.wait_until(__lk, __t) == cv_status::no_timeout;
 311     if (__count_ == 0) {
 312       __count_ = 1;
 313       __id_    = __id;
 314       return true;
 315     }
```

## include/shared_mutex:198

分类：有残留且有害。直接或经包装在 gate2 等待读者退出前设置 write_entered；仅正常超时清除，展开无回滚。

```cpp
 194   shared_mutex(const shared_mutex&)            = delete;
 195   shared_mutex& operator=(const shared_mutex&) = delete;
 196 
 197   // Exclusive ownership
 198   _LIBCPP_ACQUIRE_CAPABILITY() _LIBCPP_HIDE_FROM_ABI void lock() { return __base_.lock(); }
 199   _LIBCPP_TRY_ACQUIRE_CAPABILITY(true) _LIBCPP_HIDE_FROM_ABI bool try_lock() { return __base_.try_lock(); }
 200   _LIBCPP_RELEASE_CAPABILITY _LIBCPP_HIDE_FROM_ABI void unlock() { return __base_.unlock(); }
 201 
 202   // Shared ownership
 203   _LIBCPP_ACQUIRE_SHARED_CAPABILITY _LIBCPP_HIDE_FROM_ABI void lock_shared() { return __base_.lock_shared(); }
```

## include/shared_mutex:230

分类：有残留且有害。直接或经包装在 gate2 等待读者退出前设置 write_entered；仅正常超时清除，展开无回滚。

```cpp
 226   _LIBCPP_TRY_ACQUIRE_CAPABILITY(true) bool try_lock();
 227   template <class _Rep, class _Period>
 228   _LIBCPP_TRY_ACQUIRE_CAPABILITY(true) _LIBCPP_HIDE_FROM_ABI bool
 229   try_lock_for(const chrono::duration<_Rep, _Period>& __rel_time) {
 230     return try_lock_until(chrono::steady_clock::now() + __rel_time);
 231   }
 232 
 233   template <class _Clock, class _Duration>
 234   _LIBCPP_TRY_ACQUIRE_CAPABILITY(true) _LIBCPP_HIDE_FROM_ABI bool
 235   try_lock_until(const chrono::time_point<_Clock, _Duration>& __abs_time) {
```

## include/shared_mutex:239

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 235   try_lock_until(const chrono::time_point<_Clock, _Duration>& __abs_time) {
 236     unique_lock<mutex> __lk(__base_.__mut_);
 237     if (__base_.__state_ & __base_.__write_entered_) {
 238       while (true) {
 239         cv_status __status = __base_.__gate1_.wait_until(__lk, __abs_time);
 240         if ((__base_.__state_ & __base_.__write_entered_) == 0)
 241           break;
 242         if (__status == cv_status::timeout)
 243           return false;
 244       }
```

## include/shared_mutex:249

分类：有残留且有害。直接或经包装在 gate2 等待读者退出前设置 write_entered；仅正常超时清除，展开无回滚。

```cpp
 245     }
 246     __base_.__state_ |= __base_.__write_entered_;
 247     if (__base_.__state_ & __base_.__n_readers_) {
 248       while (true) {
 249         cv_status __status = __base_.__gate2_.wait_until(__lk, __abs_time);
 250         if ((__base_.__state_ & __base_.__n_readers_) == 0)
 251           break;
 252         if (__status == cv_status::timeout) {
 253           __base_.__state_ &= ~__base_.__write_entered_;
 254           __base_.__gate1_.notify_all();
```

## include/shared_mutex:270

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 266   _LIBCPP_TRY_ACQUIRE_SHARED_CAPABILITY(true) bool try_lock_shared();
 267   template <class _Rep, class _Period>
 268   _LIBCPP_TRY_ACQUIRE_SHARED_CAPABILITY(true) _LIBCPP_HIDE_FROM_ABI bool
 269   try_lock_shared_for(const chrono::duration<_Rep, _Period>& __rel_time) {
 270     return try_lock_shared_until(chrono::steady_clock::now() + __rel_time);
 271   }
 272 
 273   template <class _Clock, class _Duration>
 274   _LIBCPP_TRY_ACQUIRE_SHARED_CAPABILITY(true) _LIBCPP_HIDE_FROM_ABI bool
 275   try_lock_shared_until(const chrono::time_point<_Clock, _Duration>& __abs_time) {
```

## include/shared_mutex:280

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 276     unique_lock<mutex> __lk(__base_.__mut_);
 277     if ((__base_.__state_ & __base_.__write_entered_) ||
 278         (__base_.__state_ & __base_.__n_readers_) == __base_.__n_readers_) {
 279       while (true) {
 280         cv_status __status = __base_.__gate1_.wait_until(__lk, __abs_time);
 281         if ((__base_.__state_ & __base_.__write_entered_) == 0 &&
 282             (__base_.__state_ & __base_.__n_readers_) < __base_.__n_readers_)
 283           break;
 284         if (__status == cv_status::timeout)
 285           return false;
```

## include/shared_mutex:324

分类：无法判定（模板实参）。守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。

```cpp
 320   _LIBCPP_HIDE_FROM_ABI shared_lock(mutex_type& __m, adopt_lock_t) : __m_(std::addressof(__m)), __owns_(true) {}
 321 
 322   template <class _Clock, class _Duration>
 323   _LIBCPP_HIDE_FROM_ABI shared_lock(mutex_type& __m, const chrono::time_point<_Clock, _Duration>& __abs_time)
 324       : __m_(std::addressof(__m)), __owns_(__m.try_lock_shared_until(__abs_time)) {}
 325 
 326   template <class _Rep, class _Period>
 327   _LIBCPP_HIDE_FROM_ABI shared_lock(mutex_type& __m, const chrono::duration<_Rep, _Period>& __rel_time)
 328       : __m_(std::addressof(__m)), __owns_(__m.try_lock_shared_for(__rel_time)) {}
 329 
```

## include/shared_mutex:417

分类：无法判定（模板实参）。守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。

```cpp
 413   if (__m_ == nullptr)
 414     std::__throw_system_error(EPERM, "shared_lock::try_lock_until: references null mutex");
 415   if (__owns_)
 416     std::__throw_system_error(EDEADLK, "shared_lock::try_lock_until: already locked");
 417   __owns_ = __m_->try_lock_shared_until(__t);
 418   return __owns_;
 419 }
 420 
 421 template <class _Mutex>
 422 void shared_lock<_Mutex>::unlock() {
```

## src/call_once.cpp:47

分类：范围疑问。call_once 直接调用底层 helper，未经过四种入口；静态见等待侧原生 mutex 缺少退出守卫，待人工确认是否扩围。

```cpp
  43 #else // !_LIBCPP_HAS_THREADS
  44 
  45   __libcpp_mutex_lock(&mut);
  46   while (flag == once_flag::_Pending)
  47     __libcpp_condvar_wait(&cv, &mut);
  48   if (flag == once_flag::_Unset) {
  49     auto guard = std::__make_exception_guard([&flag] {
  50       __libcpp_mutex_lock(&mut);
  51       __libcpp_relaxed_store(&flag, once_flag::_Unset);
  52       __libcpp_mutex_unlock(&mut);
```

## src/condition_variable.cpp:36

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
  32 
  33 void condition_variable::wait(unique_lock<mutex>& lk) noexcept {
  34   if (!lk.owns_lock())
  35     std::__throw_system_error(EPERM, "condition_variable::wait: mutex not locked");
  36   int ec = __libcpp_condvar_wait(&__cv_, lk.mutex()->native_handle());
  37   if (ec)
  38     std::__throw_system_error(ec, "condition_variable wait failed");
  39 }
  40 
  41 void condition_variable::__do_timed_wait(unique_lock<mutex>& lk,
```

## src/condition_variable.cpp:60

分类：无法判定（调用方状态）。条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。

```cpp
  56   } else {
  57     ts.tv_sec  = ts_sec_max;
  58     ts.tv_nsec = giga::num - 1;
  59   }
  60   int ec = __libcpp_condvar_timedwait(&__cv_, lk.mutex()->native_handle(), &ts);
  61   if (ec != 0 && ec != ETIMEDOUT)
  62     std::__throw_system_error(ec, "condition_variable timed_wait failed");
  63 }
  64 
  65 void notify_all_at_thread_exit(condition_variable& cond, unique_lock<mutex> lk) {
```

## src/future.cpp:103

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
  99 }
 100 
 101 void __assoc_sub_state::copy() {
 102   unique_lock<mutex> __lk(__mut_);
 103   __sub_wait(__lk);
 104   if (__exception_ != nullptr)
 105     rethrow_exception(__exception_);
 106 }
 107 
 108 void __assoc_sub_state::wait() {
```

## src/future.cpp:110

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 106 }
 107 
 108 void __assoc_sub_state::wait() {
 109   unique_lock<mutex> __lk(__mut_);
 110   __sub_wait(__lk);
 111 }
 112 
 113 void __assoc_sub_state::__sub_wait(unique_lock<mutex>& __lk) {
 114   if (!__is_ready()) {
 115     if (__state_ & static_cast<unsigned>(deferred)) {
```

## src/future.cpp:121

分类：安全（限定内部等待分支）。等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。

```cpp
 117       __lk.unlock();
 118       __execute();
 119     } else
 120       while (!__is_ready())
 121         __cv_.wait(__lk);
 122   }
 123 }
 124 
 125 void __assoc_sub_state::__execute() { std::__throw_future_error(future_errc::no_state); }
 126 
```

## src/future.cpp:138

分类：有残留但无害（限定普通共享状态）。get 先置空 future 指针，unique_ptr/release_shared_count 负责释放原引用；消费 future 的状态保留但无孤立锁。async 最后引用另列 N。

```cpp
 134 void future<void>::get() {
 135   unique_ptr<__shared_count, __release_shared_count> __(__state_);
 136   __assoc_sub_state* __s = __state_;
 137   __state_               = nullptr;
 138   __s->copy();
 139 }
 140 
 141 promise<void>::promise() : __state_(new __assoc_sub_state) {}
 142 
 143 promise<void>::~promise() {
```

## src/mutex.cpp:82

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
  78 
  79 void timed_mutex::lock() {
  80   unique_lock<mutex> lk(__m_);
  81   while (__locked_)
  82     __cv_.wait(lk);
  83   __locked_ = true;
  84 }
  85 
  86 bool timed_mutex::try_lock() noexcept {
  87   unique_lock<mutex> lk(__m_, try_to_lock);
```

## src/mutex.cpp:117

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
 113     ++__count_;
 114     return;
 115   }
 116   while (__count_ != 0)
 117     __cv_.wait(lk);
 118   __count_ = 1;
 119   __id_    = id;
 120 }
 121 
 122 bool recursive_timed_mutex::try_lock() noexcept {
```

## src/shared_mutex.cpp:25

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
  21 
  22 void __shared_mutex_base::lock() {
  23   unique_lock<mutex> lk(__mut_);
  24   while (__state_ & __write_entered_)
  25     __gate1_.wait(lk);
  26   __state_ |= __write_entered_;
  27   while (__state_ & __n_readers_)
  28     __gate2_.wait(lk);
  29 }
  30 
```

## src/shared_mutex.cpp:28

分类：有残留且有害。直接或经包装在 gate2 等待读者退出前设置 write_entered；仅正常超时清除，展开无回滚。

```cpp
  24   while (__state_ & __write_entered_)
  25     __gate1_.wait(lk);
  26   __state_ |= __write_entered_;
  27   while (__state_ & __n_readers_)
  28     __gate2_.wait(lk);
  29 }
  30 
  31 bool __shared_mutex_base::try_lock() {
  32   unique_lock<mutex> lk(__mut_);
  33   if (__state_ == 0) {
```

## src/shared_mutex.cpp:53

分类：安全。等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。

```cpp
  49 
  50 void __shared_mutex_base::lock_shared() {
  51   unique_lock<mutex> lk(__mut_);
  52   while ((__state_ & __write_entered_) || (__state_ & __n_readers_) == __n_readers_)
  53     __gate1_.wait(lk);
  54   unsigned num_readers = (__state_ & __n_readers_) + 1;
  55   __state_ &= ~__n_readers_;
  56   __state_ |= num_readers;
  57 }
  58 
```

## src/shared_mutex.cpp:92

分类：有残留且有害。直接或经包装在 gate2 等待读者退出前设置 write_entered；仅正常超时清除，展开无回滚。

```cpp
  88 
  89 // Shared Timed Mutex
  90 // These routines are here for ABI stability
  91 shared_timed_mutex::shared_timed_mutex() : __base_() {}
  92 void shared_timed_mutex::lock() { return __base_.lock(); }
  93 bool shared_timed_mutex::try_lock() { return __base_.try_lock(); }
  94 void shared_timed_mutex::unlock() { return __base_.unlock(); }
  95 void shared_timed_mutex::lock_shared() { return __base_.lock_shared(); }
  96 bool shared_timed_mutex::try_lock_shared() { return __base_.try_lock_shared(); }
  97 void shared_timed_mutex::unlock_shared() { return __base_.unlock_shared(); }
```
