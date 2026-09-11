# Interface Compatibility Between libstdc++ and libc++

- Date: 2026-09-09
- Revision: P5, 2026-09-11; evidence is limited to the existing records checked for this revision, not a new product validation on that date.
- This is not acceptance of all products; the accompanying support statement remains subject to human review.
- Context: interfaces between the platform (migrating to libc++) and
  components that remain on libstdc++ — third-party applications,
  prebuilt libraries

---

## 1. Summary

**Compatibility depends jointly on the data crossing the interface, object lifetimes,
exception/cancellation paths, and the actual ABI configuration.**

| Interface form | Supportable | Basis |
|---|---|---|
| Plain C interface | Fixed samples passed; not unconditional support | Measured, but not proof that arbitrary C interfaces are safe |
| C++ interface passing only built-in types and PODs | Conditionally feasible | Matching cross-library Engine/Rect tests are NOT_OBSERVED; see 2.2 |
| C++ interface passing standard library objects | No current guarantee for direct use across an unaligned ABI | Silent wrong-value counterexample; not every type necessarily fails the same way |
| C++ interface throwing across the boundary | No current guarantee for direct use across an unaligned ABI | Typed catch did not match in a particular runtime combination; see 3.2 |
| C++ interface transferring ownership | No current guarantee for direct use across an unaligned ABI | Allocation, release and lifetime contracts must be explicit |

Each case is explained with code below.

These are interface fragments, omitting some headers and implementations. Providers and consumers
are separate translation units. Ellipses denote pseudocode; executable statements belong inside functions.
The C interface alternatives must not be exported together as same-name C overloads.

---

## 2. Interfaces that can be supported

### 2.1 Plain C interfaces

```c
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct engine_s* engine_handle;

engine_handle engine_create(void);
int           engine_get_width(engine_handle h);
void          engine_set_position(engine_handle h, int x, int y);
int           engine_read(engine_handle h, char* buf, size_t buflen);
void          engine_destroy(engine_handle h);

#ifdef __cplusplus
}
#endif
```

**Conditions**: these explicit parameters avoid passing standard-library objects directly, but
the target C ABI, pointer validity, lifetime and exception/cancellation contracts must still agree.
A C-shaped interface alone does not establish safety.

**Measured**: on physical armv7l hardware, a libc++ program used dlopen/dlsym to call
`u_getVersion` in the installed `/usr/lib/libicuuc.so.78.1`. It printed 78.1.0.0,
asserted major version 78, recorded libc++, libc++abi, libstdc++ and ICU loaded together, and exited 0.
This validates that fixed C-entry sample, not all ICU APIs, allocation/release, callbacks or product
functionality. The equivalent real-ICU run on x86_64 is NOT_OBSERVED in these records.

### 2.2 C++ interfaces passing only built-in types and PODs

```cpp
// Conditional interface illustration; these member calls have no corresponding cross-library measurement
class Engine {
public:
    int    width() const;                    // returns int
    void   setPosition(int x, int y);        // takes ints
    bool   isReady() const;                  // returns bool
    double scale() const;                    // returns double
};

struct Rect { int x, y, w, h; };             // check complete definitions and target ABI on both sides
Rect   getBounds();                          // check the by-value return ABI as well
```

**Conditional analysis**: compatible boundaries can in principle be designed when the target ABI,
complete type definitions, calling convention, compiler options, object lifetimes and exception
boundaries agree. Non-static member calls also pass an implicit `this`; explicit arguments alone
are insufficient. Engine/Rect have no corresponding cross-library measurements here and must not
be labelled measured support. Exact-state success for a particular Boost.Thread once_flag boundary
is a bounded sample, not proof for these classes or all POD interfaces.

### 2.3 Two additional conditions that must hold

The following cover exceptions and lifetimes; they are not an exhaustive safety checklist.

**Condition one: exceptions must not escape the boundary**

The example only illustrates converting ordinary C++ errors into error codes. A non-null out must
point to a valid writable int, and cancellation must not occur during this call interval. A null
pointer returns -3; other error returns leave out unchanged. Where cancellation is allowed, catch-all
must not swallow forced unwind: it must be recognized and propagated using the matching runtime,
with the full call chain's cleanup and non-throwing boundaries checked separately. This simplified
example is not a safe implementation for that cancellation case or a proposal to disable platform cancellation.

```cpp
// Not safe — parameters and return are int, but exceptions still cross
int engine_compute(engine_handle h) {
    auto result = internal_cpp_function();   // may throw
    return result;
}

// Ordinary-error illustration under the stated conditions; an alternative to the preceding interface
int engine_compute(engine_handle h, int* out) {
    if (out == nullptr) return -3;
    try {
        *out = internal_cpp_function();
        return 0;
    } catch (const std::exception&) {
        return -1;
    } catch (...) {
        return -2;
    }
}
```

**Measured boundary**: ordinary typed catch did not match in the particular runtime combination
described in 3.2. This does not mean all RTTI type information was lost. Termination is triggered
when an exception actually crosses a non-throwing boundary, not merely because some function
elsewhere in the call chain is declared `noexcept`.

**Condition two: allocation, release and lifetime contracts must match**

```cpp
// Not safe — mismatched allocation/release APIs, even within one library
char* engine_get_name(engine_handle h);   // internally new[]
// application side: free(name);          // does not call operator delete; invalid for a new[] result

// Explicit pairing; exception, cancellation and pointer-validity contracts still apply
char* engine_get_name(engine_handle h);
void  engine_free_string(char* s);        // freed by the provider

// Or let the caller supply the buffer
int   engine_get_name(engine_handle h, char* buf, size_t buflen);
```

**Why**: `new[]` must pair with the corresponding `delete[]`, not `free`. Cross-component release
also depends on the actual allocator/replacement new-delete, alignment, destructor, object layout
and module lifetime contracts, not library names alone. GNU and LLVM defaults can both use malloc/free;
loading both libraries does not imply two independent heaps. Provider-paired release or caller-supplied
buffers avoid guessing internal contracts, but do not by themselves validate exceptions, cancellation or pointers.

---

## 3. Interfaces that cannot be supported

### 3.1 Passing standard library objects

```cpp
// Cannot cross the boundary
std::string  engine_get_name();
void         engine_set_items(std::vector<int>);
std::map<int, std::string> engine_get_config();
```

**Why**: standard-library types cannot be assumed to have interchangeable representations.
Under the fixed x86_64 probe configuration, 10 of 33 listed concrete type instances differed in
sizeof or alignof; 23 matched on both measures. Member offsets, nodes, control blocks and operational
equivalence have not been fully verified. The table gives sizes in bytes:

| Type | libstdc++ | libc++ |
|---|---:|---:|
| `std::string` | 32 bytes | 24 bytes |
| `std::deque<int>` | 80 bytes | 48 bytes |
| `std::map<int,int>` | 48 bytes | 24 bytes |
| `std::set<int>` | 48 bytes | 24 bytes |
| `std::unordered_map<int,int>` | 56 bytes | 40 bytes |
| `std::unordered_set<int>` | 56 bytes | 40 bytes |
| `std::function<void()>` | 32 bytes | 48 bytes |
| `std::any` | 16 bytes | 32 bytes |
| `std::future<int>` | 16 bytes | 8 bytes |
| `std::promise<int>` | 24 bytes | 8 bytes |

Alignment of `std::function<void()>` also differed: GNU/LLVM was 8/16 bytes.
The GNU probe used host GCC 13.3.0; the LLVM probe used Clang 21.1.1 with cached platform libc++/libc++abi.
The libc++ header version macro was 220108, meaning 22.1.8, not the Clang compiler version.
Platform GCC 14.2 sources and the baseline export manifest are separate evidence inputs, not this probe's compiler.

**This instance linked and loaded but read an incorrect value.** Core fragments of the measured case:

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
    // The original program prints both sizeof values and length views here.
    destroy_payload(payload);       // deleted by the provider
    return producer_view == consumer_view ? 0 : 42;
}
```

**Result**: provider/consumer sizeof was 80/48; length views were 4/106884723786536.
There was no signal crash, but the consumer deliberately returned 42 to mark the mismatch, after
calling the provider's destroy_payload. The incorrect length is one recorded output, not a value
guaranteed on every run. Printing and helper definitions are omitted above; original sources and output are linked in section 7.

**Why this instance links**: the ordinary non-template function make_payload's return type is not
directly encoded in the Itanium name `_Z12make_payloadv`. `_Z9make_datav` correctly encodes make_data,
but is not the original experiment's symbol. Template functions and function types follow different
rules; ABI tags can also change names. The experiment's earlier std::string variant failed to link because of an abi:cxx11 tag.

### 3.2 Throwing across the boundary

```cpp
// Original shared type; throwing and receiving code are in separate translation units.
struct BoundaryError { int code; };
// Provider operation: throw BoundaryError{77};
// Consumer has catch (const BoundaryError&) followed by catch (...).
```

**Measured**: the tested libstdc++ plugin threw BoundaryError{77} to a libc++ consumer.
On native x86_64 and physical armv7l, the same-type catch was not entered; catch-all was entered
and the process exited 12. The relevant libc++abi personality (the function selecting handlers
during unwinding) treats another runtime's exception as foreign, which bypasses ordinary typed
matching on this path. Results depend on the actual exception runtimes, personality and symbol
binding; they do not establish failure for every DSO, direction or type. The derived EngineError type was not tested.

### 3.3 Transferring ownership

```cpp
// Cannot cross the boundary
std::unique_ptr<Widget> engine_create_widget();     // who frees it?
std::shared_ptr<Config> engine_get_config();        // where is the control block?
```

**Why**: direct cross-standard-library smart-pointer transfer is outside the current guarantee.
Risks include pointer/control-block layouts, reference-count protocols, destructors/deleters and
module lifetimes; matching outer size cannot exclude them. Crossing a boundary does not automatically
create two independent shared_ptr counts. A unique_ptr can have a custom deleter, and the release
path cannot be inferred solely from which side allocated the object.

### 3.4 Passing iterators

```cpp
// Cannot cross the boundary
std::vector<int>::iterator engine_begin();
```

**Why**: an iterator points into the container's internal structure, and the
two sides understand that structure differently.

---

## 4. Why a conversion layer is not the answer

One proposal is to convert libstdc++ objects into libc++ objects at the boundary. Explicit protocols
can be designed, but no general conversion layer was implemented or measured in this round.

**Where would the conversion function live?**

Normally each side is compiled with its own headers and ABI, exchanging explicit data representations
or handles. This does not mean the final program can link only one standard library. The two std
representations must not simply be interpreted as the same layout. For example:

```
libstdc++ side:  std::string  →  { const char* data; size_t len; }
                                        ↓ across the boundary (plain C)
libc++ side:     { const char* data; size_t len; }  →  std::string
```

**That middle layer is a C interface.**

**Three behaviors need explicit protocols rather than direct ABI reinterpretation**:

- **Ordinary exceptions** can be caught and encoded at the provider; cancellation must not be swallowed as an ordinary error;
- **Iteration** can use handles or batches without interpreting the other library's iterators;
- **Ownership** can specify paired release and module lifetimes.

**Conclusion**: this is not transparent conversion of arbitrary C++ objects. Copying, lifetime and
maintenance costs depend on the interface; not every call must copy. Not adopting a general bridge
is a project choice, not proof that no technical design space exists.

**Industry reference**: CEF isolates runtimes through a C API and supplies a C++ wrapper. ICU's
cross-version binary compatibility has conditions including stable C APIs. These are conditional
design references, not proof that arbitrary C boundaries are safe.
[CEF maintainer explanation](https://github.com/chromiumembedded/cef/issues/3836),
[ICU official conditions](https://unicode-org.github.io/icu/userguide/icu/design.html#icu-binary-compatibility).

---

## 5. How to judge whether an interface can be supported

**Screening heuristic: can this interface be expressed completely in plain C? This is not sufficient proof of safety.**

| Expressible through explicit protocols | Not directly ABI-interchangeable; needs a separate protocol |
|---|---|
| Built-in types, PODs, opaque pointers | Classes, templates, standard containers |
| Error codes | Exceptions |
| Caller-supplied buffers | Returning dynamically allocated objects |
| C function-pointer callbacks | `std::function` callbacks |

**`extern "C"` is not the test.** A function returning `std::string` can be
declared `extern "C"` — the symbol name becomes clean, but the object is
still wrong.

---

## 6. What we need from the business divisions

To assess third-party application compatibility item by item, we need:

1. **The inventory of interfaces between third-party applications and the
   platform** — which are C interfaces, which pass C++ types;
2. **Whether applications carry their own standard library** or rely on the
   platform's;
3. For interfaces that do pass C++ types, **a priority ordering** — so we
   can estimate the work of converting them to C interfaces.

**Work planning, not a completion record**: these checks remain unfinished and timing requires
separate confirmation. The platform team can inspect available platform sources; external providers
must supply interface, build, lifetime and exception/cancellation contracts. The requested ordering
and concrete work assignments remain for human confirmation.

---

## 7. Evidence scope of this document

- Size/alignment of 33 types and the minimal deque case: x86_64 only. ICU's single C entry: physical armv7l only.
- Controlled C ABI and BoundaryError: native x86_64 and physical armv7l records.
- Boost also has representative edges on native x86_64, armv7l QEMU and aarch64 QEMU. The aarch64 round added 22 edges; cumulative representative-group coverage is 49/90, not every edge or real business call.
- Complete aarch64 cancellation/current official-suite validation remains unfinished; third-party interface materials and the external denominator are unavailable.
- Arbitrary old callers, extra noexcept/no-exception frames, arbitrary templates/callbacks, asynchronous cancellation, custom allocators, arbitrary plugin-loading combinations and full member layouts are not covered.

### Old/new binding and header limitations

Compilation determines cleanup code; linking determines symbol versions. Old ELF below means a binary
that already records the old wait node:

| Case | Wait entry | Measured meaning |
|---|---|---|
| Already-linked old ELF | LLVM_22 | Retains the old termination contract |
| Old .o/.a, relinked to the dual-version library without recompilation | LLVM_22_TIZEN_1 (new default) | Depends on old call-site and cleanup-table layout; a lock remained held after thread join in a measured case, so clean exit is not promised |
| Recompile with new headers and link the dual-version library | LLVM_22_TIZEN_1 | Cleanup, unlock and subsequent progress passed under the tested conditions |
| New-header objects first linked to the old single-version library, then runtime upgraded | LLVM_22 | Still uses the recorded old node |

Dual versions cover only the ordinary wait and system-clock timed-wait library entries. Steady-clock
and custom-Clock waits and wbuffer_convert destruction are compiled into consumers; timed-writer
rollback is also in a header. Replacing the DSO cannot repair these old copies; affected translation units require recompilation.
libc++ retains ordinary-error checks, and uncaught ordinary errors can still terminate. wbuffer_convert
aligns on no automatic destructor flush, not identical output: new/old/GNU produced 3/9/0 bytes.
Buffer algorithms were unchanged; the old-to-new 9-to-3 change results from removing the destructor flush.

Sources: [size/alignment and deque originals](https://github.com/lhmax2010/libc-_replacement/tree/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/R80),
[ICU and exception originals](https://github.com/lhmax2010/libc-_replacement/tree/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/R78),
[representative Boost edges](https://github.com/lhmax2010/libc-_replacement/blob/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/R110/W3_REPORT.md),
[support statement awaiting review and detailed records](https://github.com/lhmax2010/libc-_replacement/blob/de371a6b6e80662ff349f6cf3f313ee916ccb85a/docs/progress/IMPL_0909/W3/SUPPORT_SCOPE_EN.md).
