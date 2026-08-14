/*
 * Copyright (c) 2014-2022 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * This file is licensed under the terms of MIT License or the Apache License
 * Version 2.0 of your choice. See the LICENSE.MIT file for MIT license details.
 * See the LICENSE file or the notice below for Apache License Version 2.0
 * details.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */
/**
 * @file        src/common/exceptions/TryCatch.h
 * @author      Tomasz Swierczek <t.swierczek@samsung.com>
 * @version     2.0
 * @brief       This file contains a declaration of a function for catching exceptions
 */

#ifndef SRC_COMMON_EXCEPTIONS_TRYCATCH_H_
#define SRC_COMMON_EXCEPTIONS_TRYCATCH_H_


#include <cxxabi.h>
#include <exception>
#include <functional>
#include <new>

#include <exceptions/AccessDeniedException.h>
#include <exceptions/InvalidProtocolException.h>
#include <exceptions/NoMemoryException.h>
#include <log/log.h>

#include <cynara-error.h>

namespace Cynara {

template <class F>
int tryCatch(F &&f) {
    try {
        return std::forward<F>(f)();
    } catch (const std::bad_alloc &e) {
        LOGE_NOTHROW("%s", e.what());
        return CYNARA_API_OUT_OF_MEMORY;
    } catch (const NoMemoryException &e) {
        LOGE_NOTHROW("%s", e.what());
        return CYNARA_API_OUT_OF_MEMORY;
    } catch (const InvalidProtocolException &e) {
        LOGE_NOTHROW("%s", e.what());
        return CYNARA_API_INVALID_PARAM;
    } catch (const AccessDeniedException &e) {
        LOGE_NOTHROW("%s", e.what());
        return CYNARA_API_PERMISSION_DENIED;
    } catch (const std::exception &e) {
        LOGE_NOTHROW("%s", e.what());
        return CYNARA_API_UNKNOWN_ERROR;
#if defined(__GLIBCXX__)
    } catch (const abi::__forced_unwind &) {
        /**
         * workaround for pthread_cancel, which cancels thread using this exception
         * and expects it to be rethrown in every try-catch
         */
        LOGD_NOTHROW("We are 'gracefully' stopped by pthread_cancel");
        throw;
#endif
    } catch (...) {
        LOGE_NOTHROW("Unexpected exception");
        return CYNARA_API_UNKNOWN_ERROR;
    }
}

} // namespace Cynara

#endif // SRC_COMMON_EXCEPTIONS_TRYCATCH_H_
