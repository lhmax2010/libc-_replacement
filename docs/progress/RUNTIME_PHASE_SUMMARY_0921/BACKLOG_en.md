# Paused items: individual restart conditions

Primary C: aliases, return types, derived/containing types or similar identity issues prevent a literal-name proof. Primary D: missing artifacts, call paths or configuration. Causes can overlap. Original classifications are retained, not re-investigated. PARTLY does not promise closure; YES_LOCAL_RESEARCH means local investigation can resume.

| Candidate | Primary cause | Missing evidence / restart condition | Locally resolvable | Evidence |
| --- | --- | --- | --- | --- |
| std::exception | C | Identify the actual provider throw, consumer call/catch and runtime binding for SeedGenException / Json::Exception; RTTI overlap alone is insufficient. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::ifstream | D | Match the CUpti outer structure containing ifstream* fileHandler to its build configuration, constructor and actual external consumer. | NOT_OBSERVED | [Evidence: pending](EVIDENCE.md#pending) |
| std::mutex | D | Obtain the active CEREAL_THREAD_SAFE setting and the ownership of generated cereal LockGuard / containing-object instances. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::ostringstream | D | Match Node/V8 versions and artifacts, then identify consumers of the out_stream entry returning ostringstream*. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::lock_guard | D | Trace the returned MutexedBinaryQueue::LockedBinaryQueue object across packages and its m_guard storage. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::runtime_error | C | Trace actual cereal/nlohmann runtime_error throws and consumers; distinguish header-local instantiations from provider code. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::unique_lock | D | Locate cereal unique_lock storage / Eigen aliases under actual macros and establish ownership of the containing instances. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::invalid_argument | C | Locate a non-inline Eigen/nntrainer/libtbb exception-producing provider and actual consumer under the active exception configuration. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::thread | D | Trace actual cross-package instances of Eigen EnvThread::thr_ or klay AuditTrail::dispatcher; thread::id is not thread. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::ofstream | D | Determine whether the ostream returned by cuDNN getStream actually refers to ofstream, and close the remaining public stream paths. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::istringstream | D | Trace XMLInputArchive / StringToFst local istringstream objects into istream interfaces and identify package ownership of instantiations. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::bad_alloc | C | Identify a real Thrust/libtbb provider allocation-failure path and its external caller; runtime bad_alloc support symbols do not establish propagation. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::condition_variable | D | Identify the actual consumer of the AurumXML condition_variable* constructor, its version/macros and object lifetime. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::chrono::milliseconds | C | Query FromChrono / ToChronoMilliseconds by function identity, resolving the exact Rep and ratio&lt;1,1000&gt;; return types may not be mangled. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::queue | D | Locate provider/consumer ownership of cereal queue serialization instances or OpenFST containing objects. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::optional | D | Resolve active absl optional branches, actual std versus replacement type, and consumers of AbslParseFlag / cereal instances. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::out_of_range | C | Match nlohmann/Eigen out_of_range throws to a non-inline provider, actual external caller and exception configuration. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::chrono::seconds | C | Identify consumers of FromChrono / ToChronoSeconds for the exact ratio&lt;1&gt; specialization; other duration edges cannot substitute. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::regex | C | Resolve nntrainer getRegex's exact return signature, inline status, build version and actual symbol consumer. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::istreambuf_iterator | D | Identify package ownership of concrete cereal/rapidxml/picojson/Boost istreambuf_iterator instances and any actual external reference. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::enable_shared_from_this | D | Establish the actual gRPC Channel or Node SiblingGroup object boundary, version and enable_shared_from_this / weak_this storage chain. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::chrono::microseconds | C | Match FromChrono / ToChronoMicroseconds to ratio&lt;1,1000000&gt;, its Rep and an actual external consumer. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::future | D | Establish the external consumer of BehaviorTree AsyncActionNode containing future&lt;NodeStatus&gt;; exclude inactive OpenCV declarations. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::recursive_mutex | D | Resolve real external construction of iotivity wrappers taking weak_ptr&lt;recursive_mutex&gt;, distinguishing the wrapper from the referenced mutex. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::variant | D | Establish an actual ICU Operand object boundary or cereal variant instantiation under the precise provider version. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::weak_ptr | D | Locate external references to iotivity weak_ptr&lt;recursive_mutex&gt; constructors or actual cereal weak_ptr instances; control-block symbols are insufficient. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::logic_error | C | Match actual Thrust/Open3D/Vulkan logic_error-derived throws to business call/catch paths and released artifacts. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::mt19937 | D | Identify the built TensorFlow slice_tester mt19937& instance and its consumer; distinguish test-internal calls. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::reverse_iterator | C | Resolve actual public functions returning reverse_iterator, underlying iterator type, inline instantiation and external consumer. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::uniform_int_distribution | D | Identify actual external consumers of the TensorFlow local_device_state object containing uniform_int_distribution. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::random_device | D | Identify the TensorFlow containing-object boundary for random_device; an integer produced in a default argument is not that object. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::bitset | D | Find a consumer of TensorDim(bitset&lt;MAXDIM&gt;...) outside the nntrainer source package, or actual cereal instances. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::once_flag | D | Trace oneDNN cache_blob_id instances containing once_flag across packages; static fields are not transferred with each instance. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::fstream | C | Match UDT/SRT sendfile/recvfile function identities, fstream reference signatures, actual exports and external references. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::reference_wrapper | D | Establish actual template-instantiation ownership and external consumers of AnyInvocable assignment / sigc bound_argument using reference_wrapper. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::atomic_bool | D | Locate the actual BehaviorTree/grpc outer-object boundary containing atomic&lt;bool&gt;; atomic&lt;unsigned int&gt; is not a substitute. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::unordered_multimap | D | Resolve OpenFST Matcher/StateTable or Xbyak macro-generated objects containing unordered_multimap under actual build macros. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::chrono::system_clock::time_point | D | Determine whether stt-engine Timepoint2Timespec binds its bundled grpc 1.27 or platform grpc 1.76; deployment/loading evidence may be required. | NOT_OBSERVED | [Evidence: pending](EVIDENCE.md#pending) |
| std::error_code | D | Resolve LLVM StringError(error_code) binding to platform LLVM 22 versus Rust's bundled LLVM 20 using dependencies, SONAMEs and include provenance. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::system_error | C | Trace Vulkan/BehaviorTree/Boost system_error-derived exceptions to the real external consumer and exception-runtime configuration. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::thread::id | D | Identify the actual outer-object boundary containing iotivity AtomicWrapper&lt;thread::id&gt; or Eigen created_by_thread_id_; not a thread-object boundary. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::priority_queue | D | Establish provider/consumer ownership of TFRT TimerQueue containing priority_queue or concrete cereal helper instances. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::promise | D | Obtain matching aarch64 artifacts/consumer evidence for Open3D PeerConnectionManager::Create's promise reference; x86 negative evidence does not apply. | NOT_OBSERVED | [Evidence: pending](EVIDENCE.md#pending) |
| std::uniform_real_distribution | D | Identify actual external consumers of Open3D Random instances containing uniform_real_distribution. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::multiset | D | Resolve actual cereal/zypp/glog multiset template or macro instances and their producing/consuming source packages. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::ostream_iterator | D | Determine whether RapidXML/picojson ostream_iterator temporaries enter a template instance referenced from another package. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::forward_list | D | Establish OpenFST forward_list-containing state boundaries or actual pybind11 shared-internals registration/callback paths across modules. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::default_random_engine | C | Expand default_random_engine to the actual engine and trace Scheduler's rand_engine_ via public object/function identity. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::cv_status | C | Identify consumers/instances of tsl::internal::wait_until_system_clock and relevant wait_for functions returning cv_status. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::istream_iterator | D | Identify whether Boost greg_facet's inline operator&gt;&gt; passes an istream_iterator into an instance provided by another package. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::length_error | C | Connect an actual gmpxx/OpenUSD length_error-producing entry to its actual caller; separate throw/catch snippets cannot be joined as evidence. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::exception_ptr | C | Obtain actual pybind11 ExceptionTranslator registration/call targets, exception_ptr arguments and shared-internals module relationships. | NOT_OBSERVED | [Evidence: pending](EVIDENCE.md#pending) |
| std::atomic_int | D | Identify the actual OpenUSD refBase consumer and atomic_int& object/version; the unsigned-int atomic edge does not apply. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::u16string | C | Find consumers of ICU iterator dereference or Node ToU16String returning u16string; a string_view&lt;char16_t&gt; edge is different. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::any | D | Establish actual OpenUSD TfDiagnosticInfo(any) or TensorFlow CustomCallBinding(vector&lt;any&gt;) object boundaries and exact instances. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::atomic_flag | D | Match the actual TensorFlow Interpreter / OpenUSD Dispatcher / dmlc Spinlock containing atomic_flag to external consumers and build macros. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::basic_ostringstream | C | Trace TensorFlow LogMessage construction/public object identity and basic_ostringstream&lt;char&gt; base storage across packages. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::domain_error | C | Identify actual Dali ProxyAccessible::GetChildAtIndex virtual-call target, caller and domain_error propagation; a source throw alone is insufficient. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::streampos | C | Trace actual public stream-position-returning functions and fpos&lt;state&gt; instances, accounting for inline code and aliases. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::back_insert_iterator | D | Establish provider/consumer ownership of concrete fmt get_container(back_insert_iterator) or similar instances and their containers. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |
| std::byte | C | Identify MLIR AsyncRuntime C exports using ValueStorage and actual byte* consumers; C symbol names do not encode the type. | YES_LOCAL_RESEARCH | [Evidence: pending](EVIDENCE.md#pending) |
| std::overflow_error | C | Connect an actual OpenUSD overflow_error-producing entry to its real external caller and exception configuration. | PARTLY | [Evidence: pending](EVIDENCE.md#pending) |


## Version 2: four separately tracked enum findings

The original 62 entries and classifications are unchanged. The entries below track type findings, not four new package edges.

### Stream-state enums: type identities still diverge on armv7l

**Fact chain — new measurements added in version 2:** GNU fmtflags, iostate and openmode are the named enums `_Ios_Fmtflags`, `_Ios_Iostate` and `_Ios_Openmode`; libc++ uses `unsigned int` for all three. GNU seekdir is `_Ios_Seekdir`, whereas libc++ uses `ios_base::seekdir`. Both armv7l and aarch64 show 4/4 divergences, with five consistent repetitions per library. All four have size/alignment 4/4 on both sides. The earlier target-type probe/configurations and QEMU user mode were reused; no board was used. [Evidence: TYPES.tsv](../R119_ENUM_RETEST/TYPES.tsv) — SHA256 `f1c2e9551a6a379141ab91b462612ae5799dee1dc077e2044e5fd1816705a377`

| Type | GNU encoding | libc++ encoding (same on both architectures) |
| --- | --- | --- |
| fmtflags | `St13_Ios_Fmtflags` | `j` |
| iostate | `St12_Ios_Iostate` | `j` |
| openmode | `St13_Ios_Openmode` | `j` |
| seekdir | `St12_Ios_Seekdir` | `NSt3__18ios_base7seekdirE` |

**Unlike the integer aliases:** the earlier 13 expressions selected the same integer type on the measured armv7l configuration. These four enum/builtin or enum-identity differences remain despite equal widths. They are separately recorded type findings, not four demonstrated product failures.

Compiling the real ARM podofo header produced GNU `_ZN6PoDoFo14PdfInputDevice4SeekExSt12_Ios_Seekdir` and libc++ `_ZN6PoDoFo14PdfInputDevice4SeekExNSt3__18ios_base7seekdirE`. The streamoff encoding is `x` on both sides; seekdir accounts for the remaining difference. A qualified call exposes the UND name. Both ordinary virtual-call IR bodies instead select zero-based index 6 relative to the vtable address point and call indirectly. This observes caller compilation, not provider-vtable or mixed-execution compatibility. [Evidence: SEEK_SYMBOLS.json](../R119_ENUM_RETEST/SEEK_SYMBOLS.json) — SHA256 `32ccb97b8ce8012af090b855466c334cc2eae73aae8c0cacf289cc9e1ef7f0d5` [Evidence: VIRTUAL_CALL.json](../R119_ENUM_RETEST/VIRTUAL_CALL.json) — SHA256 `f928bf2ead58d70a78ad55060a48aed7a3d374172e6c4414b740901001d97e41`

**Excluded explanations:** this is not a 4/8-byte width difference. Different function names do not directly establish different virtual-table slots or inevitable virtual-call failure. Function-identity queries covered the two ChecksumStream::flags overloads, open, PdfInputDevice::Seek and Clear: 5 confirmed declarations under 4 identities. No corresponding external UND was found in the scoped x86_64 set; the bundle::Add positive control passed. The old index's 61/85/87/12 positions are not 245 confirmed public declarations. Virtual calls, inline code and out-of-scope artifacts remain open. Existing fmtflags/openmode stream-member edges are retained. New edges: 0; the total remains 18 pairs / 23 edges. [Evidence: BOUNDARY_QUERIES.json](../R119_ENUM_RETEST/BOUNDARY_QUERIES.json) — SHA256 `ffc287e4232035231ee82c88a28f0aaeb57621d5871b5b9a4021f4096d9fcf06` [Evidence: BOUNDARY_CONTROL.json](../R119_ENUM_RETEST/BOUNDARY_CONTROL.json) — SHA256 `0a8d08bb6b8dddb528f55d176d7126f5c0af127a937ff4efc92c2deb67355092`

**What would overturn the finding:** equal complete type encodings under the actual target configuration would require revising the divergence claim. Product consequences require real cross-package calls, including virtual calls, actual providers, parameter semantics and object-lifetime evidence. Symbol/slot observations do not substitute for those execution tests.
