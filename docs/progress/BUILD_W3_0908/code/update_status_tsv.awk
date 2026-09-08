BEGIN {
    FS = OFS = "\t"
}

NR == 1 {
    print
    next
}

$1 == "llvm" {
    if (language == "zh")
        $9 = "暂停（TF 2.18 资产接入候选已通过 armv7l/x86_64；aarch64、兼容格和非 x86_64 运行格未闭合，未推送）"
    else
        $9 = "paused (the TensorFlow 2.18 integration candidate passed armv7l/x86_64, but aarch64, compatibility builds, and non-x86_64 runtime checks remain incomplete; not pushed)"
}

$1 == "bcc-tools" {
    if (language == "zh")
        $9 = "暂停（依赖已验证并推送的新 llvm；W1 门禁未闭合，未实施）"
    else
        $9 = "paused (requires a newly validated and pushed llvm; the W1 gate did not close, so no implementation was started)"
}

$1 == "bpftrace" {
    if (language == "zh")
        $9 = "暂停（依赖新的 llvm 与 bcc-tools；前置未满足，未实施）"
    else
        $9 = "paused (requires the new llvm and bcc-tools; prerequisites were not met, so no implementation was started)"
}

{
    print
}
