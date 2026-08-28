// R74 supplementation: the report block omitted headers, state, and some_work.
#include <atomic>
#include <future>
#include <pthread.h>
#include <unistd.h>

static std::atomic<int> waiter_started{0};

static int some_work() {
    usleep(500 * 1000);
    return 42;
}

std::shared_future<int> *waiting_future;

static void *waiting_side(void *) {    // 不是 noexcept
    waiter_started.store(1);
    waiting_future->wait();            // 阻塞在这里
    return (void *)1;
}

int main() {
    auto f = std::async(std::launch::async, some_work).share();
    waiting_future = &f;
    // 等 some_work 真正开始
    pthread_t waiter;
    pthread_create(&waiter, nullptr, waiting_side, nullptr);
    while (!waiter_started.load()) usleep(1000);
    pthread_cancel(waiter);            // 终止等待方
    void *joined = nullptr;
    pthread_join(waiter, &joined);
    // 期望：等待方干净退出，主线程仍能取到结果
    return (joined == PTHREAD_CANCELED && f.get() == 42) ? 0 : 1;
}
