# 逐边证据附表

这是既有静态证据的转录，不是本轮运行结果。序号与主表一致；消费方 UND 为未定义引用，提供方 DEF 为定义。每个证据索引条目提供完整 SHA256。

[证据: edges](EVIDENCE.md#edges)

[证据: arm-elf](EVIDENCE.md#arm-elf)

<a id="edge-1"></a>

## 1. appcore-agent → app-core: AppCoreBase::AddEvent

跨包对象/成员传递

值参数 shared_ptr<EventBase>; 共享所有权；是否留存未验证

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libappcore-agent.so.1.7.7 | 1688e929d9a16c516a52ddf8554c15e3daa827d101ed52daf6d5d456ea7f4d79 |
| provider | /usr/lib64/libapp-core-cpp.so.1.27.13 | 629520688916c61c07a9cb2c70a2b59fbbb406cd63b533396c31aa6197694b75 |


```text
    33: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN9tizen_cpp11AppCoreBase8AddEventESt10shared_ptrINS0_9EventBaseEE
   222: 000000000000ef4e   137 FUNC    GLOBAL DEFAULT   11 _ZN9tizen_cpp11AppCoreBase8AddEventESt10shared_ptrINS0_9EventBaseEE
```

```text
tizen_cpp::AppCoreBase::AddEvent(std::shared_ptr<tizen_cpp::AppCoreBase::EventBase>)
```

[引用/定义原记录](../R119_batch1/W1/positive/app_shared.json) — SHA256 `ea2a17c42a1be43eb20da612c0bf845817227c5b3e5f8c11e788446eb0e63f04`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-2"></a>

## 2. amd → bundle: Bundle::Add

跨包对象/成员传递

const 引用：string 与 vector<string>; 借用；不假定异侧释放

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libamd.so.1.81.6 | 1c2720d35947e8441d2edc9968ac58f232286dcbfff4a0993dd2141715e929be |
| provider | /usr/lib64/libbundle.so.0.18.15 | cb8bafe360bac445ec4304fb31771c29d4f74df1979112fa8618c7577cb2faac |


```text
    10: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN10tizen_base6Bundle3AddERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKSt6vectorIS6_SaIS6_EE
   184: 0000000000012a9a   612 FUNC    GLOBAL DEFAULT   11 _ZN10tizen_base6Bundle3AddERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKSt6vectorIS6_SaIS6_EE
```

```text
tizen_base::Bundle::Add(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&)
```

[引用/定义原记录](../R119_batch1/W1/positive/bundle_add.json) — SHA256 `c256fdb6dde8b344e03f27f82f41f22744c1de02db4fe656cd0028bcb270ae9d`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-3"></a>

## 3. amd → bundle: Bundle::Bundle(initializer_list)

跨包对象/成员传递

值视图；pair<string,string> 元素; 传视图不等于移交底层元素

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libamd.so.1.81.6 | 1c2720d35947e8441d2edc9968ac58f232286dcbfff4a0993dd2141715e929be |
| provider | /usr/lib64/libbundle.so.0.18.15 | cb8bafe360bac445ec4304fb31771c29d4f74df1979112fa8618c7577cb2faac |


```text
   253: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN10tizen_base6BundleC1ESt16initializer_listISt4pairINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES8_EE
   165: 0000000000011626   328 FUNC    GLOBAL DEFAULT   11 _ZN10tizen_base6BundleC1ESt16initializer_listISt4pairINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES8_EE
```

```text
tizen_base::Bundle::Bundle(std::initializer_list<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >)
```

[引用/定义原记录](../R119_batch1/W1/positive/bundle_pair.json) — SHA256 `16efd81c549f5102f0aa5e91d1b9a3a9f6a12f7fe1f01f2c06ef3f444d9ae95c`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-4"></a>

## 4. ise-engine-anthy → isf: scim::utf8_wcstombs

跨包对象/成员传递

wstring const 引用; 借用输入；返回对象按真实声明处理

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/scim-1.0/1.4.0/IMEngine/ise-engine-anthy.so | 3190174c6667ee0d6e9098efa58e0eaf70e9774e16b5cff3f8d10ca50d4efb33 |
| provider | /usr/lib64/libscim_imengine.so.8.2.3 | 46a3189b6945f3f1fe7b52cd0a0f6abe645c52d20da4bd2b9afa3fea7527205c |


```text
   118: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4scim13utf8_wcstombsERKNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEEE
   239: 0000000000045250   171 FUNC    GLOBAL DEFAULT   11 _ZN4scim13utf8_wcstombsERKNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEEE
```

```text
scim::utf8_wcstombs(std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > const&)
```

[引用/定义原记录](../R119_batch1/W1/positive/scim_wstring.json) — SHA256 `c9fdf7a5d438f916df502d78e3cb4f3b7ce468f0db4ee68d124dc407c2ae3df4`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-5"></a>

## 5. xwalk-extensions-common → jsoncpp: Json::Reader::parse

跨包对象/成员传递

隐式 this 指针；Reader 的 deque/stack 成员；输出引用; 消费方构造 Reader；借用调用

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libxwalk-extensions-common.so.0.0.1 | 4a9e26aa44ba38874262953e301ae0494f527b43b552596ceebcefa60d317ff5 |
| provider | /usr/lib64/libjsoncpp.so.1.9.6 | 7f86707cc2011fd9a822e4537c623716ec45ad980402e44316d6cda98e98efc1 |


```text
     1: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Json6Reader5parseEPKcS2_RNS_5ValueEb
    93: 000000000000f700  1046 FUNC    GLOBAL DEFAULT   11 _ZN4Json6Reader5parseEPKcS2_RNS_5ValueEb
```

```text
Json::Reader::parse(char const*, char const*, Json::Value&, bool)
```

[引用/定义原记录](../R119_batch1/W1/positive/json_reader_full.json) — SHA256 `8b5f9ebea651ea2f869aa460220586b2f54cfe6cee0fc8e3951e87ba28e354ad`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-6"></a>

## 6. grpc → abseil-cpp: SpinLockWait

跨包对象/成员传递

atomic<unsigned int>* 与转换表指针; 借用控制字；无移交声明

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libgpr.so.51.0.0 | 5408093db1707faff02457dcbcc2065d62ba74e561e9c7cfa562b16507afadc4 |
| provider | /usr/lib64/libabsl_spinlock_wait.so.2508.0.0 | 95d93aa5c4eb4e8f1bb97f3a6101749fd95ed9f246723262ff5f191d33e25fd3 |


```text
    89: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4absl12lts_2025081413base_internal12SpinLockWaitEPSt6atomicIjEiPKNS1_22SpinLockWaitTransitionENS1_14SchedulingModeE
     8: 0000000000001191   128 FUNC    GLOBAL DEFAULT   11 _ZN4absl12lts_2025081413base_internal12SpinLockWaitEPSt6atomicIjEiPKNS1_22SpinLockWaitTransitionENS1_14SchedulingModeE
```

```text
absl::lts_20250814::base_internal::SpinLockWait(std::atomic<unsigned int>*, int, absl::lts_20250814::base_internal::SpinLockWaitTransition const*, absl::lts_20250814::base_internal::SchedulingMode)
```

[引用/定义原记录](../R119_batch1/W1/positive/absl_atomic.json) — SHA256 `171cae6c6d14de61bd8feba2a8e7d091bdb0932d8b847f6ce82ae3a432998f9f`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-7"></a>

## 7. com.samsung.dali-demo → dali2-adaptor: ActorAccessible::GetMatches

跨包对象/成员传递

tuple 值参数；array/unordered_map/hash/equal_to 子对象; 值传递；内部资源约定未验证

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/apps/com.samsung.dali-demo/bin/builder.example | 5dddf95c846fe14ba6cd6d3d8262dbd39342606e664e45ce236a57693c492444 |
| provider | /usr/lib64/libdali2-adaptor.so.2.0.0 | 5f3ea41315f268a9c539259e22bcbbf62fc0462b7db2d5cb14674b01a6aba069 |


```text
   422: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali13Accessibility15ActorAccessible10GetMatchesESt5tupleIJSt5arrayIiLm2EEiSt13unordered_mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_St4hashISB_ESt8equal_toISB_ESaISt4pairIKSB_SB_EEEiS3_IiLm4EEiSt6vectorISB_SaISB_EEibEEjm
  3755: 00000000000c7964   192 FUNC    GLOBAL DEFAULT   11 _ZN4Dali13Accessibility15ActorAccessible10GetMatchesESt5tupleIJSt5arrayIiLm2EEiSt13unordered_mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_St4hashISB_ESt8equal_toISB_ESaISt4pairIKSB_SB_EEEiS3_IiLm4EEiSt6vectorISB_SaISB_EEibEEjm
```

```text
Dali::Accessibility::ActorAccessible::GetMatches(std::tuple<std::array<int, 2ul>, int, std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >, int, std::array<int, 4ul>, int, std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >, int, bool>, unsigned int, unsigned long)
```

[引用/定义原记录](../R119_batch1/W1/positive/dali_match.json) — SHA256 `ad96b4b2a241edc2d749d4ade4dd9bfdb93e2e40d74ab679961068caad70eeda`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-8"></a>

## 8. dali2-adaptor → dali2: Scene::AddFrameRenderedCallback

跨包对象/成员传递

unique_ptr<CallbackBase> 值参数; 明确转移独占所有权及删除器

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libdali2-adaptor.so.2.0.0 | 5f3ea41315f268a9c539259e22bcbbf62fc0462b7db2d5cb14674b01a6aba069 |
| provider | /usr/lib64/libdali2-core.so.2.0.0 | e2c707cb8ce7642627ea1c0351d3621383de73283f4f7d3f6a0ee1f2b74066b4 |


```text
   610: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali11Integration5Scene24AddFrameRenderedCallbackESt10unique_ptrINS_12CallbackBaseESt14default_deleteIS3_EEi
  1861: 0000000000140e02   187 FUNC    GLOBAL DEFAULT   11 _ZN4Dali11Integration5Scene24AddFrameRenderedCallbackESt10unique_ptrINS_12CallbackBaseESt14default_deleteIS3_EEi
```

```text
Dali::Integration::Scene::AddFrameRenderedCallback(std::unique_ptr<Dali::CallbackBase, std::default_delete<Dali::CallbackBase> >, int)
```

[引用/定义原记录](../R119_batch1/W1/positive/scene_unique.json) — SHA256 `795d7be46165fc7c76cf9c11d725bd7558d7553f7bd2b57de7693ea965b6de51`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-9"></a>

## 9. enlightenment → gtest: StringStreamToString

跨包对象/成员传递

stringstream 指针；streambuf/openmode 成员; 借用；原消费方为测试产物

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/bin/enlightenment-tests | 7f4bea59aa0668e8f0b17ce8b69f09d3bbc425c452a5a49d660843eda766cda8 |
| provider | /usr/lib64/libgtest.so.1.15.2 | 908118a8a46ce77ccfeeb878aae0a1a2c9c45e105ce02d7538943a774ab62d14 |


```text
   401: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN7testing8internal20StringStreamToStringEPNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE
   298: 0000000000037e0e   287 FUNC    GLOBAL DEFAULT   11 _ZN7testing8internal20StringStreamToStringEPNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE
```

```text
testing::internal::StringStreamToString(std::__cxx11::basic_stringstream<char, std::char_traits<char>, std::allocator<char> >*)
```

[引用/定义原记录](../R119_batch1/W1/positive/gtest_stringstream.json) — SHA256 `0434fd4f5a3169ca660e90f4cde584fe0d882ee5e2a04dc3fdd5076de9349181`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-10"></a>

## 10. app-installers → manifest-parser: DeltaParser::ParseManifest

跨包对象/成员传递

filesystem::path const 引用; 借用路径

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libapp-installers.so.1.37.2 | 99eb06f41b938cae82358f03b11a3a37df589ba82f1c57a743f7ed9592470ad0 |
| provider | /usr/lib64/libdelta-manifest-handlers.so | 625b4846ed8bfdc72f376b6800c2979508d4e3bd0dd7b2e5c4954d3290b9ba5a |


```text
   290: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN5delta11DeltaParser13ParseManifestERKNSt10filesystem7__cxx114pathE
    78: 0000000000004db0   173 FUNC    GLOBAL DEFAULT   11 _ZN5delta11DeltaParser13ParseManifestERKNSt10filesystem7__cxx114pathE
```

```text
delta::DeltaParser::ParseManifest(std::filesystem::__cxx11::path const&)
```

[引用/定义原记录](../R119_batch1/W1/positive/delta_path.json) — SHA256 `f499e12ffeb339c43395e30b692fe5c6aa71b6abebc79e5923b0b90c65c311f4`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-11"></a>

## 11. app-installers → cert-svc: SignatureValidator::checkList

跨包对象/成员传递

list const 引用；SignatureData 输出引用; 借用输入；输出释放约定待核

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libapp-installers.so.1.37.2 | 99eb06f41b938cae82358f03b11a3a37df589ba82f1c57a743f7ed9592470ad0 |
| provider | /usr/lib64/libcert-svc-vcore.so.3.0.0 | e133c4fa6c87c732ea14e916adfd5957fde5e90511e634a0834aa7baefc8f0bc |


```text
   394: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN14ValidationCore18SignatureValidator9checkListEbRKNSt7__cxx114listINS1_12basic_stringIcSt11char_traitsIcESaIcEEESaIS7_EEERNS_13SignatureDataE
   497: 000000000005ac10    83 FUNC    GLOBAL DEFAULT   11 _ZN14ValidationCore18SignatureValidator9checkListEbRKNSt7__cxx114listINS1_12basic_stringIcSt11char_traitsIcESaIcEEESaIS7_EEERNS_13SignatureDataE
```

```text
ValidationCore::SignatureValidator::checkList(bool, std::__cxx11::list<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&, ValidationCore::SignatureData&)
```

[引用/定义原记录](../R119_batch1/W1/positive/cert_list.json) — SHA256 `39df480b2e6e8eea069259d160129a8296f0d4e7a6229458aca389c499b2f581`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-12"></a>

## 12. com.samsung.dali-demo → dali2-toolkit: WebView::RegisterPageLoadStartedCallback

跨包对象/成员传递

function 值参数；回调 string const 引用; 捕获对象所有权取决于 callable

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/apps/com.samsung.dali-demo/bin/web-view.example | 9c763036319f1b5b0190874be3ee76be5cf02cffe9f140ef04492be4fc7a64a7 |
| provider | /usr/lib64/libdali2-toolkit.so.2.0.0 | c38f059de4dae3b86ed40faa64f59bb011f883652dde6515e3964c806cb01d78 |


```text
     5: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali7Toolkit7WebView31RegisterPageLoadStartedCallbackESt8functionIFvRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEE
  2909: 00000000002f9170   288 FUNC    GLOBAL DEFAULT   11 _ZN4Dali7Toolkit7WebView31RegisterPageLoadStartedCallbackESt8functionIFvRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEE
```

```text
Dali::Toolkit::WebView::RegisterPageLoadStartedCallback(std::function<void (std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)>)
```

[引用/定义原记录](../R119_batch1/W1/positive/dali_function.json) — SHA256 `959f3452609ee733424783001ddb2e166c1da98c7990e3b8f69c8ce347a07f93`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-13"></a>

## 13. dali2-toolkit → dali2: CalculateHash

跨包对象/成员传递

string_view const 引用; 借用视图及其存储

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libdali2-toolkit.so.2.0.0 | c38f059de4dae3b86ed40faa64f59bb011f883652dde6515e3964c806cb01d78 |
| provider | /usr/lib64/libdali2-core.so.2.0.0 | e2c707cb8ce7642627ea1c0351d3621383de73283f4f7d3f6a0ee1f2b74066b4 |


```text
   516: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali13CalculateHashERKSt17basic_string_viewIcSt11char_traitsIcEE
  2166: 0000000000172a6c   346 FUNC    GLOBAL DEFAULT   11 _ZN4Dali13CalculateHashERKSt17basic_string_viewIcSt11char_traitsIcEE
```

```text
Dali::CalculateHash(std::basic_string_view<char, std::char_traits<char> > const&)
```

[引用/定义原记录](../R119_batch1/W1/positive/dali_view_full.json) — SHA256 `182e95bb0d9b5cd805e79ecafd05e4c64860121b5fd2d683f4c4e7ff5ff01fcb`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-14"></a>

## 14. dali2-toolkit → dali2: ConditionalWait::WaitUntil

跨包对象/成员传递

time_point 值参数；duration/rep 成员；ScopedLock 引用; 时间值不移交堆资源

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libdali2-toolkit.so.2.0.0 | c38f059de4dae3b86ed40faa64f59bb011f883652dde6515e3964c806cb01d78 |
| provider | /usr/lib64/libdali2-core.so.2.0.0 | e2c707cb8ce7642627ea1c0351d3621383de73283f4f7d3f6a0ee1f2b74066b4 |


```text
  1047: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIlSt5ratioILl1ELl1000000000EEEEEE
   529: 0000000000179946   170 FUNC    GLOBAL DEFAULT   11 _ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIlSt5ratioILl1ELl1000000000EEEEEE
```

```text
Dali::ConditionalWait::WaitUntil(Dali::ConditionalWait::ScopedLock const&, std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long, std::ratio<1l, 1000000000l> > >)
```

[引用/定义原记录](../R119_batch1/W1/positive/dali_time.json) — SHA256 `1b725723094a8c7e0dcfb42c0ce1053a7769664f17fffff92d5b097aac50aba9`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-15"></a>

## 15. dali2-toolkit → dali2: SingletonService::GetSingleton

跨包对象/成员传递

type_info const 引用; 借用编译器产生的类型信息

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libdali2-toolkit.so.2.0.0 | c38f059de4dae3b86ed40faa64f59bb011f883652dde6515e3964c806cb01d78 |
| provider | /usr/lib64/libdali2-core.so.2.0.0 | e2c707cb8ce7642627ea1c0351d3621383de73283f4f7d3f6a0ee1f2b74066b4 |


```text
   882: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZNK4Dali16SingletonService12GetSingletonERKSt9type_info
  1683: 00000000001732ae   146 FUNC    GLOBAL DEFAULT   11 _ZNK4Dali16SingletonService12GetSingletonERKSt9type_info
```

```text
Dali::SingletonService::GetSingleton(std::type_info const&) const
```

[引用/定义原记录](../R119_batch1/W1/positive/dali_typeinfo_full.json) — SHA256 `c0a2af236475747828464feda4d69258e324f979178548a660a63d13a17b0efe`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-16"></a>

## 16. capi-media-vision → inference-engine-interface: GetInputTensorBuffers

跨包对象/成员传递

map 非 const 输出引用；less 成员; 后端填入；tensor buffer 最终释放责任未闭合

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libmv_inference.so | d86098b092dc6c47ad74f0fe093be05c9d52f874b1f20fbe7ac9eae2b2b9e9dd |
| provider | /usr/lib64/libinference-engine-interface-common.so.0.5.2 | c03b8216150d987534d79413114adf8f0427856d43b7cd26d3fc3056de76fc1d |


```text
    12: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN24InferenceEngineInterface6Common21InferenceEngineCommon21GetInputTensorBuffersERSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE31_inference_engine_tensor_bufferSt4lessIS8_ESaISt4pairIKS8_S9_EEE
   118: 000000000000a8ba   181 FUNC    GLOBAL DEFAULT   11 _ZN24InferenceEngineInterface6Common21InferenceEngineCommon21GetInputTensorBuffersERSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE31_inference_engine_tensor_bufferSt4lessIS8_ESaISt4pairIKS8_S9_EEE
```

```text
InferenceEngineInterface::Common::InferenceEngineCommon::GetInputTensorBuffers(std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, _inference_engine_tensor_buffer, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, _inference_engine_tensor_buffer> > >&)
```

[引用/定义原记录](../R119_batch1/W1/positive/inference_map.json) — SHA256 `c34d7b284ec7b42b09d833b9f617eccba786e23e6340898b6aeb5ab5f61df562`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-17"></a>

## 17. app-core → gtest: PrintStringTo

跨包对象/成员传递

string const 引用；ostream 指针及其状态成员; 借用；原消费方为单元测试产物

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/bin/app-core_unittests | ecfc8467ed08e0cd95e68a46b6967adac49a69b99c0209b9774b6714ea9cb5de |
| provider | /usr/lib64/libgtest.so.1.15.2 | 908118a8a46ce77ccfeeb878aae0a1a2c9c45e105ce02d7538943a774ab62d14 |


```text
   170: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN7testing8internal13PrintStringToERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPSo
   341: 0000000000032300   496 FUNC    GLOBAL DEFAULT   11 _ZN7testing8internal13PrintStringToERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPSo
```

```text
testing::internal::PrintStringTo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_ostream<char, std::char_traits<char> >*)
```

[引用/定义原记录](../R119_batch1/W1/positive/gtest_ostream.json) — SHA256 `a8eba8769e853b2819ef81ad1a9f7effee2836fe8f95fbf897583ea9ba95c8a8`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-18"></a>

## 18. data-provider-master → notification: SharedFile::SetPrivateSharing

跨包对象/成员传递

list<shared_ptr<...>> 与 multimap 值参数; 值参数；复制/移动及留存待核

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/share/united-service/esd/mod/libdata-provider-master.so | f266c3230bda990b65851cbddd99bc571f81bf146335d724a842221e69e9ccfe |
| provider | /usr/lib64/libnotification-ex.so.1.0.0 | 55ff369a59e61debc8d936192a40dafefe6bf4bfb4021886733abbeaeadf46cb |


```text
   136: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN12notification4item10SharedFile17SetPrivateSharingENSt7__cxx114listISt10shared_ptrINS0_12AbstractItemEESaIS6_EEESt8multimapINS2_12basic_stringIcSt11char_traitsIcESaIcEEESE_St4lessISE_ESaISt4pairIKSE_SE_EEE
   384: 000000000005faac  2517 FUNC    GLOBAL DEFAULT   11 _ZN12notification4item10SharedFile17SetPrivateSharingENSt7__cxx114listISt10shared_ptrINS0_12AbstractItemEESaIS6_EEESt8multimapINS2_12basic_stringIcSt11char_traitsIcESaIcEEESE_St4lessISE_ESaISt4pairIKSE_SE_EEE
```

```text
notification::item::SharedFile::SetPrivateSharing(std::__cxx11::list<std::shared_ptr<notification::item::AbstractItem>, std::allocator<std::shared_ptr<notification::item::AbstractItem> > >, std::multimap<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >)
```

[引用/定义原记录](../R119_batch1/W1/positive/notification_multimap.json) — SHA256 `faf6f05ea9be9860ebf8e56c6689615fc9cd70a4d1457fe4d0c19408931fc1d6`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-19"></a>

## 19. libzypp-bindings → libzypp: ResPool::setRequestedLocales

跨包对象/成员传递

unordered_set<Locale> const 引用; 借用容器

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/python3.14/site-packages/_zypp.so | f9a768b27d54f3069c5b836b716a80ac40553a8230ebd101dcee3490c72928fb |
| provider | /usr/lib64/libzypp.so.1722.1.7 | 028eb8764e708e1b25d708392b001631679eae76a512583056aeb9bd53473a56 |


```text
   720: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4zypp7ResPool19setRequestedLocalesERKSt13unordered_setINS_6LocaleESt4hashIS2_ESt8equal_toIS2_ESaIS2_EE@ZYPP_plain (2)
  2000: 00000000003178a0    23 FUNC    GLOBAL DEFAULT   12 _ZN4zypp7ResPool19setRequestedLocalesERKSt13unordered_setINS_6LocaleESt4hashIS2_ESt8equal_toIS2_ESaIS2_EE@@ZYPP_plain
```

```text
zypp::ResPool::setRequestedLocales(std::unordered_set<zypp::Locale, std::hash<zypp::Locale>, std::equal_to<zypp::Locale>, std::allocator<zypp::Locale> > const&)
```

[引用/定义原记录](../R119_batch1/W1/positive/zypp_unordered.json) — SHA256 `82d73d2288f6f347bcc3757089137e475729acfe886ca91b16f21701e1f57a28`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-20"></a>

## 20. libzypp-bindings → libzypp: ZConfig::multiversionSpec

跨包对象/成员传递

set<string> 值参数; 声明不能决定复制或移动

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/python3.14/site-packages/_zypp.so | f9a768b27d54f3069c5b836b716a80ac40553a8230ebd101dcee3490c72928fb |
| provider | /usr/lib64/libzypp.so.1722.1.7 | 028eb8764e708e1b25d708392b001631679eae76a512583056aeb9bd53473a56 |


```text
   271: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4zypp7ZConfig16multiversionSpecESt3setINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt4lessIS7_ESaIS7_EE@ZYPP_plain (2)
  3042: 0000000000361fb0   509 FUNC    GLOBAL DEFAULT   12 _ZN4zypp7ZConfig16multiversionSpecESt3setINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt4lessIS7_ESaIS7_EE@@ZYPP_plain
```

```text
zypp::ZConfig::multiversionSpec(std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >)
```

[引用/定义原记录](../R119_batch1/W1/positive/zypp_set.json) — SHA256 `049372865099a7bfbbe66299e997800063b425b294385b8e9c1d457e4caa773e`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-21"></a>

## 21. libzypp-bindings → libzypp: CheckSum::CheckSum

跨包对象/成员传递

string const 引用与 istream 引用; 借用输入及流

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/python3.14/site-packages/_zypp.so | f9a768b27d54f3069c5b836b716a80ac40553a8230ebd101dcee3490c72928fb |
| provider | /usr/lib64/libzypp.so.1722.1.7 | 028eb8764e708e1b25d708392b001631679eae76a512583056aeb9bd53473a56 |


```text
  1392: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4zypp8CheckSumC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERSi@ZYPP_plain (2)
  4401: 000000000023cf90  1009 FUNC    GLOBAL DEFAULT   12 _ZN4zypp8CheckSumC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERSi@@ZYPP_plain
```

```text
zypp::CheckSum::CheckSum(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_istream<char, std::char_traits<char> >&)
```

[引用/定义原记录](../R119_batch1/W1/positive/zypp_istream.json) — SHA256 `408a145aef12f7175f49013164d949d02406493bb4ff3ee4fd4bd021eb4940f4`

armv7l: `NOT_OBSERVED`.

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-22"></a>

## 22. dali2-ui-foundation → dali2: ConditionalWait::WaitUntil

跨包对象/成员传递

time_point 值参数；duration/rep 成员；ScopedLock 引用; 时间值不移交堆资源

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/lib64/libdali2-ui-foundation.so.2.0.0 | 4908c77cd60ee00fa358fc74f2c9ea06d5e1065163267cfa8b03cd6bb4a11d10 |
| provider | /usr/lib64/libdali2-core.so.2.0.0 | e2c707cb8ce7642627ea1c0351d3621383de73283f4f7d3f6a0ee1f2b74066b4 |


```text
   881: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIlSt5ratioILl1ELl1000000000EEEEEE
   529: 0000000000179946   170 FUNC    GLOBAL DEFAULT   11 _ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIlSt5ratioILl1ELl1000000000EEEEEE
```

```text
Dali::ConditionalWait::WaitUntil(Dali::ConditionalWait::ScopedLock const&, std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long, std::ratio<1l, 1000000000l> > >)
```

[引用/定义原记录](../R119_DIVERGENT/CONFIRMED_EDGE_PROOFS.json) — SHA256 `d224fd764a3d293dba397b2e659ca7f4b85722c8b170fb9cd4b3751636e0df6e`

### armv7l

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /home/toolchain/development/libc++_replacement/tmp/R11/extracted/71/715cf7e83b9a668c5a77c2f435edff878220c8167802319c62bb31806c0c7a60/usr/lib/libdali2-ui-foundation.so.2.0.0 | 1e118e3bae8892d46f31d3207ce5385017070e057a691df5c6bdca7b84864071 |
| provider | /home/toolchain/development/libc++_replacement/tmp/R11/extracted/d4/d41171949e282b813f19d4fc714f9a9a5cec0d7d3abae81e95aba906e29c21f1/usr/lib/libdali2-core.so.2.0.0 | 060e6c7e17c93ea7c4659a760af42d3b7ebb69c56efc93601af27434bd5dfcd0 |


```text
   596: 00000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIxSt5ratioILx1ELx1000000000EEEEEE
   180: 000fcbc9   132 FUNC    GLOBAL DEFAULT   10 _ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIxSt5ratioILx1ELx1000000000EEEEEE
```

[证据: arm-elf](EVIDENCE.md#arm-elf)

以下是既有记录中从真实 ARM 头文件交叉编译的调用方对象符号；不是 libc++ 提供方产物，也不是运行结果。

[证据: arm-signatures](EVIDENCE.md#arm-signatures)

gnu

```text
_ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIxSt5ratioILx1ELx1000000000EEEEEE
```

cxx

```text
_ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt3__16chrono10time_pointINS5_12steady_clockENS5_8durationIxNS4_5ratioILx1ELx1000000000EEEEEEE
```

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。

<a id="edge-23"></a>

## 23. zypper → libzypp: xml::Reader::Reader

跨包 C++ 对象（非直接内建差异签名）

InputStream const 引用；内部 string/shared_ptr/streamoff 状态; 借用参数；构造后持有关系未在本轮验证

### x86_64

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /usr/bin/zypper | fd8fabf9225176abcc58648eb21bcdf65555b123a7473b9b54529b474c887da5 |
| provider | /usr/lib64/libzypp.so.1722.1.7 | 028eb8764e708e1b25d708392b001631679eae76a512583056aeb9bd53473a56 |


```text
   486: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE@ZYPP_plain (2)
  2847: 00000000000e3ef0   870 FUNC    GLOBAL DEFAULT   12 _ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE@@ZYPP_plain
```

```text
zypp::xml::Reader::Reader(zypp::InputStream const&, zypp::xml::Validate const&)
```

[引用/定义原记录](../R119_DIVERGENT/CONFIRMED_EDGE_PROOFS.json) — SHA256 `d224fd764a3d293dba397b2e659ca7f4b85722c8b170fb9cd4b3751636e0df6e`

### armv7l

| Role | ELF | SHA256 |
| --- | --- | --- |
| consumer | /home/toolchain/development/libc++_replacement/tmp/R11/extracted/ca/cab2471b4d25aead1d0f70cc21afc7bd0e10ef22e96083a029bb8493e06c3694/usr/bin/zypper | c613e4dd0ddf5392b45b38222fb2da4e270f0db6c888f1114dfabc0ff767b308 |
| provider | /home/toolchain/development/libc++_replacement/tmp/R11/extracted/50/5081acad965983c505a23bddae86f4b434462ad5c88127d9313932f66edf0b5d/usr/lib/libzypp.so.1722.1.7 | 51dae5819ee9cb804b4bb765f334848a706e7ce583bd7bf4899696b526a92648 |


```text
   493: 00000000     0 FUNC    GLOBAL DEFAULT  UND _ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE@ZYPP_plain (2)
  2961: 000a112d   568 FUNC    GLOBAL DEFAULT   11 _ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE@@ZYPP_plain
```

[证据: arm-elf](EVIDENCE.md#arm-elf)

以下是既有记录中从真实 ARM 头文件交叉编译的调用方对象符号；不是 libc++ 提供方产物，也不是运行结果。

[证据: arm-signatures](EVIDENCE.md#arm-signatures)

gnu

```text
_ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE
```

cxx

```text
_ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE
```

aarch64: `NOT_OBSERVED`.

ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。
