#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <cstdio>
#include <string>

static std::atomic<int> cancel_ready{0};

template <class T> struct CancelAllocator {
    using value_type = T;
    CancelAllocator() = default;
    template <class U> CancelAllocator(const CancelAllocator<U>&) {}

    T *allocate(std::size_t n) {
        cancel_ready.store(1);
        for (;;) pthread_testcancel();   // 取消在这里被送达——位于 shrink_to_fit 内部
        return nullptr;
    }
    void deallocate(T *p, std::size_t) noexcept { ::operator delete(p); }
};

using CancelString =
    std::basic_string<char, std::char_traits<char>, CancelAllocator<char>>;

static void *worker(void *) {            // 不加 noexcept
    CancelString s(1024, 'x');
    s.reserve(65536);
    s.shrink_to_fit();                   // 触发重新分配 → 进入 allocate()
    return nullptr;
}

int main() {
    pthread_t thread{};
    void *result = nullptr;
    pthread_create(&thread, nullptr, worker, nullptr);
    while (cancel_ready.load() == 0) usleep(1000);
    pthread_cancel(thread);
    pthread_join(thread, &result);
    std::printf("RESULT joined=%s\n",
                result == PTHREAD_CANCELED ? "PTHREAD_CANCELED" : "other");
    return result == PTHREAD_CANCELED ? 0 : 1;
}
