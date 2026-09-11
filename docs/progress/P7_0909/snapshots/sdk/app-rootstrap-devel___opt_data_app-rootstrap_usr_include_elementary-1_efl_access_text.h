#ifndef ELM_INTERFACE_ATSPI_TEXT_H
#define ELM_INTERFACE_ATSPI_TEXT_H

// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#ifdef EFL_BETA_API_SUPPORT
//

// TIZEN_ONLY(20190916): efl_access_text: enable EFL_BETA_API_SUPPORT define to use beta apis
#ifndef EFL_BETA_API_SUPPORT
 #define EFL_BETA_API_SUPPORT
 #include "efl_access_text.eo.h"
 #undef EFL_BETA_API_SUPPORT
#else
 #include "efl_access_text.eo.h"
#endif
//#include "efl_access_text.eo.h"
//

/**
 * @internal
 *
 * @brief Free Efl_Access_Text_Attribute structure
 */
EAPI void elm_atspi_text_text_attribute_free(Efl_Access_Text_Attribute *attr);

/**
 * @internal
 *
 * @brief Free Efl_Access_Text_Range structure
 */
EAPI void elm_atspi_text_text_range_free(Efl_Access_Text_Range *range);

#endif
// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#endif
//
