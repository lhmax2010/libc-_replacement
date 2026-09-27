# 公开声明与调用路径的头文件证据

路径相对于隔离 sysroot；每段带完整文件 SHA256 与来源开发包。内部 helper 的声明只用于取证，夹具从公开入口到达，不直接 include 内部头。

## 边 1：usr/include/appcore_cpp/app_core_base.hh

SHA256 `2eee9ed12d299ab420949bd30b785b0816e61d51d20951b77975306cf07fab53`；RPM：`app-core-common-devel`。

```cpp
87:   int OnSetI18n() override;
88:   int OnSetEvent(IEvent::Type event) override;
89:   int OnUnsetEvent(IEvent::Type event) override;
90:   int OnTrimMemory() override;
91:   void AddEvent(std::shared_ptr<EventBase> event);
92:   bool RemoveEvent(std::shared_ptr<EventBase> event);
93:   void RaiseEvent(int event, IEvent::Type type);
94:   void RaiseEvent(const std::string& event, IEvent::Type type);
95:   void FlushMemory();
96:   bool IsBgAllowed();
97:   bool IsSuspended();
98:   void ToggleSuspendedState();
99:   void SetAppLanguage(const std::string& lang);
100:   std::string GetAppLanguage();
101:   std::string GetSystemLanguage();
102:   int SetI18n(std::string domain_name, std::string dir_name);
```

## 边 1：usr/include/appcore_cpp/interface_main_loop.hh

SHA256 `a28f0d21b7e2f7ee87c9022ddf5dd939ff36e6958104a157712e54d87a63bfda`；RPM：`app-core-common-devel`。

```cpp
25: class EXPORT_API IMainLoop {
26:  public:
27:   virtual ~IMainLoop() = default;  // LCOV_EXCL_LINE
28: 
29:   virtual void OnLoopInit(int argc, char** argv) = 0;
30:   virtual void OnLoopFinish() = 0;
31:   virtual void OnLoopRun() = 0;
32:   virtual void OnLoopExit() = 0;
33: };
34: 
35: }  // namespace tizen_cpp
36: 
37: #endif  // TIZEN_CPP_APP_CORE_CPP_INTERFACE_MAIN_LOOP_HH_
```

```cpp
26:  public:
27:   virtual ~IMainLoop() = default;  // LCOV_EXCL_LINE
28: 
29:   virtual void OnLoopInit(int argc, char** argv) = 0;
30:   virtual void OnLoopFinish() = 0;
31:   virtual void OnLoopRun() = 0;
32:   virtual void OnLoopExit() = 0;
33: };
34: 
35: }  // namespace tizen_cpp
36: 
37: #endif  // TIZEN_CPP_APP_CORE_CPP_INTERFACE_MAIN_LOOP_HH_
```

```cpp
27:   virtual ~IMainLoop() = default;  // LCOV_EXCL_LINE
28: 
29:   virtual void OnLoopInit(int argc, char** argv) = 0;
30:   virtual void OnLoopFinish() = 0;
31:   virtual void OnLoopRun() = 0;
32:   virtual void OnLoopExit() = 0;
33: };
34: 
35: }  // namespace tizen_cpp
36: 
37: #endif  // TIZEN_CPP_APP_CORE_CPP_INTERFACE_MAIN_LOOP_HH_
```

```cpp
28: 
29:   virtual void OnLoopInit(int argc, char** argv) = 0;
30:   virtual void OnLoopFinish() = 0;
31:   virtual void OnLoopRun() = 0;
32:   virtual void OnLoopExit() = 0;
33: };
34: 
35: }  // namespace tizen_cpp
36: 
37: #endif  // TIZEN_CPP_APP_CORE_CPP_INTERFACE_MAIN_LOOP_HH_
```

## 边 2：usr/include/bundle_cpp.h

SHA256 `7f818af27923d404f40a55558f8966146c80b274a321b26fa65fa1eeb9007104`；RPM：`bundle-devel`。

```cpp
242:    * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
243:    * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
244:    * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
245:   */
246:   int Add(const std::string& key, const std::string& val);
247: 
248:   /**
249:    * @brief Adds a string type key-value pair into a bundle.
250:    * @since_tizen 5.5
251:    * @param[in] key The string key
252:    * @param[in] val The array of strings
253:    * @return The operation result
254:    * @retval BUNDLE_ERROR_NONE Success
255:    * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
256:    * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
257:    * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
```

```cpp
255:    * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
256:    * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
257:    * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
258:   */
259:   int Add(const std::string& key, const std::vector<std::string>& val);
260: 
261:   /**
262:    * @brief Adds a string type key-value pair into a bundle.
263:    * @since_tizen 5.5
264:    * @param[in] key The string key
265:    * @param[in] val The array of bytes
266:    * @return The operation result
267:    * @retval BUNDLE_ERROR_NONE Success
268:    * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
269:    * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
270:    * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
```

```cpp
268:    * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
269:    * @retval BUNDLE_ERROR_KEY_EXISTS Key already exists
270:    * @retval BUNDLE_ERROR_OUT_OF_MEMORY Out of memory
271:   */
272:   int Add(const std::string& key, const std::vector<unsigned char>& val);
273: 
274:   /**
275:    * @brief Deletes a key-value object with the given key.
276:    * @since_tizen 5.5
277:    * @param[in] key The string key
278:    * @return The operation result
279:    * @retval BUNDLE_ERROR_NONE Success
280:    * @retval BUNDLE_ERROR_INVALID_PARAMETER Invalid parameter
281:    * @retval BUNDLE_ERROR_KEY_NOT_AVAILABLE Key not available
282:   */
283:   int Delete(const std::string& key);
```

## 边 3：usr/include/bundle_cpp.h

SHA256 `7f818af27923d404f40a55558f8966146c80b274a321b26fa65fa1eeb9007104`；RPM：`bundle-devel`。

```cpp
29: 
30: #include <bundle.h>
31: 
32: #include <cstdio>
33: #include <initializer_list>
34: #include <memory>
35: #include <string>
36: #include <utility>
37: #include <vector>
38: 
39: #ifndef EXPORT_API
40: #define EXPORT_API __attribute__((visibility("default")))
41: #endif
42: 
43: namespace tizen_base {
44: 
```

```cpp
140:    * @brief Constructor.
141:    * @since_tizen 6.5
142:    * @param[in] key_values The list of key-value pair
143:    */
144:   Bundle(std::initializer_list<
145:       std::pair<std::string, std::string>> key_values);
146: 
147:   /**
148:    * @brief Constructor.
149:    * @since_tizen 5.5
150:    * @param[in] raw The object for BundleRaw
151:    * @param[in] base64 @c true, @a raw is the encoded raw data using base64-encoding
152:    */
153:   explicit Bundle(BundleRaw raw, bool base64 = true);
154: 
155:   /**
```

## 边 4：usr/include/scim-1.0/scim_utility.h

SHA256 `1fb7d85c78ef52cb8704f590555c782ae88e12fa2066bec1a175b885ef5f749c`；RPM：`isf-devel`。

```cpp
109:  * @param wstr source ucs4 string.
110:  *
111:  * @return the destination utf8 string.
112:  */
113: EXAPI String utf8_wcstombs (const WideString & wstr);
114: 
115: /**
116:  * @brief Convert an ucs4 string to an utf8 string.
117:  *
118:  * @param wstr source ucs4 string.
119:  * @param len length of the source string.
120:  *
121:  * @return the destination utf8 string.
122:  */
123: EXAPI String utf8_wcstombs (const ucs4_t *wstr, int len = -1);
124: 
```

```cpp
119:  * @param len length of the source string.
120:  *
121:  * @return the destination utf8 string.
122:  */
123: EXAPI String utf8_wcstombs (const ucs4_t *wstr, int len = -1);
124: 
125: /**
126:  * @brief Read a wide char from istream.
127:  *
128:  * The content in the istream are actually in utf-8 encoding.
129:  *
130:  * @param is the stream to be read.
131:  *
132:  * @return if equal to 0 then got the end of the stream or error occurred.
133:  */
134: EXAPI ucs4_t utf8_read_wchar (std::istream &is);
```

## 边 5：usr/include/jsoncpp/json/reader.h

SHA256 `c8e50d2d3a6fd84e6b069ef4706871faec682f9bb247acc72e3ea4a281088ecb`；RPM：`jsoncpp-devel`。

```cpp
73:    *                             if Features::allowComments_ is \c false.
74:    * \return \c true if the document was successfully parsed, \c false if an
75:    * error occurred.
76:    */
77:   bool parse(const std::string& document, Value& root,
78:              bool collectComments = true);
79: 
80:   /** \brief Read a Value from a <a HREF="http://www.json.org">JSON</a>
81:    * document.
82:    *
83:    * \param      beginDoc        Pointer on the beginning of the UTF-8 encoded
84:    *                             string of the document to read.
85:    * \param      endDoc          Pointer on the end of the UTF-8 encoded string
86:    *                             of the document to read.  Must be >= beginDoc.
87:    * \param[out] root            Contains the root value of the document if it
88:    *                             was successfully parsed.
```

```cpp
92:    *                             if Features::allowComments_ is \c false.
93:    * \return \c true if the document was successfully parsed, \c false if an
94:    * error occurred.
95:    */
96:   bool parse(const char* beginDoc, const char* endDoc, Value& root,
97:              bool collectComments = true);
98: 
99:   /// \brief Parse from input stream.
100:   /// \see Json::operator>>(std::istream&, Json::Value&).
101:   bool parse(IStream& is, Value& root, bool collectComments = true);
102: 
103:   /** \brief Returns a user friendly string that list errors in the parsed
104:    * document.
105:    *
106:    * \return Formatted error message with the list of errors with their
107:    * location in the parsed document. An empty string is returned if no error
```

```cpp
97:              bool collectComments = true);
98: 
99:   /// \brief Parse from input stream.
100:   /// \see Json::operator>>(std::istream&, Json::Value&).
101:   bool parse(IStream& is, Value& root, bool collectComments = true);
102: 
103:   /** \brief Returns a user friendly string that list errors in the parsed
104:    * document.
105:    *
106:    * \return Formatted error message with the list of errors with their
107:    * location in the parsed document. An empty string is returned if no error
108:    * occurred during parsing.
109:    * \deprecated Use getFormattedErrorMessages() instead (typo fix).
110:    */
111:   JSONCPP_DEPRECATED("Use getFormattedErrorMessages() instead.")
112:   String getFormatedErrorMessages() const;
```

```cpp
266:    *                      document.
267:    * \return \c true if the document was successfully parsed, \c false if an
268:    * error occurred.
269:    */
270:   virtual bool parse(char const* beginDoc, char const* endDoc, Value* root,
271:                      String* errs);
272: 
273:   /** \brief Returns a vector of structured errors encountered while parsing.
274:    * Each parse call resets the stored list of errors.
275:    */
276:   std::vector<StructuredError> getStructuredErrors() const;
277: 
278:   class JSON_API Factory {
279:   public:
280:     virtual ~Factory() = default;
281:     /** \brief Allocate a CharReader via operator new().
```

```cpp
287: protected:
288:   class Impl {
289:   public:
290:     virtual ~Impl() = default;
291:     virtual bool parse(char const* beginDoc, char const* endDoc, Value* root,
292:                        String* errs) = 0;
293:     virtual std::vector<StructuredError> getStructuredErrors() const = 0;
294:   };
295: 
296:   explicit CharReader(std::unique_ptr<Impl> impl) : _impl(std::move(impl)) {}
297: 
298: private:
299:   std::unique_ptr<Impl> _impl;
300: }; // CharReader
301: 
302: /** \brief Build a CharReader implementation.
```

## 边 6：usr/include/absl/base/internal/spinlock_wait.h

SHA256 `0e3013e9da116026a283f881583f767dae9cc8547e8cbe7cb57acc9f2a659048`；RPM：`abseil-cpp-devel`。

```cpp
26: namespace absl {
27: ABSL_NAMESPACE_BEGIN
28: namespace base_internal {
29: 
30: // SpinLockWait() waits until it can perform one of several transitions from
31: // "from" to "to".  It returns when it performs a transition where done==true.
32: struct SpinLockWaitTransition {
33:   uint32_t from;
34:   uint32_t to;
35:   bool done;
36: };
37: 
38: // Wait until *w can transition from trans[i].from to trans[i].to for some i
39: // satisfying 0<=i<n && trans[i].done, atomically make the transition,
40: // then return the old value of *w.   Make any other atomic transitions
41: // where !trans[i].done, but continue waiting.
```

```cpp
40: // then return the old value of *w.   Make any other atomic transitions
41: // where !trans[i].done, but continue waiting.
42: //
43: // Wakeups for threads blocked on SpinLockWait do not respect priorities.
44: uint32_t SpinLockWait(std::atomic<uint32_t> *w, int n,
45:                       const SpinLockWaitTransition trans[],
46:                       SchedulingMode scheduling_mode);
47: 
48: // If possible, wake some thread that has called SpinLockDelay(w, ...). If `all`
49: // is true, wake all such threads. On some systems, this may be a no-op; on
50: // those systems, threads calling SpinLockDelay() will always wake eventually
51: // even if SpinLockWake() is never called.
52: void SpinLockWake(std::atomic<uint32_t> *w, bool all);
53: 
54: // Wait for an appropriate spin delay on iteration "loop" of a
55: // spin loop on location *w, whose previously observed value was "value".
```

## 边 6：usr/include/absl/base/call_once.h

SHA256 `9f65cf0aa3632a53eb3c162ebe1badc89b86d3d33fab3bc05da93853fa911e4f`；RPM：`abseil-cpp-devel`。

```cpp
172: 
173:   // Must do this before potentially modifying control word's state.
174:   base_internal::SchedulingHelper maybe_disable_scheduling(scheduling_mode);
175:   // Short circuit the simplest case to avoid procedure call overhead.
176:   // The base_internal::SpinLockWait() call returns either kOnceInit or
177:   // kOnceDone. If it returns kOnceDone, it must have loaded the control word
178:   // with std::memory_order_acquire and seen a value of kOnceDone.
179:   uint32_t old_control = kOnceInit;
180:   if (control->compare_exchange_strong(old_control, kOnceRunning,
181:                                        std::memory_order_relaxed) ||
182:       base_internal::SpinLockWait(control, ABSL_ARRAYSIZE(trans), trans,
183:                                   scheduling_mode) == kOnceInit) {
184:     std::invoke(std::forward<Callable>(fn), std::forward<Args>(args)...);
185:     old_control =
186:         control->exchange(base_internal::kOnceDone, std::memory_order_release);
187:     if (old_control == base_internal::kOnceWaiter) {
```

```cpp
178:   // with std::memory_order_acquire and seen a value of kOnceDone.
179:   uint32_t old_control = kOnceInit;
180:   if (control->compare_exchange_strong(old_control, kOnceRunning,
181:                                        std::memory_order_relaxed) ||
182:       base_internal::SpinLockWait(control, ABSL_ARRAYSIZE(trans), trans,
183:                                   scheduling_mode) == kOnceInit) {
184:     std::invoke(std::forward<Callable>(fn), std::forward<Args>(args)...);
185:     old_control =
186:         control->exchange(base_internal::kOnceDone, std::memory_order_release);
187:     if (old_control == base_internal::kOnceWaiter) {
188:       base_internal::SpinLockWake(control, true);
189:     }
190:   }  // else *control is already kOnceDone
191: }
192: 
193: inline std::atomic<uint32_t>* absl_nonnull ControlWord(
```

## 边 7：usr/include/dali/devel-api/adaptor-framework/actor-accessible.h

SHA256 `8e1811aa67616cd50c95ec6d0e6aa7ba53ec0b7dcac059060b540001093549bb`；RPM：`dali2-adaptor-integration-devel`。

```cpp
124:    */
125:   Dali::Bounds GetExtents(Dali::Devel::Accessibility::CoordinateType type) const override;
126: 
127:   /**
128:    * @copydoc Dali::Accessibility::Collection::GetMatches()
129:    */
130:   std::vector<Accessible*> GetMatches(MatchRule rule, uint32_t sortBy, size_t maxCount) override;
131: 
132:   /**
133:    * @copydoc Dali::Accessibility::Collection::GetMatchesInMatches()
134:    */
135:   std::vector<Accessible*> GetMatchesInMatches(MatchRule firstRule, MatchRule secondRule, uint32_t sortBy, int32_t firstCount, int32_t secondCount) override;
136: 
137:   /**
138:    * @brief Notifies this object that its children have changed.
139:    *
```

```cpp
126: 
127:   /**
128:    * @copydoc Dali::Accessibility::Collection::GetMatches()
129:    */
130:   std::vector<Accessible*> GetMatches(MatchRule rule, uint32_t sortBy, size_t maxCount) override;
131: 
132:   /**
133:    * @copydoc Dali::Accessibility::Collection::GetMatchesInMatches()
134:    */
135:   std::vector<Accessible*> GetMatchesInMatches(MatchRule firstRule, MatchRule secondRule, uint32_t sortBy, int32_t firstCount, int32_t secondCount) override;
136: 
137:   /**
138:    * @brief Notifies this object that its children have changed.
139:    *
140:    * This is useful if you maintain a custom collection of children that are not derived from
141:    * ActorAccessible and the contents or order of elements in that collection change.
```

## 边 7：usr/include/dali/devel-api/atspi-interfaces/collection.h

SHA256 `4bb5b37e7c21452614a52e3ae3a5c0494dcdd9575dddf4bb281244a5450813e8`；RPM：`dali2-adaptor-integration-devel`。

```cpp
51: 
52:   /**
53:    * MatchRule type is a tuple that only carries data of de-serialized parameter from BridgeCollection::GetMatches dbus method.
54:    */
55:   using MatchRule = std::tuple<
56:     std::array<int32_t, 2>,
57:     int32_t,
58:     std::unordered_map<std::string, std::string>,
59:     int32_t,
60:     RoleMask,
61:     int32_t,
62:     std::vector<std::string>,
63:     int32_t,
64:     bool>;
65: 
66:   /**
```

## 边 8：usr/include/dali/integration-api/scene.h

SHA256 `607cb10c41b9d2ab3c70b75ce0b6c7677719231b7a1ae656702097fd8e1de9cb`；RPM：`dali2-integration-devel`。

```cpp
355:    * This callback will be deleted once it is called.
356:    *
357:    * @note Ownership of the callback is passed onto this class.
358:    */
359:   void AddFrameRenderedCallback(std::unique_ptr<CallbackBase> callback, int32_t frameId);
360: 
361:   /**
362:    * @brief Adds a callback that is called when the frame is displayed on the display.
363:    *
364:    * @param[in] callback The function to call
365:    * @param[in] frameId The Id to specify the frame. It will be passed when the callback is called.
366:    *
367:    * @note A callback of the following type may be used:
368:    * @code
369:    *   void MyFunction( int32_t frameId );
370:    * @endcode
```

## 边 8：usr/include/dali/integration-api/scene.h

SHA256 `607cb10c41b9d2ab3c70b75ce0b6c7677719231b7a1ae656702097fd8e1de9cb`；RPM：`dali2-integration-devel`。

```cpp
106:    * @param[in] screenOrientation The rotated angle of the screen
107:    * @param[in] flags Scene policy flags
108:    * @return a handle to a newly allocated Dali resource.
109:    */
110:   static Scene New(const Graphics::RenderTargetCreateInfo& createInfo,
111:                    Size                                    size,
112:                    int32_t                                 windowOrientation = 0,
113:                    int32_t                                 screenOrientation = 0,
114:                    ScenePolicyFlagBits                     flags             = ScenePolicyFlagBits::NONE);
115: 
116:   /**
117:    * @brief Downcast an Object handle to Scene handle.
118:    *
119:    * If handle points to a Scene object the downcast produces
120:    * valid handle. If not the returned handle is left uninitialized.
121:    * @param[in] handle to An object
```

## 边 9：usr/include/gtest/internal/gtest-string.h

SHA256 `0cc08c746bceb2b101c61ffbc905c598eb62fee3c262c4585c21b10d8a3e6508`；RPM：`gtest-devel`。

```cpp
169: };           // class String
170: 
171: // Gets the content of the stringstream's buffer as an std::string.  Each '\0'
172: // character in the buffer is replaced with "\\0".
173: GTEST_API_ std::string StringStreamToString(::std::stringstream* stream);
174: 
175: }  // namespace internal
176: }  // namespace testing
177: 
178: #endif  // GOOGLETEST_INCLUDE_GTEST_INTERNAL_GTEST_STRING_H_
```

## 边 9：usr/include/gtest/gtest.h

SHA256 `55dfd674c0619529501ea5dcb5470cecc6becb5bde03d8a58855c1631f71bc46`；RPM：`gtest-devel`。

```cpp
1596:   rhs_ss.precision(std::numeric_limits<RawType>::digits10 + 2);
1597:   rhs_ss << rhs_value;
1598: 
1599:   return EqFailure(lhs_expression, rhs_expression,
1600:                    StringStreamToString(&lhs_ss), StringStreamToString(&rhs_ss),
1601:                    false);
1602: }
1603: 
1604: // Helper function for implementing ASSERT_NEAR.
1605: //
1606: // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
1607: GTEST_API_ AssertionResult DoubleNearPredFormat(const char* expr1,
1608:                                                 const char* expr2,
1609:                                                 const char* abs_error_expr,
1610:                                                 double val1, double val2,
1611:                                                 double abs_error);
```

## 边 10：usr/include/delta/delta_parser.h

SHA256 `9e1a342e5ff330b20b18cf3cedde3d1d8e9c07d20ded5edac55f16bbcf509cab`；RPM：`manifest-parser-devel`。

```cpp
17:  * @brief The DeltaParser class
18:  *        Parser class of delta info file.
19:  *
20:  * Instance of this class may be used to parse delta file.
21:  * Depending on boolean result of @ref ParseManifest method, client code may
22:  * call:
23:  *  - on success -> @ref GetManifestData(), passing the key of ManifestData
24:  *                  instance that it is interested in.
25:  *  - on failure -> @ref GetErrorMessage(), to get value of error which was set
26:  *                  during the processing of config.xml
27:  *
28:  * To investigate which key do you need to get certain parsed piece of data,
29:  * check the key reported by handler's @ref ManifestHandler::Key() method.
30:  * Key returned by this method is the key to access data set by handler.
31:  */
32: class DeltaParser {
```

```cpp
35: 
36:   std::shared_ptr<const parser::ManifestData> GetManifestData(
37:       const std::string& key);
38:   const std::string& GetErrorMessage() const;
39:   bool ParseManifest(const std::filesystem::path& path);
40: 
41:  private:
42:   std::unique_ptr<parser::ManifestParser> parser_;
43:   std::string error_;
44: };
45: 
46: }  // namespace delta
47: 
48: #endif  // DELTA_DELTA_PARSER_H_
```

## 边 10：usr/include/delta/delta_handler.h

SHA256 `66bb29b6a13c5613cb9fee2a6810f0b58c7373189bf50e0f84c6c17fda328366`；RPM：`manifest-parser-devel`。

```cpp
19:  public:
20:   void set_added(const std::vector<std::string>& added) {
21:     added_ = added;
22:   }
23:   const std::vector<std::string>& added() const {
24:     return added_;
25:   }
26:   void set_modified(const std::vector<std::string>& modified) {
27:     modified_ = modified;
28:   }
29:   const std::vector<std::string>& modified() const {
30:     return modified_;
31:   }
32:   void set_removed(const std::vector<std::string>& removed) {
33:     removed_ = removed;
34:   }
```

## 边 11：usr/include/cert-svc/vcore/SignatureValidator.h

SHA256 `2bb0d714341e589d4d3fcba9bcca6cf9f5f0b545e94a97a53aa175f3f6be7128`；RPM：`cert-svc-devel`。

```cpp
74: 	VCerr check(const std::string &contentPath,
75: 				bool checkOcsp,
76: 				bool checkReferences,
77: 				SignatureData &outData);
78: 	VCerr checkList(bool checkOcsp,
79: 					const UriList &uriList,
80: 					SignatureData &outData);
81: 
82: 	VCerr checkAll(bool checkOcsp,
83: 				   bool checkReferences,
84: 				   SignatureDataMap &sigDataMap);
85: 	VCerr checkListAll(bool checkOcsp,
86: 					   const UriList &uriList,
87: 					   SignatureDataMap &sigDataMap);
88: 
89: 	/*
```

## 边 12：usr/include/dali-toolkit/devel-api/controls/web-view/web-view.h

SHA256 `922a31007a38c290fafebf3e2b8879d6ddf56dbb3d1b16d92e81ab3f05c0e5ea`；RPM：`dali2-toolkit-integration-devel`。

```cpp
683:    * @brief Callback to be called when page loading is started.
684:    *
685:    * @param[in] callback
686:    */
687:   void RegisterPageLoadStartedCallback(Dali::WebEnginePlugin::WebEnginePageLoadCallback callback);
688: 
689:   /**
690:    * @brief Callback to be called when page loading is in progress.
691:    *
692:    * @param[in] callback
693:    */
694:   void RegisterPageLoadInProgressCallback(Dali::WebEnginePlugin::WebEnginePageLoadCallback callback);
695: 
696:   /**
697:    * @brief Callback to be called when page loading is finished.
698:    *
```

## 边 13：usr/include/dali/devel-api/common/hash.h

SHA256 `8793aa4de669967c3cd18e42ce8182b63cb49fac7c96779f988c9807eba690c6`；RPM：`dali2-integration-devel`。

```cpp
33:  * @brief Create a hash code for a string
34:  * @param toHash string to hash
35:  * @return hash code
36:  */
37: DALI_CORE_API std::size_t CalculateHash(const std::string& toHash);
38: 
39: /**
40:  * @brief Create a hash code for 2 strings combined.
41:  * Allows a hash to be calculated without concatenating the strings and allocating any memory.
42:  * @param string1 first string
43:  * @param string2 second string
44:  * @return hash code
45:  */
46: DALI_CORE_API std::size_t CalculateHash(const std::string& string1, const std::string& string2);
47: 
48: /**
```

```cpp
42:  * @param string1 first string
43:  * @param string2 second string
44:  * @return hash code
45:  */
46: DALI_CORE_API std::size_t CalculateHash(const std::string& string1, const std::string& string2);
47: 
48: /**
49:  * @brief Create a hash code for a string
50:  * @param toHash string to hash
51:  * @param terminator character terminating the hashing
52:  * @return hash code
53:  */
54: DALI_CORE_API std::size_t CalculateHash(const std::string& toHash, char terminator);
55: 
56: /**
57:  * @brief Create a hash code for a string_view
```

```cpp
50:  * @param toHash string to hash
51:  * @param terminator character terminating the hashing
52:  * @return hash code
53:  */
54: DALI_CORE_API std::size_t CalculateHash(const std::string& toHash, char terminator);
55: 
56: /**
57:  * @brief Create a hash code for a string_view
58:  * @param toHash string_view to hash
59:  * @return hash code
60:  */
61: DALI_CORE_API std::size_t CalculateHash(const std::string_view& toHash);
62: 
63: /**
64:  * @brief Create a hash code for 2 string_views combined.
65:  * Allows a hash to be calculated without concatenating the string_views and allocating any memory.
```

```cpp
57:  * @brief Create a hash code for a string_view
58:  * @param toHash string_view to hash
59:  * @return hash code
60:  */
61: DALI_CORE_API std::size_t CalculateHash(const std::string_view& toHash);
62: 
63: /**
64:  * @brief Create a hash code for 2 string_views combined.
65:  * Allows a hash to be calculated without concatenating the string_views and allocating any memory.
66:  * @param string1 first string_view
67:  * @param string2 second string_view
68:  * @return hash code
69:  */
70: DALI_CORE_API std::size_t CalculateHash(const std::string_view& string1, const std::string_view& string2);
71: 
72: /**
```

```cpp
66:  * @param string1 first string_view
67:  * @param string2 second string_view
68:  * @return hash code
69:  */
70: DALI_CORE_API std::size_t CalculateHash(const std::string_view& string1, const std::string_view& string2);
71: 
72: /**
73:  * @brief Create a hash code for a string_view
74:  * @param toHash string_view to hash
75:  * @param terminator character terminating the hashing
76:  * @return hash code
77:  */
78: DALI_CORE_API std::size_t CalculateHash(const std::string_view& toHash, char terminator);
79: 
80: /**
81:  * @brief Create a hash code for a std::vector<std::uint8_t>
```

```cpp
74:  * @param toHash string_view to hash
75:  * @param terminator character terminating the hashing
76:  * @return hash code
77:  */
78: DALI_CORE_API std::size_t CalculateHash(const std::string_view& toHash, char terminator);
79: 
80: /**
81:  * @brief Create a hash code for a std::vector<std::uint8_t>
82:  * @param toHash list of std::uint8_t to hash
83:  * @return hash code
84:  */
85: DALI_CORE_API std::size_t CalculateHash(const std::vector<std::uint8_t>& toHash);
86: 
87: /**
88:  * @brief Create a hash code for a Dali::Vector<std::uint8_t>
89:  * @param toHash list of std::uint8_t to hash
```

```cpp
81:  * @brief Create a hash code for a std::vector<std::uint8_t>
82:  * @param toHash list of std::uint8_t to hash
83:  * @return hash code
84:  */
85: DALI_CORE_API std::size_t CalculateHash(const std::vector<std::uint8_t>& toHash);
86: 
87: /**
88:  * @brief Create a hash code for a Dali::Vector<std::uint8_t>
89:  * @param toHash list of std::uint8_t to hash
90:  * @return hash code
91:  */
92: DALI_CORE_API std::size_t CalculateHash(const Dali::Vector<std::uint8_t>& toHash);
93: 
94: } // namespace Dali
95: 
96: #endif // DALI_HASH
```

```cpp
88:  * @brief Create a hash code for a Dali::Vector<std::uint8_t>
89:  * @param toHash list of std::uint8_t to hash
90:  * @return hash code
91:  */
92: DALI_CORE_API std::size_t CalculateHash(const Dali::Vector<std::uint8_t>& toHash);
93: 
94: } // namespace Dali
95: 
96: #endif // DALI_HASH
```

## 边 14：usr/include/dali/devel-api/threading/conditional-wait.h

SHA256 `2eba76c4110dca0de24d5aef15f029a1d8496fd33e9c81a5f37594b5c3a7dc83`；RPM：`dali2-integration-devel`。

```cpp
129:    * @param[in] scope A pre-existing lock on the internal state of this object.
130:    * @param[in] timePoint Maximum time point to wait.
131:    * @pre scope must have been passed this ConditionalWait during its construction.
132:    */
133:   void WaitUntil(const ScopedLock& scope, TimePoint timePoint);
134: 
135:   /**
136:    * @brief Return the count of threads waiting for this conditional
137:    * @return count of waits
138:    */
139:   unsigned int GetWaitCount() const;
140: 
141: private:
142:   // Not implemented as ConditionalWait is not copyable
143:   ConditionalWait(const ConditionalWait&);
144:   const ConditionalWait& operator=(const ConditionalWait&);
```

## 边 15：usr/include/dali/devel-api/common/singleton-service.h

SHA256 `dba1defef3f262e75290de37c7594ba2a7c6516ce5641c9cd1cfaf91df80ab7f`；RPM：`dali2-integration-devel`。

```cpp
85:    * @note This is not intended for application developers.
86:    * @param[in] info The type info of the given type.
87:    * @return the Dali handle if it is registered as a singleton or an uninitialized handle.
88:    */
89:   BaseHandle GetSingleton(const std::type_info& info) const;
90: 
91: public: // Not intended for application developers
92:   /**
93:    * @brief This constructor is used by SingletonService::Get().
94:    * @param[in] singletonService A pointer to the internal singleton-service object.
95:    */
96:   explicit DALI_INTERNAL SingletonService(Internal::ThreadLocalStorage* singletonService);
97: };
98: 
99: } // namespace Dali
100: 
```

## 边 15：usr/include/dali/devel-api/common/singleton-service.h

SHA256 `dba1defef3f262e75290de37c7594ba2a7c6516ce5641c9cd1cfaf91df80ab7f`；RPM：`dali2-integration-devel`。

```cpp
31: 
32: /**
33:  * @brief Allows the registration of a class as a singleton
34:  *
35:  * @note This class is created by the Application class and is destroyed when the Application class is destroyed.
36:  *
37:  * @see Application
38:  */
39: class DALI_CORE_API SingletonService : public BaseHandle
40: {
41: public:
42:   /**
43:    * @brief Create an uninitialized handle.
44:    *
45:    * This can be initialized by calling SingletonService::Get().
46:    */
```

```cpp
33:  * @brief Allows the registration of a class as a singleton
34:  *
35:  * @note This class is created by the Application class and is destroyed when the Application class is destroyed.
36:  *
37:  * @see Application
38:  */
39: class DALI_CORE_API SingletonService : public BaseHandle
40: {
41: public:
42:   /**
43:    * @brief Create an uninitialized handle.
44:    *
45:    * This can be initialized by calling SingletonService::Get().
46:    */
47:   SingletonService();
48: 
```

## 边 16：usr/include/media/inference_engine_common.h

SHA256 `033e1efd2c467c635926c0bfaccbcc788c139ac9d4e5ec471965d1a561a2e775`；RPM：`inference-engine-interface-common-devel`。

```cpp
89: 		 * @since_tizen 6.0
90: 		 * @param[out] buffers A backend engine should add input tensor buffers allocated itself to buffers vector.
91: 		 *              Otherwise, it should put buffers to be empty.
92: 		 */
93: 		virtual int GetInputTensorBuffers(IETensorBuffer &buffers) = 0;
94: 
95: 		/**
96: 		 * @brief Get output tensor buffers from a given backend engine.
97: 		 * @details This function requests a backend engine output tensor buffers.
98: 		 *          If the backend engine is able to allocate the output tensor buffers internally, then
99: 		 *          it has to add the output tensor buffers to buffers vector. By doing this, upper layer
100: 		 *          will request a inference with the output tensor buffers.
101: 		 *          Otherwise, the backend engine should just return INFERENCE_ENGINE_ERROR_NONE so that
102: 		 *          upper layer can allocate output tensor buffers according to output layer property.
103: 		 *          As for the output layer property, you can see GetOutputLyaerProperty function.
104: 		 *
```

## 边 16：usr/include/media/inference_engine_common_impl.h

SHA256 `761908162ed8bfa4a899389f8e38edb39aede20c0605e59de2f3d7acb68378fd`；RPM：`inference-engine-interface-common-devel`。

```cpp
70: 		 *
71: 		 * @since_tizen 6.0
72: 		 * @param[in] config A configuraion data needed to load a backend library.
73: 		 */
74: 		int BindBackend(inference_engine_config *config);
75: 
76: 		/**
77: 		 * @brief Unload a backend engine library.
78: 		 * @details This function unload a backend engine library.
79: 		 *
80: 		 * @since_tizen 6.0
81: 		 */
82: 		void UnbindBackend(void);
83: 
84: 		/**
85: 		 * @brief Set target devices.
```

## 边 16：usr/include/media/inference_engine_common_impl.h

SHA256 `761908162ed8bfa4a899389f8e38edb39aede20c0605e59de2f3d7acb68378fd`；RPM：`inference-engine-interface-common-devel`。

```cpp
127: 		 * @since_tizen 6.0
128: 		 * @param[out] buffers A backend engine should add input tensor buffers allocated itself to buffers vector.
129: 		 *              Otherwise, it should put buffers to be empty.
130: 		 */
131: 		int GetInputTensorBuffers(IETensorBuffer &buffers);
132: 
133: 		/**
134: 		 * @brief Get output tensor buffers from a given backend engine.
135: 		 * @details This function requests a backend engine output tensor buffers.
136: 		 *          If the backend engine is able to allocate the output tensor buffers internally, then
137: 		 *          it has to add the output tensor buffers to buffers vector. By doing this, upper layer
138: 		 *          will request a inference with the output tensor buffers.
139: 		 *          Otherwise, the backend engine should just return INFERENCE_ENGINE_ERROR_NONE so that
140: 		 *          upper layer can allocate output tensor buffers according to output layer property.
141: 		 *          As for the output layer property, you can see GetOutputLyaerProperty function.
142: 		 *
```

## 边 17：usr/include/gtest/gtest-printers.h

SHA256 `c67d04e3d6f6b5b2fb2aad4bc093a9cf674cfba23fc8f29b4b37bb90be7c8321`；RPM：`gtest-devel`。

```cpp
691:   }
692: }
693: 
694: // Overloads for ::std::string.
695: GTEST_API_ void PrintStringTo(const ::std::string& s, ::std::ostream* os);
696: inline void PrintTo(const ::std::string& s, ::std::ostream* os) {
697:   PrintStringTo(s, os);
698: }
699: 
700: // Overloads for ::std::u8string
701: #ifdef __cpp_lib_char8_t
702: GTEST_API_ void PrintU8StringTo(const ::std::u8string& s, ::std::ostream* os);
703: inline void PrintTo(const ::std::u8string& s, ::std::ostream* os) {
704:   PrintU8StringTo(s, os);
705: }
706: #endif
```

```cpp
693: 
694: // Overloads for ::std::string.
695: GTEST_API_ void PrintStringTo(const ::std::string& s, ::std::ostream* os);
696: inline void PrintTo(const ::std::string& s, ::std::ostream* os) {
697:   PrintStringTo(s, os);
698: }
699: 
700: // Overloads for ::std::u8string
701: #ifdef __cpp_lib_char8_t
702: GTEST_API_ void PrintU8StringTo(const ::std::u8string& s, ::std::ostream* os);
703: inline void PrintTo(const ::std::u8string& s, ::std::ostream* os) {
704:   PrintU8StringTo(s, os);
705: }
706: #endif
707: 
708: // Overloads for ::std::u16string
```

## 边 18：usr/include/notification-ex/shared_file.h

SHA256 `956b7e2ecc8e088ac60320ba4b9c7e78fa40b6fa81870e486d9b7618b40dd380`；RPM：`notification-ex-devel`。

```cpp
41:   virtual ~SharedFile();
42: 
43:   bool IsPrivatePath(std::string path) const;
44:   std::string GetDataPath(std::string app_id, std::string path) const;
45:   int SetPrivateSharing(std::list<std::shared_ptr<AbstractItem>> notiList,
46:           std::multimap<std::string, std::string> receiver_group_map);
47:   int UpdatePrivateSharing(std::list<std::shared_ptr<AbstractItem>> notiList,
48:           std::multimap<std::string, std::string> receiver_group_map);
49:   int RemovePrivateSharing(std::list<std::shared_ptr<AbstractItem>> notiList,
50:           std::multimap<std::string, std::string> receiver_group_map);
51:   static int CopyPrivateFile(std::shared_ptr<item::AbstractItem>added_item);
52: 
53:  private:
54:   class SharingData {
55:    public:
56:     SharingData();
```

## 边 19：usr/include/zypp/ResPool.h

SHA256 `e43c7870a235acb254603ad38535205f0eac7bce5cb0eb70d0bd6988b81a2427`；RPM：`libzypp-devel`。

```cpp
398:       /** Set the requested locales.
399:        * Languages to be supported by the system, e.g. language specific
400:        * packages to be installed.
401:        */
402:       void setRequestedLocales( const LocaleSet & locales_r );
403: 
404:       /** Add one \ref Locale to the set of requested locales.
405:        * Return \c true if \c locale_r was newly added to the set.
406:       */
407:       bool addRequestedLocale( const Locale & locale_r );
408: 
409:       /** Erase one \ref Locale from the set of requested locales.
410:       * Return \c false if \c locale_r was not found in the set.
411:        */
412:       bool eraseRequestedLocale( const Locale & locale_r );
413: 
```

```cpp
411:        */
412:       bool eraseRequestedLocale( const Locale & locale_r );
413: 
414:       /** Return the requested locales.
415:        * \see \ref setRequestedLocales
416:       */
417:       const LocaleSet & getRequestedLocales() const;
418: 
419:       /** Whether this \ref Locale is in the set of requested locales. */
420:       bool isRequestedLocale( const Locale & locale_r ) const;
421: 
422:       /** Get the set of available locales.
423:        * This is computed from the package data so it actually
424:        * represents all locales packages claim to support.
425:        */
426:       const LocaleSet & getAvailableLocales() const;
```

## 边 20：usr/include/zypp/ZConfig.h

SHA256 `76f5b2f8b0423f251362efac73413d9be562b3437eefb69e8918cc4888232192`；RPM：`libzypp-devel`。

```cpp
442:        *
443:        * \see \ref sat::SolvableType
444:        */
445:       //@{
446:       const std::set<std::string> & multiversionSpec() const;
447:       void multiversionSpec( std::set<std::string> new_r );
448:       void clearMultiversionSpec();
449:       void addMultiversionSpec( const std::string & name_r );
450:       void removeMultiversionSpec( const std::string & name_r );
451:       //@}
452: 
453:       /**
454:        * Path where zypp can find or create lock file (configPath()/locks)
455:        * \ingroup g_ZC_CONFIGFILES
456:        */
457:       Pathname locksFile() const;
```

```cpp
443:        * \see \ref sat::SolvableType
444:        */
445:       //@{
446:       const std::set<std::string> & multiversionSpec() const;
447:       void multiversionSpec( std::set<std::string> new_r );
448:       void clearMultiversionSpec();
449:       void addMultiversionSpec( const std::string & name_r );
450:       void removeMultiversionSpec( const std::string & name_r );
451:       //@}
452: 
453:       /**
454:        * Path where zypp can find or create lock file (configPath()/locks)
455:        * \ingroup g_ZC_CONFIGFILES
456:        */
457:       Pathname locksFile() const;
458: 
```

## 边 21：usr/include/zypp/CheckSum.h

SHA256 `2d90f1a4231894218e139d1da44d294c11475b2dbb1e21bbca21a2ba11342fda`；RPM：`libzypp-devel`。

```cpp
51: 
52:     /**
53:      * Reads the content of \param input_r and computes the checksum.
54:      */
55:     CheckSum( const std::string & type, std::istream & input_r );
56: 
57:     /** Ctor from temporary istream */
58:     CheckSum( const std::string & type, std::istream && input_r )
59:       : CheckSum( type, input_r )
60:     {}
61: 
62:   public:
63:     static const std::string & md5Type();
64:     static const std::string & shaType();
65:     static const std::string & sha1Type();
66:     static const std::string & sha224Type();
```

```cpp
80:     //@}
81: 
82:     /** \name Reads the content of \param input_r and computes the checksum. */
83:     //@{
84:     static CheckSum md5( std::istream & input_r )		{ return  CheckSum( md5Type(), input_r ); }
85:     static CheckSum sha( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
```

```cpp
81: 
82:     /** \name Reads the content of \param input_r and computes the checksum. */
83:     //@{
84:     static CheckSum md5( std::istream & input_r )		{ return  CheckSum( md5Type(), input_r ); }
85:     static CheckSum sha( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
96:     static CheckSum sha256( std::istream && input_r )		{ return  CheckSum( sha256Type(), input_r ); }
```

```cpp
82:     /** \name Reads the content of \param input_r and computes the checksum. */
83:     //@{
84:     static CheckSum md5( std::istream & input_r )		{ return  CheckSum( md5Type(), input_r ); }
85:     static CheckSum sha( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
96:     static CheckSum sha256( std::istream && input_r )		{ return  CheckSum( sha256Type(), input_r ); }
97:     static CheckSum sha384( std::istream && input_r )		{ return  CheckSum( sha384Type(), input_r ); }
```

```cpp
83:     //@{
84:     static CheckSum md5( std::istream & input_r )		{ return  CheckSum( md5Type(), input_r ); }
85:     static CheckSum sha( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
96:     static CheckSum sha256( std::istream && input_r )		{ return  CheckSum( sha256Type(), input_r ); }
97:     static CheckSum sha384( std::istream && input_r )		{ return  CheckSum( sha384Type(), input_r ); }
98:     static CheckSum sha512( std::istream && input_r )		{ return  CheckSum( sha512Type(), input_r ); }
```

```cpp
84:     static CheckSum md5( std::istream & input_r )		{ return  CheckSum( md5Type(), input_r ); }
85:     static CheckSum sha( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
96:     static CheckSum sha256( std::istream && input_r )		{ return  CheckSum( sha256Type(), input_r ); }
97:     static CheckSum sha384( std::istream && input_r )		{ return  CheckSum( sha384Type(), input_r ); }
98:     static CheckSum sha512( std::istream && input_r )		{ return  CheckSum( sha512Type(), input_r ); }
99:     //@}
```

```cpp
85:     static CheckSum sha( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
96:     static CheckSum sha256( std::istream && input_r )		{ return  CheckSum( sha256Type(), input_r ); }
97:     static CheckSum sha384( std::istream && input_r )		{ return  CheckSum( sha384Type(), input_r ); }
98:     static CheckSum sha512( std::istream && input_r )		{ return  CheckSum( sha512Type(), input_r ); }
99:     //@}
100: 
```

```cpp
86:     static CheckSum sha1( std::istream & input_r )		{ return  CheckSum( sha1Type(), input_r ); }
87:     static CheckSum sha224( std::istream & input_r )		{ return  CheckSum( sha224Type(), input_r ); }
88:     static CheckSum sha256( std::istream & input_r )		{ return  CheckSum( sha256Type(), input_r ); }
89:     static CheckSum sha384( std::istream & input_r )		{ return  CheckSum( sha384Type(), input_r ); }
90:     static CheckSum sha512( std::istream & input_r )		{ return  CheckSum( sha512Type(), input_r ); }
91: 
92:     static CheckSum md5( std::istream && input_r )		{ return  CheckSum( md5Type(), input_r ); }
93:     static CheckSum sha( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
94:     static CheckSum sha1( std::istream && input_r )		{ return  CheckSum( sha1Type(), input_r ); }
95:     static CheckSum sha224( std::istream && input_r )		{ return  CheckSum( sha224Type(), input_r ); }
96:     static CheckSum sha256( std::istream && input_r )		{ return  CheckSum( sha256Type(), input_r ); }
97:     static CheckSum sha384( std::istream && input_r )		{ return  CheckSum( sha384Type(), input_r ); }
98:     static CheckSum sha512( std::istream && input_r )		{ return  CheckSum( sha512Type(), input_r ); }
99:     //@}
100: 
101:     /** \name Reads the content of \param input_r and computes the checksum. */
```

## 边 22：usr/include/dali/devel-api/threading/conditional-wait.h

SHA256 `2eba76c4110dca0de24d5aef15f029a1d8496fd33e9c81a5f37594b5c3a7dc83`；RPM：`dali2-integration-devel`。

```cpp
129:    * @param[in] scope A pre-existing lock on the internal state of this object.
130:    * @param[in] timePoint Maximum time point to wait.
131:    * @pre scope must have been passed this ConditionalWait during its construction.
132:    */
133:   void WaitUntil(const ScopedLock& scope, TimePoint timePoint);
134: 
135:   /**
136:    * @brief Return the count of threads waiting for this conditional
137:    * @return count of waits
138:    */
139:   unsigned int GetWaitCount() const;
140: 
141: private:
142:   // Not implemented as ConditionalWait is not copyable
143:   ConditionalWait(const ConditionalWait&);
144:   const ConditionalWait& operator=(const ConditionalWait&);
```

## 边 23：usr/include/zypp/parser/xml/Reader.h

SHA256 `77cff7e7bb5e8cf67f28340c8899a96166f46ac54892a7c673f931e4568cd035`；RPM：`libzypp-devel`。

```cpp
95:     class Reader : private zypp::base::NonCopyable
96:     {
97:     public:
98:       /** Ctor. Setup xmlTextReader and advance to the 1st Node. */
99:       Reader( const InputStream & stream_r,
100:               const Validate & validate_r = Validate::none() );
101: 
102:       /** Dtor. */
103:       ~Reader();
104: 
105:     public:
106: 
107:       /**
108:        *  If the current node is not empty, advances the reader to the next
109:        *  node, and returns the value
110:        *
```

## 边 23：usr/include/zypp/base/InputStream.h

SHA256 `2b0a1f0246938dae67c0e006067ac2cb92bbbbff7ce64c36c64c6bf2a9bdde06`；RPM：`libzypp-devel`。

```cpp
37:    * Per default the name is "STDIN", the path to an input file
38:    * or empty.
39:    *
40:    * \code
41:    * void parse( const InputStream & input = InputStream() )
42:    * {
43:    *   // process input.stream() and refer to input.name()
44:    *   // in log messages.
45:    * }
46:    *
47:    * parse();                  // std::cin
48:    * parse( "/some/file" );    // file
49:    * parse( "/some/file.gz" ); // gziped file
50:    * std::istream & mystream;
51:    * parse( mystream );        // some existing stream
52:    * parse( InputStream( mystream,
```

```cpp
48:    * parse( "/some/file" );    // file
49:    * parse( "/some/file.gz" ); // gziped file
50:    * std::istream & mystream;
51:    * parse( mystream );        // some existing stream
52:    * parse( InputStream( mystream,
53:    *                     "my stream's name" ) );
54:    * \endcode
55:   */
56:   class InputStream
57:   {
58:   public:
59:     /** Default ctor providing \c std::cin. */
60:     InputStream();
61: 
62:     /** Ctor providing an aleady existig \c std::istream. */
63:     InputStream( std::istream & stream_r,
```

```cpp
56:   class InputStream
57:   {
58:   public:
59:     /** Default ctor providing \c std::cin. */
60:     InputStream();
61: 
62:     /** Ctor providing an aleady existig \c std::istream. */
63:     InputStream( std::istream & stream_r,
64:                  const std::string & name_r = std::string() );
65: 
66:     /** Ctor for reading a (gziped) file. */
67:     InputStream( const Pathname & file_r );
68: 
69:     /** Ctor for reading a (gziped) file. */
70:     InputStream( const Pathname & file_r,
71:                  const std::string & name_r );
```

```cpp
59:     /** Default ctor providing \c std::cin. */
60:     InputStream();
61: 
62:     /** Ctor providing an aleady existig \c std::istream. */
63:     InputStream( std::istream & stream_r,
64:                  const std::string & name_r = std::string() );
65: 
66:     /** Ctor for reading a (gziped) file. */
67:     InputStream( const Pathname & file_r );
68: 
69:     /** Ctor for reading a (gziped) file. */
70:     InputStream( const Pathname & file_r,
71:                  const std::string & name_r );
72: 
73:     /** Ctor for reading a (gziped) file. */
74:     InputStream( const std::string & file_r );
```

```cpp
63:     InputStream( std::istream & stream_r,
64:                  const std::string & name_r = std::string() );
65: 
66:     /** Ctor for reading a (gziped) file. */
67:     InputStream( const Pathname & file_r );
68: 
69:     /** Ctor for reading a (gziped) file. */
70:     InputStream( const Pathname & file_r,
71:                  const std::string & name_r );
72: 
73:     /** Ctor for reading a (gziped) file. */
74:     InputStream( const std::string & file_r );
75: 
76:     /** Ctor for reading a (gziped) file. */
77:     InputStream( const std::string & file_r,
78:                  const std::string & name_r );
```

```cpp
66:     /** Ctor for reading a (gziped) file. */
67:     InputStream( const Pathname & file_r );
68: 
69:     /** Ctor for reading a (gziped) file. */
70:     InputStream( const Pathname & file_r,
71:                  const std::string & name_r );
72: 
73:     /** Ctor for reading a (gziped) file. */
74:     InputStream( const std::string & file_r );
75: 
76:     /** Ctor for reading a (gziped) file. */
77:     InputStream( const std::string & file_r,
78:                  const std::string & name_r );
79: 
80:     /** Ctor for reading a (gziped) file. */
81:     InputStream( const char * file_r );
```

```cpp
70:     InputStream( const Pathname & file_r,
71:                  const std::string & name_r );
72: 
73:     /** Ctor for reading a (gziped) file. */
74:     InputStream( const std::string & file_r );
75: 
76:     /** Ctor for reading a (gziped) file. */
77:     InputStream( const std::string & file_r,
78:                  const std::string & name_r );
79: 
80:     /** Ctor for reading a (gziped) file. */
81:     InputStream( const char * file_r );
82: 
83:     /** Ctor for reading a (gziped) file. */
84:     InputStream( const char * file_r,
85:                  const std::string & name_r );
```

```cpp
73:     /** Ctor for reading a (gziped) file. */
74:     InputStream( const std::string & file_r );
75: 
76:     /** Ctor for reading a (gziped) file. */
77:     InputStream( const std::string & file_r,
78:                  const std::string & name_r );
79: 
80:     /** Ctor for reading a (gziped) file. */
81:     InputStream( const char * file_r );
82: 
83:     /** Ctor for reading a (gziped) file. */
84:     InputStream( const char * file_r,
85:                  const std::string & name_r );
86: 
87:     /** Dtor. */
88:     ~InputStream();
```

```cpp
77:     InputStream( const std::string & file_r,
78:                  const std::string & name_r );
79: 
80:     /** Ctor for reading a (gziped) file. */
81:     InputStream( const char * file_r );
82: 
83:     /** Ctor for reading a (gziped) file. */
84:     InputStream( const char * file_r,
85:                  const std::string & name_r );
86: 
87:     /** Dtor. */
88:     ~InputStream();
89: 
90:     /** The std::istream.
91:      * \note The provided std::istream is never \c const.
92:     */
```

```cpp
80:     /** Ctor for reading a (gziped) file. */
81:     InputStream( const char * file_r );
82: 
83:     /** Ctor for reading a (gziped) file. */
84:     InputStream( const char * file_r,
85:                  const std::string & name_r );
86: 
87:     /** Dtor. */
88:     ~InputStream();
89: 
90:     /** The std::istream.
91:      * \note The provided std::istream is never \c const.
92:     */
93:     std::istream & stream() const
94:     { return *_stream; }
95: 
```

```cpp
84:     InputStream( const char * file_r,
85:                  const std::string & name_r );
86: 
87:     /** Dtor. */
88:     ~InputStream();
89: 
90:     /** The std::istream.
91:      * \note The provided std::istream is never \c const.
92:     */
93:     std::istream & stream() const
94:     { return *_stream; }
95: 
96:     /** Allow implicit conversion to std::istream.*/
97:     operator std::istream &() const
98:     { return *_stream; }
99: 
```
