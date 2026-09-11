// 不安全 —— 参数和返回值都是 int，但异常会穿过去
int engine_compute(engine_handle h) {
    auto result = internal_cpp_function();   // 这里可能抛
    return result;
}

// 安全 —— 挡住异常，转成错误码
int engine_compute(engine_handle h, int* out) {
    try {
        *out = internal_cpp_function();
        return 0;
    } catch (const std::exception&) {
        return -1;
    } catch (...) {
        return -2;
    }
}
