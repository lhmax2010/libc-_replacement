# Interface Compatibility Between libstdc++ and libc++

- Date: 2026-09-09
- Context: interfaces between the platform (migrating to libc++) and
  components that remain on libstdc++ — third-party applications,
  prebuilt libraries

---

## 1. Summary

**Compatibility does not depend on which standard library is used. It
depends on what crosses the interface boundary.**

| Interface form | Supportable | Basis |
|---|---|---|
| Plain C interface | **Yes** | Measured |
| C++ interface passing only built-in types and PODs | **Yes** | See section 2.2 |
| C++ interface passing standard library objects | **No** | Measured, fails silently |
| C++ interface throwing across the boundary | **No** | Measured, type identity lost |
| C++ interface transferring ownership | **No** | Allocators differ |

Each case is explained with code below.

---

## 2. Interfaces that can be supported

### 2.1 Plain C interfaces

```c
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

**Why this is safe**: only `int`, `char*`, `size_t` and an opaque pointer
cross the boundary. Their layout is fixed by the platform C ABI, not by the
standard library.

**Measured**: a real ICU library built against libstdc++ was called through
its C interface from a program built against libc++; both sides behaved
correctly.

### 2.2 C++ interfaces passing only built-in types and PODs

```cpp
class Engine {
public:
    int    width() const;                    // returns int
    void   setPosition(int x, int y);        // takes ints
    bool   isReady() const;                  // returns bool
    double scale() const;                    // returns double
};

struct Rect { int x, y, w, h; };             // POD, layout fixed by C ABI
Rect   getBounds();                          // returned by value — safe
```

**Why this is safe**: although these are C++ declarations, everything
crossing the boundary is a built-in type or a POD. Their layout is
determined by the platform ABI rather than by the standard library.

**The presence of a class is not the problem — the form of the data that
crosses the boundary is.**

### 2.3 Two additional conditions that must hold

Even with the right interface form, the following two must be satisfied.

**Condition one: exceptions must not escape the boundary**

```cpp
// Not safe — parameters and return are int, but exceptions still cross
int engine_compute(engine_handle h) {
    auto result = internal_cpp_function();   // may throw
    return result;
}

// Safe — contain the exception, return an error code
int engine_compute(engine_handle h, int* out) {
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

**Why**: when an exception crosses between the two standard libraries, the
receiving side cannot catch it by type — the type identities are
independent between the two runtimes — and it falls through to
`catch (...)`. If the receiving side's call chain contains a function marked
as non-throwing, **the process terminates**.

**Condition two: allocation and deallocation must be on the same side**

```cpp
// Not safe — allocated on one side, freed on the other
char* engine_get_name(engine_handle h);   // internally new[]
// application side: free(name);          // different operator delete

// Safe — provide a matching release function
char* engine_get_name(engine_handle h);
void  engine_free_string(char* s);        // freed by the provider

// Or let the caller supply the buffer
int   engine_get_name(engine_handle h, char* buf, size_t buflen);
```

**Why**: each standard library has its own `operator new/delete` and
internal heap state. Freeing across the boundary corrupts it.

---

## 3. Interfaces that cannot be supported

### 3.1 Passing standard library objects

```cpp
// Cannot cross the boundary
std::string  engine_get_name();
void         engine_set_items(std::vector<int>);
std::map<int, std::string> engine_get_config();
```

**Why**: the same type has a different memory layout in the two libraries.
We compared 33 commonly used types; **10 differ**:

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

**Neither compilation nor linking reports anything.** A reproducible case:

```cpp
// Provider, built against libstdc++
std::deque<int>* make_data() {
    auto* d = new std::deque<int>{1, 2, 3, 4};
    return d;                        // reports a length of 4
}

// Consumer, built against libc++
auto* d = make_data();
printf("%zu\n", d->size());          // reads 106884723786536
```

**Result**: links, loads, **the process exits normally without crashing** —
but the data is wrong.

**Why the linker cannot detect this**: C++ name mangling **does not encode
the return type into the symbol name**. Both sides mangle `make_data` to
`_Z9make_datav`, an exact match.

### 3.2 Throwing across the boundary

```cpp
// Cannot cross the boundary
class EngineError : public std::runtime_error { ... };

void engine_process();               // throws EngineError internally

// Application side
try {
    engine_process();
} catch (const EngineError& e) {     // will not catch — type identity differs
    ...
} catch (...) {                      // only this is reached
    ...
}
```

**Measured**: typed exceptions crossing the boundary cannot be caught by
type and are only caught by `catch (...)`.

### 3.3 Transferring ownership

```cpp
// Cannot cross the boundary
std::unique_ptr<Widget> engine_create_widget();     // who frees it?
std::shared_ptr<Config> engine_get_config();        // where is the control block?
```

**Why**: releasing a smart pointer calls the `operator delete` of the side
that constructed it; a `shared_ptr`'s reference-count control block also
remains on the original side, so the two sides count independently.

### 3.4 Passing iterators

```cpp
// Cannot cross the boundary
std::vector<int>::iterator engine_begin();
```

**Why**: an iterator points into the container's internal structure, and the
two sides understand that structure differently.

---

## 4. Why a conversion layer is not the answer

One proposal is to convert libstdc++ objects into libc++ objects at the
boundary. This is technically possible, but it degenerates into a C
interface and leaves gaps that cannot be covered.

**Where would the conversion function live?**

A translation unit can only link one standard library, so the conversion has
to be split across both sides, with something that contains no C++ types in
between:

```
libstdc++ side:  std::string  →  { const char* data; size_t len; }
                                        ↓ across the boundary (plain C)
libc++ side:     { const char* data; size_t len; }  →  std::string
```

**That middle layer is a C interface.**

**And three things cannot be wrapped**:

- **Exceptions** — there is no point at which to convert while an exception
  is propagating;
- **Iterators** — they point into internal structures; converted, they are
  dangling;
- **Ownership** — after conversion, who frees it remains unanswered.

**Conclusion**: the limit form of a conversion layer is a C interface, but
with a copy on every call, a pair of conversion functions per type to
maintain, and — worse — it hides the problem: it looks like C++ while having
entirely different performance characteristics.

**Industry reference**: Chromium's embedding framework CEF provides both a
C++ and a C API, **and the reason for the C API is precisely that the C++
ABI is not compatible across compilers**. ICU follows the same design.

---

## 5. How to judge whether an interface can be supported

**The test: can this interface be expressed completely in plain C?**

| Yes | No |
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

**What we will do on the platform side**: audit the existing C interfaces
against the two conditions in section 2.3 (are exceptions contained; are
allocation and release on the same side). Only we can check this.

---

## 7. Evidence scope of this document

- Layout comparison: 33 types, x86_64;
- Cross-boundary measurements: real components (ICU, Boost), x86_64 and
  armv7l;
- Exceptions across the boundary: measured on both architectures;
- **Not covered**: run-time validation on aarch64; the actual interfaces
  used by third-party applications (materials not available to us).
