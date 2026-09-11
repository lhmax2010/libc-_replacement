#ifndef ELM_INTEFARCE_SCROLLER_H
#define ELM_INTEFARCE_SCROLLER_H

/**
 * @internal
 * @addtogroup Widget
 * @{
 *
 * @section elm-scrollable-interface The Elementary Scrollable Interface
 *
 * This is a common interface for widgets having @b scrollable views.
 * Widgets using/implementing this must use the
 * @c EVAS_SMART_SUBCLASS_IFACE_NEW macro (instead of the
 * @c EVAS_SMART_SUBCLASS_NEW one) when declaring its smart class,
 * so an interface is also declared.
 *
 * The scrollable interface comes built with Elementary and is exposed
 * as #ELM_SCROLLABLE_IFACE.
 *
 * The interface API is explained in details at
 * #Elm_Scrollable_Smart_Interface.
 *
 * An Elementary scrollable interface will handle an internal @b
 * panning object. It has the function of clipping and moving the
 * actual scrollable content around, by the command of the scrollable
 * interface calls. Though it's not the common case, one might
 * want/have to change some aspects of the internal panning object
 * behavior.  For that, we have it also exposed here --
 * #Elm_Pan_Smart_Class. Use elm_pan_smart_class_get() to build your
 * custom panning object, when creating a scrollable widget (again,
 * only if you need a custom panning object) and set it with
 * Elm_Scrollable_Smart_Interface::extern_pan_set.
 */

#include <elm_pan_eo.legacy.h>
#include <elm_scroller.h>

/**
 * Elementary scroller panning base smart data.
 */
typedef struct _Elm_Pan_Smart_Data Elm_Pan_Smart_Data;
struct _Elm_Pan_Smart_Data
{
   Evas_Object                   *self;
   Evas_Object                   *content;
   Evas_Object                   *interface_object;
   Evas_Coord                     x, y, w, h;
   Evas_Coord                     content_w, content_h, px, py;
};

/**
 * Elementary scrollable interface base data.
 */
typedef void      (*Elm_Interface_Scrollable_Cb)(Evas_Object *, void *data);
typedef void      (*Elm_Interface_Scrollable_Min_Limit_Cb)(Evas_Object *obj, Eina_Bool w, Eina_Bool h);
typedef void      (*Elm_Interface_Scrollable_Resize_Cb)(Evas_Object *obj, Evas_Coord w, Evas_Coord h);
//TIZEN_ONLY(20190529): Scroll align feature
typedef Eina_Bool (*Elm_Interface_Scrollable_Item_Scroll_Aligned_Get_Cb)(Evas_Object *obj, void *data);
//
typedef struct _Elm_Scrollable_Smart_Interface_Data
  Elm_Scrollable_Smart_Interface_Data;

#include "elm_interface_scrollable.eo.h"

struct _Elm_Scrollable_Smart_Interface_Data
{
   Evas_Coord                    x, y, w, h;
   Evas_Coord                    wx, wy, ww, wh; /**< Last "wanted" geometry */

   Evas_Object                  *obj;
   Evas_Object                  *content;
   Evas_Object                  *pan_obj;
   Evas_Object                  *edje_obj;
   Evas_Object                  *event_rect;

   Evas_Object                  *parent_widget;

   Elm_Scroller_Policy           hbar_flags, vbar_flags;
   Elm_Scroller_Single_Direction one_direction_at_a_time;
   Efl_Ui_Layout_Orientation     block;

//TIZEN_ONLY(20190710): Tunning scroll in case user wants to move just 1 item.
   float velocity_under_threshold;
//

   struct
   {
      Evas_Coord x, y;
      Evas_Coord sx, sy;
      Evas_Coord dx, dy;
      Evas_Coord pdx, pdy;
      Evas_Coord bx, by;
      Evas_Coord ax, ay;
      Evas_Coord bx0, by0;
      Evas_Coord b0x, b0y;
      Evas_Coord b2x, b2y;

      struct
      {
         Evas_Coord x, y;
         double     timestamp;
      } history[60];

      double est_timestamp_diff;

      double          dragged_began_timestamp;
      double          anim_start;
      double          anim_start2;
      double          anim_start3;
      double          anim_dur;

      double          onhold_vx, onhold_vy, onhold_tlast,
                      onhold_vxe, onhold_vye;
      // TIZEN_ONLY(20191231): Page flick
      double          extra_time;
      //
      double          last_time_x_wheel;
      double          last_time_y_wheel;

      Evas_Coord      hold_x, hold_y;
      Evas_Coord      locked_x, locked_y;
      int             hdir, vdir;

      Ecore_Idle_Enterer *hold_enterer;

      Eina_Bool       hold_animator : 1;
      Eina_Bool       onhold_animator : 1;
      Eina_Bool       momentum_animator : 1; /**< an animator which is called whenever a scroller is moving due to a flick action(mouse down, move, up) */
      Eina_Bool       bounce_x_animator : 1; /**< an animator to express the bouncing animation on x axis. */
      Eina_Bool       bounce_y_animator : 1; /**< an animator to express the bouncing animation on y axis. */

      Eina_Bool       last_hold_x_wheel : 1;
      Eina_Bool       last_hold_y_wheel : 1;
      Eina_Bool       bounce_x_hold : 1;
      Eina_Bool       bounce_y_hold : 1;
      Eina_Bool       dragged_began : 1;
      Eina_Bool       want_dragged : 1;
      Eina_Bool       hold_parent : 1;
      Eina_Bool       want_reset : 1;
      Eina_Bool       cancelled : 1;
      Eina_Bool       dragged : 1;
      Eina_Bool       locked : 1;
      Eina_Bool       scroll : 1;
      Eina_Bool       dir_x : 1;
      Eina_Bool       dir_y : 1;
      Eina_Bool       hold : 1;
      Eina_Bool       now : 1;
   } down;

// TIZEN_ONLY(20191231): Page flick
   struct
   {
      struct
      {
         Evas_Coord x, y;
         double   timestamp, localtimestamp;
      } history;
   } down2;

   struct
   {
      struct
      {
         Evas_Coord x, y;
         double   timestamp, localtimestamp;
      } history[500];
      int count;
   } move;
//

   struct
   {
      Evas_Coord w, h;
      Eina_Bool  resized : 1;
   } content_info;

   struct
   {
      Evas_Coord x, y;
   } step, page, current_page;

   struct
   {
      Elm_Interface_Scrollable_Cb drag_start;
      Elm_Interface_Scrollable_Cb drag_stop;
      Elm_Interface_Scrollable_Cb animate_start;
      Elm_Interface_Scrollable_Cb animate_stop;
      Elm_Interface_Scrollable_Cb scroll;
      Elm_Interface_Scrollable_Cb scroll_left;
      Elm_Interface_Scrollable_Cb scroll_right;
      Elm_Interface_Scrollable_Cb scroll_up;
      Elm_Interface_Scrollable_Cb scroll_down;
      Elm_Interface_Scrollable_Cb edge_left;
      Elm_Interface_Scrollable_Cb edge_right;
      Elm_Interface_Scrollable_Cb edge_top;
      Elm_Interface_Scrollable_Cb edge_bottom;
      Elm_Interface_Scrollable_Cb vbar_drag;
      Elm_Interface_Scrollable_Cb vbar_press;
      Elm_Interface_Scrollable_Cb vbar_unpress;
      Elm_Interface_Scrollable_Cb hbar_drag;
      Elm_Interface_Scrollable_Cb hbar_press;
      Elm_Interface_Scrollable_Cb hbar_unpress;
      Elm_Interface_Scrollable_Cb page_change;
      //TIZEN_ONLY(20190219): fix page snap behavior.
      Elm_Interface_Scrollable_Cb scroll_start;
      Elm_Interface_Scrollable_Cb scroll_stop;
      //

      Elm_Interface_Scrollable_Min_Limit_Cb content_min_limit;
      Elm_Interface_Scrollable_Resize_Cb content_viewport_resize;
      //TIZEN_ONLY(20190529): Scroll align feature
      Elm_Interface_Scrollable_Cb item_scroll_align_end_set;
      Elm_Interface_Scrollable_Cb item_scroll_align_end_unset;
      Elm_Interface_Scrollable_Item_Scroll_Aligned_Get_Cb item_scroll_aligned_get;
      //
   } cb_func;

   struct
   {
      struct
      {
         Evas_Coord      start, end;
         double          t_start, t_end;
         // TIZEN_ONLY(20191231): Page flick
         int             cancel;
         //
         Eina_Bool       animator;
      } x, y;
   } scrollto;

   double     pagerel_h, pagerel_v;
   // TIZEN_ONLY(20191231): Page flick
   double thumbscroll_friction;
   //
   Evas_Coord pagesize_h, pagesize_v;
   int        page_limit_h, page_limit_v;
   int        current_calc;

   double       last_wheel_mul;
   unsigned int last_wheel;

   unsigned char size_adjust_recurse;
   unsigned char size_count;
   void         *event_info;

   double                         gravity_x, gravity_y;
   Evas_Coord                     prev_cw, prev_ch;

   Eina_Bool  size_adjust_recurse_abort : 1;

   Eina_Bool  momentum_animator_disabled : 1;
   Eina_Bool  bounce_animator_disabled : 1;
   Eina_Bool  page_snap_horiz : 1;
   Eina_Bool  page_snap_vert : 1;
   Eina_Bool  wheel_disabled : 1;
   Eina_Bool  hbar_visible : 1;
   Eina_Bool  vbar_visible : 1;
   Eina_Bool  bounce_horiz : 1;
   Eina_Bool  bounce_vert : 1;
   Eina_Bool  is_mirrored : 1;
   Eina_Bool  extern_pan : 1;
   Eina_Bool  bouncemey : 1;
   Eina_Bool  bouncemex : 1;
   Eina_Bool  freeze : 1;
   Eina_Bool  freeze_want : 1;
   Eina_Bool  hold : 1;
   Eina_Bool  min_w : 1;
   Eina_Bool  min_h : 1;
   Eina_Bool  go_left : 1;
   Eina_Bool  go_right : 1;
   Eina_Bool  go_up : 1;
   Eina_Bool  go_down : 1;
   Eina_Bool  loop_h : 1;
   Eina_Bool  loop_v : 1;
   //TIZEN_ONLY(20190219): fix page snap behavior.
   Eina_Bool  scrolling : 1;
   //

   void *manager; /* Efl_Ui_Focus_Manager */

   //TIZEN_ONLY(20191209): Genlist looping implementation
   Eina_Bool  y_forward_scroll : 1;
   //
   // TIZEN_ONLY(20191231): Page flick
   //TIZEN_ONLY(20190529): Scroll align feature
   Eina_Bool  is_page_flick : 1;
   Eina_Bool  force_align : 1;
   //
   Eina_Bool  is_unset_cb_called : 1;
   //
   //TIZEN_ONLY(24Jan2020): Back To Top Feature
   Eina_Bool     is_backtotop_shown   : 1;
   Eina_Bool     is_backtotop_enabled : 1;
   Evas_Smart_Cb backtotop_cb;
   //
   //TIZEN_ONLY(20180118): Modify logics to make scroller stable on tizen
   struct
   {
      Ecore_Job *bar_size_adjust;
      Ecore_Job *bar_pos_adjust;
      Ecore_Job *page_adjust;
   } adjust_job;

   //TIZEN_ONLY(20200106): Support wearable rotary event.
   struct
   {
      int detent_count;
      int current_page;
   } rotary_animation_info;
   //

   ////FIXME FIXME FIXME: Would you please fix me ?
   struct
   {
      Evas_Coord h, v;
      int loop_cnt;
   } requested_page;
   //END
};

#define ELM_SCROLLABLE_CHECK(obj, ...)                                       \
                                                                             \
  if (!efl_isa(obj, ELM_INTERFACE_SCROLLABLE_MIXIN))                    \
    {                                                                        \
       ERR("The object (%p) doesn't implement the Elementary scrollable"     \
            " interface", obj);                                              \
       if (getenv("ELM_ERROR_ABORT")) abort();                               \
       return __VA_ARGS__;                                                   \
    }

#ifdef EFL_BETA_API_SUPPORT
EAPI void elm_pan_gravity_set(Elm_Pan *, double x, double) EINA_DEPRECATED;
EAPI void elm_pan_gravity_get(const Elm_Pan *, double *, double *) EINA_DEPRECATED;
#endif
// TIZEN_ONLY(20190529) Scroll align feature
EAPI void _elm_interface_scrollable_item_scroll_align_end_set_cb_set(Elm_Pan *, Elm_Interface_Scrollable_Cb);
EAPI void _elm_interface_scrollable_item_scroll_align_end_unset_cb_set(Elm_Pan *, Elm_Interface_Scrollable_Cb);
EAPI void _elm_interface_scrollable_item_scroll_aligned_get_cb_set(Elm_Pan *, Elm_Interface_Scrollable_Item_Scroll_Aligned_Get_Cb);
EAPI void _elm_interface_scrollable_item_scroll_align_force_set(Elm_Pan *, Eina_Bool enabled);
//

/**
 * @}
 */

#endif
