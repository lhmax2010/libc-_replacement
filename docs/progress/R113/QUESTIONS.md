# R113 尚存疑问

1. armv7l 物理板 `192.168.108.26:26101` 当前不可连接；缺少恢复后的板卡访问，
   因而 R111/R112 两个取消矩阵及 `.cantunwind` 核查均为 `NOT_AVAILABLE`。
2. 目标 armv7l 的 libstdc++ 构建究竟选择 pthread rwlock 后端还是条件变量 fallback，
   本轮没有取得对应预处理宏或目标头构建配置。
3. libstdc++ `async` 最后共享状态析构中的 non-throwing join 只经源码确认；取消穿过
   该帧时的实际诊断和清理轨迹为 `NOT_OBSERVED`。
4. 本轮双版本实测限于 x86_64 GNU ELF 工具链；armv7l、aarch64 以及发布构建系统
   对相同版本脚本和绑定规则的结果为 `NOT_OBSERVED`。

以上留给人工审阅，不在本轮推断补全。
