# 13 项 armv7l 逐项结果

环境：交叉编译 + QEMU 用户态，GNU / libc++ 各五轮。完整 TOKEN 两侧均 `9TypeTokenIxE`；DIRECT 两侧均 `x`。

| 项 | 类型 | GNU 实际类型 | libc++ 实际类型 | GNU/CXX 编码 | size | align | 结果 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| T0001 | `std::streamoff` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0014 | `std::char_traits<char>::off_type` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0019 | `std::char_traits<wchar_t>::off_type` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0024 | `std::char_traits<char16_t>::off_type` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0029 | `std::char_traits<char32_t>::off_type` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0356 | `std::istreambuf_iterator<char>::difference_type` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0365 | `std::chrono::system_clock::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0369 | `std::chrono::steady_clock::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0373 | `std::chrono::high_resolution_clock::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0378 | `std::chrono::nanoseconds::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0380 | `std::chrono::microseconds::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0382 | `std::chrono::milliseconds::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
| T0384 | `std::chrono::seconds::rep` | long long | long long | `x` / `x` | 8 / 8 | 8 / 8 | IDENTICAL |
