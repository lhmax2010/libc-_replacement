# 历史“50 个处理位置”的逐行核对

“历史 50 行”按当时表中 `CONFIRMED_RETHROW` 且非文档路径筛出。下表逐行保留源码 RPM、包内路径、行号和代码原文；核对结果明确标出该行是否真的是强制展开 catch 子句。这里的“位置”以源码 RPM 实例、文件和行号共同标识。

## `boost`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine/detail/pull_coroutine_object.hpp:120` | `catch ( abi::__forced_unwind const&)` | boost::coroutines::detail::pull_coroutine_object::run() (line 105) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine/detail/pull_coroutine_object.hpp:207` | `catch ( abi::__forced_unwind const&)` | boost::coroutines::detail::pull_coroutine_object::run() (line 192) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine/detail/pull_coroutine_object.hpp:294` | `catch ( abi::__forced_unwind const&)` | boost::coroutines::detail::pull_coroutine_object::run() (line 279) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine/detail/push_coroutine_object.hpp:132` | `catch ( abi::__forced_unwind const&)` | boost::coroutines::detail::push_coroutine_object::run(R * result) (line 117) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine/detail/push_coroutine_object.hpp:219` | `catch ( abi::__forced_unwind const&)` | boost::coroutines::detail::push_coroutine_object::run(R * result) (line 204) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine/detail/push_coroutine_object.hpp:306` | `catch ( abi::__forced_unwind const&)` | boost::coroutines::detail::push_coroutine_object::run() (line 291) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/pull_control_block_cc.ipp:63` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 50) in boost::coroutines2::detail::pull_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/pull_control_block_cc.ipp:92` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 50) in boost::coroutines2::detail::pull_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/pull_control_block_cc.ipp:219` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 206) in boost::coroutines2::detail::pull_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/pull_control_block_cc.ipp:248` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 206) in boost::coroutines2::detail::pull_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/pull_control_block_cc.ipp:351` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 338) in boost::coroutines2::detail::pull_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/pull_control_block_cc.ipp:380` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 338) in boost::coroutines2::detail::pull_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/push_control_block_cc.ipp:63` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 49) in boost::coroutines2::detail::push_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/push_control_block_cc.ipp:94` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 49) in boost::coroutines2::detail::push_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/push_control_block_cc.ipp:194` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 180) in boost::coroutines2::detail::push_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/push_control_block_cc.ipp:225` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 180) in boost::coroutines2::detail::push_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/push_control_block_cc.ipp:312` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 298) in boost::coroutines2::detail::push_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/coroutine2/detail/push_control_block_cc.ipp:343` | `} catch ( abi::__forced_unwind const&) {` | lambda (line 298) in boost::coroutines2::detail::push_coroutine::control_block::control_block(context::preallocated palloc,StackAllocator && salloc,Fn && fn) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/fiber/condition_variable.hpp:78` | `} catch ( abi::__forced_unwind const&) {` | boost::fibers::condition_variable_any::wait(LockType & lt) (line 66) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/fiber/condition_variable.hpp:110` | `} catch ( abi::__forced_unwind const&) {` | boost::fibers::condition_variable_any::wait_until(LockType & lt,std::chrono::time_point<Clock,Duration> const & timeout_time_) (line 94) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/fiber/future/detail/task_object.hpp:68` | `} catch ( abi::__forced_unwind const&) {` | boost::fibers::detail::task_object::run(Args &&...args) (line 56) | `ACTUAL_CATCH_CLAUSE` |
| `boost-1.83.0-2.2.src.rpm` | `boost/fiber/future/detail/task_object.hpp:142` | `} catch ( abi::__forced_unwind const&) {` | boost::fibers::detail::task_object::run(Args &&...args) (line 131) | `ACTUAL_CATCH_CLAUSE` |

## `gcc`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `gcc-14.2.0-1.12.src.rpm` | `gcc/testsuite/g++.dg/abi/forced.C:14` | `catch (abi::__forced_unwind &)` | main() (line 8) | `ACTUAL_CATCH_CLAUSE` |
| `gcc-14.2.0-1.12.src.rpm` | `libstdc++-v3/include/bits/basic_string.tcc:390` | `\tcatch (const __cxxabiv1::__forced_unwind&)` | std::basic_string::reserve() (line 365) | `ACTUAL_CATCH_CLAUSE` |
| `gcc-14.2.0-1.12.src.rpm` | `libstdc++-v3/include/bits/cow_string.h:3733` | `\tcatch (const __cxxabiv1::__forced_unwind&)` | std::basic_string::reserve() (line 3722) | `ACTUAL_CATCH_CLAUSE` |
| `gcc-14.2.0-1.14.src.rpm` | `gcc/testsuite/g++.dg/abi/forced.C:14` | `catch (abi::__forced_unwind &)` | main() (line 8) | `ACTUAL_CATCH_CLAUSE` |
| `gcc-14.2.0-1.14.src.rpm` | `libstdc++-v3/include/bits/basic_string.tcc:390` | `\tcatch (const __cxxabiv1::__forced_unwind&)` | std::basic_string::reserve() (line 365) | `ACTUAL_CATCH_CLAUSE` |
| `gcc-14.2.0-1.14.src.rpm` | `libstdc++-v3/include/bits/cow_string.h:3733` | `\tcatch (const __cxxabiv1::__forced_unwind&)` | std::basic_string::reserve() (line 3722) | `ACTUAL_CATCH_CLAUSE` |

## `key-manager`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client-async/client-manager-async.cpp:321` | `\t} catch (const abi::__forced_unwind &) {` | CKM::ManagerAsync::tryCatchHelper(Observer & observer,const std::function<void ()> & func) (line 307) | `ACTUAL_CATCH_CLAUSE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client-async/client-manager-async.cpp:322` | `\t\tLogDebug("abi::__forced_unwind caught. Thread cancelation.");` | CKM::ManagerAsync::tryCatchHelper(Observer & observer,const std::function<void ()> & func) (line 307) | `NON_CLAUSE_TOKEN_LINE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client-capi/ckmc-manager.cpp:1370` | `\t} catch (const abi::__forced_unwind &) {` | ckmc_cipher_free(ckmc_cipher_ctx_h context) (line 1358) | `ACTUAL_CATCH_CLAUSE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client-capi/ckmc-manager.cpp:1371` | `\t\tLogDebug("abi::__forced_unwind caught. Thread cancelation.");` | ckmc_cipher_free(ckmc_cipher_ctx_h context) (line 1358) | `NON_CLAUSE_TOKEN_LINE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client/client-common.cpp:327` | `\t} catch (const abi::__forced_unwind &) {` | CKM::try_catch(const std::function<int ()> & func) (line 308) | `ACTUAL_CATCH_CLAUSE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client/client-common.cpp:328` | `\t\tLogDebug("abi::__forced_unwind caught. Thread cancelation.");` | CKM::try_catch(const std::function<int ()> & func) (line 308) | `NON_CLAUSE_TOKEN_LINE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client/client-common.cpp:349` | `\t} catch (const abi::__forced_unwind &) {` | CKM::try_catch_enclosure(const std::function<int ()> & func) (line 337) | `ACTUAL_CATCH_CLAUSE` |
| `key-manager-0.1.90-1.src.rpm` | `src/manager/client/client-common.cpp:350` | `\t\tLogDebug("abi::__forced_unwind caught. Thread cancelation.");` | CKM::try_catch_enclosure(const std::function<int ()> & func) (line 337) | `NON_CLAUSE_TOKEN_LINE` |
| `key-manager-0.1.90-1.src.rpm` | `unit-tests/test_client-common.cpp:54` | `} catch (const abi::__forced_unwind& ) {` | lambda (line 51) in __anone27e8c080111::test_forced_unwind_throw(ConvertingFunction func) | `ACTUAL_CATCH_CLAUSE` |

## `libcynara-commons`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `libcynara-commons-0.26.0-1.src.rpm` | `src/common/exceptions/TryCatch.h:65` | `} catch (const abi::__forced_unwind &) {` | Cynara::tryCatch(F && f) (line 47) | `ACTUAL_CATCH_CLAUSE` |

## `llvm`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `llvm-22.1.8-18.1.src.rpm` | `packaging/mlgo_aarch_model.tar.gz!/mlgo_sysroot/include/external/pybind11/_virtual_includes/pybind11/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `llvm-22.1.8-18.1.src.rpm` | `packaging/mlgo_aarch_model.tar.gz!/mlgo_sysroot/include/external/pybind11/include/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `llvm-22.1.8-18.1.src.rpm` | `packaging/mlgo_arm_model.tar.gz!/mlgo_sysroot/include/external/pybind11/_virtual_includes/pybind11/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `llvm-22.1.8-18.1.src.rpm` | `packaging/mlgo_arm_model.tar.gz!/mlgo_sysroot/include/external/pybind11/include/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `llvm-22.1.8-18.1.src.rpm` | `packaging/mlgo_x86_model.tar.gz!/mlgo_sysroot/include/external/pybind11/_virtual_includes/pybind11/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `llvm-22.1.8-18.1.src.rpm` | `packaging/mlgo_x86_model.tar.gz!/mlgo_sysroot/include/external/pybind11/include/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |

## `security-manager`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `security-manager-2.4.0-0.src.rpm` | `src/common/include/utils.h:70` | `} catch (abi::__forced_unwind &) {` | SecurityManager::try_catch(F && f) (line 67) | `ACTUAL_CATCH_CLAUSE` |

## `tensorflow2`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `tensorflow2-2.18.0-1.2.src.rpm` | `packaging/tensorflow2-aot.tar.gz!/include/external/pybind11/_virtual_includes/pybind11/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `tensorflow2-2.18.0-1.2.src.rpm` | `packaging/tensorflow2-aot.tar.gz!/include/external/pybind11/include/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `tensorflow2-2.18.0-1.3.src.rpm` | `packaging/tensorflow2-aot.tar.gz!/include/external/pybind11/_virtual_includes/pybind11/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |
| `tensorflow2-2.18.0-1.3.src.rpm` | `packaging/tensorflow2-aot.tar.gz!/include/external/pybind11/include/pybind11/pybind11.h:989` | `} catch (abi::__forced_unwind &) {` | cpp_function::dispatcher(PyObject * self,PyObject * args_in,PyObject * kwargs_in) (line 669) | `ACTUAL_CATCH_CLAUSE` |

## `webauthn`

| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |
|---|---|---|---|---|
| `webauthn-1.0.13-1.src.rpm` | `srcs/common/utils.h:42` | `} catch (abi::__forced_unwind &) {` | WA::try_catch(F && f) (line 39) | `ACTUAL_CATCH_CLAUSE` |
