# 11 个 package sandbox 远端核对

核对使用各仓的 `origin` 与精确 ref
`refs/heads/sandbox/lhmax2025/libcxx-migration`。命令形态如下；各网络调用
设置 20 或 30 秒超时，没有更改任何 ref：

```bash
for p in abseil-cpp bcc-tools boost bpftrace icu jsoncpp 'libsigc++' llvm pcre taglib tensorflow2; do
  d="codes/R104/$p"
  printf 'PACKAGE\t%s\n' "$p"
  git -C "$d" remote get-url origin
  /usr/bin/timeout 30s git -C "$d" ls-remote origin refs/heads/sandbox/lhmax2025/libcxx-migration
done
```

网络核对分为两次执行；第一次在输出 llvm 仓 URL 后到达该工具调用的 30 秒
返回窗口，第二次从 llvm 继续。该现象不是 Git 判据失败。合并后的实际结果：

```text
abseil-cpp  7a4f04dad6d30ba09c406837cdd23ad779a69210
bcc-tools   dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2
boost       4168e873584ae50a12733262026c007dce18ec89
bpftrace    30e51cd665360f85b665308dc13ba27df0c5f739
icu         dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330
jsoncpp     9cbab147f44097d5b8da2b3ef38f75376a282d01
libsigc++   4f2f592d2f88eb6a3a949cec8970a4cd5d40165b
llvm        2d23367d74afbf2bb1e9e4013fce072b3a154109
pcre        09a5bfc9a96c4235628ce800db3b95bf28ef2f23
taglib      8b552bdf27c913a93bca664c41c3fafb214962a1
tensorflow2 6d76139029ca4162249667f0db65a73387a94cfe
```

八个完成包与既有适配 SHA 一致；另外三个仍是实施前基线。单独核对
`libcxx-runtimes` 分支：

```bash
/usr/bin/timeout 30s git -C codes/R104/llvm ls-remote origin refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

输出：

```text
c3f8578a4db871d9d6de96d751f4c2ea7b1638fa refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
ls_remote_exit=0
```
