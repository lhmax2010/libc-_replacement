# Base-layer libc++ adaptation: implementation status update

This file updates implementation status only. It does not change any package
decision, rationale, or evidence in the published inventory.

| Status | Count | Packages |
| --- | ---: | --- |
| Completed and pushed | 8 | `abseil-cpp`, `boost`, `icu`, `jsoncpp`, `libsigc++`, `pcre`, `taglib`, `tensorflow2` |
| Paused; validation gate incomplete | 3 | `llvm`, `bcc-tools`, `bpftrace` |

Seven completed packages passed all six build-and-product cells (three
architectures × libc++/GCC). For `tensorflow2`, all three libc++ cells and the
aarch64/x86_64 GCC cells completed. The armv7l GCC candidate and the unmodified
baseline failed at the same XNNPACK step for the same 48 assembler errors,
before reaching the modified `elementwise.cc`; that cell therefore passed the
predeclared no-regression-relative-to-baseline criterion. Compilation of the
modified source on the armv7l GCC path remains `NOT_OBSERVED`.

TensorFlow 2.18/libc++ AOT assets have now passed complete LLVM builds and
product checks on all three architectures. AOT-model execution passed natively
on x86_64, on a physical armv7l board, and under aarch64 QEMU user mode. The
compatibility gate remains incomplete: the full armv7l GCC build passed, but a
cross-date RPM comparison cannot establish complete non-libc++ product
equivalence; the aarch64 and x86_64 GCC/non-libc++ builds were stopped at the
eight-hour limit before producing packages. The LLVM candidate was therefore
neither committed nor pushed. Because `bcc-tools` and `bpftrace` require a
newly validated and pushed LLVM package, neither dependent package was
implemented in this run.
