extern "C" int native_probe(void*);
// 仅用于验证最终 ELF 的真实链接依赖；本入口不作为有效平台上下文。
int main() { return native_probe(nullptr); }
