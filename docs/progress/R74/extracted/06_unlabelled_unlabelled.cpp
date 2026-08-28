static void *worker(void *) {          // 同样不是 noexcept
    for (int i = 0; ; ++i) {
        std::cout << i << std::endl;   // 底层的写操作本身就是取消点
    }
    return nullptr;
}

int main() {
    pthread_t t;
    pthread_create(&t, nullptr, worker, nullptr);
    usleep(50 * 1000);
    pthread_cancel(t);
    void *result = nullptr;
    pthread_join(t, &result);
    return result == PTHREAD_CANCELED ? 0 : 1;
}
