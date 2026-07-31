# T7 字节拼接方法

生成阶段只用 `grep -aboF` 定位唯一字节锚点，并用 `dd bs=1` 按
prefix / frozen fragment / suffix 顺序拼接；没有用 `sed`、`awk` 等行式工具
重写正式配置。各锚点字节偏移见 `byte_splice_offsets.tsv`。

buildconf 的 D5 字节来源选择规格允许的第一项：21.1.1 线正式 buildconf，
SHA256 为 `7181723c90079f88de345c261a57c01b480ce72d65cce12672f61859e419fe86`。
从该源以字节偏移抽取完整 compilation/D5 区块与 `libc++-devel` Required
片段，插入后再从正式副本抽取同区块，逐字 `cmp=0`。

验证阶段使用反向字节拼接剥离新增段；两级 `cmp` 均退出 0。diff 只用于
取证，变更行经排序后与 21.1.1 已验证差异的多重集比较，均退出 0。
