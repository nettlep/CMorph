// -----------------------------------------------------------------------------
//   _____ __  __                  _
//  / ____|  \/  |                | |
// | |    | \  / | ___  _ __ _ __ | |__
// | |    | |\/| |/ _ \| '__| '_ \| '_ \
// | |____| |  | | (_) | |  | |_) | | | |
//  \_____|_|  |_|\___/|_|  | .__/|_| |_|
//                          | |                    
//                          |_|                    
//
// The Complete Morph - public source release
// -----------------------------------------------------------------------------
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// -----------------------------------------------------------------------------
//
// Notes about this software:
//
// CMorph generates high quality (even by today's standards) two-dimensional
// morphs from two static input images. It was written for DOS a number of years
// ago, using the Watcom compiler, back before C++ was the rage.
//
// Some of this source is ugly, but it should all be fairly readable.
//
// You will not be able to simply compile this program into an EXE because not
// all components of the program exist (there are some other libraries that are
// required, but are unavailable for public release at this time.) These missing
// libraries are the Int86 lib (DOS software interrupts), FLIC lib (animations)
// and the Image lib (loading/saving of image files.)
//
// This source code is best viewed with 4-character tabs.
//
// -----------------------------------------------------------------------------
/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±                                                                          ±
  ±   Copyright (c) 1994 Paul D. Nettle.  All Rights Reserved.               ±
  ±                                                                          ±
  ±   [CMDEFS.H    ] - CMorph defines                                        ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  VERSION_MS  2
#define  VERSION_LS  10

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

/* MAKE SURE TO UPDATE ERROR.C's "ErrorBox" ROUTINE WHEN MODIFYING THIS LIST! */

#define E_NONE 0

enum{
   E_NOOPEN = 100,
   E_NOWRITE,
   E_NOREAD,
   E_BADMESHFILE,
   E_INVMESHFILE,
   E_INVALIDMESH,
   E_BADRESOLUTION,
   E_BADX,
   E_CORRUPTMESH,
   E_BADARITH,
   E_BADBACK,
   E_NOFILTERFILE,
   E_NOFILTERNAME,
   E_INVFILTOPER,
   E_NOMEMMENU,
   E_NOMEMMSH,
   E_NOMEMGRF,
   E_NOGRF,
   E_NOMEMZM,
   E_NOMOUSE,
   E_UNKNOWN
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  GREY_START           (0)
#define  GREY_END             (63)
#define  RED_START            (64)
#define  RED_END              (127)
#define  GREEN_START          (128)
#define  GREEN_END            (191)
#define  BLUE_START           (192)
#define  BLUE_END             (255)

#define  DARK_GREY            (GREY_START)
#define  LIGHT_GREY           (GREY_END)
#define  DARK_RED             (RED_START)
#define  LIGHT_RED            (RED_END)
#define  DARK_GREEN           (GREEN_START)
#define  LIGHT_GREEN          (GREEN_END)
#define  DARK_BLUE            (BLUE_START)
#define  LIGHT_BLUE           (BLUE_END)

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   DIALOG_BUTTON,
   DIALOG_HEADING,
   DIALOG_SUBHEADING,
   DIALOG_TEXTBOX,
   DIALOG_NUMBOX,
   DIALOG_HLINE,
   DIALOG_VLINE,
   DIALOG_END
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   RADIO_BUTTON,
   STAND_BUTTON
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   BUTTON_UP,
   BUTTON_DOWN
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   ARITH_OFF,
   ARITH_AVE,
   ARITH_ADD,
   ARITH_SUB,
   ARITH_OVR
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   BACK_OFF,
   BACK_ON
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   NO_AUTOREW,
   AUTOREW
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   FLIC_OFF,
   FLIC_ON
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   COLOR,
   MONOCHROME
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  PREF_WIDTH           510
#define  PREF_HEIGHT          390

#define  PREV_WIDTH           200
#define  PREV_HEIGHT          110

#define  PROC_WIDTH           630
#define  PROC_HEIGHT          150

#define  OPER_WIDTH           140
#define  OPER_HEIGHT          250

#define  EDIT_WIDTH           280
#define  EDIT_HEIGHT          230

#define  FILE_WIDTH           630
#define  FILE_HEIGHT          290

#define  FLIC_WIDTH           300
#define  FLIC_HEIGHT          145

#define  ZOOM_BLOCK_SIZE      2

#define  MESH_SIZE            100

#define  TEXT_HEIGHT          8
#define  TEXT_WIDTH           8

#define  REMAINING_RFU        978

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  BLINK_10MSECS        30

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  ALT_A                0x1E
#define  ALT_B                0x30
#define  ALT_C                0x2E
#define  ALT_D                0x20
#define  ALT_E                0x12
#define  ALT_F                0x21
#define  ALT_G                0x22
#define  ALT_H                0x23
#define  ALT_I                0x17
#define  ALT_L                0x26
#define  ALT_M                0x32
#define  ALT_O                0x18
#define  ALT_P                0x19
#define  ALT_R                0x13
#define  ALT_S                0x1F
#define  ALT_T                0x14
#define  ALT_U                0x16
#define  ALT_V                0x2F
#define  ALT_W                0x11
#define  ALT_X                0x2D
#define  ALT_Z                0x2C
#define  F1                   0x3B
#define  CTRL_F9              102

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   MAINMENU,
   PERFMENU,
   OPERMENU,
   EDITMENU,
   FILEMENU,
   PROCMENU,
   PREVMENU,
   FLICMENU
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   VGA,
   SVGA
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  FILE_HEADER          "This is a mesh file from The Complete Morph"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  MOUSE_INT            0x33
#define  VIDEO_INT            0x10

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  DEFAULT_FLIC_FILE    "MORPH"
#define  DEFAULT_FILTER_FILE  "FILTERS.TXT"
#define  DEFAULT_MESH         "NONAME.MSH"
#define  DEFAULT_FRAMES       10

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  MOUSE_INIT           0x0000
#define  MOUSE_READ_INF       0x0003
#define  MOUSE_SET_POS        0x0004
#define  MOUSE_SET_COL        0x0007
#define  MOUSE_SET_ROW        0x0008

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   LEFT,
   TOP,
   RIGHT,
   BOTTOM
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   CROSS,
   BULLSEYE
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   CATMULL,
   SPLINE,
   STRAIGHT
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   MORPH,
   WARP,
   FADE
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   NO_SINGLE,
   SINGLE
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

enum{
   SP_WARP,
   SP_INTRP
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [CMDEFS.H    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
