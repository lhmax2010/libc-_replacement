/*
 * libfeedback
 * Copyright (c) 2000-2020 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance with the terms of the license agreement
 * you entered into with SAMSUNG ELECTRONICS.
 * SAMSUNG make no representations or warranties about the suitability
 * of the software, either express or implied, including but not
 * limited to the implied warranties of merchantability, fitness for
 * a particular purpose, or non-infringement.
 * SAMSUNG shall not be liable for any damages suffered by licensee as
 * a result of using, modifying or distributing this software or its derivatives.
 */


#ifndef __FEEDBACK_IDS_PRODUCT_H__
#define __FEEDBACK_IDS_PRODUCT_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_FEEDBACK_MODULE
 * @{
 */

/**
 * @brief Enumeration for feedback interface of the product pre-defined patterns.
 * @details Each feedback pattern can have separate media files of each types.
 *          But depending on vendor design, pattern may not have any type of file.
 * @since_tizen 4.0.0.4
 */
typedef enum {
	FEEDBACK_PATTERN_BREATHING_EXHALE = 12000,    /**< feedback pattern for breathing exhale */
	FEEDBACK_PATTERN_BOS, /**< feedback pattern for bixby begin of speech */
	FEEDBACK_PATTERN_EOS, /**< feedback pattern for bixby end of speech */
	FEEDBACK_PATTERN_UDS, /**< feedback pattern for bixby undetected speech */
} feedback_pattern_product_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_PRODUCT_H__
