# T1-0008 处置重写单点确认(评审方:Claude,2026-07-27)

输入:`p1_exit_review_v3.tar.gz`,SHA256 实测
`4cc5e814c94c34943403b3c3eff891616ca7837862ef8965ff53537f18450f55`(吻合);
MANIFEST 全量校验非 OK 行 = 0。

## 裁定:**CONFIRM**

**先如实记录:GPT 的异议成立,且指向本方上轮 CLOSED 判定中的一处失察。**
本方核可原两分支时,把 S4(UidSandboxing 对账)的消解力误延伸到了整个
分量;`cross_repo_edges_7_51.tsv` 实核的两条边——
`capi-appfw-capmgr→boost-program-options`(`options_description` 的
`operator<<(std::ostream&,…)`)与 `security-manager→boost-program-options`
(`abstract_variables_map::operator[](std::string const&)`)——均为
D2_RETAINED 的 CPP_ABI/INTERFACE 边,三架构齐在,签名直接携带
libstdc++ 字符串类型,与 UidSandboxing 无关。原"S4 摘除 →
boost-program-options 独立 Base 先行"分支执行 = provider 迁 libc++ 而
两个 Unified 消费者留 libstdc++,正是执行单元闭包规则要禁止的拆分。

## 重写后两分支与数据自洽性

- **S4 PASS 分支**:整分量(4 包)退出波 1,转独立跨 repo 原子批次,
  Base+Unified 成对晋级(机制同波 4)。`t1_0008_branch_closure.tsv`
  逐边核验:两条 7/51 边的 src/dst 均落
  `T1_0008_CROSS_REPO_ATOMIC_4`,same_batch=YES,外部可见性规则为成对
  promotion——闭包成立;
- **S4 不满足分支**:整分量并入波 1 的 23/26 路径,单一 allowlist/镜像
  事务,两条边 same_batch=YES——闭包成立;
- decision_record 增加的硬约束("不得提前按任一分支记 PASS,不得在
  任一分支拆分四成员")把两分支之外的路径全部封死。

## 四处同源性核验

| 文档 | 状态 |
|---|---|
| decision_record(D-G3 R2) | 重写条款 + "S4 只定排期分支、不构成拆分依据"逐字在案 ✓ |
| 台账 | `shared_main_tier1.tsv` T1-0008 四包与 PENDING 标签未动(与"S4 前不收敛"一致);`base_first_6.tsv` 改 5 包(basis=D-G3 R2)✓ |
| 排期 | Base 先行行改"5 包 + boost-log 伴随";新增 T1-0008 跨 repo 原子批次独立行,两分支去向写明 ✓ |
| 波 1 条件分支 | "原 boost 三包条件分支相应并入"显式衔接;S4 不满足时 boost-program-options 以伴随成员进入 23/26,与 wave1 卷宗条件逻辑相容 ✓ |

## 新矛盾检查:未发现

唯一的条件性联动已被显式承接而非遗漏:若 S4 不满足且 boost 三包条件
分支触发,boost-filesystem/log/thread 随波 1 迁移,9 月 Base 先行波的
实际执行面将相应缩减——该情形由既有"波 1 裁决结果必须回写台账与
D-G3"义务覆盖,属分支解析后的机械回写,不构成文本矛盾。注意执行时
勿漏:S4 分支落定后,排期 9 月 Base 行与 `base_first_6.tsv` 需按回写
义务同步一次。

**结论:CONFIRM——重写后的 T1-0008 处置与执行单元闭包规则、7/51 边
数据完全自洽,四处同源,无新矛盾;上轮 P1_EXIT_PASS 总裁定在此修正
之上继续成立。**
