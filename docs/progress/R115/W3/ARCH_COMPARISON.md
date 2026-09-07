# 架构逐格对照

每格 5 次；完整值断言见各轮 stdout 与 validate.py。表中 0 不用作无问题结论。

| 二进制/形态 | x86_64 原生退出码 | armv7l 物理板退出码 | 逐项事件一致 |
|---|---|---|---|
| wait_retained / wait | 86 ×5 | 86 ×5 | 是（非仅退出码） |
| wait_retained / system_clock | 86 ×5 | 86 ×5 | 是（非仅退出码） |
| wait_retained / steady_clock | 86 ×5 | 86 ×5 | 是（非仅退出码） |
| wait_retained / custom_clock | 86 ×5 | 86 ×5 | 是（非仅退出码） |
| wait_removed / wait | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| wait_removed / system_clock | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| wait_removed / steady_clock | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| wait_removed / custom_clock | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| old_noisy /  | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| old_quiet /  | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| old_fno /  | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| old_fno_quiet /  | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| facility / timed_mutex | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| facility / shared_mutex | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| facility / shared_timed_mutex | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| facility / future_wait | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| facility / cv_any_wait | 0 ×5 | 0 ×5 | 是（非仅退出码） |
| facility / cv_any_throw | 86 ×5 | 86 ×5 | 是（非仅退出码） |
| old_retained（补充） | 86 ×5 | 86 ×5 | 是（补充脚本断言） |
| new_noisy（补充） | 0 ×5 | 0 ×5 | 是（补充脚本断言） |
| new_quiet（补充） | 0 ×5 | 0 ×5 | 是（补充脚本断言） |
