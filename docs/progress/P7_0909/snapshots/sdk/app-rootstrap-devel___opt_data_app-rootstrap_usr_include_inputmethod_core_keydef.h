/*
 * Copyright (c) 2026 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_UIX_INPUTMETHOD_CORE_KEYDEF_H__
#define __TIZEN_UIX_INPUTMETHOD_CORE_KEYDEF_H__

/**
 * @file inputmethod_core_keydef.h
 * @brief This file contains key symbol and mask enumeration.
 */

 /**
 * @brief Enumeration for the key symbols.
 *        If key symbol & 0xff000000 == 0x01000000 then this key symbol is directly encoded 24-bit UCS character.
 *        The UCS value is key symbol & 0x00ffffff.
 * @details Defines the list of key symbols supported by the system.
 *          Note that certain key symbols may not be available on all devices.
 * @since_tizen 11.0
 */
typedef enum {
	INPUTMETHOD_KEY_BackSpace = 0xFF08, /**< The backspace key */
	INPUTMETHOD_KEY_Tab = 0xFF09, /**< The tab key */
	INPUTMETHOD_KEY_Linefeed = 0xFF0A, /**< The linefeed key */
	INPUTMETHOD_KEY_Clear = 0xFF0B, /**< The clear key */
	INPUTMETHOD_KEY_Return = 0xFF0D, /**< The return key */
	INPUTMETHOD_KEY_Pause = 0xFF13, /**< The pause key */
	INPUTMETHOD_KEY_Scroll_Lock = 0xFF14, /**< The scroll lock key */
	INPUTMETHOD_KEY_Sys_Req = 0xFF15, /**< The sys req key */
	INPUTMETHOD_KEY_Escape = 0xFF1B, /**< The escape key */
	INPUTMETHOD_KEY_Delete = 0xFFFF, /**< The delete key */

	/* Cursor control & motion */
	INPUTMETHOD_KEY_Home = 0xFF50, /**< The home key */
	INPUTMETHOD_KEY_Left = 0xFF51, /**< The left directional key */
	INPUTMETHOD_KEY_Up = 0xFF52, /**< The up directional key */
	INPUTMETHOD_KEY_Right = 0xFF53, /**< The right directional key */
	INPUTMETHOD_KEY_Down = 0xFF54, /**< The down directional key */
	INPUTMETHOD_KEY_Prior = 0xFF55, /**< The prior, previous key */
	INPUTMETHOD_KEY_Page_Up = 0xFF55, /**< The page up key */
	INPUTMETHOD_KEY_Next = 0xFF56, /**< The next key */
	INPUTMETHOD_KEY_Page_Down = 0xFF56, /**< The page down key */
	INPUTMETHOD_KEY_End = 0xFF57, /**< The end key */
	INPUTMETHOD_KEY_Begin = 0xFF58, /**< The begin key */

	/* Misc Functions */
	INPUTMETHOD_KEY_Select = 0xFF60, /**< The select key */
	INPUTMETHOD_KEY_Print = 0xFF61, /**< The print key */
	INPUTMETHOD_KEY_Execute = 0xFF62, /**< The execute, run, do key */
	INPUTMETHOD_KEY_Insert = 0xFF63, /**< The insert key */
	INPUTMETHOD_KEY_Undo = 0xFF65, /**< The undo key */
	INPUTMETHOD_KEY_Redo = 0xFF66, /**< The redo key */
	INPUTMETHOD_KEY_Menu = 0xFF67, /**< The menu key */
	INPUTMETHOD_KEY_Find = 0xFF68, /**< The find key */
	INPUTMETHOD_KEY_Cancel = 0xFF69, /**< The cancel, stop, abort, exit key */
	INPUTMETHOD_KEY_Help = 0xFF6A, /**< The help key */
	INPUTMETHOD_KEY_Break = 0xFF6B, /**< The break key */
	INPUTMETHOD_KEY_Mode_switch = 0xFF7E, /**< The character set switch key */
	INPUTMETHOD_KEY_Num_Lock = 0xFF7F, /**< The num lock key */

	/* Keypad */
	INPUTMETHOD_KEY_KP_Space = 0xFF80, /**< The Numpad space key */
	INPUTMETHOD_KEY_KP_Tab = 0xFF89, /**< The Numpad tab key */
	INPUTMETHOD_KEY_KP_Enter = 0xFF8D, /**< The Numpad enter key */
	INPUTMETHOD_KEY_KP_F1 = 0xFF91, /**< The Numpad function 1 key */
	INPUTMETHOD_KEY_KP_F2 = 0xFF92, /**< The Numpad function 2 key */
	INPUTMETHOD_KEY_KP_F3 = 0xFF93, /**< The Numpad function 3 key */
	INPUTMETHOD_KEY_KP_F4 = 0xFF94, /**< The Numpad function 4 key */
	INPUTMETHOD_KEY_KP_Home = 0xFF95, /**< The Numpad home key */
	INPUTMETHOD_KEY_KP_Left = 0xFF96, /**< The Numpad left key */
	INPUTMETHOD_KEY_KP_Up = 0xFF97, /**< The Numpad up key */
	INPUTMETHOD_KEY_KP_Right = 0xFF98, /**< The Numpad right key */
	INPUTMETHOD_KEY_KP_Down = 0xFF99, /**< The Numpad down key */
	INPUTMETHOD_KEY_KP_Prior = 0xFF9A, /**< The Numpad prior, previous key */
	INPUTMETHOD_KEY_KP_Page_Up = 0xFF9A, /**< The Numpad page up key */
	INPUTMETHOD_KEY_KP_Next = 0xFF9B, /**< The Numpad next key */
	INPUTMETHOD_KEY_KP_Page_Down = 0xFF9B, /**< The Numpad page down key */
	INPUTMETHOD_KEY_KP_End = 0xFF9C, /**< The Numpad end key */
	INPUTMETHOD_KEY_KP_Begin = 0xFF9D, /**< The Numpad begin key */
	INPUTMETHOD_KEY_KP_Insert = 0xFF9E, /**< The Numpad insert key */
	INPUTMETHOD_KEY_KP_Delete = 0xFF9F, /**< The Numpad delete key */
	INPUTMETHOD_KEY_KP_Equal = 0xFFBD, /**< The Numpad equal key */
	INPUTMETHOD_KEY_KP_Multiply = 0xFFAA, /**< The Numpad multiply key */
	INPUTMETHOD_KEY_KP_Add = 0xFFAB, /**< The Numpad add key */
	INPUTMETHOD_KEY_KP_Separator = 0xFFAC, /**< The Numpad separator key */
	INPUTMETHOD_KEY_KP_Subtract = 0xFFAD, /**< The Numpad subtract key */
	INPUTMETHOD_KEY_KP_Decimal = 0xFFAE, /**< The Numpad decimal key */
	INPUTMETHOD_KEY_KP_Divide = 0xFFAF, /**< The Numpad divide key */

	INPUTMETHOD_KEY_KP_0 = 0xFFB0, /**< The Numpad 0 key */
	INPUTMETHOD_KEY_KP_1 = 0xFFB1, /**< The Numpad 1 key */
	INPUTMETHOD_KEY_KP_2 = 0xFFB2, /**< The Numpad 2 key */
	INPUTMETHOD_KEY_KP_3 = 0xFFB3, /**< The Numpad 3 key */
	INPUTMETHOD_KEY_KP_4 = 0xFFB4, /**< The Numpad 4 key */
	INPUTMETHOD_KEY_KP_5 = 0xFFB5, /**< The Numpad 5 key */
	INPUTMETHOD_KEY_KP_6 = 0xFFB6, /**< The Numpad 6 key */
	INPUTMETHOD_KEY_KP_7 = 0xFFB7, /**< The Numpad 7 key */
	INPUTMETHOD_KEY_KP_8 = 0xFFB8, /**< The Numpad 8 key */
	INPUTMETHOD_KEY_KP_9 = 0xFFB9, /**< The Numpad 9 key */

	/* Auxilliary Functions */
	INPUTMETHOD_KEY_F1 = 0xFFBE, /**< The function 1 key */
	INPUTMETHOD_KEY_F2 = 0xFFBF, /**< The function 2 key */
	INPUTMETHOD_KEY_F3 = 0xFFC0, /**< The function 3 key */
	INPUTMETHOD_KEY_F4 = 0xFFC1, /**< The function 4 key */
	INPUTMETHOD_KEY_F5 = 0xFFC2, /**< The function 5 key */
	INPUTMETHOD_KEY_F6 = 0xFFC3, /**< The function 6 key */
	INPUTMETHOD_KEY_F7 = 0xFFC4, /**< The function 7 key */
	INPUTMETHOD_KEY_F8 = 0xFFC5, /**< The function 8 key */
	INPUTMETHOD_KEY_F9 = 0xFFC6, /**< The function 9 key */
	INPUTMETHOD_KEY_F10 = 0xFFC7, /**< The function 10 key */
	INPUTMETHOD_KEY_F11 = 0xFFC8, /**< The function 11 key */
	INPUTMETHOD_KEY_F12 = 0xFFC9, /**< The function 12 key */
	INPUTMETHOD_KEY_F13 = 0xFFCA, /**< The function 13 key */
	INPUTMETHOD_KEY_F14 = 0xFFCB, /**< The function 14 key */
	INPUTMETHOD_KEY_F15 = 0xFFCC, /**< The function 15 key */
	INPUTMETHOD_KEY_F16 = 0xFFCD, /**< The function 16 key */
	INPUTMETHOD_KEY_F17 = 0xFFCE, /**< The function 17 key */
	INPUTMETHOD_KEY_F18 = 0xFFCF, /**< The function 18 key */
	INPUTMETHOD_KEY_F19 = 0xFFD0, /**< The function 19 key */
	INPUTMETHOD_KEY_F20 = 0xFFD1, /**< The function 20 key */
	INPUTMETHOD_KEY_F21 = 0xFFD2, /**< The function 21 key */
	INPUTMETHOD_KEY_F22 = 0xFFD3, /**< The function 22 key */
	INPUTMETHOD_KEY_F23 = 0xFFD4, /**< The function 23 key */
	INPUTMETHOD_KEY_F24 = 0xFFD5, /**< The function 24 key */
	INPUTMETHOD_KEY_F25 = 0xFFD6, /**< The function 25 key */
	INPUTMETHOD_KEY_F26 = 0xFFD7, /**< The function 26 key */
	INPUTMETHOD_KEY_F27 = 0xFFD8, /**< The function 27 key */
	INPUTMETHOD_KEY_F28 = 0xFFD9, /**< The function 28 key */
	INPUTMETHOD_KEY_F29 = 0xFFDA, /**< The function 29 key */
	INPUTMETHOD_KEY_F30 = 0xFFDB, /**< The function 30 key */
	INPUTMETHOD_KEY_F31 = 0xFFDC, /**< The function 31 key */
	INPUTMETHOD_KEY_F32 = 0xFFDD, /**< The function 32 key */
	INPUTMETHOD_KEY_F33 = 0xFFDE, /**< The function 33 key */
	INPUTMETHOD_KEY_F34 = 0xFFDF, /**< The function 34 key */
	INPUTMETHOD_KEY_F35 = 0xFFE0, /**< The function 35 key */

	/* Modifier keys */
	INPUTMETHOD_KEY_Shift_L = 0xFFE1, /**< The left shift key */
	INPUTMETHOD_KEY_Shift_R = 0xFFE2, /**< The right shift key */
	INPUTMETHOD_KEY_Control_L = 0xFFE3, /**< The left control key */
	INPUTMETHOD_KEY_Control_R = 0xFFE4, /**< The right control key */
	INPUTMETHOD_KEY_Caps_Lock = 0xFFE5, /**< The caps lock key */
	INPUTMETHOD_KEY_Shift_Lock = 0xFFE6, /**< The shift lock key */

	INPUTMETHOD_KEY_Meta_L = 0xFFE7, /**< The left meta key */
	INPUTMETHOD_KEY_Meta_R = 0xFFE8, /**< The right meta key */
	INPUTMETHOD_KEY_Alt_L = 0xFFE9, /**< The left alt key */
	INPUTMETHOD_KEY_Alt_R = 0xFFEA, /**< The right alt key */
	INPUTMETHOD_KEY_Super_L = 0xFFEB, /**< The left super key */
	INPUTMETHOD_KEY_Super_R = 0xFFEC, /**< The right super key */
	INPUTMETHOD_KEY_Hyper_L = 0xFFED, /**< The left hyper key */
	INPUTMETHOD_KEY_Hyper_R = 0xFFEE, /**< The right hyper key */

	/* Latin 1 */
	INPUTMETHOD_KEY_space = 0x020, /**< The space key */
	INPUTMETHOD_KEY_exclam = 0x021, /**< The exclamation key */
	INPUTMETHOD_KEY_quotedbl = 0x022, /**< The quotedbl key */
	INPUTMETHOD_KEY_numbersign = 0x023, /**< The number sign key */
	INPUTMETHOD_KEY_dollar = 0x024, /**< The dollar key */
	INPUTMETHOD_KEY_percent = 0x025, /**< The percent key */
	INPUTMETHOD_KEY_ampersand = 0x026, /**< The ampersand key */
	INPUTMETHOD_KEY_apostrophe = 0x027, /**< The apostrophe key */
	INPUTMETHOD_KEY_parenleft = 0x028, /**< The parenleft key */
	INPUTMETHOD_KEY_parenright = 0x029, /**< The parenright key */
	INPUTMETHOD_KEY_asterisk = 0x02a, /**< The asterisk key */
	INPUTMETHOD_KEY_plus = 0x02b, /**< The plus key */
	INPUTMETHOD_KEY_comma = 0x02c, /**< The comma key */
	INPUTMETHOD_KEY_minus = 0x02d, /**< The minus key */
	INPUTMETHOD_KEY_period = 0x02e, /**< The period key */
	INPUTMETHOD_KEY_slash = 0x02f, /**< The slash key */
	INPUTMETHOD_KEY_0 = 0x030, /**< The 0 key */
	INPUTMETHOD_KEY_1 = 0x031, /**< The 1 key */
	INPUTMETHOD_KEY_2 = 0x032, /**< The 2 key */
	INPUTMETHOD_KEY_3 = 0x033, /**< The 3 key */
	INPUTMETHOD_KEY_4 = 0x034, /**< The 4 key */
	INPUTMETHOD_KEY_5 = 0x035, /**< The 5 key */
	INPUTMETHOD_KEY_6 = 0x036, /**< The 6 key */
	INPUTMETHOD_KEY_7 = 0x037, /**< The 7 key */
	INPUTMETHOD_KEY_8 = 0x038, /**< The 8 key */
	INPUTMETHOD_KEY_9 = 0x039, /**< The 9 key */
	INPUTMETHOD_KEY_colon = 0x03a, /**< The colon key */
	INPUTMETHOD_KEY_semicolon = 0x03b, /**< The semicolon key */
	INPUTMETHOD_KEY_less = 0x03c, /**< The less key */
	INPUTMETHOD_KEY_equal = 0x03d, /**< The equal key */
	INPUTMETHOD_KEY_greater = 0x03e, /**< The greater key */
	INPUTMETHOD_KEY_question = 0x03f, /**< The question key */
	INPUTMETHOD_KEY_at = 0x040, /**< The at key */
	INPUTMETHOD_KEY_A = 0x041, /**< The A key */
	INPUTMETHOD_KEY_B = 0x042, /**< The B key */
	INPUTMETHOD_KEY_C = 0x043, /**< The C key */
	INPUTMETHOD_KEY_D = 0x044, /**< The D key */
	INPUTMETHOD_KEY_E = 0x045, /**< The E key */
	INPUTMETHOD_KEY_F = 0x046, /**< The F key */
	INPUTMETHOD_KEY_G = 0x047, /**< The G key */
	INPUTMETHOD_KEY_H = 0x048, /**< The H key */
	INPUTMETHOD_KEY_I = 0x049, /**< The I key */
	INPUTMETHOD_KEY_J = 0x04a, /**< The J key */
	INPUTMETHOD_KEY_K = 0x04b, /**< The K key */
	INPUTMETHOD_KEY_L = 0x04c, /**< The L key */
	INPUTMETHOD_KEY_M = 0x04d, /**< The M key */
	INPUTMETHOD_KEY_N = 0x04e, /**< The N key */
	INPUTMETHOD_KEY_O = 0x04f, /**< The O key */
	INPUTMETHOD_KEY_P = 0x050, /**< The P key */
	INPUTMETHOD_KEY_Q = 0x051, /**< The Q key */
	INPUTMETHOD_KEY_R = 0x052, /**< The R key */
	INPUTMETHOD_KEY_S = 0x053, /**< The S key */
	INPUTMETHOD_KEY_T = 0x054, /**< The T key */
	INPUTMETHOD_KEY_U = 0x055, /**< The U key */
	INPUTMETHOD_KEY_V = 0x056, /**< The V key */
	INPUTMETHOD_KEY_W = 0x057, /**< The W key */
	INPUTMETHOD_KEY_X = 0x058, /**< The X key */
	INPUTMETHOD_KEY_Y = 0x059, /**< The Y key */
	INPUTMETHOD_KEY_Z = 0x05a, /**< The Z key */
	INPUTMETHOD_KEY_bracketleft = 0x05b, /**< The left bracket key */
	INPUTMETHOD_KEY_backslash = 0x05c, /**< The backslash key */
	INPUTMETHOD_KEY_bracketright = 0x05d, /**< The right bracket key */
	INPUTMETHOD_KEY_asciicircum = 0x05e, /**< The circumflex key */
	INPUTMETHOD_KEY_underscore = 0x05f, /**< The underscore key */
	INPUTMETHOD_KEY_grave = 0x060, /**< The grave key */
	INPUTMETHOD_KEY_a = 0x061, /**< The a key */
	INPUTMETHOD_KEY_b = 0x062, /**< The b key */
	INPUTMETHOD_KEY_c = 0x063, /**< The c key */
	INPUTMETHOD_KEY_d = 0x064, /**< The d key */
	INPUTMETHOD_KEY_e = 0x065, /**< The e key */
	INPUTMETHOD_KEY_f = 0x066, /**< The f key */
	INPUTMETHOD_KEY_g = 0x067, /**< The g key */
	INPUTMETHOD_KEY_h = 0x068, /**< The h key */
	INPUTMETHOD_KEY_i = 0x069, /**< The i key */
	INPUTMETHOD_KEY_j = 0x06a, /**< The j key */
	INPUTMETHOD_KEY_k = 0x06b, /**< The k key */
	INPUTMETHOD_KEY_l = 0x06c, /**< The l key */
	INPUTMETHOD_KEY_m = 0x06d, /**< The m key */
	INPUTMETHOD_KEY_n = 0x06e, /**< The n key */
	INPUTMETHOD_KEY_o = 0x06f, /**< The o key */
	INPUTMETHOD_KEY_p = 0x070, /**< The p key */
	INPUTMETHOD_KEY_q = 0x071, /**< The q key */
	INPUTMETHOD_KEY_r = 0x072, /**< The r key */
	INPUTMETHOD_KEY_s = 0x073, /**< The s key */
	INPUTMETHOD_KEY_t = 0x074, /**< The t key */
	INPUTMETHOD_KEY_u = 0x075, /**< The u key */
	INPUTMETHOD_KEY_v = 0x076, /**< The v key */
	INPUTMETHOD_KEY_w = 0x077, /**< The w key */
	INPUTMETHOD_KEY_x = 0x078, /**< The x key */
	INPUTMETHOD_KEY_y = 0x079, /**< The y key */
	INPUTMETHOD_KEY_z = 0x07a, /**< The z key */
	INPUTMETHOD_KEY_braceleft = 0x07b, /**< The left brace key */
	INPUTMETHOD_KEY_bar = 0x07c, /**< The bar key */
	INPUTMETHOD_KEY_braceright = 0x07d, /**< The right brace key */
	INPUTMETHOD_KEY_asciitilde = 0x07e, /**< The tilde key */
} inputmethod_key_symbol_e;

/**
 * @brief Enumeration for the key masks.
 *        The key masks indicate which modifier keys is pressed down during the keyboard hit.
 *        The special #INPUTMETHOD_KEY_MASK_RELEASED indicates the key release event.
 * @since_tizen 11.0
 */
typedef enum {
	INPUTMETHOD_KEY_MASK_PRESSED = 0, /**< Key press event without modifier key */
	INPUTMETHOD_KEY_MASK_SHIFT = (1<<0), /**< The Shift key is pressed down */
	INPUTMETHOD_KEY_MASK_CAPSLOCK = (1<<1), /**< The CapsLock key is pressed down */
	INPUTMETHOD_KEY_MASK_CONTROL = (1<<2), /**< The Control key is pressed down */
	INPUTMETHOD_KEY_MASK_ALT = (1<<3), /**< The Alt key is pressed down */
	INPUTMETHOD_KEY_MASK_META = (1<<4), /**< The Meta key is pressed down */
	INPUTMETHOD_KEY_MASK_WIN = (1<<5), /**< The Win (between Control and Alt) is pressed down */
	INPUTMETHOD_KEY_MASK_HYPER = (1<<6), /**< The Hyper key is pressed down */
	INPUTMETHOD_KEY_MASK_NUMLOCK = (1<<7), /**< The NumLock key is pressed down */
	INPUTMETHOD_KEY_MASK_RELEASED = (1<<15) /**< Key release event */
} inputmethod_key_mask_e;

/**
 * @}
 */


#endif /* __TIZEN_UIX_INPUTMETHOD_CORE_KEYDEF_H__ */