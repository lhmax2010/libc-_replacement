// 提供方（libstdc++ 构建）
std::deque<int>* make_data() {
    auto* d = new std::deque<int>{1, 2, 3, 4};
    return d;                        // 报告长度 4
}

// 消费方（libc++ 构建）
auto* d = make_data();
printf("%zu\n", d->size());          // 读到 106884723786536
