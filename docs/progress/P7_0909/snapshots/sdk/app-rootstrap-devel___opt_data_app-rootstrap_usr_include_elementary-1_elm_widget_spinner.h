#ifndef ELM_WIDGET_SPINNER_H
#define ELM_WIDGET_SPINNER_H

#include "Elementary.h"

#include <Eio.h>

/* DO NOT USE THIS HEADER UNLESS YOU ARE PREPARED FOR BREAKING OF YOUR
 * CODE. THIS IS ELEMENTARY'S INTERNAL WIDGET API (for now) AND IS NOT
 * FINAL. CALL elm_widget_api_check(ELM_INTERNAL_API_VERSION) TO CHECK
 * IT AT RUNTIME.
 */

/**
 * @internal
 * @addtogroup Widget
 * @{
 *
 * @section elm-spinner-class The Elementary Spinner Class
 *
 * Elementary, besides having the @ref Spinner widget, exposes its
 * foundation -- the Elementary Spinner Class -- in order to create other
 * widgets which are a spinner with some more logic on top.
 */

/**
 * Base layout smart data extended with spinner instance data.
 */

typedef enum _Elm_Spinner_Format_Type
{
   SPINNER_FORMAT_FLOAT,
   SPINNER_FORMAT_INT,
   SPINNER_FORMAT_INVALID
} Elm_Spinner_Format_Type;

typedef struct _Elm_Spinner_Data    Elm_Spinner_Data;
struct _Elm_Spinner_Data
{
   Evas_Object          *ent, *inc_button, *dec_button, *text_button;
   //TIZEN_ONLY(20191210): List Spinner feature
   Evas_Object          *genlist, *g_rect;
   Elm_Genlist_Item_Class *itc;
   Elm_Object_Item      *aligned_item, *next_item, *prev_item;
   Elm_Object_Item      *access_aligned_item, *access_next_item, *access_prev_item;
   //
   //TIZEN_ONLY(20200413): flick to open feature
   Evas_Object           *g_layer;
   //
   const char           *label;
   double                val, val_min, val_max, val_base;
   double                step; /**< step for the value change. 1 by default. */
   double                drag_prev_pos, drag_val_step;
   double                spin_speed, interval, first_interval;
   //TIZEN_ONLY(20200413): flick to open feature
   int                   flick_dir;
   //
   int                   round;
   int                   decimal_points;
   Ecore_Timer          *delay_change_timer; /**< a timer for a delay,changed smart callback */
   Ecore_Timer          *spin_timer; /**< a timer for a repeated spinner value change on mouse down */
   Ecore_Timer          *longpress_timer; /**< a timer to detect long press. After longpress timeout,
                                          start continuous change of values until mouse up */
   Eina_List            *special_values;
   //TIZEN_ONLY(20191226): List Spinner feature
   Eina_List            *genlist_items;
   //
   Elm_Spinner_Format_Type format_type;

   Eina_Bool             entry_visible : 1;
   Eina_Bool             entry_reactivate : 1;
   Eina_Bool             dragging : 1;
   Eina_Bool             editable : 1;
   Eina_Bool             wrap : 1;
   Eina_Bool             val_updated : 1;
   Eina_Bool             button_layout : 1;
   Eina_Bool             inc_btn_activated : 1;
   //TIZEN_ONLY(20191210): List Spinner feature
   Eina_Bool             list_spinner : 1;
   Eina_Bool             scrolling: 1;
   Eina_Bool             genlist_visible : 1;
   //
};

typedef struct _Elm_Spinner_Special_Value Elm_Spinner_Special_Value;
struct _Elm_Spinner_Special_Value
{
   double      value;
   const char *label;
};

//TIZEN_ONLY(20191210): List Spinner feature
typedef struct _Elm_Spinner_Item_Data    Elm_Spinner_Item_Data;
struct _Elm_Spinner_Item_Data
{
   double                     value;
   Elm_Spinner_Data          *spinner_data;
};
//

/**
 * @}
 */

#define ELM_SPINNER_DATA_GET(o, sd) \
  Elm_Spinner_Data * sd = efl_data_scope_get(o, ELM_SPINNER_CLASS)

#define ELM_SPINNER_DATA_GET_OR_RETURN(o, ptr)       \
  ELM_SPINNER_DATA_GET(o, ptr);                      \
  if (EINA_UNLIKELY(!ptr))                           \
    {                                                \
       ERR("No widget data for object %p (%s)",      \
           o, evas_object_type_get(o));              \
       return;                                       \
    }

#define ELM_SPINNER_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  ELM_SPINNER_DATA_GET(o, ptr);                         \
  if (EINA_UNLIKELY(!ptr))                              \
    {                                                   \
       ERR("No widget data for object %p (%s)",         \
           o, evas_object_type_get(o));                 \
       return val;                                      \
    }

#define ELM_SPINNER_CHECK(obj)                              \
  if (EINA_UNLIKELY(!efl_isa((obj), ELM_SPINNER_CLASS))) \
    return

#endif
