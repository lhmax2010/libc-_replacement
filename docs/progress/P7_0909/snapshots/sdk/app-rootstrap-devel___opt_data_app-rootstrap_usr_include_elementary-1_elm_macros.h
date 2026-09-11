/* handy macros */
#define ELM_RECTS_INTERSECT(x, y, w, h, xx, yy, ww, hh) (((x) < ((xx) + (ww))) && ((y) < ((yy) + (hh))) && (((x) + (w)) > (xx)) && (((y) + (h)) > (yy)))
#define ELM_PI 3.14159265358979323846
#define ELM_SCALE_SIZE(x) ((int)(((double)(x) * elm_config_scale_get()) / elm_app_base_scale_get() + 0.5))

// checks if the point(xx, yy) stays out of the rectangle(x, y, w, h) area.
#define ELM_RECTS_POINT_OUT(x, y, w, h, xx, yy) (((xx) < (x)) || ((yy) < (y)) || ((xx) > ((x) + (w))) || ((yy) > ((y) + (h))))

// check if the rect (x, y, w, h) includes whole body of rect (xx, yy, ww, hh)
#define ELM_RECTS_INCLUDE(x, y, w, h, xx, yy, ww, hh) (((x) <= (xx)) && (((x) + (w)) >= ((xx + (ww))) && ((y) <= (yy)) && (((y) + (h)) >= ((yy) + (hh)))))

// check if the rect (x,y,w,h) is either left of or stays out of body of rect(xx,yy,ww,hh)
#define ELM_RECTS_X_AXIS_OUT(x, y, w, h, xx, yy, ww, hh) (((x) < (xx)) || (((x) + (w)) > ((xx) + (ww))) || (((y) + (h)) > ((yy) + (hh))))

// check if the rect (x,y,w,h) is either top of or stays out of body of rect(xx,yy,ww,hh)
#define ELM_RECTS_Y_AXIS_OUT(x, y, w, h, xx, yy, ww, hh) (((y) < (yy)) || (((x) + (w)) > ((xx) + (ww))) || (((y) + (h)) > ((yy) + (hh))))

//TIZEN_ONLY(20230327): add layout set API.
// Window is placed on the left half of the screen
#define ELM_WIN_LAYOUT_TYPE_LEFT_HALF 2, 1, 0, 0, 1, 1
// Window is placed on the right half of the screen
#define ELM_WIN_LAYOUT_TYPE_RIGHT_HALF 2, 1, 1, 0, 1, 1
// Window is placed on the top half of the screen
#define ELM_WIN_LAYOUT_TYPE_TOP_HALF 1, 2, 0, 0, 1, 1
// Window is placed on the bottom half of the screen
#define ELM_WIN_LAYOUT_TYPE_BOTTOM_HALF 1, 2, 0, 1, 1, 1
// Window is placed on the upper-left quarter of the screen
#define ELM_WIN_LAYOUT_TYPE_UPPER_LEFT_QUARTER 2, 2, 0, 0, 1, 1
// Window is placed on the upper-right quarter of the screen
#define ELM_WIN_LAYOUT_TYPE_UPPER_RIGHT_QUATER 2, 2, 1, 0, 1, 1
// Window is placed on the lower-left quarter of the screen
#define ELM_WIN_LAYOUT_TYPE_LOWER_LEFT_QUATER 2, 2, 0, 1, 1, 1
// Window is placed on the lower-right quarter of the screen
#define ELM_WIN_LAYOUT_TYPE_LOWER_RIGHT_QUATER 2, 2, 1, 1, 1, 1
// Window is placed on the left third of the screen horizontally
#define ELM_WIN_LAYOUT_TYPE_LEFT_THIRD 3, 1, 0, 0, 1, 1
// Window is placed on the center third of the screen horizontally
#define ELM_WIN_LAYOUT_TYPE_CENTER_THIRD 3, 1, 1, 0, 1, 1
// Window is placed on the right third of the screen horizontally
#define ELM_WIN_LAYOUT_TYPE_RIGHT_THIRD 3, 1, 2, 0, 1, 1
// Window is placed on the top third of the screen vertically
#define ELM_WIN_LAYOUT_TYPE_TOP_THIRD 1, 3, 0, 0, 1, 1
// Window is placed on the middle third of the screen vertically
#define ELM_WIN_LAYOUT_TYPE_MIDDLE_THIRD 1, 3, 0, 1, 1, 1
// Window is placed on the bottom third of the screen vertically
#define ELM_WIN_LAYOUT_TYPE_BOTTOM_THIRD 1, 3, 0, 2, 1, 1
//

