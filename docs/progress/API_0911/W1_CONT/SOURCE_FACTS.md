# 真实源码的核查摘录

源文件未修改。完整归档/成员身份见 source_members.json、extension_source_members.json；此处保留原始行号，便于独立复核。

## EWK implementation

源路径：`/home/toolchain/development/libcxx_runtime_validation/tmp/API_0911/W1_CONT/chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc`；SHA256：`a903f1a977ce4553339710fa0dfa0424a53479d6545629911a54537861d24d80`。

```text
1: /**
2:  * @file  ewk_cookie_parser
3:  * @brief EWK Cookie Parser
4:  *
5:  * This class exposes the Chromium cookie parser. It allows
6:  * for ewk components to handle cookie-alike structures without
7:  * re-inventing the wheel.
8:  *
9:  * Copyright 2020 by Samsung Electronics, Inc.,
10:  *
11:  * This software is the confidential and proprietary information
12:  * of Samsung Electronics, Inc. ("Confidential Information").  You
13:  * shall not disclose such Confidential Information and shall use
14:  * it only in accordance with the terms of the license agreement
15:  * you entered into with Samsung.
16:  */
17:
18: #include "ewk_cookie_parser.h"
19:
20: #include <ctime>
21: #include <string>
22: #include "net/cookies/cookie_util.h"
23: #include "net/cookies/parsed_cookie.h"
24: #include "private/ewk_private.h"
25:
26: #ifdef __cplusplus
27: extern "C" {
28: #endif
29:
30: EXPORT_API Eina_Bool ewk_parse_cookie(const std::string& cookie_str,
31:                                       EWKCookieContents& cookie) {
32:   net::ParsedCookie new_cookie(cookie_str);
33:   if (!new_cookie.IsValid() || new_cookie.IsHttpOnly() || new_cookie.IsSecure())
34:     return EINA_FALSE;
35:
36:   cookie.name = new_cookie.Name();
37:   cookie.value = new_cookie.Value();
38:
39:   // Handle optional domain value
40:   auto domain = new_cookie.Domain();
41:   if (domain.has_value()) {
42:     cookie.domain = domain.value();
43:   }
44:
45:   // Handle optional path value
46:   auto path = new_cookie.Path();
47:   if (path.has_value()) {
48:     cookie.path = path.value();
49:   }
50:
51:   // Take "max-age" over "Expires" as expires is depreciated, so only supported
52:   // for backwards compatibility. With the assumption that if you are using both
53:   // then "Expires" is there to support old browsers. (see HTTP - spec).
54:   auto max_age = new_cookie.MaxAge();
55:   if (max_age.has_value()) {
56:     // Convert string to integer without using exceptions
57:     std::string max_age_str(max_age.value());
58:     char* end_ptr;
59:     long max_age_val = std::strtol(max_age_str.c_str(), &end_ptr, 10);
60:
61:     // Check if conversion was successful (entire string was consumed)
62:     if (end_ptr != max_age_str.c_str() && *end_ptr == '\0' &&
63:         max_age_val >= 0) {
64:       cookie.expiry_date_utc = time(nullptr) + static_cast<time_t>(max_age_val);
65:     } else {
66:       // If conversion fails, treat as session cookie
67:       cookie.expiry_date_utc = 0;
68:     }
69:   } else {
70:     auto expires = new_cookie.Expires();
71:     if (expires.has_value()) {
72:       base::Time expiration_time =
73:           net::cookie_util::ParseCookieExpirationTime(expires.value());
74:       cookie.expiry_date_utc = expiration_time.ToTimeT();
75:     } else {
76:       // This is a session cookie. Set expiry time to 0
77:       cookie.expiry_date_utc = 0;
78:     }
79:   }
80:   return EINA_TRUE;
81: }
82:
83: #ifdef __cplusplus
84: }
85: #endif
```

## Browser callback and receiver

源路径：`/home/toolchain/development/libcxx_runtime_validation/tmp/API_0911/W1_CONT/chromium_extension_source/chromium-efl-1.1.144/tizen_src/chromium_impl/components/xwalk_extensions/browser/xwalk_extension.cc`；SHA256：`3a1407ad8c46a1377e868832a2aa9027b2cbde61251763d1950923fe817ed87e`。

```text
115:
116:   static void SyncMessagingSetSyncReply(
117:       XW_Instance xw_instance, const char* reply) {
118:     auto* instance = GetExtensionInstance(xw_instance);
119:     if (instance)
120:       instance->SyncReplyToJS(reply);
121:   }
122:
123:   static void SyncMessagingSetSyncBinaryReply(
124:       XW_Instance xw_instance, const std::vector<uint8_t>& reply) {
125:     auto* instance = GetExtensionInstance(xw_instance);
126:     if (instance)
127:       instance->SyncBinaryReplyToJS(reply);
128:   }
129:
130:   static void EntryPointsSetExtraJSEntryPoints(
131:       XW_Extension xw_extension, const char** entry_points) {
132:     XWalkExtension* extension = GetExtension(xw_extension);
250:   if (!callback)
251:     return std::string();
252:
253:   callback(xw_instance_, message.c_str());
254:   return std::move(sync_reply_);
255: }
256:
257: // sendSyncMessage(message: string): Uint8Array
258: std::vector<uint8_t> XWalkExtension::Instance::SendSyncMessageWithBinaryReply(
259:     const std::string& message) {
260: #if BUILDFLAG(IS_TIZEN_TV)
261:   LOG(INFO) << "[WebAPI] message:" << message;
262: #endif
263:   XW_HandleSyncMessageCallback callback =
264:       extension_->handle_sync_msg_callback_;
265:   if (!callback)
266:     return std::vector<uint8_t>{};
267:
268:   callback(xw_instance_, message.c_str());
269:   return std::move(sync_binary_reply_);
330:   if (post_binary_message_callback_)
331:     post_binary_message_callback_(message, size);
332: }
333:
334: void XWalkExtension::Instance::SyncReplyToJS(const std::string& reply) {
335:   sync_reply_ = reply;
336: }
337:
338: void XWalkExtension::Instance::SyncBinaryReplyToJS(
339:     const std::vector<uint8_t>& reply) {
340:   sync_binary_reply_ = reply;
341: }
342:
343: XWalkExtension::XWalkExtension(const std::string& path)
344:     : lazy_loading_(false), library_path_(path) {}
345:
395:     return &messagingInterface2;
396:   }
397:
398:   if (interface_name == XW_INTERNAL_SYNC_MESSAGING_INTERFACE_1) {
399:     static const XW_Internal_SyncMessagingInterface_1
400:         syncMessagingInterface1 = {
401:       Interface::SyncMessagingRegister,
402:       Interface::SyncMessagingSetSyncReply,
403:       Interface::SyncMessagingSetSyncBinaryReply
404:     };
405:     return &syncMessagingInterface1;
406:   }
407:
408:   if (interface_name == XW_INTERNAL_ENTRY_POINTS_INTERFACE_1) {
470:
471:   XW_Initialize_Func initialize = reinterpret_cast<XW_Initialize_Func>(
472:       dlsym(handle_, "XW_Initialize"));
473:   if (!initialize) {
474:     LOG(ERROR) << "Error loading extension '" << library_path
475:                << "' : couldn't get XW_Initialize function.";
476:     dlclose(handle_);
477:     return false;
478:   }
479:
480:   xw_extension_ = ++last_xw_extension_id_;
481:   int ret = initialize(xw_extension_, GetInterface);
482:   if (ret != XW_OK) {
483:     LOG(ERROR) << "Error loading extension '" << library_path
484:                << "' : XW_Initialize() returned error value.";
485:     dlclose(handle_);
486:     return false;
487:   }
488:
489:   initialized_ = true;
490:   return true;
491: }
492:
493: XWalkExtension::Instance* XWalkExtension::CreateInstance() {
494:   base::AutoLock lock(instance_lock);
495:   auto id = XWalkExtensionManager::GetInstance()->GetNextInstanceID();
496:   if (id == 0) {
497:     LOG(ERROR) << "Exceed xwalk extension limit";
498:     return nullptr;
499:   }
500:   Initialize(true);
501:   return new Instance(this, id);
502: }
503:
```

## Service callback is not the browser implementation

源路径：`/home/toolchain/development/libcxx_runtime_validation/tmp/API_0911/W1_CONT/chromium_source/chromium-efl-1.1.144/wrt/src/service/node/xwalk_extension_node.cc`；SHA256：`48b402353a4aa1f851895561dd686243efb2a550f0e12bf2a3d24732a8f022a6`。

```text
134:   if (extension_instance) {
135:     extension_instance->SyncReplyToJS(reply);
136:   }
137: }
138:
139: void SyncMessagingSetSyncBinaryReply(XW_Instance xw_instance_id,
140:                                      const std::vector<uint8_t>& reply) {
141:   LOG(INFO) << "Not supported in service application";
142: }
143:
144: void EntryPointsSetExtraJSEntryPoints(XW_Extension xw_extension_id,
145:                                       const char** entry_points) {
146:   auto* extension_interface = GetExtensionInterface(xw_extension_id);
```
