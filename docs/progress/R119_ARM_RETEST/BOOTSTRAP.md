# 开工与范围

仅一条执行通道，不启动子代理，不并行运行命令；nice19、ionice3、RLIMIT_AS 30% 沿用已有 resume_limited.py。优先交叉编译/QEMU，不接触板子。

建立 recorder 前的定位命令补录：

```text
pwd
退出码 0
/home/toolchain/development/libcxx_runtime_validation

rg --files -g AGENTS.md -g '!codes/**' -g '!tmp/**' -g '!config/**' -g '!docs/**' .
退出码 1（未找到）
```

本次只写 docs/progress/R119_ARM_RETEST/ 与 tmp/R119_ARM_RETEST/；不覆盖旧产物。后续命令原文、stdout、stderr、退出码和时间在 raw/。
