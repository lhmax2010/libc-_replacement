# 标准原文来源记录

- 文档：WG21 N4861，2020-04-01，Working Draft, Standard for Programming
  Language C++
- URL：<https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4861.pdf>
- 位置：D.21.2 `[depr.conversions.buffer]`，PDF 第 1688 页，第 11 段
- 核查结果：`~wbuffer_convert()` 的 Effects 段仅列出析构函数删除 `cvtptr`；
  该段没有列出析构同步或刷新底层 streambuf。

本文件只记录来源和核查结果，不作方案判断。
