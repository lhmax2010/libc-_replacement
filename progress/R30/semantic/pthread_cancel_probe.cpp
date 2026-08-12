#include <pthread.h>
#include <unistd.h>

#include <atomic>
#include <cstdio>
#include <cstdlib>

#include <cxxabi.h>

static std::atomic<int> started{0};
static std::atomic<int> destructor_count{0};
static std::atomic<int> forced_guard_count{0};
static std::atomic<int> catch_all_count{0};
static std::atomic<int> returned_count{0};

struct Cleanup {
    ~Cleanup() {
        ++destructor_count;
        std::puts("EVENT destructor");
    }
};

static void *worker(void *) {
    Cleanup cleanup;
    try {
        started.store(1);
        for (;;) {
            pthread_testcancel();
            usleep(1000);
        }
#if defined(__GLIBCXX__)
    } catch (const __cxxabiv1::__forced_unwind &) {
        ++forced_guard_count;
        std::puts("EVENT forced_guard_rethrow");
        throw;
#endif
    } catch (...) {
#if defined(_LIBCPP_VERSION)
        if (__cxxabiv1::__cxa_current_exception_type() == nullptr) {
            ++forced_guard_count;
            std::puts("EVENT foreign_guard_rethrow");
            throw;
        }
#endif
        ++catch_all_count;
        std::puts("EVENT catch_all");
    }
    ++returned_count;
    std::puts("EVENT worker_returned");
    return nullptr;
}

int main() {
    pthread_t thread{};
    void *result = nullptr;
    const int create_rc = pthread_create(&thread, nullptr, worker, nullptr);
    if (create_rc != 0) {
        std::printf("RESULT create_rc=%d\n", create_rc);
        return 10;
    }
    while (started.load() == 0) {
        usleep(1000);
    }
    const int cancel_rc = pthread_cancel(thread);
    const int join_rc = pthread_join(thread, &result);
    const bool canceled = result == PTHREAD_CANCELED;
    std::printf(
        "RESULT cancel_rc=%d join_rc=%d canceled=%d destructor=%d "
        "forced_guard=%d catch_all=%d returned=%d\n",
        cancel_rc, join_rc, canceled ? 1 : 0, destructor_count.load(),
        forced_guard_count.load(), catch_all_count.load(),
        returned_count.load());

    const bool ok = cancel_rc == 0 && join_rc == 0 && canceled &&
                    destructor_count.load() == 1 &&
                    catch_all_count.load() == 0 && returned_count.load() == 0;
    return ok ? 0 : 20;
}
