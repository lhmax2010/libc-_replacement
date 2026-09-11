#ifndef _ECORE_WL2_H_
# define _ECORE_WL2_H_

# include <Eina.h>
# include <Ecore.h>
# include <wayland-client.h>
# include <wayland-cursor.h>
# include <xkbcommon/xkbcommon.h>
# include "Ecore_Wayland2.h"

# define WL_HIDE_DEPRECATED
# include <wayland-server.h>

# ifdef EAPI
#  undef EAPI
# endif

# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif

# ifdef __cplusplus
extern "C" {
# endif

#define ECORE_WL2_SURFACE_INTERFACE_VERSION 1

/** @internal */
typedef struct _Ecore_Wl2_Output Ecore_Wl2_Output;
/** @internal */
typedef struct _Ecore_Wl2_Seat Ecore_Wl2_Seat;
/** @internal */
typedef struct _Ecore_Wl2_Pointer Ecore_Wl2_Pointer;
/** @internal */
typedef struct _Ecore_Wl2_Keyboard Ecore_Wl2_Keyboard;
/** @internal */
typedef struct _Ecore_Wl2_Touch Ecore_Wl2_Touch;
/** @internal */
typedef struct _Ecore_Wl2_Offer Ecore_Wl2_Offer;
/** @internal */
typedef struct _Ecore_Wl2_Surface Ecore_Wl2_Surface;
// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
/** @internal */
typedef struct _Ecore_Wl2_Tizen_Input_Eo_Device Ecore_Wl2_Tizen_Input_Eo_Device;
/** @internal */
typedef struct _Ecore_Wl2_Tizen_Input_Device Ecore_Wl2_Tizen_Input_Device;
//
// TIZEN_ONLY(20250109): support ecore_wl2_screen API
/**
 * @internal
 *
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Screen
 * Type for Ecore_Wl2_Screen
 */
typedef struct _Ecore_Wl2_Screen Ecore_Wl2_Screen;
// END of TIZEN_ONLY(20250109)

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Connect
 * Type for Ecore_Wl2_Event_Connect
 */
typedef struct _Ecore_Wl2_Event_Connection Ecore_Wl2_Event_Connect;

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Disconnect
 * Type for Ecore_Wl2_Event_Disconnect
 */
typedef struct _Ecore_Wl2_Event_Connection Ecore_Wl2_Event_Disconnect;

struct _Ecore_Wl2_Event_Connection
{
   Ecore_Wl2_Display *display;
};

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Global
 * Type for _Ecore_Wl2_Event_Global
 */
typedef struct _Ecore_Wl2_Event_Global
{
   Ecore_Wl2_Display *display;
   Eina_Stringshare *interface;
   unsigned int id, version;
} Ecore_Wl2_Event_Global;

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Configure
 * Type for Ecore_Wl2_Event_Window_Configure
 */
typedef struct _Ecore_Wl2_Event_Window_Configure
{
   unsigned int win, event_win, edges;
// TIZEN_ONLY
   int x, y;
//
   unsigned int w, h;
   unsigned int states;
} Ecore_Wl2_Event_Window_Configure;

/**
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Configure_Complete
 * Type for Ecore_Wl2_Event_Window_Configure_Complete
 */
typedef struct _Ecore_Wl2_Event_Window_Configure_Complete
{
   unsigned int win;
} Ecore_Wl2_Event_Window_Configure_Complete;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Show
 * Type for Ecore_Wl2_Event_Window_Show
 */
typedef struct _Ecore_Wl2_Event_Window_Show
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
   // TIZEN_ONLY
   unsigned int data[4];
   //
} Ecore_Wl2_Event_Window_Show;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Hide
 * Type for Ecore_Wl2_Event_Window_Hide
 */
typedef struct _Ecore_Wl2_Event_Window_Hide
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
} Ecore_Wl2_Event_Window_Hide;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Activate
 * Type for Ecore_Wl2_Event_Window_Activate
 */
typedef struct _Ecore_Wl2_Event_Window_Activate
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
} Ecore_Wl2_Event_Window_Activate;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Deactivate
 * Type for Ecore_Wl2_Event_Window_Deactivate
 */
typedef struct _Ecore_Wl2_Event_Window_Deactivate
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
} Ecore_Wl2_Event_Window_Deactivate;

typedef struct _Ecore_Wl2_Event_Window_Common
{
   unsigned int win;
   unsigned int parent_win;
   unsigned int event_win;
} Ecore_Wl2_Event_Window_Common;

/*
 * @ingroup Ecore_Wl2_Group
 *
 * @typedef Ecore_Wl2_Event_Window_Lower
 * Type for Ecore_Wl2_Event_Window_Lower
 */
typedef struct _Ecore_Wl2_Event_Window_Lower
{
   unsigned int win;
   unsigned int timestamp;
} Ecore_Wl2_Event_Window_Lower;

/* matches protocol values */
/** @internal */
typedef enum
{
   ECORE_WL2_DRAG_ACTION_NONE = 0,
   ECORE_WL2_DRAG_ACTION_COPY = 1,
   ECORE_WL2_DRAG_ACTION_MOVE = 2,
   ECORE_WL2_DRAG_ACTION_ASK = 4,
   ECORE_WL2_DRAG_ACTION_LAST = 5,
} Ecore_Wl2_Drag_Action;

/** @internal */
typedef enum
{
  ECORE_WL2_SEAT_CAPABILITIES_NONE = 0,
  ECORE_WL2_SEAT_CAPABILITIES_POINTER = 1,
  ECORE_WL2_SEAT_CAPABILITIES_KEYBOARD = 2,
  ECORE_WL2_SEAT_CAPABILITIES_TOUCH = 4
} Ecore_Wl2_Seat_Capabilities;

/** @internal */
typedef struct _Ecore_Wl2_Global
{
   Eina_Stringshare *interface;
   unsigned int id, version;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Global;

/** @internal */
typedef struct _Ecore_Wl2_Event_Focus_In
{
   unsigned int window;
   unsigned int timestamp;
   Eo *dev; //The seat device
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Focus_In;

/** @internal */
typedef struct _Ecore_Wl2_Event_Focus_Out
{
   unsigned int window;
   unsigned int timestamp;
   Eo *dev; //The seat device
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Focus_Out;

/** @internal */
typedef struct _Ecore_Wl2_Event_Dnd_Enter
{
   unsigned int win, source;
   Ecore_Wl2_Offer *offer;
   int x, y;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Enter;

/** @internal */
typedef struct _Ecore_Wl2_Event_Dnd_Leave
{
   unsigned int win, source;
   Ecore_Wl2_Offer *offer;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Leave;

/** @internal */
typedef struct _Ecore_Wl2_Event_Dnd_Motion
{
   unsigned int win, source;
   Ecore_Wl2_Offer *offer;
   int x, y;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Motion;

/** @internal */
typedef struct _Ecore_Wl2_Event_Dnd_Drop
{
   unsigned int win, source;
   int x, y;
   Ecore_Wl2_Offer *offer;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Drop;

/** @internal */
typedef struct _Ecore_Wl2_Event_Dnd_End
{
   unsigned int win, source;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_End;

/** @internal */
struct _Ecore_Wl2_Event_Data_Source_Event
{
   unsigned int win, source;
   Ecore_Wl2_Drag_Action action;
   unsigned int seat;
   uint32_t serial;
   Ecore_Wl2_Display *display;
};

/** @internal */
typedef struct Ecore_Wl2_Event_Data_Source_End
{
   unsigned int win, source;
   Ecore_Wl2_Drag_Action action;
   unsigned int seat;
   uint32_t serial;
   Ecore_Wl2_Display *display;
   Eina_Bool cancelled;
} Ecore_Wl2_Event_Data_Source_End;

/** @internal */
typedef struct _Ecore_Wl2_Event_Data_Source_Event Ecore_Wl2_Event_Data_Source_Drop;
/** @internal */
typedef struct _Ecore_Wl2_Event_Data_Source_Event Ecore_Wl2_Event_Data_Source_Action;

/** @internal */
typedef struct _Ecore_Wl2_Event_Data_Source_Target
{
   char *type;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Data_Source_Target;

/** @internal */
typedef struct _Ecore_Wl2_Event_Data_Source_Send
{
   char *type;
   int fd;
   unsigned int seat;
   uint32_t serial;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Data_Source_Send;

/** @internal */
typedef struct Ecore_Wl2_Event_Seat_Selection
{
   //TIZEN_ONLY(20180724): Supporting copy and paste for cbhm
   int num_types;
   const char **types;
   //
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Selection;

/** @internal */
typedef struct _Ecore_Wl2_Event_Seat_Name
{
   Eina_Stringshare *name;
   unsigned int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Name;

/** @internal */
typedef struct _Ecore_Wl2_Event_Seat_Capabilities
{
   unsigned int id;
   Eina_Bool pointer_enabled : 1;
   Eina_Bool keyboard_enabled : 1;
   Eina_Bool touch_enabled : 1;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Capabilities;

/** @internal */
typedef enum
{
   ECORE_WL2_DEVICE_TYPE_SEAT,
   ECORE_WL2_DEVICE_TYPE_POINTER,
   ECORE_WL2_DEVICE_TYPE_KEYBOARD,
   ECORE_WL2_DEVICE_TYPE_TOUCH
} Ecore_Wl2_Device_Type;

/** @internal */
typedef struct _Ecore_Wl2_Event_Device
{
   Eo *dev;
   int window_id;
   unsigned int seat_id;
   Ecore_Wl2_Device_Type type;
} Ecore_Wl2_Event_Device;

/** @internal */
typedef enum
{
   ECORE_WL2_SELECTION_CNP,
   ECORE_WL2_SELECTION_DND
} Ecore_Wl2_Selection_Type;

/** @internal */
typedef enum
{
   ECORE_WL2_WINDOW_STATE_NONE = 0,
   ECORE_WL2_WINDOW_STATE_FULLSCREEN = (1 << 0),
   ECORE_WL2_WINDOW_STATE_MAXIMIZED = (1 << 1),
} Ecore_Wl2_Window_States;

/** @internal */
typedef struct _Ecore_Wl2_Event_Input_Keymap_Changed
{
   unsigned int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Keymap_Changed;

/** @internal */
typedef struct Ecore_Wl2_Event_Seat_Keyboard_Repeat_Changed
{
   unsigned int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Keyboard_Repeat_Changed;

/** @internal */
typedef struct _Ecore_Wl2_Event_Sync_Done
{
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Sync_Done;

/** @internal */
typedef struct _Ecore_Wl2_Event_Offer_Data_Ready
{
   Ecore_Wl2_Offer *offer;
   char *data;
   int len;
   const char *mimetype;
   unsigned int seat;
   Ecore_Wl2_Display *display;
   // TIZEN_ONLY(230921): add window id for recognize which window is received data
   unsigned int win;
   //
} Ecore_Wl2_Event_Offer_Data_Ready;

/** @internal */
typedef struct _Ecore_Wl2_Event_Output_Transform
{
   Ecore_Wl2_Output *output;
   int transform, old_transform;
} Ecore_Wl2_Event_Output_Transform;

/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Rotation
{
   unsigned int win;
   int rotation, w, h, angle;
   Eina_Bool resize : 1;
} Ecore_Wl2_Event_Window_Rotation;

/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Prepare;
/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Prepare_Done;
/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Request;
/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Done;

/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Iconify_State_Change
{
   unsigned int win;
   unsigned int iconified;
   unsigned int force;
} Ecore_Wl2_Event_Window_Iconify_State_Change;

/** @internal */
typedef enum _Ecore_Wl2_Window_Type
{
   ECORE_WL2_WINDOW_TYPE_NONE,
   ECORE_WL2_WINDOW_TYPE_TOPLEVEL,
   ECORE_WL2_WINDOW_TYPE_MENU,
   ECORE_WL2_WINDOW_TYPE_DND,
   ECORE_WL2_WINDOW_TYPE_CUSTOM,
   ECORE_WL2_WINDOW_TYPE_NOTIFICATION, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_UTILITY, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_DIALOG, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_DOCK, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_SPLASH, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_DESKTOP,
   ECORE_WL2_WINDOW_TYPE_LAST
} Ecore_Wl2_Window_Type;

// TIZEN_ONLY(20171107): added wl enumerations from ecore_wayland to ecore_wl2
/** @internal */
typedef enum _Ecore_Wl2_Window_Visibility_Type
{
   ECORE_WL2_WINDOW_VISIBILITY_TYPE_UNKNOWN = 0,
   ECORE_WL2_WINDOW_VISIBILITY_TYPE_UNOBSCURED = 1,
   ECORE_WL2_WINDOW_VISIBILITY_TYPE_PARTIALLY_OBSCURED = 2,
   ECORE_WL2_WINDOW_VISIBILITY_TYPE_FULLY_OBSCURED = 3,
   ECORE_WL2_WINDOW_VISIBILITY_TYPE_PRE_UNOBSCURED = 4,
} Ecore_Wl2_Window_Visibility_Type;

/** @internal */
typedef enum _Ecore_Wl2_Window_Stack_Mode
{
   ECORE_WL2_WINDOW_STACK_NONE  = 0,
   ECORE_WL2_WINDOW_STACK_ABOVE = 1,
   ECORE_WL2_WINDOW_STACK_BELOW = 2,
} Ecore_Wl2_Window_Stack_Mode;

/** @internal */
typedef enum _Ecore_Wl2_Conformant_Part_Type
{
   ECORE_WL2_INDICATOR_PART = 0,
   ECORE_WL2_KEYBOARD_PART = 1,
   ECORE_WL2_CLIPBOARD_PART = 2
} Ecore_Wl2_Conformant_Part_Type;

/** @internal */
typedef enum _Ecore_Wl2_Virtual_Keyboard_State
{
   ECORE_WL2_VIRTUAL_KEYBOARD_STATE_UNKNOWN = 0,
   ECORE_WL2_VIRTUAL_KEYBOARD_STATE_OFF,
   ECORE_WL2_VIRTUAL_KEYBOARD_STATE_ON,
} Ecore_Wl2_Virtual_Keyboard_State;

/** @internal */
typedef enum _Ecore_Wl2_Indicator_State
{
   ECORE_WL2_INDICATOR_STATE_UNKNOWN = 0,
   ECORE_WL2_INDICATOR_STATE_OFF,
   ECORE_WL2_INDICATOR_STATE_ON
} Ecore_Wl2_Indicator_State;

/** @internal */
typedef enum _Ecore_Wl2_Indicator_Opacity_Mode
{
   ECORE_WL2_INDICATOR_OPACITY_UNKNOWN = 0,
   ECORE_WL2_INDICATOR_OPAQUE,
   ECORE_WL2_INDICATOR_TRANSLUCENT,
   ECORE_WL2_INDICATOR_TRANSPARENT,
   ECORE_WL2_INDICATOR_BG_TRANSPARENT,
} Ecore_Wl2_Indicator_Opacity_Mode;

/** @internal */
typedef enum _Ecore_Wl2_Indicator_Visible_Type
{
   ECORE_WL2_INDICATOR_VISIBLE_TYPE_HIDDEN = 0,
   ECORE_WL2_INDICATOR_VISIBLE_TYPE_SHOWN,
} Ecore_Wl2_Indicator_Visible_Type;

/** @internal */
typedef enum _Ecore_Wl2_Clipboard_State
{
   ECORE_WL2_CLIPBOARD_STATE_UNKNOWN = 0,
   ECORE_WL2_CLIPBOARD_STATE_OFF,
   ECORE_WL2_CLIPBOARD_STATE_ON
} Ecore_Wl2_Clipboard_State;
//

// TIZEN_ONLY(20221121): support a maximize direction
typedef enum _Ecore_Wl2_Maximize_Direction
{
   ECORE_WL2_MAXIMIZE_DIRECTION_NONE, // unmaximize
   ECORE_WL2_MAXIMIZE_DIRECTION_ALL, // maximize to full screen
   ECORE_WL2_MAXIMIZE_DIRECTION_LEFT, // maximize to left side of screen
   ECORE_WL2_MAXIMIZE_DIRECTION_RIGHT, // maximize to right side of screen
} Ecore_Wl2_Maximize_Direction;
//

// TIZEN_ONLY(20240718) : add relative_motion grab/ungrab API
typedef enum _Ecore_Wl2_Pointer_Boundary
{
   ECORE_WL2_POINTER_BOUNDARY_NONE = 0,
   ECORE_WL2_POINTER_BOUNDARY_TOP,
   ECORE_WL2_POINTER_BOUNDARY_RIGHT,
   ECORE_WL2_POINTER_BOUNDARY_BOTTOM,
   ECORE_WL2_POINTER_BOUNDARY_LEFT,
} Ecore_Wl2_Pointer_Boundary;

/** @internal */
typedef struct _Ecore_Wl2_Event_Aux_Hint_Allowed
{
   unsigned int win;
   int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Aux_Hint_Allowed;

/** @internal */
typedef struct _Ecore_Wl2_Event_Aux_Hint_Supported
{
   unsigned int win;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Aux_Hint_Supported;

/** @internal */
typedef struct Ecore_Wl2_Event_Aux_Message
{
   unsigned int win;
   Eina_Stringshare *key;
   Eina_Stringshare *val;
   Eina_List *options;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Aux_Message;

/** @internal */
typedef struct Ecore_Wl2_Event_Window_Offscreen
{
   unsigned int win;
} Ecore_Wl2_Event_Window_Offscreen;

/** @internal */
typedef struct _Ecore_Wl2_Buffer Ecore_Wl2_Buffer;

/** @internal */
typedef enum _Ecore_Wl2_Buffer_Type
{
   ECORE_WL2_BUFFER_NONE = 0,
   ECORE_WL2_BUFFER_SHM = 1,
   ECORE_WL2_BUFFER_DMABUF = 2
} Ecore_Wl2_Buffer_Type;

// TIZEN_ONLY(20171107): added wl evet structures from ecore_wayland to ecore_wl2
/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Visibility_Change
{
   unsigned int win;
   int          fully_obscured;
} Ecore_Wl2_Event_Window_Visibility_Change;

/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Pre_Visibility_Change
{
   unsigned int win;
   Ecore_Wl2_Window_Visibility_Type type;
   unsigned int option;
} Ecore_Wl2_Event_Window_Pre_Visibility_Change;

/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Redraw_Request
{
   unsigned int win;
} Ecore_Wl2_Event_Window_Redraw_Request;

/** @internal */
typedef struct _Ecore_Wl2_Event_Dnd_Position
{
   unsigned int win, source;
   struct
     {
        int x, y;
     } position;
} Ecore_Wl2_Event_Dnd_Position;

/** @internal */
typedef struct _Ecore_Wl2_Event_Data_Source_Cancelled
{
   unsigned int win, source;
} Ecore_Wl2_Event_Data_Source_Cancelled;

/** @internal */
typedef struct _Ecore_Wl2_Event_Selection_Data_Ready
{
   char *data;
   int len;
   Eina_Bool done;
   char **types;
   int num_types;
   Ecore_Wl2_Selection_Type sel_type;
} Ecore_Wl2_Event_Selection_Data_Ready;

/** @internal */
typedef struct _Ecore_Wl2_Event_Conformant_Change
{
   unsigned int win;
   Ecore_Wl2_Conformant_Part_Type part_type;
   Eina_Bool state;
} Ecore_Wl2_Event_Conformant_Change;

typedef struct _Ecore_Wl2_Event_Effect
{
   int win;
   unsigned int type;
} Ecore_Wl2_Event_Effect;

/** @internal */
typedef struct _Ecore_Wl2_Event_Effect Ecore_Wl2_Event_Effect_Start;
/** @internal */
typedef struct _Ecore_Wl2_Event_Effect Ecore_Wl2_Event_Effect_End;

/** @internal */
typedef struct _Ecore_Wl2_Event_Ignore_Output_Transform
{
   Ecore_Wl2_Window *win;
   Eina_Bool ignore;
} Ecore_Wl2_Event_Ignore_Output_Transform;

/** @internal */
typedef struct _Ecore_Wl2_Event_Indicator_Flick
{
   unsigned int win;
   int type;
} Ecore_Wl2_Event_Indicator_Flick;

/** @internal */
typedef struct _Ecore_Wl2_Event_Clipboard_Data_Selected
{
   unsigned int win;
} Ecore_Wl2_Event_Clipboard_Data_Selected;
//

// TIZEN_ONLY(20211120)
/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Interactive_Move_Done
{
   unsigned int win;
   int x;
   int y;
   unsigned int w;
   unsigned int h;
   int angle;
} Ecore_Wl2_Event_Window_Interactive_Move_Done;

/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Interactive_Resize_Done
{
   unsigned int win;
   int x;
   int y;
   unsigned int w;
   unsigned int h;
   int angle;
} Ecore_Wl2_Event_Window_Interactive_Resize_Done;
//

/** @internal */
typedef struct _Ecore_Wl2_Keygrab_Info
{
   int key;
   int mode;
   int err;
} Ecore_Wl2_Keygrab_Info;

/** @internal */
typedef struct _Ecore_Wl2_Keyungrab_Info
{
   int key;
   int err;
} Ecore_Wl2_Keyungrab_Info;

/** @internal */
typedef struct _Ecore_Wl2_Surface_Interface
{
   int id;
   int version;

   void *(*setup)(Ecore_Wl2_Window *win);
   void (*destroy)(Ecore_Wl2_Surface *surface, void *priv_data);
   void (*reconfigure)(Ecore_Wl2_Surface *surface, void *priv_data, int w, int h, uint32_t flags, Eina_Bool alpha);
   void *(*data_get)(Ecore_Wl2_Surface *surface, void *priv_data, int *w, int *h);
   int  (*assign)(Ecore_Wl2_Surface *surface, void *priv_data);
   void (*post)(Ecore_Wl2_Surface *surface, void *priv_data, Eina_Rectangle *rects, unsigned int count);
   void (*flush)(Ecore_Wl2_Surface *surface, void *priv_data, Eina_Bool purge);
   void (*set_serial)(Ecore_Wl2_Surface *surface, void *priv_data, unsigned int serial);
} Ecore_Wl2_Surface_Interface;

/** @internal */
typedef void (*Ecore_Wl2_Bind_Cb)(struct wl_client *client, void *data, uint32_t version, uint32_t id);
/** @internal */
typedef void (*Ecore_Wl2_Unbind_Cb)(struct wl_resource *resource);

// TIZEN_ONLY(20230906) : add events for pointer_constraints
/** @internal */
typedef struct _Ecore_Wl2_Event_Pointer_Constraints
{
   unsigned int win;
   int x;
   int y;
   Eina_Bool locked;
   Eina_Bool confined;
} Ecore_Wl2_Event_Pointer_Constraints;
//

// TIZEN_ONLY(20250109) : support wtz_surface
/** @internal */
typedef struct _Ecore_Wl2_Event_Window_Screen_Assigned
{
   unsigned int win;
   Ecore_Wl2_Screen *screen;
   const char *screen_name;
} Ecore_Wl2_Event_Window_Screen_Assigned;
// END of TIZEN_ONLY(20250109)

EAPI extern int ECORE_WL2_EVENT_DISCONNECT; /** @since 1.18 */
EAPI extern int ECORE_WL2_EVENT_CONNECT; /** @since 1.18 */
EAPI extern int ECORE_WL2_EVENT_GLOBAL_ADDED; /** @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_GLOBAL_REMOVED; /** @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_FOCUS_IN; /** @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_FOCUS_OUT; /** @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DND_ENTER; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DND_LEAVE; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DND_MOTION; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DND_DROP; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DND_END; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DATA_SOURCE_END; /** @internal @since 1.18 */
EAPI extern int ECORE_WL2_EVENT_DATA_SOURCE_DROP; /** @internal @since 1.18 */
EAPI extern int ECORE_WL2_EVENT_DATA_SOURCE_ACTION; /** @internal @since 1.18 */
EAPI extern int ECORE_WL2_EVENT_DATA_SOURCE_TARGET; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_DATA_SOURCE_SEND; /** @internal @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_CONFIGURE; /** @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_SYNC_DONE; /** @since 1.17 */
EAPI extern int ECORE_WL2_EVENT_OFFER_DATA_READY; /** @internal @since 1.19 */
EAPI extern int ECORE_WL2_EVENT_SEAT_NAME_CHANGED; /** @since 1.19 */
EAPI extern int ECORE_WL2_EVENT_SEAT_CAPABILITIES_CHANGED; /** @since 1.19 */
EAPI extern int ECORE_WL2_EVENT_DEVICE_ADDED; /** @since 1.19 */
EAPI extern int ECORE_WL2_EVENT_DEVICE_REMOVED; /** @since 1.19 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE; /** @since 1.19 */
EAPI extern int ECORE_WL2_EVENT_SEAT_KEYMAP_CHANGED; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_SEAT_SELECTION; /** @internal  @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_OUTPUT_TRANSFORM; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ROTATE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_REQUEST; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_DONE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_AUX_HINT_ALLOWED; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_AUX_HINT_SUPPORTED; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_AUX_MESSAGE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_SHOW; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_HIDE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ACTIVATE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_DEACTIVATE; /** @since 1.20 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE; /** @since 1.21 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_OFFSCREEN; /** @since 1.21 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_CREATE; /** @since 1.25 */
EAPI extern int ECORE_WL2_EVENT_WINDOW_DESTROY; /** @since 1.25 */
// TIZEN_ONLY(20171107): added wl events from ecore_wayland to ecore_wl2
EAPI extern int ECORE_WL2_EVENT_WINDOW_VISIBILITY_CHANGE;
EAPI extern int ECORE_WL2_EVENT_WINDOW_PRE_VISIBILITY_CHANGE;
EAPI extern int ECORE_WL2_EVENT_WINDOW_LOWER;
EAPI extern int ECORE_WL2_EVENT_WINDOW_REDRAW_REQUEST;
EAPI extern int ECORE_WL2_EVENT_DND_POSITION; /** @internal */
EAPI extern int ECORE_WL2_EVENT_DND_OFFER; /** @internal @since 1.8, but NOT exist currently */
EAPI extern int ECORE_WL2_EVENT_DATA_SOURCE_CANCELLED; /** @internal @since 1.7, but NOT exist currently */
EAPI extern int ECORE_WL2_EVENT_SELECTION_DATA_READY; /** @internal @since 1.7, but NOT exist currently */
EAPI extern int ECORE_WL2_EVENT_CONFORMANT_CHANGE;
EAPI extern int ECORE_WL2_EVENT_EFFECT_START;
EAPI extern int ECORE_WL2_EVENT_EFFECT_END;
EAPI extern int ECORE_WL2_EVENT_IGNORE_OUTPUT_TRANSFORM;
EAPI extern int ECORE_WL2_EVENT_INDICATOR_FLICK; /** @internal */
EAPI extern int ECORE_WL2_EVENT_CLIPBOARD_DATA_SELECTED; /** @internal */
//
// TIZEN_ONLY(20211120)
EAPI extern int ECORE_WL2_EVENT_WINDOW_INTERACTIVE_MOVE_DONE;
EAPI extern int ECORE_WL2_EVENT_WINDOW_INTERACTIVE_RESIZE_DONE;
//

// TIZEN_ONLY(20230906) : add events for pointer_constraints
EAPI extern int ECORE_WL2_EVENT_POINTER_CONSTRAINTS;
//
// TIZEN_ONLY(20250109) : support wtz_surface
EAPI extern int ECORE_WL2_EVENT_WINDOW_SCREEN_ASSIGNED;
//

/**
 * @internal
 *
 * Create a new Wayland display
 *
 * @brief This function is typically used to create a new display for
 * use with compositors, or to create a new display for use in nested
 * compositors.
 *
 * @param name The display target name to create. If @c NULL, a default
 *             display name will be assigned.
 * @return The newly created Ecore_Wl2_Display
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI Ecore_Wl2_Display *ecore_wl2_display_create(const char *name);

/**
 * @internal
 *
 * Destroy an existing Wayland display
 *
 * @brief This function is typically used by servers to terminate an
 * existing Wayland display.
 *
 * @param display The display to terminate
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI void ecore_wl2_display_destroy(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Terminate a Wayland display's main loop
 *
 * @brief This function is typically used by servers to terminate the
 * Wayland display main loop. This is usually only called when a server
 * encounters an error.
 *
 * @param display The Ecore_Wl2_Display to terminate
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI void ecore_wl2_display_terminate(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Retrieve the existing Wayland display
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland display from
 *
 * @return The wl_display which this Ecore_Wl2_Display is using
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI struct wl_display *ecore_wl2_display_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Retrieve the wl_shm from a given Ecore_Wl2_Display
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland shm interface from
 *
 * @return The wl_shm which this Ecore_Wl2_Display is using
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI struct wl_shm *ecore_wl2_display_shm_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Retrieve the wl_dmabuf from a given Ecore_Wl2_Display
 *
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland dmabuf interface from
 *
 * @return The wl_dmabuf which this Ecore_Wl2_Display is using
 *
 * @ingroup Ecore_Wl2_Display_Group
 *
 * @note This is intended for client use only and should be used only
 *       after ecore_wl2_display_connect().  Also, the return type is
 *       void * instead of zpw_linux_dmabuf_v1 * since we don't want
 *       to change our public API every time the version changes in
 *       wayland-protocols.
 *
 * @since 1.18
 */
EAPI void *ecore_wl2_display_dmabuf_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Return an Eina_Iterator that can be used to iterate through globals
 *
 * @param display The Ecore_Wl2_Display for which to return a global iterator
 *
 * @note The caller of this function should free the returned Eina_Iterator
 * when finished with it.
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI Eina_Iterator *ecore_wl2_display_globals_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Get all the Ecore_Wl2_Input from the display.
 *
 * @param display The display
 *
 * @return A Eina_Iterator of Ecore_Wl2_Input or @c NULL on error
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.19
 */
EAPI Eina_Iterator *ecore_wl2_display_inputs_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Find a seat for a given display object using the seat id
 *
 * @param display The display
 * @param id The seat id
 *
 * @return The corresponding Ecore_Wl2_Input object or @c NULL if no match is found
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
EAPI Ecore_Wl2_Input *ecore_wl2_display_input_find(const Ecore_Wl2_Display *display, unsigned int id);

/**
 * @internal
 *
 * Find a seat for a given display object using the seat id
 *
 * @param display The display
 * @param name The seat name
 *
 * @return The corresponding Ecore_Wl2_Input object or @c NULL if no match is found
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
EAPI Ecore_Wl2_Input *ecore_wl2_display_input_find_by_name(const Ecore_Wl2_Display *display, const char *name);

/**
 * @internal
 *
 * Retrieves the Wayland Registry used for the current Wayland display.
 *
 * @param display The display to get the registry of
 *
 * @return The current wayland registry, or NULL on error
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI struct wl_registry *ecore_wl2_display_registry_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Check if the display has performed a sync
 *
 * @param display The display
 *
 * @return True if the display sync has occurred
 * @see ECORE_WL2_EVENT_SYNC_DONE
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_display_sync_is_done(const Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Get the name of the display object
 *
 * @param display The display
 *
 * @return The name of the display object
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
EAPI const char *ecore_wl2_display_name_get(const Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Finds an Ecore_Wl2_Window based on wl_surface
 *
 * @param display The display to search for the window
 * @param surface The wl_surface of the window to find
 *
 * @return The Ecore_Wl2_Window if found, or NULL if no such window exists
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.24
 */
EAPI Ecore_Wl2_Window *ecore_wl2_display_window_find_by_surface(Ecore_Wl2_Display *display, struct wl_surface *surface);

/**
 * @internal
 *
 * Gets the wl_compositor which belongs to this display
 *
 * @param display The Ecore_Wl2_Display to get the compositor of
 *
 * @return The wl_compositor associated with this display
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.24
 */
EAPI struct wl_compositor *ecore_wl2_display_compositor_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Return the version of the display's compositor object
 *
 * @param disp the display to get the compositor object version from
 *
 * @return the version of the display's compositor object
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
EAPI int ecore_wl2_display_compositor_version_get(Ecore_Wl2_Display *disp);

/**
 * Unload keymap memory from all inputs and xkb context of a given display
 *
 * @param display The Ecore_Wl2_Display to unload keymap from
 *
 * @note Key string would not be available after this function.
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI void ecore_wl2_display_keymap_unload(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * Set a callback to be called just before the window is closed and freed
 *
 * @param window The window to listen to for a xdg toplevel close callback
 * @param cb The callback function to call being passed data and window
 * @param data The Data pointer to pass as data to the callback
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI void ecore_wl2_window_close_callback_set(Ecore_Wl2_Window *window, void (*cb) (void *data, Ecore_Wl2_Window *win), void *data);

/**
 * @internal
 *
 * Set a callback to be called after the window's visibility is changed
 *
 * @param window The window to listen to for a tizen visibility cb notify callback
 * @param cb The callback function to call being passed data, window and visibility
 * @param data The Data pointer to pass as data to the callback
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 7.0
 */
EAPI void ecore_wl2_window_visibility_callback_set(Ecore_Wl2_Window *window, void (*cb) (void *data, Ecore_Wl2_Window *win, Eina_Bool visibility), void *data);

/**
 * @internal
 *
 * Get the wl_surface which belongs to this window
 *
 * @param window The Ecore_Wl2_Window to get the surface of
 *
 * @return The wl_surface associated with this window.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI struct wl_surface *ecore_wl2_window_surface_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Get the id of a given Ecore_Wl2_Window
 *
 * @param window The window to return the id of
 *
 * @return The id of the given window, or -1 on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI int ecore_wl2_window_surface_id_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * @see evas_object_size_hint_aspect_set
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
EAPI void ecore_wl2_window_aspect_set(Ecore_Wl2_Window *window, int w, int h, unsigned int aspect);

/**
 * @internal
 *
 * @see evas_object_size_hint_aspect_get
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI void ecore_wl2_window_aspect_get(Ecore_Wl2_Window *window, int *w, int *h, unsigned int *aspect);

/**
 * @internal
 *
 * @brief Begin moving a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window which to move
 * @param input The seat on which the move is active on
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_move(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * @brief Begin resizing a given window
 *
 * @param window The Ecore_Wl2_Window which to resize
 * @param input The seat on which the resize is active
 * @param location The edge of the window from which the resize should start
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_resize(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input, int location);

// TIZEN_ONLY(20171108): tizen window function
/** @internal */
EAPI void ecore_wl2_window_parent_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window *parent);
/** @internal */
EAPI void ecore_wl2_window_stack_mode_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window_Stack_Mode mode);
/** @internal */
EAPI void ecore_wl2_window_position_set(Ecore_Wl2_Window *window, int x, int y);
/** @internal */
EAPI int ecore_wl2_window_active_angle_get(Ecore_Wl2_Window *window);
//

//TIZEN_ONLY(20210721)
/**
 * @internal
 *
 * Set a parent window of Ecore_Wl2_Window
 *
 * This makes a relationship between parent and child. And child can choose where it is placed.
 * If a @p place_below is true, then the window is always placed under the parent.
 * Otherwise, it is placed on the parent.
 *
 * @param window The window on which to set the parent
 * @param parent The parent window
 * @param place_below If @p place_below is EINA_TRUE, the @p window is placed under the @p parent,
 *                    Otherwise, the @p window is placed on the @p parent
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 6.5
 */
EAPI void ecore_wl2_window_transient_parent_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window *parent, Eina_Bool place_below);
//

//TIZEN_ONLY(20171216): add ecore_wl2_window_find
/** @internal */
EAPI Ecore_Wl2_Window *ecore_wl2_window_find(unsigned int id);
//

/**
 * @internal
 *
 * Set a given window's transparent property
 *
 * @param window The window on which to set the transparent property
 * @param transparent EINA_TRUE to set window as transparent,
 *                    EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_transparent_set(Ecore_Wl2_Window *window, Eina_Bool transparent);

// TIZEN_ONLY(20171108) : Get a window's transparent property
/** @internal */
EAPI Eina_Bool ecore_wl2_window_transparent_get(Ecore_Wl2_Window *window);
//

/**
 * @internal
 *
 * Set the opaque region of the Ecore_Wl2_Window
 *
 * @param window The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_opaque_region_set(Ecore_Wl2_Window *window, int x, int y, int w, int h);

// TIZEN_ONLY(20160201) : support to handle input rectangle
/**
 * @internal
 * @brief Sets the input rectangle for a Wayland window
 *
 * Defines the area within the window that can receive input events.
 * This completely replaces any existing input region with the new rectangle.
 *
 * @param win The window to modify (If win is NULL, no operation is performed.)
 * @param input_rect Pointer to Eina_Rectangle defining the input area (If input_rect is NULL, no operation is performed.)
 * @note ecore_wl2_window_commit() should be called to apply the changes.
 *
 * @see Eina_Rectangle
 */
EAPI void ecore_wl2_window_input_rect_set(Ecore_Wl2_Window *win, Eina_Rectangle *input_rect);

/**
 * @internal
 * @brief Adds an input rectangle to a window's input region
 *
 * Combines the new rectangle with any existing input regions using a union operation.
 * The resulting input region will accept events in either the existing or new areas.
 *
 * @param win The window to modify (If win is NULL, no operation is performed.)
 * @param input_rect Pointer to Eina_Rectangle defining the area to add (If input_rect is NULL, no operation is performed.)
 * @note ecore_wl2_window_commit() should be called to apply the changes.
 *
 * @see Eina_Rectangle
 */
EAPI void ecore_wl2_window_input_rect_add(Ecore_Wl2_Window *win, Eina_Rectangle *input_rect);

/**
 * @internal
 * @brief Subtracts an area from a window's input region
 *
 * Removes the specified rectangle from the window's input region.
 * The resulting region will not accept events in the subtracted area.
 *
 * @param win The window to modify (If win is NULL, no operation is performed.)
 * @param input_rect Pointer to Eina_Rectangle defining the area to remove (If input_rect is NULL, no operation is performed.)
 * @note ecore_wl2_window_commit() should be called to apply the changes.
 *
 * @note Can be used to create non-rectangular input regions
 * @see Eina_Rectangle
 */
EAPI void ecore_wl2_window_input_rect_subtract(Ecore_Wl2_Window *win, Eina_Rectangle *input_rect);
//

/**
 * @internal
 *
 * Get the opaque region of the Ecore_Wl2_Window
 *
 * @param window The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI void ecore_wl2_window_opaque_region_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h);

/**
 * @internal
 *
 * Set the input region of the Ecore_Wl2_Window.
 *
 * To set an empty region, pass width and height as 0.
 *
 * An empty input region means the entire window surface will accept input.
 *
 * @param window The window to set the input region of
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 * @note ecore_wl2_window_commit() should be called to apply the changes.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_input_region_set(Ecore_Wl2_Window *window, int x, int y, int w, int h);

/**
 * @internal
 *
 * Get the input region of the Ecore_Wl2_Window.
 *
 * @param window The window to set the input region of
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI void ecore_wl2_window_input_region_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h);

/**
 * @internal
 *
 * Get if a given window is maximized
 *
 * @param window The window to get the maximized state of
 *
 * @return EINA_TRUE if window is maximized, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI Eina_Bool ecore_wl2_window_maximized_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set the maximized state of a given window
 *
 * @param window The window to set the maximized state of
 * @param maximized EINA_TRUE to set maximized, EINA_FALSE to unset
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_maximized_set(Ecore_Wl2_Window *window, Eina_Bool maximized);

// TIZEN_ONLY(20221121): support a maximize direction
/**
 * @internal
 *
 * Set the maximized state of a given window with a maximize direction
 * If the type is ECORE_WL2_MAXIMIZE_DIRECTION_NONE, this means unmaximizing window.
 * This is same to call ecore_wl2_window_maximized_set(window, EINA_FALSE).
 * If the type is ECORE_WL2_MAXIMIZE_DIRECTION_ALL, this is same to call ecore_wl2_window_maximized_set(window, EINA_TRUE).
 *
 * @param window The window to set the maximized state of
 * @param direction ECORE_WL2_MAXIMIZE_DIRECTION_NONE to unmaximize, otherwise maximize to specific direction
 *
 * @see ecore_wl2_window_maximized_set
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI void ecore_wl2_window_maximized_set_with_direction(Ecore_Wl2_Window *window, Ecore_Wl2_Maximize_Direction direction);

/**
 * @internal
 *
 * Set the maximized state of a given window with specific size
 * This function unmaximizes the window to the specified size.
 *
 * @param window The window to maximize
 * @param maximized EINA_TRUE to set maximized, EINA_FALSE to unset
 * @param w The width to set for the unmaximized window
 * @param h The height to set for the unmaximized window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 11.0
 */
EAPI void ecore_wl2_window_maximized_set_with_size(Ecore_Wl2_Window *window, Eina_Bool maximized, int w, int h);

// TIZEN_ONLY(20230320): support a layout window
/** @internal */
EAPI Eina_Bool
ecore_wl2_window_layout_set(Ecore_Wl2_Window * window, unsigned int num_cols, unsigned int num_rows, unsigned int column, unsigned int row, unsigned int col_span, unsigned int row_span);
//

/**
 * @internal
 *
 * Get if a given window is fullscreen
 *
 * @param window The window to get the fullscreen state of
 *
 * @return EINA_TRUE if window is fullscreen, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI Eina_Bool ecore_wl2_window_fullscreen_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set the fullscreen state of a given window
 *
 * @param window The window to set the fullscreen state of
 * @param fullscreen EINA_TRUE to set fullscreen, EINA_FALSE to unset
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_fullscreen_set(Ecore_Wl2_Window *window, Eina_Bool fullscreen);

/**
 * @internal
 *
 * Get if a given window is rotated
 *
 * @param window The window to get the rotation of
 *
 * @return The amount of rotation for this window, or -1 on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI int ecore_wl2_window_rotation_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set the rotation of a given window
 *
 * @param window The window to set the rotation of
 * @param rotation The amount of rotation
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_rotation_set(Ecore_Wl2_Window *window, int rotation);

/**
 * @internal
 *
 * Set the class of a given window
 *
 * @param window The window to set the class of
 * @param clas The class of the window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_class_set(Ecore_Wl2_Window *window, const char *clas);

/**
 * @internal
 *
 * Get the class of a given window
 *
 * @param window The window to set the class of
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI const char *ecore_wl2_window_class_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Get the geometry of a given window
 *
 * @brief The window geometry returned here is the window geometry as
 * recognized by xdg_surface_set_window_geometry. As such, it represents the
 * "visible bounds" of a window from the user's perspective.
 *
 * @param window The window to get the geometry of
 * @param x The left point of the window geometry
 * @param y The top point of the window geometry
 * @param w The width of the window geometry
 * @param h The height of the window geometry
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_geometry_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h);

/**
 * @internal
 *
 * Set the geometry of a given window
 *
 * @brief The window geometry referenced here is the window geometry as
 * recognized by xdg_surface_set_window_geometry. As such, it represents the
 * "visible bounds" of a window from the user's perspective.
 *
 * @param window The window to set the geometry of
 * @param x The left point of the window geometry
 * @param y The top point of the window geometry
 * @param w The width of the window geometry
 * @param h The height of the window geometry
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_geometry_set(Ecore_Wl2_Window *window, int x, int y, int w, int h);

//TIZEN_ONLY(20220325): added min / max size set to window
/**
 * @internal
 *
 * Set the minimum size of a given window
 *
 * @param window The window to set the minimum size of
 * @param w The minimum width of the window size
 * @param h The minimum height of the window size
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_minimum_size_set(Ecore_Wl2_Window *window, int w, int h);

/**
 * @internal
 *
 * Set the maximum size of a given window
 *
 * @param window The window to set the maximum size of
 * @param w The maximum width of the window size
 * @param h The maximum height of the window size
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_maximum_size_set(Ecore_Wl2_Window *window, int w, int h);
//

/**
 * @internal
 *
 * Get the iconified state of a given window
 *
 * @param window The window to get the iconified state of
 *
 * @return EINA_TRUE if window is iconified, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI Eina_Bool ecore_wl2_window_iconified_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Iconify a window
 *
 * @param win The window to iconifiy
 * @param iconified The new iconified state to set
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_iconified_set(Ecore_Wl2_Window *window, Eina_Bool iconified);

// TIZEN_ONLY(20151231) : handling iconic state on tizen
/** @internal */
EAPI void ecore_wl2_window_iconify_state_update(Ecore_Wl2_Window *window, Eina_Bool iconified, Eina_Bool send_event);
//

/**
 * @internal
 *
 * Set the type of a given window
 *
 * @param window The window to set the type of
 * @param type The Ecore_Wl2_Window_Type to set on the window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
EAPI void ecore_wl2_window_type_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window_Type type);

/**
 * @internal
 *
 * Get the type of a given window
 *
 * @see Ecore_Wl2_Window_Type
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI Ecore_Wl2_Window_Type ecore_wl2_window_type_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Find the output that a given window is on
 *
 * @param window The window to find the output for
 *
 * @return An Ecore_Wl2_Output if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Ecore_Wl2_Output *ecore_wl2_window_output_find(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set if window rotation is supported by the window manager
 *
 * @param window
 * @param enabled
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_wm_rotation_supported_set(Ecore_Wl2_Window *window, Eina_Bool enabled);

/**
 * @internal
 *
 * Get if window rotation is supported by the window manager
 *
 * @param window
 *
 * @return EINA_TRUE if supported, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_window_wm_rotation_supported_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set if an application has set window rotation
 *
 * @param window
 * @param set
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_rotation_app_set(Ecore_Wl2_Window *window, Eina_Bool set);

/**
 * @internal
 *
 * Get if an application has set window rotation
 *
 * @param window
 *
 * @return EINA_TRUE if set, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_window_rotation_app_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set preferred rotation on a given window
 *
 * @param window The window to set preferred rotation on
 * @param rot The value of the preferred rotation to set
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_preferred_rotation_set(Ecore_Wl2_Window *window, int rot);

/**
 * @internal
 *
 * Get preferred rotation for a given window
 *
 * @param window The window to get preferred rotation from
 *
 * @return Given windows preferred rotation
 *
 * @ingroup Ecore_Wl2_Window
 * @since 1.20
 */
EAPI int ecore_wl2_window_preferred_rotation_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set a windows available rotations
 *
 * @param window
 * @param rots
 * @param count
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_available_rotations_set(Ecore_Wl2_Window *window, const int *rots, unsigned int count);

/**
 * @internal
 *
 * Get a windows available rotations
 *
 * @param window
 * @param rots
 * @param count
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_window_available_rotations_get(Ecore_Wl2_Window *window, int **rots, unsigned int *count);
/** @internal */
EAPI void ecore_wl2_window_rotation_change_prepare_send(Ecore_Wl2_Window *window, int rot, int w, int h, Eina_Bool resize);
/** @internal */
EAPI void ecore_wl2_window_rotation_change_prepare_done_send(Ecore_Wl2_Window *window, int rot);
/** @internal */
EAPI void ecore_wl2_window_rotation_change_request_send(Ecore_Wl2_Window *window, int rot);
/** @internal */
EAPI void ecore_wl2_window_rotation_change_done_send(Ecore_Wl2_Window *window, int rot, int w, int h);
// TIZEN_ONLY
/** @internal */
EAPI void ecore_wl2_window_rotation_geometry_set(Ecore_Wl2_Window *win, int rot, int x, int y, int w, int h);
/** @internal */
EAPI void ecore_wl2_window_rotation_changed_callback_set(Ecore_Wl2_Window *win, void *data, void (*func)(Ecore_Wl2_Window *win, int rot, Eina_Bool resize, int w, int h, void *data));
//

// TIZEN_ONLY(20210330): support aux_hint generate
/**
 * @internal
 *
 * Generate a auxiliary hint to a given window
 * It generates auxiliary hint ID in function and return the ID.
 * If there is already exist auxiliary hint that have the same name with "hint" parameter,
 * then change value of the hint and return exist hint's ID.
 *
 * @param win The window to add auxiliary hint
 * @param hint The name of hint to add
 * @param val The value of hint to add
 *
 * @return Hint ID on success, -1 on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI int ecore_wl2_window_aux_hint_generate(Ecore_Wl2_Window *win, const char *hint, const char *val);

/**
 * @internal
 *
 * Get the hint ID of a given window
 *
 * @param win The window to get auxiliary hint ID
 * @param hint The name of hint to get ID
 *
 * @return Hint ID on success, -1 on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI int ecore_wl2_window_aux_hint_id_get(Ecore_Wl2_Window *win, const char *hint);

/**
 * @internal
 *
 * Get the hint value of a given window
 *
 * @param win The window to get auxiliary hint value
 * @param id The ID of hint to get hint value
 *
 * @return Hint value on success, NULL on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI const char *ecore_wl2_window_aux_hint_value_get(Ecore_Wl2_Window *win, int id);
// END OF TIZEN_ONLY

/**
 * @internal
 *
 * Get list of supported auxiliary window hints
 *
 * @param window
 *
 * @return An Eina_List of supported auxiliary hints, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_List *ecore_wl2_window_aux_hints_supported_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Add a supported auxiliary hint to a given window
 *
 * @param window
 * @param id
 * @param hint
 * @param val
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_aux_hint_add(Ecore_Wl2_Window *window, int id, const char *hint, const char *val);

/**
 * @internal
 *
 * Change an auxiliary hint on a given window
 *
 * @param window
 * @param id
 * @param val
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_aux_hint_change(Ecore_Wl2_Window *window, int id, const char *val);

/**
 * @internal
 *
 * Delete an auxiliary hint on a given window
 *
 * @param window
 * @param id
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_aux_hint_del(Ecore_Wl2_Window *window, int id);

/**
 * @internal
 *
 * @brief Get the activated state of a window
 *
 * @param window The window to get activated state from
 *
 * @return @c EINA_TRUE if activated
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_window_activated_get(const Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * @brief Set the seat for a popup window to be used with grab
 *
 * @param window The window
 * @param input The seat
 *
 * Use this function for desktop shell requests involving popup grabs which require
 * a seat for the grab.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_popup_input_set(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * @brief Get the seat for a popup window to be used with grab
 *
 * @param window The window
 *
 * @return Returns Ecore_Wl2_Input if the window has an input.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI Ecore_Wl2_Input *ecore_wl2_window_popup_input_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Check if a window has a shell surface - without one it can't be visible.
 *
 * @param win The window to check
 *
 * @return Returns true if the window has an associated shell surface.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.19
 */
EAPI Eina_Bool ecore_wl2_window_shell_surface_exists(Ecore_Wl2_Window *win);

// TIZEN_ONLY(171108) : get shell surface of a given window
/** @internal */
EAPI struct zxdg_surface_v6 *ecore_wl2_window_shell_surface_get(Ecore_Wl2_Window *window);
//

/**
 * @internal
 *
 * Set the role of a given window
 *
 * @param window
 * @param role
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_role_set(Ecore_Wl2_Window *window, const char *role);

/**
 * @internal
 *
 * Get the role of a given window
 *
 * @param window The window to set the class role
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI const char *ecore_wl2_window_role_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Set if a given window is in floating mode
 *
 * @param window The window to set floating mode on
 * @param floating EINA_TRUE if this window should be in floating mode, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI void ecore_wl2_window_floating_mode_set(Ecore_Wl2_Window *window, Eina_Bool floating);

/**
 * @internal
 *
 * Get if a given window is in floating mode
 *
 * @param window The window to get floating mode
 *
 * @return EINA_TRUE if floating, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_window_floating_mode_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Finds a window by surface
 *
 * @param surface The surface to find the window of
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
EAPI Ecore_Wl2_Window *ecore_wl2_window_surface_find(struct wl_surface *surface);

/*
 * @internal
 *
 * Check if a wayland window's surface is in the pending state.
 *
 * A surface is pending if it's been commit but we haven't received a
 * frame callback for it yet.  This mean's we're not ready to draw yet.
 *
 * @param window The window whose surface we want to check
 *
 * @return whether the window's surface is pending or not.
 *
 * @since 1.21
 */
EAPI Eina_Bool ecore_wl2_window_pending_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Get if a given window is resizing
 *
 * @param window The window to check for resizing
 *
 * @return EINA_TRUE if resizing, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
EAPI Eina_Bool ecore_wl2_window_resizing_get(Ecore_Wl2_Window *window);

/**
 * @internal
 *
 * Latch window state at the start of an update
 *
 * When async render takes place we continue to dispatch wayland
 * events from the main loop. We need to defer any changes to
 * window state from those events until the update is complete.
 *
 * Events deferred during an update will automatically fire
 * immediately after the caller calls ecore_wl2_window_commit.
 *
 * @param window The window whose state we want to latch
 *
 * @since 1.21
 */
EAPI void ecore_wl2_window_update_begin(Ecore_Wl2_Window *window);

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
/** @internal */
EAPI void ecore_wl2_window_pointer_set(Ecore_Wl2_Window *win, struct wl_surface *surface, int hot_x, int hot_y);
/** @internal */
EAPI void ecore_wl2_window_cursor_from_name_set(Ecore_Wl2_Window *win, const char *cursor_name);
/** @internal */
EAPI void ecore_wl2_window_cursor_default_restore(Ecore_Wl2_Window *win);
//
// TIZEN_ONLY(20150703) : support conformant
/** @internal */
EAPI void ecore_wl2_window_conformant_set(Ecore_Wl2_Window *win, unsigned int is_conformant);
/** @internal */
EAPI Eina_Bool ecore_wl2_window_conformant_get(Ecore_Wl2_Window *win);
//

// TIZEN_ONLY(20171108) : add functions for indicator
/** @internal */
EAPI void ecore_wl2_window_indicator_geometry_set(Ecore_Wl2_Window *win, int x, int y, int w, int h);
/** @internal */
EAPI Eina_Bool ecore_wl2_window_indicator_geometry_get(Ecore_Wl2_Window *win, int *x, int *y, int *w, int *h);
/** @internal */
EAPI void ecore_wl2_window_indicator_state_set(Ecore_Wl2_Window *win, Ecore_Wl2_Indicator_State state);
/** @internal */
EAPI Ecore_Wl2_Indicator_State ecore_wl2_window_indicator_state_get(Ecore_Wl2_Window *win);
/** @internal */
EAPI void ecore_wl2_window_indicator_opacity_set(Ecore_Wl2_Window *win, Ecore_Wl2_Indicator_Opacity_Mode mode);
/** @internal */
EAPI Ecore_Wl2_Indicator_Opacity_Mode ecore_wl2_window_indicator_opacity_get(Ecore_Wl2_Window *win);
/** @internal */
EAPI void ecore_wl2_indicator_visible_type_set(Ecore_Wl2_Window *win, Ecore_Wl2_Indicator_Visible_Type type);
/** @internal */
EAPI Ecore_Wl2_Indicator_Visible_Type ecore_wl2_indicator_visible_type_get(Ecore_Wl2_Window *win);
//

// TIZEN_ONLY(20171108) : add functions for clipboard
/** @internal */
EAPI void ecore_wl2_window_clipboard_geometry_set(Ecore_Wl2_Window *win, int x, int y, int w, int h);
/** @internal */
EAPI Eina_Bool ecore_wl2_window_clipboard_geometry_get(Ecore_Wl2_Window *win, int *x, int *y, int *w, int *h);
/** @internal */
EAPI void ecore_wl2_window_clipboard_state_set(Ecore_Wl2_Window *win, Ecore_Wl2_Clipboard_State state);
/** @internal */
EAPI Ecore_Wl2_Clipboard_State ecore_wl2_window_clipboard_state_get(Ecore_Wl2_Window *win);
/** @internal */
EAPI void ecore_wl2_clipboard_show(Ecore_Wl2_Window *win);
/** @internal */
EAPI void ecore_wl2_clipboard_hide(Ecore_Wl2_Window *win);
/** @internal */
EAPI Eina_Bool ecore_wl2_clipboard_data_only_set(Eina_Bool data_only);
//

// TIZEN_ONLY(20171108) : add functions for keyboard
/** @internal */
EAPI void ecore_wl2_window_keyboard_geometry_set(Ecore_Wl2_Window *win, int x, int y, int w, int h);
/** @internal */
EAPI Eina_Bool ecore_wl2_window_keyboard_geometry_get(Ecore_Wl2_Window *win, int *x, int *y, int *w, int *h);
/** @internal */
EAPI void ecore_wl2_window_keyboard_state_set(Ecore_Wl2_Window *win, Ecore_Wl2_Virtual_Keyboard_State state);
/** @internal */
EAPI Ecore_Wl2_Virtual_Keyboard_State ecore_wl2_window_keyboard_state_get(Ecore_Wl2_Window *win);
//
// TIZEN_ONLY(20171107): add ecore_wl2_window_input_get() EAPI
/**
 * @internal
 * @brief Gets the input device (seat) associated with a Wayland window
 *
 * Retrieves the input device (seat) that is currently interacting with
 * the specified window. This can be used to access input-related functionality
 * for the window.
 *
 * @param win The window to get the input device from
 * @return The Ecore_Wl2_Input object representing the input device,
 *         or NULL if no input device is associated with the window
 *
 * @see Ecore_Wl2_Input
 */
EAPI Ecore_Wl2_Input *ecore_wl2_window_input_get(Ecore_Wl2_Window *win);
//
// TIZEN_ONLY(20200326): add ecore_wl2_window_pointer/keyboard_get() EAPI
/**
 * @internal
 * @brief Gets the pointer input device associated with a Wayland window
 *
 * Retrieves the pointer input device (mouse/touchpad) that is currently
 * interacting with the specified window.
 *
 * @param win The window to get the pointer device from
 * @return The Ecore_Wl2_Input object representing the pointer device,
 *         or NULL if no pointer is associated with the window
 *
 * @see Ecore_Wl2_Input
 */
EAPI Ecore_Wl2_Input *ecore_wl2_window_pointer_get(Ecore_Wl2_Window *win);

/**
 * @internal
 * @brief Gets the keyboard input device associated with a Wayland window
 *
 * Retrieves the keyboard input device that is currently interacting
 * with the specified window.
 *
 * @param win The window to get the keyboard device from
 * @return The Ecore_Wl2_Input object representing the keyboard device,
 *         or NULL if no keyboard is associated with the window
 *
 * @see Ecore_Wl2_Input
 */
EAPI Ecore_Wl2_Input *ecore_wl2_window_keyboard_get(Ecore_Wl2_Window *win);
//
// TIZEN_ONLY(20171114): support a pointer warp
/**
 * @internal
 * @brief Warps (moves) the pointer to specified coordinates within a window
 *
 * Programmatically moves the pointer to the specified coordinates relative
 * to the window. This can be used to implement custom pointer control.
 *
 * @param win The window to warp pointer within
 * @param x The x coordinate relative to window
 * @param y The y coordinate relative to window
 * @return EINA_TRUE on success, EINA_FALSE on failure
 *
 * @note Requires appropriate permissions to move pointer programmatically
 */
EAPI Eina_Bool ecore_wl2_window_pointer_warp(Ecore_Wl2_Window *win, int x, int y);
//
//TIZEN_ONLY(20171115): support output transform
/** @internal */
EAPI Eina_Bool ecore_wl2_window_ignore_output_transform_get(Ecore_Wl2_Window *win);
//
//TIZEN_ONLY(20180810): support client demand move resize
/** @internal */
EAPI void ecore_wl2_window_sync_geometry_set(Ecore_Wl2_Window *window, uint32_t serial, int x, int y, int w, int h);
//

/**
 * @internal
 *
 * Get the wl_seat that an input is using
 *
 * @param input The Ecore_Wl2_Input to get the seat of
 *
 * @return The wl_seat of this input, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.17
 */
EAPI struct wl_seat *ecore_wl2_input_seat_get(Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Get the seat capabilities for a given input.
 *
 * @param input The input
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.19
 */
EAPI Ecore_Wl2_Seat_Capabilities ecore_wl2_input_seat_capabilities_get(Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Get the wayland's seat id from an input.
 *
 * @param input The input
 *
 * @return The seat id
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.19
 */
EAPI unsigned int ecore_wl2_input_seat_id_get(Ecore_Wl2_Input *input) EINA_WARN_UNUSED_RESULT;

/**
 * @internal
 *
 * Get the display object of an input
 *
 * @param input The input
 *
 * @return The display
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI Ecore_Wl2_Display *ecore_wl2_input_display_get(const Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Get the xkb_keymap object of an input
 *
 * @param input The input
 *
 * @return The xkb_keymap object
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI struct xkb_keymap *ecore_wl2_input_keymap_get(const Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Get the name of an input
 *
 * @param input The input
 *
 * @return The name
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI Eina_Stringshare *ecore_wl2_input_name_get(Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Get the keyboard repeat rate and delay of an input
 *
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI Eina_Bool ecore_wl2_input_keyboard_repeat_get(const Ecore_Wl2_Input *input, double *rate, double *delay);

/**
 * @internal
 *
 * Set the keyboard repeat rate and delay of an input
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 * @return True if repeat is enabled
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.24
 */
EAPI Eina_Bool ecore_wl2_input_keyboard_repeat_set(Ecore_Wl2_Input *input, double rate, double delay);

/**
 * @internal
 *
 * Get the keyboard repeat rate and delay of horizontal way
 *
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Ecore_Wl2_Input_Group
 */
EAPI Eina_Bool ecore_wl2_input_keyboard_horizontal_way_repeat_get(const Ecore_Wl2_Input *input, double *rate, double *delay);

/**
 * @internal
 *
 * Set the keyboard repeat rate and delay of horizontal way
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Ecore_Wl2_Input_Group
 */
EAPI Eina_Bool ecore_wl2_input_keyboard_horizontal_way_repeat_set(Ecore_Wl2_Input *input, double rate, double delay);

/**
 * @internal
 *
 * Get the keyboard repeat rate and delay of vertical way
 *
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Ecore_Wl2_Input_Group
 */
EAPI Eina_Bool ecore_wl2_input_keyboard_vertical_way_repeat_get(const Ecore_Wl2_Input *input, double *rate, double *delay);

/**
 * @internal
 *
 * Set the keyboard repeat rate and delay of vertical way
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Ecore_Wl2_Input_Group
 */
EAPI Eina_Bool ecore_wl2_input_keyboard_vertical_way_repeat_set(Ecore_Wl2_Input *input, double rate, double delay);

/**
 * @internal
 *
 * Set a given wl_surface to use as the pointer on a seat
 *
 * @param input The seat to set this surface as the pointer on
 * @param surface The surface to use as the pointer
 * @param hot_x The x coordinate to use as the cursor hot spot
 * @param hot_y The y coordinate to use as the cursor hot spot
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI void ecore_wl2_input_pointer_set(Ecore_Wl2_Input *input, struct wl_surface *surface, int hot_x, int hot_y);

/**
 * @internal
 *
 * Set a specific cursor on a given input
 *
 * @brief This function will try to find a matching cursor inside the existing
 * cursor theme and set the pointer for the specified seat to be
 * the specified cursor
 *
 * @param input The seat to set the cursor on
 * @param cursor The name of the cursor to try and set
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
EAPI void ecore_wl2_input_cursor_from_name_set(Ecore_Wl2_Input *input, const char *cursor);

// TIZEN_ONLY(20171207): add functions to set client's custom cursors
/** @internal */
EAPI void ecore_wl2_input_pointer_set(Ecore_Wl2_Input *input, struct wl_surface *surface, int hot_x, int hot_y);
/** @internal */
EAPI struct wl_cursor *ecore_wl2_input_cursor_get(Ecore_Wl2_Input *input, const char *cursor_name);
/** @internal */
EAPI void ecore_wl2_input_cursor_size_set(Ecore_Wl2_Input *input, const int size);
/** @internal */
EAPI void ecore_wl2_input_cursor_theme_name_set(Ecore_Wl2_Input *input, const char *cursor_theme_name);
/** @internal */
EAPI void ecore_wl2_input_cursor_default_restore(Ecore_Wl2_Input *input);
//

/**
 * @defgroup Ecore_Wl2_Dnd_Group Wayland Library Drag-n-Drop Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Drag-n-Drop
 */

/**
 * @internal
 *
 * Set the types which are supported by a possible drag and drop operation.
 * This call initializes a data source and offers the given mimetypes
 *
 * @param input the input where to add on the data source
 * @param types a null-terminated array of mimetypes
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.17
 */
EAPI void ecore_wl2_dnd_drag_types_set(Ecore_Wl2_Input *input, const char **types);

/**
 * @internal
 *
 * Start a drag on the given input
 *
 * @param input the input to use
 * @param window the window which is the origin of the drag operation
 * @param drag_window the window which is used as window of the visible hint.
 *
 * @return The serial for the start_drag request
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.17
 */
EAPI uint32_t ecore_wl2_dnd_drag_start(Ecore_Wl2_Input *input, Ecore_Wl2_Window *window, Ecore_Wl2_Window *drag_window);

/**
 * @internal
 *
 * Call wl_data_source.set_actions on an existing source
 *
 * @param input the input to use
 *
 * @see ecore_wl2_dnd_drag_start for a more useful function.
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.20
 */
EAPI void ecore_wl2_dnd_set_actions(Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * End a drag started by a call to ecore_wl2_dnd_drag_start
 *
 * @param input the input object on which the drag was started
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.17
 */
EAPI void ecore_wl2_dnd_drag_end(Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Get the offer which is currently responsible for the clipboard
 *
 * @param input the input object to use
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI Ecore_Wl2_Offer* ecore_wl2_dnd_selection_get(Ecore_Wl2_Input *input);

/**
 * @internal
 *
 * Set the types which are available from this client
 * Later the event ECORE_WL2_EVENT_DATA_SOURCE_SEND is emitted,
 * where the caller of this api must write the data (encoded in the given mimetype) to the fd
 *
 * @param input the input to provide this types on
 * @param types a null-terminated array of mimetypes supported by the client
 *
 * @return serial of request on success, 0 on failure
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.17
 */
EAPI uint32_t ecore_wl2_dnd_selection_set(Ecore_Wl2_Input *input, const char **types);

/**
 * @internal
 *
 * Clear the selection currently setted on this input.
 *
 * @param input the input to clear
 *
 * @return serial of request on success, 0 on failure
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.17
 */
EAPI uint32_t ecore_wl2_dnd_selection_clear(Ecore_Wl2_Input *input);

// TIZEN_ONLY(20211018): added ecore_wl2_offer functions to ecore_wl2
/**
 * @internal
 *
 * Get the actions available from the data source
 *
 * @param offer Offer object to use
 *
 * @return or´ed values from Ecore_Wl2_Drag_Action which are describing the available actions
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI Ecore_Wl2_Drag_Action ecore_wl2_offer_actions_get(Ecore_Wl2_Offer *offer);

/**
 * @internal
 *
 * Set the actions which are supported by you
 *
 * @param offer Offer object to use
 * @param actions A or´ed value of multiple Ecore_Wl2_Drag_Action values
 * @param action the preferred action out of the actions
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI void ecore_wl2_offer_actions_set(Ecore_Wl2_Offer *offer, Ecore_Wl2_Drag_Action actions, Ecore_Wl2_Drag_Action action);

/**
 * @internal
 *
 * Get action which is set by either the data source or in the last call of actions_set
 *
 * @param offer Offer object to use
 *
 * @return the preferred action
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI Ecore_Wl2_Drag_Action ecore_wl2_offer_action_get(Ecore_Wl2_Offer *offer);

/**
 * @internal
 *
 * Get the mime types which are given by the source
 *
 * @param offer the offer to query
 *
 * @return a eina array of strdup´ed strings, this array must NOT be changed or freed
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI Eina_Array* ecore_wl2_offer_mimes_get(Ecore_Wl2_Offer *offer);

/**
 * @internal
 *
 * Set mimetypes you are accepting under this offer
 *
 * @param offer the offer to use
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI void ecore_wl2_offer_mimes_set(Ecore_Wl2_Offer *offer, Eina_Array *mimes);

/**
 * @internal
 *
 * Accept a single mime type for an offer
 *
 * @param offer the offer to use
 * @param mime_type the mime type
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.20
 */
EAPI void ecore_wl2_offer_accept(Ecore_Wl2_Offer *offer, const char *mime_type);

/**
 * @internal
 *
 * Request the data from this offer.
 * The event ECORE_WL2_EVENT_OFFER_DATA_READY is called when the data is available.
 * There offer will be not destroyed as long as requested data is not emitted by the event.
 *
 * @param offer the offer to use
 * @param mime the mimetype to receive
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI void ecore_wl2_offer_receive(Ecore_Wl2_Offer *offer, char *mime);

/**
 * @internal
 *
 * Request the data from this offer on an externally managed fd.
 * The event ECORE_WL2_EVENT_OFFER_DATA_READY is called when the data is available.
 * There offer will be not destroyed as long as requested data is not emitted by the event.
 *
 * @param offer the offer to use
 * @param mime the mimetype to receive
 * @param fd the fd to pass for receiving
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.20
 */
EAPI void ecore_wl2_offer_proxy_receive(Ecore_Wl2_Offer *offer, const char *mime, int fd);

/**
 * @internal
 *
 * End the use of a proxy received offer. This may invalidate the offer object
 *
 * @param offer the offer
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.20
 */
EAPI void ecore_wl2_offer_proxy_receive_end(Ecore_Wl2_Offer *offer);

/**
 * @internal
 *
 * Check if the given offer supports the given mimetype
 *
 * @param offer the offer to use
 * @param mime the mimetype to check
 *
 * @return Returns true if the mimetype is supported by this offer, false if not
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI Eina_Bool ecore_wl2_offer_supports_mime(Ecore_Wl2_Offer *offer, const char *mime);

/**
 * @internal
 *
 * Mark this offer as finished
 * This will call the dnd_finished event on the source of the sender.
 *
 * @param offer the offer to use
 *
 * @ingroup Ecore_Wl2_Dnd_Group
 * @since 1.19
 */
EAPI void ecore_wl2_offer_finish(Ecore_Wl2_Offer *offer);
//

/**
 * @defgroup Ecore_Wl2_Output_Group Wayland Library Output Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Outputs
 */

/**
 * @internal
 *
 * Return the DPI of a given output
 *
 * This is a simplistic call to get DPI. It does not account for differing
 * DPI in the x and y axes nor does it account for multihead or xinerama and
 * xrandr where different parts of the screen may have different DPI etc.
 *
 * @param output The output to get the DPI of
 *
 * @return the general screen DPI (dots/pixels per inch).
 *
 * @ingroup Ecore_Wl2_Output_Group
 * @since 1.17
 */
EAPI int ecore_wl2_output_dpi_get(Ecore_Wl2_Output *output);

/**
 * @internal
 *
 * Get the current transform of a given output
 *
 * @param output The output to get the transform of
 *
 * @return The output's current transform value
 *
 * @ingroup Ecore_Wl2_Output_Group
 * @since 1.20
 */
EAPI int ecore_wl2_output_transform_get(Ecore_Wl2_Output *output);

/**
 * @internal
 *
 * Disable session recovery for any further connections.  Must be called
 * before connecting.  This is irreversible and not intended for general
 * use.
 *
 * @since 1.19
 */
EAPI void ecore_wl2_session_recovery_disable(void);

// TIZEN_ONLY(20250109): support ecore_wl2_screen API
/**
 * @internal
 *
 * @brief Retrieve the screen list from connected display.
 *
 * @param display The target display
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI Eina_List *ecore_wl2_display_screens_get(Ecore_Wl2_Display *display);

/**
 * @internal
 *
 * @brief Retrieve the size of given screen.
 *
 * @param screen The target screen
 * @param(out) w width of the screen
 * @param(out) h height of the screen
 *
 * @ingroup Ecore_Wl2_Screen_Group
 */
EAPI void ecore_wl2_screen_size_get(Ecore_Wl2_Screen *screen, int *w, int *h);

/**
 * @internal
 *
 * @brief Retrieve the name of given screen.
 *
 * @param screen The target screen
 * @return The name of screen
 *
 * @ingroup Ecore_Wl2_Screen_Group
 */
EAPI const char *ecore_wl2_screen_name_get(Ecore_Wl2_Screen *screen);

/**
 * @internal
 *
 * @brief Assign the given window into given screen.
 *
 * @param screen The target screen
 * @param window The target window to assign screen
 *
 * @ingroup Ecore_Wl2_Screen_Group
 */
EAPI void ecore_wl2_screen_window_assign(Ecore_Wl2_Screen *screen, Ecore_Wl2_Window *window);
// END of TIZEN_ONLY(20250109)

// TIZEN_ONLY(20250805): support default screen of display
/**
 * @internal
 *
 * @brief Set the default screen of the windows belongs to the display.
 *
 * Set the default screen of the windows belongs to the display.
 * After this function is executed,
 * all newly created ecore_wl2_window objects will belong to this screen.
 * However, it does not alter the screen assignment of existing windows.
 *
 * @param display The target display
 * @param screen The target screen to set as default
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI void ecore_wl2_display_default_screen_set(Ecore_Wl2_Display *ewd, Ecore_Wl2_Screen *ews);

/**
 * @internal
 *
 * @brief Retrieve the default screen of the given display.
 *
 * @param display The target display
 * @return NULL if there's no default screen, or the default screen of the display
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI Ecore_Wl2_Screen *ecore_wl2_display_default_screen_get(Ecore_Wl2_Display *ewd);
// END of TIZEN_ONLY(20250805)

// TIZEN_ONLY(250828) : support multi screen
/**
 * @internal
 *
 * @brief Retrieve the list of windows belong to the given display.
 *
 * @param display The display to get window list
 * @return NULL if there's no window or invalid display, or the list of windows belong to the display as Eina_List.
 *
 * @note The caller of this function should free the returned Eina_List when finished with it.
 *
 * @ingroup Ecore_Wl2_Display_Group
 */
EAPI Eina_List *ecore_wl2_display_window_list_get(Ecore_Wl2_Display *display);
// END of TIZEN_ONLY(250828)

/* TODO: doxygen if we are keeping any of the below functions public */
/** @internal */
EAPI Ecore_Wl2_Surface *ecore_wl2_surface_create(Ecore_Wl2_Window *win, Eina_Bool alpha);

// TIZEN_ONLY(20171107): support a tizen_keyrouter interface
EAPI void ecore_wl2_display_sync(Ecore_Wl2_Display *display);

/**
 * @platform
 * @brief Sets keygrab value of the window
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * This function grabs the @c key of window using @c grab_mode
 *
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/keygrab
 *
 * @param win The target Ecore_Wl2_Window
 * @param key The string is the keyname to grab
 * @param mod A combination of modifier keys that must be present to
 *              trigger the event. Not supported yet
 * @param not_mod A combination of modifier keys that must not be
 *                present to trigger the event. Not supported yet
 * @param priority Not supported yet
 * @param grab_mode Describes how the key should be grabbed, wrt. focus and
 *                  stacking
 *
 * @return @c true on success, @c false otherwise
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_ALREADY_IN_PROGRESS The key already has been grabbed
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_Bool ecore_wl2_window_keygrab_set(Ecore_Wl2_Window *win, const char *key, int mod EINA_UNUSED, int not_mod EINA_UNUSED, int priority EINA_UNUSED, Ecore_Wl2_Window_Keygrab_Mode grab_mode);


/**
 * @platform
 * @brief Unsets keygrab value of the window
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * This function unset keygrab value. Ungrab @c key of window.
 *
 * @privlevel platform
 * @privilege http://tizen.org/privilege/keygrab
 *
 * @param win The target Ecore_Wl2_Window
 * @param key The string is the keyname to grab
 * @param mod A combination of modifier keys that must be present to
 *              trigger the event. Not supported yet
 * @param not_mod A combination of modifier keys that must not be
 *                present to trigger the event. Not supported yet
 *
 * @return @c true on success, @c false otherwise
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_ALREADY_IN_PROGRESS The key already has been grabbed
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_Bool ecore_wl2_window_keygrab_unset(Ecore_Wl2_Window *win, const char *key, int mod EINA_UNUSED, int any_mod EINA_UNUSED);
//

/**
 * @platform
 * @brief Sets keygrab values of the window.
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * The returned list should be freed with @c eina_list_free() when you no
 * longer need it.
 *
 * @privlevel platform
 * @privilege http://tizen.org/privilege/keygrab
 * @param win The window
 * @param infos The list of keys and grab modes for setting keygrab
 *
 * @return NULL, or the list of keys being failed keygrab set.
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @Example
 *
 * Ecore_Wl2_Window_Keygrab_Info *info = malloc(sizeof(Ecore_Wl2_Window_Keygrab_Info));
 * info->key = "XF86AudioLowerVolume";
 * info->mode = ECORE_WL2_WINDOW_KEYGRAB_TOPMOST;
 * infos = eina_list_append(infos, info);
 *
 * ecore_wl2_window_keygrab_list_set(win, infos);
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_List *ecore_wl2_window_keygrab_list_set(Ecore_Wl2_Window *win, Eina_List *infos);

/**
 * @platform
 * @brief Unsets keygrab values of the window.
 * @warning third-party application can use ECORE_WL2_WINDOW_KEYGRAB_TOPMOST mode
 *          without keygrab privilege.
 *          don't use other modes. those are not for third-party application.
 *
 * The returned list should be freed with @c eina_list_free() when you no
 * longer need it.
 *
 * @privlevel platform
 * @privilege http://tizen.org/privilege/keygrab
 *
 * @param win The window
 * @param infos The list of infos for unsetting keygrab
 *
 * @return NULL, or the list of keys being failed keygrab unset.
 *
 * @exception #TIZEN_ERROR_NONE Successfully set
 * @exception #TIZEN_ERROR_INVALID_PARAMETER Invalid parameter has given
 * @exception #TIZEN_ERROR_PERMISSION_DENIED Privilege permission denied
 *
 * @Example
 *
 * Ecore_Wl2_Window_Keygrab_Info *info = malloc(sizeof(Ecore_Wl2_Window_Keygrab_Info));
 * info->key = "XF86AudioLowerVolume";
 * infos = eina_list_append(infos, info);
 *
 * ecore_wl2_window_keygrab_list_unset(win, infos);
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Eina_List *ecore_wl2_window_keygrab_list_unset(Ecore_Wl2_Window *win, Eina_List *infos);

// TIZEN_ONLY(20171107): add ecore_wl2_window_input_get() EAPI
EAPI Ecore_Wl2_Input *ecore_wl2_window_input_get(Ecore_Wl2_Window *win);
//
// TIZEN_ONLY(20200326): add ecore_wl2_window_pointer/keyboard_get() EAPI
/** @internal */
EAPI Ecore_Wl2_Input *ecore_wl2_window_pointer_get(Ecore_Wl2_Window *win);
/** @internal */
EAPI Ecore_Wl2_Input *ecore_wl2_window_keyboard_get(Ecore_Wl2_Window *win);
//
// TIZEN_ONLY(20171114): support a pointer warp
EAPI Eina_Bool ecore_wl2_window_pointer_warp(Ecore_Wl2_Window *win, int x, int y);
//
// TIZEN_ONLY(20200601): support sync between ui and video

//TIZEN_ONLY(20171108): add a new API to ecore_wl2_sync
/** @internal */
EAPI void ecore_wl2_sync(void);
//

//TIZEN_ONLY(20220322): add resource id to window attribute
/** @internal */
EAPI unsigned int ecore_wl2_window_resource_id_get(Ecore_Wl2_Window *window);
//

// TIZEN_ONLY(20221208): support default cursor configuration
/** @internal */
EAPI void ecore_wl2_cursor_config_name_set(const char *name);
//

//TIZEN_ONLY(20221228): add resize request set for ignoring configure event
/** @internal */
EAPI void ecore_wl2_window_resize_request_set(Ecore_Wl2_Window *window, int w, int h);
//
//TIZEN_ONLY(20230220): unset pending resize request
/** @internal */
EAPI void ecore_wl2_window_resize_request_unset(Ecore_Wl2_Window *window);
//

//TIZEN_ONLY(20230306): support pin mode
/** @internal */
EAPI void ecore_wl2_window_pin_mode_set(Ecore_Wl2_Window *window, Eina_Bool pinned);
/** @internal */
EAPI Eina_Bool ecore_wl2_window_pin_mode_get(Ecore_Wl2_Window *window);
//

// TIZEN_ONLY(20230801) : support zwp pointer constraints protocol
/**
 * @brief Lock pointer to a window with constraints
 * @param win The window to lock pointer to
 * @return EINA_TRUE if lock was successful, EINA_FALSE otherwise
 *
 * This function locks the pointer to the specified window, preventing it
 * from moving outside the window bounds. This is typically used for
 * implementing features like first-person view controls or drawing applications
 * where pointer confinement is needed.
 *
 * @see ecore_wl2_window_pointer_constraints_unlock_pointer
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI Eina_Bool ecore_wl2_window_pointer_constraints_lock_pointer(Ecore_Wl2_Window *win);

/**
 * @brief Unlock pointer from constraints
 * @param win The window to unlock pointer from
 * @return EINA_TRUE if unlock was successful, EINA_FALSE otherwise
 *
 * This function releases a previously established pointer lock
 * for the specified window. This should be called when pointer
 * confinement is no longer needed.
 *
 * @see ecore_wl2_window_pointer_constraints_lock_pointer
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI Eina_Bool ecore_wl2_window_pointer_constraints_unlock_pointer(Ecore_Wl2_Window *win);

/**
 * @brief Set locked pointer region for a window
 * @param win The window to set region for
 * @param x The x coordinate of region
 * @param y The y coordinate of region
 * @param w The width of region
 * @param h The height of region
 *
 * This function sets the region where the pointer is locked when
 * ecore_wl2_window_pointer_constraints_lock_pointer() is active.
 * The pointer will be confined to this region.
 *
 * @see ecore_wl2_window_pointer_constraints_lock_pointer
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI void ecore_wl2_window_locked_pointer_region_set(Ecore_Wl2_Window *win, int x, int y, int w, int h);

/**
 * @brief Set cursor position hint for locked pointer to prevent cursor jump
 * @param win The window to set hint for
 * @param x The x coordinate of hint position
 * @param y The y coordinate of hint position
 *
 * This function provides a hint for the cursor position when the pointer
 * is locked. The compositor may use this to position the cursor when
 * the lock is activated.
 *
 * @see ecore_wl2_window_pointer_constraints_lock_pointer
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI void ecore_wl2_window_locked_pointer_cursor_position_hint_set(Ecore_Wl2_Window *win, int x, int y);

// TIZEN_ONLY(20230821) : add cursor_visible set API
/**
 * @brief Set cursor visibility for a window
 * @param win The window to set cursor visibility
 * @param visible EINA_TRUE to show cursor, EINA_FALSE to hide
 *
 * This function controls whether the cursor is visible when over
 * the specified window.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI void ecore_wl2_window_cursor_visible_set(Ecore_Wl2_Window *win, Eina_Bool visible);
//

// TIZEN_ONLY(20230823) : add keyboard_grab/ungrab API
/**
 * @brief Grab keyboard input for a window
 * @param win The window to grab keyboard input for
 * @param subtype The type of keyboard grab (0 for normal grab)
 * @return EINA_TRUE if grab was successful, EINA_FALSE otherwise
 *
 * This function grabs keyboard input events for the specified window.
 * The subtype parameter can be used to specify special grab types.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI Eina_Bool ecore_wl2_window_keyboard_grab(Ecore_Wl2_Window *win, unsigned int subtype);

/**
 * @brief Release keyboard grab for a window
 * @param win The window to release keyboard grab from
 * @return EINA_TRUE if ungrab was successful, EINA_FALSE otherwise
 *
 * This function releases a previously established keyboard grab
 * for the specified window.
 *
 * @see ecore_wl2_window_keyboard_grab
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 8.0
 */
EAPI Eina_Bool ecore_wl2_window_keyboard_ungrab(Ecore_Wl2_Window *win);

// TIZEN_ONLY(20240327): Support for modal window
EAPI void ecore_wl2_window_modal_set(Ecore_Wl2_Window *window, Eina_Bool modal);
EAPI Eina_Bool ecore_wl2_window_modal_get(Ecore_Wl2_Window *window);
//

// TIZEN_ONLY(20240718) : add relative_motion grab/ungrab API
/**
 * @brief Grab relative pointer motion for a window
 * @param win The window to grab relative motion for
 * @param boundary The boundary type for the relative motion grab
 * @return EINA_TRUE if grab was successful, EINA_FALSE otherwise
 *
 * This function grabs relative pointer motion events for the specified window.
 * The boundary parameter defines the edge where the relative motion should be
 * constrained (top, bottom, left, right, or none).
 *
 * This is typically used for implementing edge-scrolling or other boundary-aware
 * pointer interactions.
 *
 * @see Ecore_Wl2_Pointer_Boundary
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 9.0
 */
EAPI Eina_Bool ecore_wl2_window_relative_motion_grab(Ecore_Wl2_Window *win, Ecore_Wl2_Pointer_Boundary boundary);

/**
 * @brief Release relative pointer motion grab for a window
 * @param win The window to release relative motion grab from
 * @return EINA_TRUE if ungrab was successful, EINA_FALSE otherwise
 *
 * This function releases a previously established relative pointer motion grab
 * for the specified window. It should be called when boundary-aware pointer
 * interactions (like edge-scrolling) are no longer needed.
 *
 * @see ecore_wl2_window_relative_motion_grab
 * @ingroup Ecore_Wl2_Window_Group
 * @since_tizen 9.0
 */
EAPI Eina_Bool ecore_wl2_window_relative_motion_ungrab(Ecore_Wl2_Window *win);
//

// TIZEN_ONLY(20250805): support default screen of display
/**
 * @internal
 *
 * @brief Retrieve the screen of the given window.
 *
 * Retrieve the screen of the given window.
 * The screen is assigned by the display server when the window is created.
 * Therefore, the screen of the window could be NULL
 * until the window receives event from display server.
 *
 * @param window The target window to get screen
 * @return NULL if the window belongs to any screen, or the screen that the window blongs to
 *
 * @ingroup Ecore_Wl2_Window_Group
 */
EAPI Ecore_Wl2_Screen *ecore_wl2_window_screen_get(Ecore_Wl2_Window *win);
// END of TIZEN_ONLY(20250805)

# undef EAPI
# define EAPI

# ifdef __cplusplus
}
# endif

#endif
