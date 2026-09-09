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

TensorFlow 2.18/libc++ AOT assets have now passed static ABI and three-
architecture CMake integration checks. Complete armv7l and x86_64 LLVM builds
passed, as did native x86_64 release-mode AOT-model execution. The aarch64
complete build stopped at 7,376/7,545 because of the time limit; GCC/non-libc++
compatibility builds and the armv7l/aarch64 model-execution cells are also
incomplete. The LLVM candidate was therefore neither committed nor pushed.
Because `bcc-tools` and `bpftrace` require the newly validated LLVM package,
neither dependent package was implemented in this run.
