# libc++ 迁移补丁候选目录

每个源码包一个子目录；`patch/` 只接收 `git format-patch` 形态文件，
状态由 `INDEX.tsv` 维护。R30 已为首批两项归档候选 patch，但
pthread_cancel 语义门禁失败，均保持“待开发”；归档仅供复现失败，
不得视为待评审或可提交补丁。
