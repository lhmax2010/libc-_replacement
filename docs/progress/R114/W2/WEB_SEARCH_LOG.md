# 网页检索说明

日期：2026-09-07。以下为 web 查询原文，返回数据保存在工具会话；该工具不提供 shell 退出码，故退出码栏为“不适用”，未虚构 exit=0。
能够通过本机取得的源文件/提交 JSON 则另以 record.py 记录 curl 原文、stdout、stderr、实际退出码及时间。

执行的主题查询包括：

```text
site.gcc.gnu.org "shared_mutex" "pthread_rwlock" "2015"
site.libcxx.llvm.org shared_mutex pthread_rwlock
site.gcc.gnu.org bugzilla shared_mutex cancellation
site.gcc.gnu.org/legacy-ml/libstdc++ "pthread_rwlock" "2015"
site.gcc.gnu.org/pipermail/libstdc++ "shared_mutex" "performance" "2015"
site.reviews.llvm.org "shared_mutex" "pthread_rwlock"
"shared_mutex" "Wakely" "2015" "pthread"
"shared_mutex" "pthread_rwlock" "libc++" "Howard"
"shared_mutex" "cancellation" site:gcc.gnu.org
"libstdc++: Add POSIX variant of shared_timed_mutex"
"libc++" "shared_mutex" "pthread_rwlock_t" "performance"
site:github.com/llvm/llvm-project/issues shared_mutex pthread performance
site:lists.llvm.org shared_mutex pthread rwlock
site:gcc.gnu.org "shared_mutex" "cancel" "Bug"
"libc++" "shared_mutex" "fairness"
"shared_mutex" "cancellation" gcc bugzilla
"shared_mutex" "pthread_rwlock" site:reviews.llvm.org
site:gcc.gnu.org "shared_mutex" "cancelled"
site:gcc.gnu.org "shared_mutex" "canceled"
site:lists.llvm.org "shared_mutex" "pthread"
```

精确动机定位后打开原邮件、GCC 提交、N2406、LLVM PR70151；错误处理讨论打开 2025-11/064216。
N2406 内查 `fair` 与 `pthread_rwlock`；后者无该字面串，但正文使用 OS supplied read-write mutex，
所以没有把字符串零命中写成“文档不讨论 OS 读写锁”。
Bugzilla PR116586 打开为 Anubis Access Denied；curl 查询入口 HTTP403。
Open Group 两个 rwlock 文档打开失败；表中的属性事实转用 glibc 固定标签 pthread.h 与平台 GCC 头。

检索正向对照为成功取得 2015 引入邮件、真实 GCC 错误处理讨论、LLVM 相关 PR。
没有把未定位精确取消缺陷报告解读为该报告不存在。
