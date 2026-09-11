# ABI Differences Between libstdc++ and libc++ (Preliminary)

- Date: 2026-09-09
- Revision: P5, 2026-09-11; evidence is limited to the existing records checked for this revision, not a new product validation on that date.
- This is not acceptance of all products; the accompanying support statement remains subject to human review.
- Status: **Preliminary** — member-level comparison of internal
  implementations is not yet done; see section 5
- Size/alignment probes: x86_64; host GCC 13.3.0 on the GNU side; Clang 21.1.1 with cached platform libc++/libc++abi on the LLVM side. The libc++ header macro 220108 means 22.1.8, not the compiler version. Platform GCC 14.2 sources and its baseline symbol manifest are separate inputs.

---

## 1. Which layers agree and which differ

This distinction underlies everything else.

| Layer | The two libraries | Consequence |
|---|---|---|
| Name mangling rules | Both use Itanium C++ ABI; actual names still depend on namespaces, ABI tags and other details | This instance matches, not every interface necessarily links |
| Calling convention | Target ABI, complete definitions and compiler options must be checked | Agreement is a prerequisite, not complete proof of safety |
| Built-in types and PODs | Compatible boundaries can be designed under matching target ABI and other conditions | Matching Engine/Rect cross-library calls are NOT_OBSERVED; implicit this, lifetime and exceptions also matter |
| **Standard-library representations** | Some measured sizes/alignments differ; full equivalence of the remainder is unproved | A silent wrong-value counterexample exists, not a universal same-failure result |
| Inline namespace | Different (`__cxx11` / `__1`) | Some symbol names differ; linking fails |
| Exceptions and RTTI | Actual runtimes, personality and symbol binding affect matching | BoundaryError from the tested GNU plugin to libc++ did not enter typed catch; this is not loss of all type information |
| Allocators and internal state | Actual allocator, destructor, alignment and lifetime contracts must be checked | Two loaded libraries do not imply two independent heaps; new[]/free is invalid even within one library |

**Linking successfully does not establish correctness.** Interface data, lifetimes, exception/cancellation
paths and the actual ABI configuration jointly determine compatibility. Fixed C samples passing do not
prove arbitrary C interfaces safe. Direct standard-object, exception and ownership transfer across an
unaligned ABI is outside the current guarantee. Some combinations fail to link; others run with wrong values.

---

## 2. Object layout differences

Under the fixed x86_64 configuration, 10 of 33 concrete type instances differed in sizeof or alignof;
23 matched on both. Member offsets, nodes, control blocks and operational equivalence have not been fully verified.

### 2.1 Types whose size or alignment differs

| Type | libstdc++ | libc++ | Delta |
|---|---:|---:|---:|
| `std::any` | 16 | **32** | +16 |
| `std::function<void()>` | 32 | **48** | +16 |
| `std::deque<int>` | **80** | 48 | −32 |
| `std::map<int,int>` | **48** | 24 | −24 |
| `std::set<int>` | **48** | 24 | −24 |
| `std::unordered_map<int,int>` | **56** | 40 | −16 |
| `std::unordered_set<int>` | **56** | 40 | −16 |
| `std::string` | **32** | 24 | −8 |
| `std::future<int>` | **16** | 8 | −8 |
| `std::promise<int>` | **24** | 8 | −16 |

**Note the direction is not consistent** — libc++ is larger for some types
and smaller for others. This reflects different design trade-offs, not
simply a more compact newer implementation.

Values and deltas are bytes; delta means libc++ minus libstdc++. Alignment of std::function<void()> also differs: GNU/LLVM is 8/16 bytes.

### 2.2 Types with matching size and alignment

The remaining 23 (`std::array`, `std::pair`, `std::vector`,
`std::unique_ptr`, `std::shared_ptr` and others) have the same size and
alignment.

**Equal size does not mean they can safely cross the boundary** — the
meaning and invariants of the internal fields may still differ. This round
compared size and alignment only; **member-level comparison has not been
done** (see section 5).

### 2.3 The measured consequence

These are core fragments of the original experiment. Provider and consumer are separate translation
units; headers, printing and helper definitions are omitted.

```cpp
// Provider, built against libstdc++
std::deque<int>* make_payload() {
    return new std::deque<int>{11, 22, 33, 44};
}

// Consumer, built against libc++
int main() {
    auto* payload = make_payload();
    const auto producer_view = provider_size(payload);
    const auto consumer_view = payload->size();
    destroy_payload(payload);       // deleted by the provider
    return producer_view == consumer_view ? 0 : 42;
}
```

**It linked and loaded without a signal crash, but deliberately exited 42 to flag a wrong value.**
Provider/consumer sizeof was 80/48 and length views were 4/106884723786536; the object was released
through the provider's destroy_payload. The wrong length is one recorded output, not a fixed value on every run.

**Why this instance links**: the ordinary non-template function make_payload's return type is not
directly encoded in the Itanium name `_Z12make_payloadv`. `_Z9make_datav` correctly encodes make_data,
but is not the original symbol. Template functions and function types have different rules; ABI tags
can change names. The earlier string variant failed to link because of an abi:cxx11 tag.

---

## 3. Exported symbol differences

| Item | libstdc++ | libc++ |
|---|---:|---:|
| Input entries deduplicated by name after removing versions | 6,057 (GCC x86_64 baseline manifest) | 1,969 (cached platform libc++ defined-export record) |
| **Exact name intersection, ignoring versions** | **17** | |

6,057 = 4,546 FUNC + 1,511 OBJECT. These are not counts of all public C++ APIs or a direct size
comparison of two live libraries measured by the same command. Constructor/destructor ABI variants
count as separate names. The 17 entries use GLIBCXX_*/CXXABI_* on GNU and LLVM_22 on LLVM; version contracts differ.

**All 17 names have had their standard-level purposes checked individually; grouped behavioral probes
produced matching output with each runtime correctly paired.** Interchangeability under mismatched
pairing, symbol preemption/dlsym, dual-runtime TLS or cross-provider exception copying/destruction
has not been proved. Matching names and purposes do not make arbitrary mixing safe.

**Sources of the difference**:

1. **Inline namespaces** — libstdc++ uses `std::__cxx11`, libc++ uses
   `std::__1`, so many same-named types mangle differently;
2. **Implementation strategy** — libc++ makes extensive use of header
   inlining (`_LIBCPP_HIDE_FROM_ABI`), so the code is compiled into the
   consumer rather than exported; libstdc++ keeps more in the library.

**The consequence of the second point**: much of libc++'s implementation is
**compiled into each consumer's binary**, and updating the library does not
change those copies.

---

## 4. Differences relating to thread cancellation

This table compares fixed sources without local patches, not the current patched runtime. GNU is
GCC 14.2 source 49c50605… under include/src/libsupc++; LLVM is 5ed6c772… under libcxx/include and src.
Typed-catch syntax points include extensions, compatibility sources and unevaluated conditional branches;
they are not compiled instances, reachable cancellation points or defect counts. The LLVM scan excludes
libcxxabi's own recognition logic; the original zero-match check had a positive control.

| Item | libstdc++ | libc++ |
|---|---|---|
| Marker type for recognising forced unwind | **Provided** (`abi::__forced_unwind`) | **Not provided** |
| Explicit typed-catch recognition syntax points in the stated directories | 65 points / 16 files | 0 in original unpatched libcxx |
| Exception specification on `condition_variable::wait` | **None** (GCC removed it to support cancellation) | **`noexcept`** |
| `wbuffer_convert` destructor | `= default`, no sync | Calls sync |
| `shared_mutex` implementation | pthread_rwlock in the inspected configuration; macros can select a CV fallback | Its own state machine |

**Limits of the last row**: glibc 2.40 pthread read/write locks use user-space state, atomics and futex;
their acquisition path does not provide a deferred cancellation point. This is not automatic kernel
restoration after cancellation. Macros including _GLIBCXX_USE_PTHREAD_RWLOCK_T select the libstdc++
backend; architecture alone does not identify every build. Original libc++ writers reserve a writer
bit before waiting. If cancellation can unwind through that wait without rollback, state remains;
retaining the exception specification may terminate first. This patch batch adds the relevant rollback.
Neither backend establishes safety for arbitrary asynchronous cancellation.

---

## 5. Not covered by this version

**The following remain unfinished; timing and assignments require separate human confirmation:**

1. **Member-level layout comparison** — this version compared size and
   alignment only. Explaining "internal implementation differences"
   requires comparing, for each type:
   - data member types, order and padding;
   - vtable structure and virtual function ordering;
   - internal helper types (nodes, control blocks, iterator representation);
   - allocation and growth strategies;

2. **Whether the 23 same-sized types are genuinely equivalent** — equal size
   does not imply equal field meaning;

3. **Interchangeability of the 17 same-name symbols when mixed** — purpose checks and correctly paired probes exist; the mixed combinations listed in section 3 remain unproved;

4. **All exception/RTTI combinations** — the tested GNU plugin threw BoundaryError{77}; native x86_64
   and physical armv7l entered catch-all and exited 12. The relevant libc++abi personality's typed catch
   first requires native_exception; the foreign path does not match. This is not loss of all RTTI.
   Arbitrary types, directions and symbol bindings remain uncovered;

5. **Architecture coverage** — the full 33-type table and deque are x86_64 only; ICU's single entry is
   physical armv7l only; controlled C ABI and the exception sample have native x86_64/physical armv7l
   records. Boost also has representative native x86_64, armv7l QEMU and aarch64 QEMU edges. The aarch64
   round added 22, with cumulative representative-group coverage of 49/90, not all edges. Complete
   aarch64 cancellation/current official-suite validation remains unfinished.

Other gaps include arbitrary old callers, extra noexcept/no-exception frames, arbitrary templates/callbacks,
asynchronous cancellation, custom allocators, arbitrary plugin-loading combinations, full member layouts
and actual third-party interfaces. The external-component denominator is unavailable.

### Old/new binding and header limitations

Compilation determines cleanup code; linking determines symbol versions. Old ELF here means a binary
that already records the old wait node.

| Compilation/linking case | Wait entry | Measured boundary |
|---|---|---|
| Already-linked old ELF | LLVM_22 | Retains the old termination contract |
| Old .o/.a relinked to the dual-version library without recompilation | LLVM_22_TIZEN_1 (new default) | Depends on call-site and cleanup-table layout; the quiet sample joined with a lock still held, so clean exit is not guaranteed |
| Recompile with new headers and link the dual-version library | LLVM_22_TIZEN_1 | Cleanup, unlock and subsequent progress passed under the tested conditions |
| New-header objects first linked to the old single-version library, then runtime upgraded | LLVM_22 | Still uses the old node recorded at link time |

Dual versions cover only the ordinary-wait and system-clock timed-wait library entries. Steady-clock
and custom-Clock waits, wbuffer_convert destruction and timed-writer rollback have header implementations.
Updating the DSO cannot repair old compiled copies; affected translation units require recompilation.

---

## 6. Which differences need to be made consistent

**This section states the project's adopted principles, not a standard requirement or a measured
claim that all behavior of the two libraries is equivalent.** It describes the specific alignment
targets and limits already decided; absence of a standard requirement does not prove technical impossibility.

| Difference | Consistency needed | Our handling |
|---|---|---|
| Thread cancellation recognition | Decided target | First patch batch adds the marker type and covers 50 sites = 20 header + 30 library sites; 36 actual new typed handlers = 20 + 14 + 2, with two aggregate handlers covering 16 library sites |
| `condition_variable` exception specification | **Yes** | Removed, matching libstdc++ |
| `wbuffer_convert` destructor behaviour | Decided target | No automatic flush; not identical output byte counts |
| `shared_mutex` cancellation safety | Decided target | Rollback on the two writer paths; the GNU pthread backend's acquisition lacks a deferred cancellation point, which is not automatic rollback |
| **Object layout** | Interchangeability is outside this round's scope | Managed through interface boundaries; no claim of technical impossibility |
| Exported symbol set | No | Different implementation strategies; a normal difference |
| Inline namespace | No | It in fact provides partial protection (symbols do not collide) |

Wait cancellation unwinding, ordinary errors, read/write-lock cancellation response and destructor
output must be considered separately. libc++ retains its error checks; uncaught ordinary errors can
still terminate, and cleanup is not guaranteed. wbuffer_convert aligns on no automatic destructor
flush: new/old/GNU probes output 3/9/0 bytes, with buffer algorithms unchanged. GNU and libc++ already
had different buffering strategies; old libc++'s 9-to-new-3 change results from removing the destructor
flush. Object-layout and symbol-set interchangeability are outside this round's targets.

---

## 7. Data sources

- Layout comparison: 33 types, x86_64; see the accompanying
  `abi_layout_comparison.tsv`;
- Cross-library instance: constructed and executed, x86_64;
- Symbol counts: GCC x86_64 baseline manifest and cached platform libc++ defined-export text, parsed as specified in section 3;
- Cancellation: fixed unpatched-source inspection is separate from itemized patched-runtime measurements; source syntax counts are not runtime results.

Verifiable sources: [size/alignment, deque and the 17 same-name symbols](https://github.com/lhmax2010/libc-_replacement/tree/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/R80),
[numeric and source-tree recheck index](https://github.com/lhmax2010/libc-_replacement/blob/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/P4_0909/NUMBER_AUDIT.md),
[support statement awaiting review and itemized original records](https://github.com/lhmax2010/libc-_replacement/blob/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/IMPL_0909/W3/SUPPORT_SCOPE_EN.md).
