#ifndef _ECORE_AUDIO_OUT_TIZEN_EO_H_
#define _ECORE_AUDIO_OUT_TIZEN_EO_H_

#ifndef _ECORE_AUDIO_OUT_TIZEN_EO_CLASS_TYPE
#define _ECORE_AUDIO_OUT_TIZEN_EO_CLASS_TYPE

typedef Eo Ecore_Audio_Out_Tizen;

#endif

#ifndef _ECORE_AUDIO_OUT_TIZEN_EO_TYPES
#define _ECORE_AUDIO_OUT_TIZEN_EO_TYPES


#endif
#ifdef EFL_BETA_API_SUPPORT
#define ECORE_AUDIO_OUT_TIZEN_CLASS ecore_audio_out_tizen_class_get()

EWAPI const Efl_Class *ecore_audio_out_tizen_class_get(void) EINA_CONST;
#endif /* EFL_BETA_API_SUPPORT */

#endif
