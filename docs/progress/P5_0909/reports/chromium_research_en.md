# Chromium's libc++ configuration and the applicability of LLVM unwinding to Tizen

Evidence cut-off: 7 September 2026. Prepared for technical review. This is an applicability assessment, not an implementation or adoption recommendation.

## Executive findings

**A Google-wide or Chromium-wide specification banning `pthread_cancel` has not been established.** Chromium's `base::Thread` uses cooperative shutdown, but the pinned main repository contains a real cancellation call in its bundled gRPC POSIX implementation. Google's public restriction on C++ exceptions does not establish a restriction on POSIX thread cancellation. Internal policy documents were unavailable. [Thread implementation](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc), [gRPC implementation](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/third_party/grpc/source/src/core/util/posix/thd.cc), [public style guide](https://google.github.io/styleguide/cppguide.html).

**An isolated llvm-libgcc build worked for the tested native x86_64 configuration, but it has not been validated as a drop-in Tizen replacement.** Across 150 executions, changing the unwinder did not bypass retained `noexcept` boundaries, repair shared-mutex writer state, or eliminate the unpatched C++ ABI library's cancellation-rethrow termination. [Measurements](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md), [cell summary](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/measurement_summary.tsv).

**ChromeOS did migrate its system unwinder; that history is not proof of arbitrary prebuilt-binary compatibility.** Merged changes enabled all boards, updated the SDK and recorded system dependencies in December 2021. They establish coordinated toolchain integration, not the compatibility denominator or deployment contract needed for Tizen's external binaries. [Board enablement](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3335237), [SDK update](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3339453).

There were no replacement Tizen or ChromeOS images in this experiment. ARM and aarch64 runs with the experimental unwinder are `NOT_OBSERVED`. None of the eight assessment categories can be promoted to a platform-wide acceptance result.

## Evidence terminology and versions

“Static inspection” means source, configuration, ELF binary-format or commit inspection. “External report” means a project author's reported result, not our reproduction. “Measured” means an executed probe with archived commands, output and exit status. “Analysis” denotes a conditional inference. `NOT_OBSERVED` means not observed; `NOT_AVAILABLE` means the required material was unavailable. Process survival is not proof of valid object state or correct resource handling.

Chromium was pinned to `ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1`, with libc++ revision `97b436da4c33663581d394f4ee0a5977fc38c2f4` verified against its dependency manifest. The locally built LLVM tree was a different revision, `5ed6c77278dfa7a470667cf1a137723d3c96fe60`. Chromium's source inspection and the local runtime experiment must not be conflated. [References and retrieval index](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/REFERENCES.md).

## Chromium: configuration and cancellation

### What the configuration establishes

Chromium enables its custom libc++ by default. Component builds can use a shared standard library; non-component builds use a static configuration. The libc++abi target also depends on executable symbol-export settings. The default condition for custom LLVM libunwind is Fuchsia or Android: **Linux does not, by default, enable the bundled unwinder through that setting.** The standard library uses ABI namespace `__Cr` and ABI version 2. Symbol isolation does not translate C++ object layouts across libraries. [C++ configuration](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/c++/c++.gni), [unwinder configuration](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/unwind.gni), [ABI configuration](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/buildtools/third_party/libc++/__config_site).

`base::Thread::StopSoon()` posts shutdown work, while `Stop()` joins the underlying thread. This establishes the wrapper's cooperative protocol, not a universal constraint on third-party code. [Source](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc).

### Repository search and its limits

The archive contained 505,965 regular files. The explicitly selected source, header, build and documentation extensions and filenames yielded 195,796 scanned text files; 310,156 files had nonselected extensions and 13 contained NUL bytes. This did not include every separately fetched dependency repository, macro expansion, aliases or binaries. Every search term had a synthetic positive control, with an additional real thread-source control. [Search results](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/chromium_usage.tsv), [counts](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/054_scan_full_archive.stdout), [controls and follow-up](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/057_supplement.stdout).

| Search | Observed result |
|---|---|
| `pthread_cancel` | Two lines in one gRPC file: one actual call at `thd.cc:204`, one diagnostic string at line 206. Non-Android POSIX `Thread::Kill()` performs cancellation. |
| Indirect call and build entry | `work_stealing_thread_pool.cc:162` calls `Thread::Kill` from `DumpSignalHandler`; `third_party/grpc/BUILD.gn:2285` lists the POSIX implementation. Actual reachability in a particular shipped product remains unobserved. |
| Cancellation mode controls | No matches for `pthread_setcanceltype` or `PTHREAD_CANCEL_ASYNCHRONOUS` in the selected scope. This does not establish absence in all dependencies or products. |
| Standard shared mutex | Two `std::shared_mutex` mentions in one performance document, not current instantiation code; no `std::shared_timed_mutex` matches. Aliases and independent dependency repositories remain outside that conclusion. |
| `wbuffer_convert` | One header-manifest entry, not a consumer. No product-wide nonuse conclusion follows. |
| Boost includes | One `boost/locale.hpp` include in protobuf's vendored utf8_range `boost.cpp`; inclusion in a final product was not established. |

Chromium's StatisticsRecorder historically tried `std::shared_mutex`. In 2023 its maintainer reported returning to `base::Lock`, after observing workload-dependent overhead and improving how the original lock was used. This was not a cancellation rollback fix, and it disproves an “it was never used” assertion. [Maintainer discussion](https://groups.google.com/a/chromium.org/g/cxx/c/bIlGr1URn8I).

The public Google Style Guide had no exact cancellation-related matches in the inspected text, with its exception section serving as a positive control. Internal policy is `NOT_AVAILABLE`. A prohibition on language exceptions cannot be substituted for a documented thread-cancellation policy. [Guide snapshot](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/sources/google_style.txt), [search record](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/015_chromium_search.stdout).

### lightweight-web-engine and Tizen's different assumptions

The inspected source package identifies itself as **Starfish**, including its engine build and `liblightweight-web-engine` outputs. It does not establish Chromium ancestry. Its worker implementation enables asynchronous cancellation and calls `pthread_cancel` under `OS_POSIX && !STARFISH_ANDROID`, not a Tizen-only condition. The private upstream history was unavailable; the attempted public Tizen remote returned HTTP 403. Who introduced the behavior, when, and the exact ancestry are therefore `NOT_AVAILABLE`. We cannot attribute it to Chromium upstream or to a Tizen-only patch. [Package snapshots](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshot_origins.tsv), [source context](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/049_lwe_context.stdout), [remote failure](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/048_lwe_remote.stderr).

Analysis: Tizen has existing cancellation users, asynchronous-cancellation exposure, cross-standard-library semantic boundaries and an unquantified external-binary population. Adopting Chromium's link configuration does not enforce a cooperative protocol on those consumers. That protocol is an application-level precondition, not a property supplied by an unwinder.

## llvm-libgcc and forced unwinding

llvm-libgcc combines compiler-rt builtins with LLVM libunwind and supplies libgcc-style files and symbol versions. It does not supply libc++ object-layout compatibility, standard-library rollback guards or two historical libc++ function behaviors. The project explicitly leaves distribution-level compatibility to the integrator and requires opt-in. [Project documentation](https://github.com/llvm/llvm-project/blob/main/llvm-libgcc/docs/LLVMLibgcc.md), [documentation for the built revision](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/LLVMLibgcc.rst).

Forced unwinding is the cleanup protocol used by glibc cancellation; it is not an ordinary C++ exception search. LLVM's DWARF implementation implements `_Unwind_ForcedUnwind`, records a stop callback and argument, and drives cleanup with forced-unwind flags. GNU implements the corresponding protocol too. Matching exported names do not make their private contexts interchangeable. ARM EHABI, the ARM exception-handling ABI, has separate state and continuation conventions. [LLVM implementation](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/UnwindLevel1.c), [ARM implementation](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/Unwind-EHABI.cpp), [GNU implementation](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/gcc-unwind.inc).

The inspected glibc 2.40 source obtains backtrace, forced-unwind, CFA, architecture-conditional IP, resume and personality interfaces from `libgcc_s.so.1`, with additional architecture hooks. The experimental DSO supplies the six entries inspected for x86_64, and the actual host glibc exercised cancellation and backtrace successfully. **This validates an interface subset, not the entire GNU libgcc ABI or every architecture.** [glibc source](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/glibc-unwind-link.c), [symbol checks](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/041_analysis.stdout).

Relevant limitations include mixed-provider private contexts, historical ARM forced-unwind defects, physical file/SONAME requirements, and compiler-builtin symbol-version differences. ARM support was actively repaired in 2021: it is incorrect to claim LLVM simply never handles forced unwinding. This does not establish a complete ChromeOS cancellation acceptance matrix. [ARM fix](https://reviews.llvm.org/D109856), [mixed-provider discussion](https://reviews.llvm.org/D150897), [related distribution crash report](https://github.com/llvm/llvm-project/issues/90041).

## What ChromeOS migration history actually proves

The browser's libc++ migration and the operating system's unwinder migration were separate changes. The 2017 browser announcement discussed C++14 and control-flow integrity; it does not establish a simultaneous conversion of every OS C++ library. [Browser announcement](https://groups.google.com/a/chromium.org/g/chromium-os-dev/c/e8pJRHLisIg).

| Period | Verified public evidence |
|---|---|
| 2021 proposal | D106703 describes a controlled SDK/toolchain context and discusses static libgcc interaction. That proposal and its early associated change were abandoned; their draft test fields are not release evidence. |
| October 2021 | A merged fix made `libgcc_s.so.1` the physical file, reversing the alias direction to resolve NaCl breakage. The author reported the NaCl test passing. |
| November 2021 | binutils needed an explicit pthread link dependency; Breakpad needed a pkg-config path correction for a 64-bit crash reporter built on arm32. |
| December 2021 | Merged changes enabled libunwind across boards, updated the SDK and added system dependencies. Multiple board tests were reported, but their complete logs/images were not obtained. |
| February 2022 | The successor llvm-libgcc project landed upstream through D108416. This date is not the date of ChromeOS's earlier downstream enablement. |
| July 2023 | Earlier installation of llvm-libunwind broke glibc bootstrap builds; a build-time sysroot change addressed it. This was not a blanket deployed-system rollback to GNU unwinding. |

Sources: [proposal](https://reviews.llvm.org/D106703?id=361316), [file-layout fix](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3246062), [pthread dependency](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3298526), [Breakpad](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3292637), [system dependency](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3343875), [upstream project](https://reviews.llvm.org/D108416), [glibc bootstrap fix](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/4677913). Retrieved commit status, SHA and messages are in the [change record](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/052_cros_changes.stdout).

The current ebuild describes a synthesized `libgcc_s.so.1`, version scripts and frame APIs. This is configuration evidence, not an observation of a current running image. [ebuild](https://chromium.googlesource.com/chromiumos/overlays/chromiumos-overlay/+/main/sys-libs/llvm-libunwind/llvm-libunwind-9999.ebuild), [snapshot](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/sources/cros_ebuild.txt).

For old binaries, compatible names/version nodes and coordinated SDK changes are evidenced. An arbitrary external-binary support contract, a complete old/new root-filesystem transition and the plugin denominator are `NOT_AVAILABLE`. Neither “everything was rebuilt”, “everything was reflashed” nor “symbol versions alone solved it” is established as the complete answer. GCC compatibility nodes are also not the same as dual libc++ symbols retaining different wait semantics.

For cancellation, implementations, ARM fixes and reports of mixed-unwinder problems exist. A complete ChromeOS matrix covering cancellation mode, library combination and architecture was not obtained. Neither universal validation nor universal nonuse is justified.

## Tizen applicability: the eight requested categories

DSO means a dynamic shared object. “Image” below means an integrated target system, not an isolated host process.

| Category | Configuration or static evidence | Measured / image evidence | Applicability and missing evidence |
|---|---|---|---|
| Provider layout | Local aliases resolve to `libunwind.so.1.0`, SONAME `libunwind.so.1`; ChromeOS's configured physical layout differs. | Experimental mapping verified per host run; replacement target images not observed. | Loader success does not establish sandbox, installer or dependency-scanner compatibility. |
| Exports and versions | 198 defined name/version pairs in the Tizen x86 sysroot; 231 in the experiment; 108 platform pairs absent from the experimental set. This count includes 15 ABS version-node names in the platform table. | ELF inspection, not a running-image denominator. | The export set is not a superset. Some names exist under different/no versions; actual consumer imports are needed to identify affected binaries. It does not mean 108 missing functions or 108 failing consumers. |
| `pthread_cancel` | glibc forced unwinding still traverses ABI-library and language/cleanup boundaries. | GNU and LLVM host cells agree; target images not observed. | Limited feasibility, not safety for every catch site, asynchronous cancellation or library algorithm. |
| `backtrace` | glibc dynamically resolves unwinder entry points. | Main/program-plugin frame-count assertions passed five times per provider. | Signal contexts, JIT registration, optimized production stacks and crash reporters remain outside coverage. |
| Cross-DSO behavior | Context and personality implementations must form a coherent protocol. | Plugin throws integer 42; caller catches; cleanup count 1, five times per provider. | One coherent ABI-pair sample, not cross-standard-library object-layout validation or mixed-provider ordering. |
| `dlopen` plugins | Plugins can introduce providers and version requirements. | `RTLD_NOW|RTLD_LOCAL`, throw/cleanup and close passed with mapping checks. | No production legacy plugins, deep binding, separate link namespaces or static embedded unwinders tested. |
| Prebuilt binaries | GNU-dependent platform artifacts exist; external-product denominator unavailable. | Only purpose-built probes ran. | Compatibility cannot be promised without BOMs, binaries, version imports, rebuild declarations and update boundaries. |
| armv7l gnu-eabi | Documentation names armv7a gnueabihf; Tizen macros show EABI/VFP instruction capability but not `__ARM_PCS_VFP`. Script acceptance does not check that calling-convention macro. | Preprocessing and existing sysroot ELF inspected; experimental ARM runs/images not observed. | gnueabi and gnueabihf cannot be equated; actual floating-point calling convention and EHABI combinations remain unvalidated. |

Evidence: [DSO identity](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/023_experimental_identity.stdout), [symbol comparison](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/symbol_comparison.tsv), [ARM macros](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/034_arm_macros.stdout), [version script](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/gcc_s.ver.in), [ARM ELF](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/053_local_arch_metadata.stdout), [test design](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md).

## Local measurements and their interpretation

This was native x86_64 on Ubuntu glibc 2.39, using Tizen compiler wrappers and existing libc++/libc++abi artifacts—not a native Tizen image or QEMU. All experimental binaries were under the isolated directory. Every formal run checked `/proc/self/maps` and the versioned forced-unwind symbol's provider. The LLVM cells rejected other detected libgcc_s/libunwind shared mappings. [Host identity](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/051_host_environment.stdout), [design and raw evidence](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md).

| Case | GNU provider | LLVM provider | Behavioral result |
|---|---|---|---|
| Retained specification: ordinary wait and three clock variants | 5/5 per cell | 5/5 per cell | Terminate handler entered; outer cleanup count 0. |
| Removed specification: same four variants | 5/5 per cell | 5/5 per cell | Joined as cancelled; cleanup count 1. Inline paths were recompiled with the corresponding headers. |
| Ordinary/timed writer without rollback | 5/5 per cell | 5/5 per cell | Writer bit 2147483648 remained after the original reader left; subsequent acquisition failed. |
| Ordinary/timed writer with rollback | 5/5 per cell | 5/5 per cell | Writer bit cleared, existing reader preserved, subsequent real reader/writer progress, final state 0. |
| Backtrace and plugin exception | 5/5 | 5/5 | Frame-count thresholds, exact value 42, cleanup 1 and successful close. |
| Patched ABI pair: cancellation caught and rethrown | 5/5 | 5/5 | Catch 1, cleanup 1, cancelled join. |
| Unpatched coherent ABI pair: same rethrow | 5/5 SIGABRT | 5/5 SIGABRT | Foreign-exception diagnostic, not a clean cancelled join. |

Total: 30 cells × 5 runs. Retained-specification exit 86 is an instrumented terminate-handler marker, not a signal. A damaged-state control exiting zero means the expected damage was asserted, not that the library is healthy. Smoke tests and an initial incoherent-library loading failure were excluded from the matrix and retained separately.

No claim covers every library site, every ordinary error path, asynchronous cancellation, all timing schedules, arbitrary Clock/template parameters or user callbacks. [Complete measurements](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/measurements.tsv).

## Comparison with the seven known problems

An absent product-level observation must not be rewritten as “they never encountered it”, nor assigned an unsupported explanation.

The eight GNU-only/source locations without equivalent protection are four experimental executor
sites, two GNU vstring sites, one condition_variable_any relock guard and one TR2 dynamic_bitset site.
They are not eight measured failures in the current LLVM runtime. The two previously unreachable
protections refer to the non-throwing boundaries in `basic_string::shrink_to_fit` and the
`basic_ostream<char>::sentry` destructor, not the two code defects repaired later. No new runtime results
for these sites were added in this investigation.

| Known Tizen problem | Chromium / ChromeOS evidence | Consequence of changing unwinder |
|---|---|---|
| Catch-all swallows cancellation | Complete product-level behavior not observed; bundled cancellation exists. | Our new probe tests rethrow, not swallowing. ABI handling remains relevant; no automatic obsolescence established. |
| Missing internal libc++ protection | Inspected writer paths lack rollback; not every internal catch was re-audited. | Both writer residuals reproduced with LLVM. Boundary removal and state restoration are separate concerns. |
| `noexcept` termination | Pinned libc++ still has the relevant wait specifications. | Both providers terminate in retained-specification cells. |
| Eight GNU-only protection locations | Exact corresponding product handling unavailable. | llvm-libgcc does not add missing high-level libc++ paths; these eight were not remeasured. |
| Two ineffective protections | Exact counterpart validation not observed. | No new evidence supersedes the existing result; individual new-provider tests remain missing. |
| Asynchronous cancellation bypass | A universal public prohibition was not established; Starfish explicitly enables asynchronous cancellation. | Changing unwinders does not itself change cancellation timing or make resource operations atomic; no new asynchronous matrix was run. |
| Cross-library object-layout mismatch | ABI namespace isolation and controlled builds are not object conversion; arbitrary legacy policy unavailable. | An unwinder does not alter standard-library/Boost object layouts. Integer exceptions are not layout-bearing object tests. |

## Comparison with the four established Tizen decisions

| Tizen decision | Chromium / Google | llvm-libgcc / ChromeOS | Is it the same treatment? |
|---|---|---|---|
| Remove wait specifications and roll back writer state | Inspected dependency retains specifications and unguarded writer-state waits; actual cancellation of those product paths not observed. | LLVM reproduces retained-boundary termination and unguarded residual state locally; exact ChromeOS library patches unavailable. | No evidence that unwinder replacement substitutes for either change. |
| Dual symbols for old callers | `__Cr` and controlled builds are not dual historical semantics for one function; arbitrary old-binary contract unavailable. | GCC compatibility versions and SDK coordination exist, but not proof of dual libc++ wait behavior. | Related compatibility mechanisms, not demonstrated equivalence. Old inline caller metadata is not repaired by a new DSO. |
| Stop destructor synchronization in `wbuffer_convert` | Inspected destructor still calls `__close()`, which synchronizes. Header-list-only search results do not establish all-product nonuse. | Unwinder choice does not change this algorithm; a precise ChromeOS local change was not obtained. | Inspected Chromium source differs from the Tizen decision; the reason cannot be filled in with an unproved nonuse premise. |
| Migrate Boost itself to remove cross-standard-library edges | Custom-library third-party builds can share an ABI; historical objections to broad Boost adoption coexist with a vendored include example. | System unwinder migration is not proof every third-party C++ library migrated to libc++. | A layout boundary disappears only when its actual endpoints become ABI-compatible, not merely when an unwinder is unified. |

Sources: [condition-variable source](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/src/condition_variable.cpp), [wait header](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/include/__condition_variable/condition_variable.h), [writer source](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/src/shared_mutex.cpp), [timed writer header](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/include/shared_mutex), [conversion buffer](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/include/__locale_dir/wbuffer_convert.h), [historical Boost discussion](https://codereview.chromium.org/2242483003/).

## Other migration problems and reusable diagnostics

The independent [reference issue catalogue](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/REFERENCE_ISSUES.md) documents discovery, reported correction, residual uncertainty, Tizen conditions, diagnostic blind spots and whether a historical solution can be adapted. It contains the following evidenced cases, not an exhaustive history:

| Issue | Finding and Tizen diagnostic boundary |
|---|---|
| Stale build artifacts | Mixed standard-library objects caused link trouble; clean/incremental build identity checks are relevant. Successfully linked microprobes do not inspect application caches. |
| GPU/ANGLE integration | The libc++ default switch was reverted after GPU-builder failures and relanded following an ANGLE roll. The exact faulty subchange was not reconstructed; actual Tizen graphics builds/tests are needed to establish equivalence. |
| Static-initializer accounting | Initialization moved from a shared library into executables, changing counted budgets. This was not evidence of increased total startup work; compare like-for-like accounting and actual performance. |
| NaCl provider file layout | A real-file versus alias-direction change fixed a loader failure. Tizen's relevant sandbox/component inventory is missing; ordinary dlopen success cannot settle it. |
| Implicit pthread linkage | binutils needed an explicit dependency. These probes already use `-pthread`, so they cannot expose that application-link blind spot. |
| Breakpad multilib paths | Hard-coded pkg-config paths impeded an arm32/64 build. Inspect actual bitness, sysroot paths and the production crash-reporting path. |
| Mixed unwind providers | External crash reports expose private-context mixing. Mapping checks detect some dynamic cases, not hidden static implementations; provider identity and plugin order matter. |
| ARM EHABI | Historical fixes establish architecture-specific work. Native x86 success is not ARM validation; test actual ABI, stop callbacks, cleanup and continuation paths. |
| glibc bootstrap order | Earlier unwinder installation broke toolchain construction. Building a probe in a complete sysroot cannot inspect the bootstrap dependency graph. |
| Shared-lock workload behavior | Chromium returned to its original lock after workload-specific evaluation. Correctness probes do not measure contention, starvation or tail latency; its product lock choice is not transferable without workload evidence. |
| Version-script/export differences | This is a **new local integration finding**, not an attributed Chromium historical incident. lld enabled our build but did not resolve 108 absent platform name/version pairs; compare actual consumer imports. |

Each entry links its primary record in the catalogue. Unknown residual status means no completeness claim. Historical configuration changes generally need platform adaptation; they do not authorize blanket cache deletion, library replacement, link-flag changes or higher performance thresholds. Conditional diagnostic suggestions are not a deployment recommendation or priority order.

## Why Tizen does not adopt Chromium's approach

Here, “does not adopt” means **not treating Chromium's cooperative shutdown protocol as a replacement for platform-wide cancellation support in Tizen**. It does not reject Chromium's toolchain or ABI-isolation techniques. The table distinguishes demonstrated practices from broader assumptions that the evidence does not establish.

| Chromium prerequisite or practice | Evidence (URL) | Tizen circumstances and why direct reuse is insufficient | Evidence (URL) |
|---|---|---|---|
| Threads using `base::Thread` shut down through a cooperative request, posted exit work and join. | [Pinned thread.cc](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc) | Inspected platform sources contain real cancellation calls. Conditional paths in lightweight-web-engine and OpenMP enable asynchronous mode; they cannot all be assumed to terminate through this wrapper. Conditional reachability is not proof of execution in every product. | [Two-component source analysis and asynchronous-cancellation measurements](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R108/W3_REPORT.md) |
| Cooperative shutdown depends on callers adopting and following its protocol; the wrapper does not establish a repository-wide enforced rule. The selected main-repository text contains only one direct `pthread_cancel` call, but that call exists. | [Actual gRPC call](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/third_party/grpc/source/src/core/util/posix/thd.cc), [scoped search results](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R116/chromium_usage.tsv) | External applications, supplier components and plugins are not fully covered. Current evidence cannot guarantee universal compliance, and a library update alone cannot rewrite unavailable caller source to use this protocol. This is not a claim that product-level constraints could never be established. | [External-component gaps, required material and responsible parties](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R110/W1_REPORT.md) |
| Common build settings and pinned dependencies can keep managed targets consistent. **They do not prove that a monorepo always rebuilds everything or disregards all old binaries.** | [Standard-library build settings](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/c++/c++.gni), [pinned dependencies](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/DEPS) | Five prebuilt DSOs were confirmed as copied rather than rebuilt during migration. External third-party artifacts and rebuild policies still lack a denominator. Rebuilding both sides of every boundary therefore cannot be assumed, nor can counts or policies be invented for unavailable external products. | [Non-rebuilt-component inventory](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R108/W2_REPORT.md), [external support-scope gaps](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R110/W1_REPORT.md) |
| ABI namespace `__Cr` separates standard-library symbols; it does not convert objects between implementations. | [Pinned __config_site](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/buildtools/third_party/libc++/__config_site) | A libstdc++ provider and libc++ consumer linked and loaded successfully, yet disagreed on the same deque's size and element count. Namespace separation does not repair internal layout differences absent from the interface name. This was a minimal x86_64 probe, not proof that every production edge fails. | [Cross-library object-layout example and raw-evidence index](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R80/REPORT.md) |
| Ordinary Linux defaults to the system unwinder rather than the bundled libunwind; where the system supplies GNU libgcc, GNU remains the provider. This is not a claim about every Linux product, including ChromeOS. | [Pinned unwind.gni](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/unwind.gni) | **With GNU on both sides of the comparison, the unwinder is not the differentiator.** The 30 local GNU/LLVM cells additionally show that changing providers does not remove specification-triggered termination or writer residual state. These are host measurements, not target-image validation. | [30-cell design and environment identity](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R116/MEASUREMENTS.md), [cell results](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R116/measurement_summary.tsv) |

The following summary concerns the inspected cooperative paths and Tizen's stated support objective. It does not assert that all Chromium products avoid cancellation, or that library fixes make arbitrary call chains, asynchronous cancellation and user callbacks safe. All architecture, call-chain and unresolved-path limitations elsewhere in this report still apply.

**In short, this Chromium approach is “conventions plus avoiding cancellation on compliant paths”, whereas Tizen's approach is “library fixes plus cancellation support within defined boundaries”: the former depends on callers following the cooperative protocol, while the latter does not require every caller to abandon cancellation; the difference is in prerequisites, not superiority.**

## Applicability conclusions and impact on existing work

**Chromium: selected techniques are reusable; its configuration is not a complete Tizen cancellation-safety solution.** Cooperative shutdown and symbol isolation depend on consumer protocols and ABI boundaries. Tizen's cancellation users and external-binary gaps remain. A specification-level universal exemption from cancellation was not substantiated.

**llvm-libgcc: limited local build/runtime feasibility is established; platform replacement compatibility is not.** Export differences, provider-layout differences, missing target-image and ARM measurements, and unavailable external binaries prevent an acceptance conclusion.

**Revalidation is not the same as rewriting.** If the provider changes, the four existing patches, five language boundaries, asynchronous-cancellation consumers, old-symbol callers, destructor behavior and Boost layouts all need their evidence scoped to the new combination. Only part of that x86 matrix was added here. Nothing establishes that all four patches must be rewritten or can be removed. The two writer instances still needed rollback for the measured state restoration; arbitrary templates and callbacks were not proven. Object layouts and old inline exception metadata are not repaired by an unwinder swap.

Reusable evidence methods include pinned identities, actual-provider checks, versioned-export comparison, controlled rebuilds and cross-DSO/plugin value assertions. Unsupported shortcuts include a universal cancellation ban, assuming arbitrary third-party binaries share a controlled SDK, equating ARM with x86, or assuming matching entry-point names imply compatible private contexts.

Outstanding material comprises internal Google policy, Starfish history, ChromeOS legacy/plugin/cancellation acceptance evidence, Tizen external BOMs and version imports, and ARM/aarch64 experimental-provider and actual-image results. These are explicit limits for headquarters review, not a basis for selecting a deployment strategy here.
