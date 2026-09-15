// Copyright 2019 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __EWK_INTERFACE_MAIN__
#define __EWK_INTERFACE_MAIN__

namespace ewk_interface {

extern void* g_impl_lib_handle;
extern void* open_library();

}  // namespace ewk_interface

#if defined(SAMSUNG_NEXT_BROWSER)
void* ewk_dlsym(const char* function_name);
#endif
#endif  //__EWK_INTERFACE_MAIN__
