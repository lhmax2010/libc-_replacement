#pragma once
#include <ewk_cookie_parser.h>
// Built for the caller ABI; the original wrapper passes references only.
extern "C" Eina_Bool p12_ewk_parse_cookie(const std::string&, EWKCookieContents&);
