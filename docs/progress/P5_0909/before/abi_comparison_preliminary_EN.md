# ABI Differences Between libstdc++ and libc++ (Preliminary)

- Date: 2026-09-09
- Status: **Preliminary** — member-level comparison of internal
  implementations is not yet done; see section 5
- Measured on: x86_64; libstdc++ (GCC 14.2), libc++ 22.1.8

---

## 1. Which layers agree and which differ

This distinction underlies everything else.

| Layer | The two libraries | Consequence |
|---|---|---|
| Name mangling rules | **Same** (both Itanium C++ ABI) | **The linker sees symbols as matching** |
| Calling convention | **Same** (determined by the platform ABI) | Argument passing works |
| Layout of built-in types and PODs | **Same** | Safe across the boundary |
| **Layout of standard library types** | **Different** | **Passing objects fails silently** |
| Inline namespace | Different (`__cxx11` / `__1`) | Some symbol names differ; linking fails |
| Exception and RTTI type identity | **Independent** | Typed exceptions cannot be caught across the boundary |
| Allocator and internal state | Independent | Releasing across the boundary corrupts the heap |

**The danger comes precisely from the first three rows agreeing.** If even
the mangling differed, linking would fail outright and the problem would
surface immediately. Because the symbols match, the problem is deferred to
run time — and does not crash.

---

## 2. Object layout differences

33 commonly used types were compared; **10 differ**.

### 2.1 Types whose layout differs

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

### 2.2 Types whose layout agrees

The remaining 23 (`std::array`, `std::pair`, `std::vector`,
`std::unique_ptr`, `std::shared_ptr` and others) have the same size and
alignment.

**Equal size does not mean they can safely cross the boundary** — the
meaning and invariants of the internal fields may still differ. This round
compared size and alignment only; **member-level comparison has not been
done** (see section 5).

### 2.3 The measured consequence

```cpp
// Provider, built against libstdc++
std::deque<int>* make_data() {
    return new std::deque<int>{1, 2, 3, 4};   // reports a length of 4
}

// Consumer, built against libc++
auto* d = make_data();
printf("%zu\n", d->size());                   // reads 106884723786536
```

**Links, loads, the process exits normally without crashing — but the data
is wrong.**

**Why the linker cannot detect this**: C++ name mangling does not encode the
return type into the symbol name; both sides mangle `make_data` to
`_Z9make_datav`.

---

## 3. Exported symbol differences

| Item | libstdc++ | libc++ |
|---|---:|---:|
| Exported C++ symbols | 6,057 | 1,969 |
| **Identical names** | **17** | |

**Whether those 17 are semantically equivalent has not been verified
individually.**

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

This is what we actually encountered during the migration, so it is listed
separately.

| Item | libstdc++ | libc++ |
|---|---|---|
| Marker type for recognising forced unwind | **Provided** (`abi::__forced_unwind`) | **Not provided** |
| Recognition clauses inside the standard library | About 50 | **None** |
| Exception specification on `condition_variable::wait` | **None** (GCC removed it to support cancellation) | **`noexcept`** |
| `wbuffer_convert` destructor | `= default`, no sync | Calls sync |
| `shared_mutex` implementation | `pthread_rwlock` on x86_64 | **Its own state machine** |

**Consequence of the last row**: libc++'s state machine does not roll back
when a writer is cancelled, leaving the lock permanently unusable;
libstdc++, using the system primitive, has the state managed by the kernel
and does not have this problem.

---

## 5. Not covered by this version

**The following require separate work, estimated at several days:**

1. **Member-level layout comparison** — this version compared size and
   alignment only. Explaining "internal implementation differences"
   requires comparing, for each type:
   - data member types, order and padding;
   - vtable structure and virtual function ordering;
   - internal helper types (nodes, control blocks, iterator representation);
   - allocation and growth strategies;

2. **Whether the 23 same-sized types are genuinely equivalent** — equal size
   does not imply equal field meaning;

3. **Semantic equivalence of the 17 identically named symbols** — not yet
   verified individually;

4. **The specific mechanisms behind exception and RTTI differences** — this
   version states only that type identity is independent, without detailing
   how each implementation does it;

5. **Architecture coverage** — this data is from x86_64; armv7l and aarch64
   have not been compared, and since pointer width and alignment rules
   differ, **some conclusions may not carry over**.

---

## 6. Which differences need to be made consistent

**Our operating principle: wherever libstdc++ provides a reference, align
with it.**

| Difference | Consistency needed | Our handling |
|---|---|---|
| Thread cancellation recognition | **Yes** | Marker type and ~50 protection sites added |
| `condition_variable` exception specification | **Yes** | Removed, matching libstdc++ |
| `wbuffer_convert` destructor behaviour | **Yes** | Changed to not synchronise, matching libstdc++ |
| `shared_mutex` cancellation safety | **Yes** | State rollback added (libstdc++ avoids this by using the system primitive) |
| **Object layout** | **No, and not achievable** | Two independent implementations; the standard does not require agreement. Managed through interface boundaries instead |
| Exported symbol set | No | Different implementation strategies; a normal difference |
| Inline namespace | No | It in fact provides partial protection (symbols do not collide) |

**The criterion**: **behavioural differences** (the same code behaving
differently under the two libraries) must be aligned; **implementation
differences** (layout, symbol sets) are not made to agree — they are managed
at the interface boundary.

---

## 7. Data sources

- Layout comparison: 33 types, x86_64; see the accompanying
  `abi_layout_comparison.tsv`;
- Cross-library instance: constructed and executed, x86_64;
- Symbol counts: actual exported symbol tables of both libraries;
- Cancellation-related differences: source inspected site by site, plus
  measurements on two architectures.
