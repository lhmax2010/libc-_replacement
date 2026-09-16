/**
 * @file  ewk_cookie_parser
 * @brief EWK Cookie Parser
 *
 * This class exposes the Chromium cookie parser. It allows
 * for ewk components to handle cookie-alike structures without
 * re-inventing the wheel.
 *
 * Copyright 2020 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include "ewk_cookie_parser.h"

#include <ctime>
#include <string>
#include "net/cookies/cookie_util.h"
#include "net/cookies/parsed_cookie.h"
#include "private/ewk_private.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_API Eina_Bool ewk_parse_cookie(const std::string& cookie_str,
                                      EWKCookieContents& cookie) {
  net::ParsedCookie new_cookie(cookie_str);
  if (!new_cookie.IsValid() || new_cookie.IsHttpOnly() || new_cookie.IsSecure())
    return EINA_FALSE;

  cookie.name = new_cookie.Name();
  cookie.value = new_cookie.Value();

  // Handle optional domain value
  auto domain = new_cookie.Domain();
  if (domain.has_value()) {
    cookie.domain = domain.value();
  }

  // Handle optional path value
  auto path = new_cookie.Path();
  if (path.has_value()) {
    cookie.path = path.value();
  }

  // Take "max-age" over "Expires" as expires is depreciated, so only supported
  // for backwards compatibility. With the assumption that if you are using both
  // then "Expires" is there to support old browsers. (see HTTP - spec).
  auto max_age = new_cookie.MaxAge();
  if (max_age.has_value()) {
    // Convert string to integer without using exceptions
    std::string max_age_str(max_age.value());
    char* end_ptr;
    long max_age_val = std::strtol(max_age_str.c_str(), &end_ptr, 10);

    // Check if conversion was successful (entire string was consumed)
    if (end_ptr != max_age_str.c_str() && *end_ptr == '\0' &&
        max_age_val >= 0) {
      cookie.expiry_date_utc = time(nullptr) + static_cast<time_t>(max_age_val);
    } else {
      // If conversion fails, treat as session cookie
      cookie.expiry_date_utc = 0;
    }
  } else {
    auto expires = new_cookie.Expires();
    if (expires.has_value()) {
      base::Time expiration_time =
          net::cookie_util::ParseCookieExpirationTime(expires.value());
      cookie.expiry_date_utc = expiration_time.ToTimeT();
    } else {
      // This is a session cookie. Set expiry time to 0
      cookie.expiry_date_utc = 0;
    }
  }
  return EINA_TRUE;
}

#ifdef __cplusplus
}
#endif
