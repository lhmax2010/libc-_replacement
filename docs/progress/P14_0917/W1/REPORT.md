# 双异常的 GNU-only 对照

状态：**对照完成，一致终止；未发现 adaptor 特有的双异常终止。** 架构为 x86_64 原生；armv7l 为 `NOT_OBSERVED_BOARD_OFFLINE`。

直接调用与上一轮相同的 `p13_test::point`，头文件逐字复制，源/副本 SHA256 见 FIXTURE_IDENTITY.json。程序只链接 GNU 标准库，不经过 adaptor，不调用 EWK。各进程归档 `/proc/self/maps` 并断言 GNU 库存在、libc++ 不存在；另有 5 次显式加载 libc++ 的正向检测对照，排除“检测器总报不存在”。

| 同一夹具情形 | GNU-only（各 5 次） | 上一轮 adaptor 两放法（各 5 次） |
| --- | --- | --- |
| 析构内抛出第二异常但自己捕获 | 存活；外层接到 bad_alloc；nested=1、作用域析构完成 | 返回映射错误并存活 |
| bad_alloc 展开中第二异常逸出析构 | 5/5 SIGABRT，原始输出保留 | 两放法各 5/5 SIGABRT |
| 正常退出时析构抛出 | 存活；外层精确接到 Custom{314}；作用域析构完成 | false/EIO，存活 |
| 捕获第一个后改抛另一异常 | 存活；外层接到 invalid_argument；作用域析构完成 | false/EINVAL，存活 |

**关键限定：夹具的析构显式声明 `noexcept(false)`，不是隐式 noexcept。** 因此第三行 GNU-only 不终止。没有把它改成隐式 noexcept 来制造不同结果，也没有把“GNU 外层 catch 接到异常”写成“GNU 原接口返回 false/EIO”。本格未证明 adaptor 比 GNU 更宽容，只证明对同步异常采用不同表达方式。

C++ 草案 [except.terminate](https://eel.is/c++draft/except.terminate) 明确将“展开时对象析构以抛异常结束”列为 terminate 条件。结合上述实测，可表述为：**此项 GNU-only 与双运行时 adaptor 路径行为相同，是 C++ 语言规则，非本方案引入。** 这不是对任意库配置、任意析构或其他架构的全覆盖结论。

原始输出、信号（记录为 -6）、具体捕获类型和计数见 raw/case_*.{stdout,stderr,exitcode}；结构化索引见 RESULTS.json。没有运行取消测试。
