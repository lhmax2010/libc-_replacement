#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <cstdio>

#if defined(__GLIBCXX__)
#include <cxxabi.h>          // libstdc++ 才有识别类型
#endif

static std::atomic<int> started{0};

struct Cleanup {
    ~Cleanup() { std::puts("EVENT destructor"); }
};

static void *worker(void *) {      // 注意：不加 noexcept
    Cleanup cleanup;               // 用于确认清理代码确实执行了
    try {
        started.store(1);
        for (;;) {
            pthread_testcancel();  // 明确的取消点
            usleep(1000);
        }
#if defined(__GLIBCXX__)
    } catch (const __cxxabiv1::__forced_unwind &) {
        std::puts("EVENT forced_guard_rethrow");
        throw;                     // 识别出来，原样放行
#endif
    } catch (...) {
        std::puts("EVENT catch_all");   // 吞掉——问题就出在这里
    }
    std::puts("EVENT worker_returned");
    return nullptr;
}

int main() {
    pthread_t thread{};
    void *result = nullptr;
    pthread_create(&thread, nullptr, worker, nullptr);   // 用裸 pthread，不用 std::thread
    while (started.load() == 0) usleep(1000);
    pthread_cancel(thread);
    pthread_join(thread, &result);
    std::printf("RESULT joined=%s\n",
                result == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "other");
    return result == PTHREAD_CANCELED ? 0 : 1;
}
