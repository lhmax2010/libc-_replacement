# 首次 libclang 配置失败输出恢复记录

首次配置 raw/046 的构建日志被 raw/049 重试时同名日志覆盖。
以下内容来自当时工具已返回的完整 tail 输出，不冒充未改动的原始日志文件。
raw/046 的命令、起止时间、退出码 1 以及 events 中该次失败记录均保留。
监控脚本随后加入重试前另存旧日志逻辑；执行前版本另存 executed-v1。

```text
clang version 22.1.8
Target: x86_64-tizen-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /usr/bin/clang++.cfg
CMake Error: CMake was unable to find a build program corresponding to "Ninja".  CMAKE_MAKE_PROGRAM is not set.  You probably need to select a different build tool.
-- Configuring incomplete, errors occurred!
```

技术性修正：旧 bcc 构建根不带 Ninja；将现有 Tizen 运行时构建根中的 Ninja
复制到本轮 tmp/NIGHT_0917/tools，脚本固定 PATH 加入该目录，不安装到旧根。
