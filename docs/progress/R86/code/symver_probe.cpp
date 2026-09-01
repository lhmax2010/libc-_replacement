extern "C" int old_impl() noexcept { return 22; }
extern "C" int new_impl() { return 23; }

asm(".symver old_impl,api@LLVM_22");
asm(".symver new_impl,api@@LLVM_23");
