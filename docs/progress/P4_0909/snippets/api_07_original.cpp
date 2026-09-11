// 不能跨边界
class EngineError : public std::runtime_error { ... };

void engine_process();               // 内部抛 EngineError

// 应用侧
try {
    engine_process();
} catch (const EngineError& e) {     // 接不住 —— 类型信息对不上
    ...
} catch (...) {                      // 只能落到这里
    ...
}
