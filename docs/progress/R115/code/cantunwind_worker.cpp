extern "C" void pthread_testcancel();
extern "C" int worker_entered;
extern "C" void* cantunwind_worker(void*) {
  __atomic_store_n(&worker_entered,1,__ATOMIC_RELEASE);
  for (;;) pthread_testcancel();
}
