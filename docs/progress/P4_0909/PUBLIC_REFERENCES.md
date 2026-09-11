# 本轮公开依据与访问口径

本任务以仓内原始实验为数值依据。以下公开的一手文档用于核查通用技术解释，不能替代
平台实测。网页访问与仓内 snapshot／curl 的结果分别记录，浏览工具不能读取某页不代表
该页不存在；明确的 404 以 `URL_RESULTS.tsv` 的逐条 HTTP 记录为准。

| 公开一手资料 | 用途／核对到的规则 |
|---|---|
| [Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html) | 普通非模板函数名字通常不编码返回类型；模板函数／函数类型规则不同。ABI 还规定 this 等隐式参数和对象模型，不能把签名中的显式内置参数当作全部边界 |
| [CEF 项目维护者：Shared installation support](https://github.com/chromiumembedded/cef/issues/3836) | C API 隔离运行时与实现，C++ wrapper 是应用侧包装；不能用此资料支持“转换层技术上无法处理所有权” |
| [ICU 官方设计与二进制兼容条件](https://unicode-org.github.io/icu/userguide/icu/design.html#icu-binary-compatibility) | 稳定 C API、特定构建配置等是组合条件；仅 C 签名不是跨任意版本绝对承诺 |

CEF 原 Bitbucket GeneralUsage 入口经浏览工具未能读取，改查同项目维护者的一手 issue；
没有将二手概括冒充官方规范。Chromium 的固定 thread.cc／unwind.gni 等部分地址浏览工具
返回 Internal Error，但本机 curl 的结果独立为 200；失败分仓源码地址本机明确为 404。

五个修正的 libcxx 地址来自原 `R116/code/acquire.py` 中实际抓取命令，而非猜测新仓或改用
main。重新取得 TEXT 并解码后，五份内容均与既有固定版本快照相同；见 raw/020、
CORRECTED_URLS.tsv。13 个补充节链接用当前 GET 核实，不用历史“当时可访问”的记录代替。

HTTP 200 仅说明当前通道可访问，不证明总部网络权限、页面脚本行为或其中所有技术断言。
正文技术断言仍依赖固定源代码／元数据／原始实测，而非单独依赖状态码。
