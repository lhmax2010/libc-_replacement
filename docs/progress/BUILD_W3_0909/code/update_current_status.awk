BEGIN {
    FS = OFS = "\t"
}

NR == 1 {
    print
    next
}

$1 == "llvm" {
    if (language == "zh")
        $9 = "暂停（三架构 libc++ 构建与 MLGO 执行已通过；armv7l GCC 通过但非 libc++ 整包等价仅部分，aarch64/x86_64 兼容格未完成，未推送）"
    else
        $9 = "paused (all three libc++ builds and MLGO execution checks passed; armv7l GCC passed but complete non-libc++ product equivalence remains partial, and the aarch64/x86_64 compatibility cells are incomplete; not pushed)"
}

{
    print
}
