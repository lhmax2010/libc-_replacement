/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_APPFW_PACKAGE_INFO_INTERNAL_H
#define __TIZEN_APPFW_PACKAGE_INFO_INTERNAL_H

#include <package_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks whether the package is preloaded.
 * @since_tizen 9.0
 * @param[in]  package_info The package information
 * @param[out] update       The update info of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_is_update_package(package_info_h package_info, bool *update);


#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_PACKAGE_INFO_INTERNAL_H */