# 模板适用边界

三个模板只冻结已实测通过的共同骨架：同 spec 双构建、独立第二源码/构建目录、staging 安装、不同 SONAME、私有开发接口，以及消费者由中央配置选择。

必须逐包定制的项目包括：实际库/SONAME/ABI major、原构建选项、子包依赖、内部 DSO 依赖替换、模块发现目录、静态库拆包、pkg-config 模块集合、CMake imported targets、代码生成器和源树文件名冲突。模板不代表其余 73 包已构建验证。

对 76 包现有二进制形态的实测分母为 76 个源码身份：CMake 52、autotools 13、meson 7、other 5（构建系统标签可重叠）；静态库 15、pkg-config 70、CMake config 7。1077 条二进制 RPM 文件清单全部可读，失败 0。详见 `../tables/provider_applicability.tsv` 与 `provider_applicability_summary.tsv`。
