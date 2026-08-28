// codes/llvm/libcxx/src/condition_variable.cpp:33-37

void condition_variable::wait(unique_lock<mutex>& lk) noexcept {
  if (!lk.owns_lock())
    std::__throw_system_error(EPERM, "condition_variable::wait: mutex not locked");
  int ec = __libcpp_condvar_wait(&__cv_, lk.mutex()->native_handle());
  ...
}
