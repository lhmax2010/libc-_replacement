/*
 * Copyright (c) 2012, 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_CAPI_NETWORK_SESSION_H__
#define __TIZEN_CAPI_NETWORK_SESSION_H__

/* standard library header */
#include <gio/gio.h>

/* SLP library header */

/* local header */
#include "smartcard-types.h"
#ifdef __cplusplus
#include "SessionHelper.h"
#endif /* __cplusplus */

/* LCOV_EXCL_START */

#ifdef __cplusplus
namespace smartcard_service_api
{
	class Reader;

	class Session:public SessionHelper
	{
private:
		void *context;
		void *handle;
		void *proxy;

		Session(void *context, Reader *reader, void *handle);
		~Session();

		int openChannel(int id, const ByteArray &aid, openChannelCallback callback, void *userData);

		Channel *openChannelSync(int id, const ByteArray &aid);

		Channel *openChannelSync(int id, const ByteArray &aid, unsigned char P2);

		static void session_get_atr_cb(GObject *source_object,
			GAsyncResult *res, gpointer user_data);
		static void session_open_channel_cb(GObject *source_object,
			GAsyncResult *res, gpointer user_data);
		static void session_close_cb(GObject *source_object,
			GAsyncResult *res, gpointer user_data);

public:
		void closeChannels();

		int getATR(getATRCallback callback, void *userData);
		int close(closeSessionCallback callback, void *userData);

		int openBasicChannel(const ByteArray &aid, openChannelCallback callback, void *userData);
		int openBasicChannel(const unsigned char *aid, unsigned int length, openChannelCallback callback, void *userData);
		int openLogicalChannel(const ByteArray &aid, openChannelCallback callback, void *userData);
		int openLogicalChannel(const unsigned char *aid, unsigned int length, openChannelCallback callback, void *userData);

		const ByteArray getATRSync();

		void closeSync();

		Channel *openBasicChannelSync(const ByteArray &aid);

		Channel *openBasicChannelSync(const unsigned char *aid, unsigned int length);

		Channel *openLogicalChannelSync(const ByteArray &aid);

		Channel *openLogicalChannelSync(const unsigned char *aid, unsigned int length);

		Channel *openBasicChannelSync(const ByteArray &aid, unsigned char P2);

		Channel *openBasicChannelSync(const unsigned char *aid, unsigned int length, unsigned char P2);

		Channel *openLogicalChannelSync(const ByteArray &aid, unsigned char P2);

		Channel *openLogicalChannelSync(const unsigned char *aid, unsigned int length, unsigned char P2);

		size_t getChannelCount() const;
		void *getHandle() { return handle; }

		friend class Reader;
	};
} /* namespace smartcard_service_api */
#endif /* __cplusplus */

/* export C API */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int session_get_reader(session_h handle, int* reader_handle);
int session_is_closed(session_h handle, bool* is_closed);
int session_close_channels(session_h handle);
int session_get_atr_sync(session_h handle, unsigned char **buffer, unsigned int *length);
int session_close_sync(session_h handle);
int session_open_basic_channel_sync(session_h handle, unsigned char *aid,
	unsigned int length, unsigned char P2, int* channel_handle);
int session_open_logical_channel_sync(session_h handle, unsigned char *aid,
	unsigned int length, unsigned char P2, int* channel_handle);
///

int session_get_atr(session_h handle, session_get_atr_cb callback,
	void *userData);
int session_close(session_h handle, session_close_session_cb callback,
	void *userData);
int session_open_basic_channel(session_h handle, unsigned char *aid,
	unsigned int length, session_open_channel_cb callback, void *userData);
int session_open_logical_channel(session_h handle, unsigned char *aid,
	unsigned int length, session_open_channel_cb callback, void *userData);
size_t session_get_channel_count(session_h handle);
__attribute__((deprecated)) void session_destroy_instance(session_h handle);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_CAPI_NETWORK_SESSION_H__ */

/* LCOV_EXCL_STOP */

