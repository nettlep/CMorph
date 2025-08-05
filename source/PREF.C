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
  ±   Copyright (c) 1993, 1994 Paul D. Nettle.  All Rights Reserved.         ±
  ±                                                                          ±
  ±   [PREF.C      ] - Preferences routines                                  ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   char  *PrefImage;
static   char  *PrefImageComplete;
static   int   PrefImageOK = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  Preferences = {
   {"Preferences Menu"},
   {
      {{ 10, 20,  0,  0}, DIALOG_HEADING,    0,              0, "Spline types:"},

      {{ 15, 32,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Mesh:"},
      {{ 20, 44,  0,  0}, DIALOG_BUTTON,     0,              0, &MeshCATButton},
      {{ 20, 64,  0,  0}, DIALOG_BUTTON,     0,              0, &MeshSPLButton},
      {{ 20, 84,  0,  0}, DIALOG_BUTTON,     0,              0, &MeshSTRButton},

      {{124, 70,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Tightness %:"},
      {{220, 64,  0,  0}, DIALOG_NUMBOX,     &WarpTightness, 0, &WarpTightnessPrompt},

      {{254, 32,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Interpolation:"},
      {{264, 44,  0,  0}, DIALOG_BUTTON,     0,              0, &InterpMeshCATButton},
      {{264, 64,  0,  0}, DIALOG_BUTTON,     0,              0, &InterpMeshSPLButton},
      {{264, 84,  0,  0}, DIALOG_BUTTON,     0,              0, &InterpMeshSTRButton},

      {{364, 70,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Tightness %:"},
      {{460, 64,  0,  0}, DIALOG_NUMBOX,     &InterpTightness, 0, &InterpTightnessPrompt},

      {{  5,105, -5,  0}, DIALOG_HLINE,      0,              0, ""},
      {{ 10,110,  0,  0}, DIALOG_HEADING,    0,              0, "Screen settings:"},

      {{ 15,122,  0,  0}, DIALOG_SUBHEADING, 0,              0, "General Settings:"},
      {{ 20,134,  0,  0}, DIALOG_BUTTON,     0,              0, &CurvesButton},
      {{ 20,154,  0,  0}, DIALOG_BUTTON,     0,              0, &ZoomButton},
      {{ 20,174,  0,  0}, DIALOG_BUTTON,     0,              0, &KwikScreenButton},
      {{ 20,194,  0,  0}, DIALOG_BUTTON,     0,              0, &SoundEnableButton},

      {{175,122,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Cursor Color:"},
      {{180,134,  0,  0}, DIALOG_BUTTON,     0,              0, &CursRButton},
      {{180,154,  0,  0}, DIALOG_BUTTON,     0,              0, &CursGButton},
      {{180,174,  0,  0}, DIALOG_BUTTON,     0,              0, &CursBButton},
      {{180,194,  0,  0}, DIALOG_BUTTON,     0,              0, &CursXButton},

      {{295,122,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Mesh Color:"},
      {{300,134,  0,  0}, DIALOG_BUTTON,     0,              0, &MColorRButton},
      {{300,154,  0,  0}, DIALOG_BUTTON,     0,              0, &MColorGButton},
      {{300,174,  0,  0}, DIALOG_BUTTON,     0,              0, &MColorBButton},
      {{300,194,  0,  0}, DIALOG_BUTTON,     0,              0, &MColorXButton},

      {{  5,215, -5,  0}, DIALOG_HLINE,      0,              0, ""},
      {{ 10,220,  0,  0}, DIALOG_HEADING,    0,              0, "Output image format:"},
      {{ 15,232,  0,  0}, DIALOG_BUTTON,     0,              0, &OutputTGAButton},
      {{ 70,232,  0,  0}, DIALOG_BUTTON,     0,              0, &OutputIPIButton},
      {{125,232,  0,  0}, DIALOG_BUTTON,     0,              0, &OutputIMGButton},
      {{180,232,  0,  0}, DIALOG_BUTTON,     0,              0, &OutputGIFButton},
      {{235,232,  0,  0}, DIALOG_BUTTON,     0,              0, &OutputBMPButton},
      {{290,232,  0,  0}, DIALOG_BUTTON,     0,              0, &OutputPCXButton},

      {{355,232,  0,  0}, DIALOG_BUTTON,     0,              0, &TGAUpsideDownButton},

      {{  5,253, -5,  0}, DIALOG_HLINE,      0,              0, ""},
      {{ 10,258,  0,  0}, DIALOG_HEADING,    0,              0, "Operation:"},
      {{ 15,270,  0,  0}, DIALOG_BUTTON,     0,              0, &MorphButton},
      {{100,270,  0,  0}, DIALOG_BUTTON,     0,              0, &WarpButton},
      {{180,270,  0,  0}, DIALOG_BUTTON,     0,              0, &FadeButton},
      {{355,270,  0,  0}, DIALOG_BUTTON,     0,              0, &FastMorphButton},

      {{  5,291, -5,  0}, DIALOG_HLINE,      0,              0, ""},
      {{ 10,296,  0,  0}, DIALOG_HEADING,    0,              0, "Output configuration:"},
      {{ 15,313,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Fade 50%:"},
      {{ 87,308,  0,  0}, DIALOG_NUMBOX,     &Fade50,        0, &Fade50Prompt},
      {{135,313,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Start %:"},
      {{199,308,  0,  0}, DIALOG_NUMBOX,     &FStart,        0, &FadeStartPrompt},
      {{247,313,  0,  0}, DIALOG_SUBHEADING, 0,              0, "End %:"},
      {{295,308,  0,  0}, DIALOG_NUMBOX,     &FEnd,          0, &FadeEndPrompt},
      {{ 15,334,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Warp 50%:"},
      {{ 87,329,  0,  0}, DIALOG_NUMBOX,     &Warp50,        0, &Warp50Prompt},
      {{135,334,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Start %:"},
      {{199,329,  0,  0}, DIALOG_NUMBOX,     &WStart,        0, &WarpStartPrompt},
      {{247,334,  0,  0}, DIALOG_SUBHEADING, 0,              0, "End %:"},
      {{295,329,  0,  0}, DIALOG_NUMBOX,     &WEnd,          0, &WarpEndPrompt},

      {{355,308,  0,  0}, DIALOG_BUTTON,     0,              0, &ShowBetweensButton},

      {{  5,350, -5,  0}, DIALOG_HLINE,      0,              0, ""},
      {{ 10,355,  0,  0}, DIALOG_HEADING,    0,              0, "Frame control:"},
      {{ 15,372,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Total:"},
      {{ 63,367,  0,  0}, DIALOG_NUMBOX,     &TotalFrames,   0, &FramesPrompt},
      {{120,372,  0,  0}, DIALOG_SUBHEADING, 0,              0, "First:"},
      {{168,367,  0,  0}, DIALOG_NUMBOX,     &StartFrame,    0, &StartFramePrompt},
      {{230,372,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Final:"},
      {{278,367,  0,  0}, DIALOG_NUMBOX,     &EndFrame,      0, &EndFramePrompt},
      {{340,372,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Offset:"},
      {{396,367,  0,  0}, DIALOG_NUMBOX,     &FrameOffset,   0, &FrameOffPrompt},

      {{  0,  0,  0,  0}, DIALOG_END,        0,              0, ""}
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawPerfButton( )
{
   DrawButton( SourceInfoWindow.Left + 5, 
               SourceInfoWindow.Bottom + 5,
               &PerfButton );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowPreferencesWindow( )
{
int   i;
_RECT TRect;

   UpdateButtonStates( );

   PrefRect.Left = GI->CenX - PREF_WIDTH/2;
   PrefRect.Right = PrefRect.Left + PREF_WIDTH;
   PrefRect.Top  = GI->CenY - PREF_HEIGHT/2;
   PrefRect.Bottom = PrefRect.Top + PREF_HEIGHT;

   PrefImage = malloc(_imagesize( PrefRect.Left, PrefRect.Top, PrefRect.Right, PrefRect.Bottom ));

   if (!PrefImage)
      return( E_NOMEMMENU );

   _getimage(PrefRect.Left, PrefRect.Top, PrefRect.Right, PrefRect.Bottom, PrefImage);
   
   if (UpdateFastPref && FastScreen && PrefImageOK)
   {
      free( PrefImageComplete );
      PrefImageOK = 0;
      UpdateFastPref = 0;
   }

   if (FastScreen && !PrefImageOK)
   {
      PrefImageComplete = malloc(_imagesize( PrefRect.Left, PrefRect.Top, PrefRect.Right, PrefRect.Bottom ));

      if (!PrefImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && PrefImageOK)
   {
      _putimage( PrefRect.Left, PrefRect.Top, PrefImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &PrefRect, PREF_WINDOW_COLOR );

   TRect = PrefRect;
   TRect.Bottom = PrefRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, PREF_TITLE_COLOR );

   DrawOutRect( PrefRect.Left, PrefRect.Top, PrefRect.Right, PrefRect.Bottom );

   _setcolor(PREF_TITLE_TEXT_COLOR);
   _moveto( PrefRect.Left + (PrefRect.Right - PrefRect.Left) / 2 - 
            strlen(Preferences.Title) * TEXT_WIDTH / 2, PrefRect.Top + 5 );
   _outgtext( Preferences.Title );

   for (i = 0; Preferences.Elements[i].Type != DIALOG_END; i++)
      switch( Preferences.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(PREF_DB_HEAD_COLOR);
            _moveto( PrefRect.Left + Preferences.Elements[i].Rect.Left,
                     PrefRect.Top + Preferences.Elements[i].Rect.Top );
            _outgtext( Preferences.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(PREF_DB_SHEAD_COLOR);
            _moveto( PrefRect.Left + Preferences.Elements[i].Rect.Left,
                     PrefRect.Top + Preferences.Elements[i].Rect.Top );
            _outgtext( Preferences.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(PREF_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( PrefRect.Left + Preferences.Elements[i].Rect.Left,
                     PrefRect.Top + Preferences.Elements[i].Rect.Top );
            _lineto( PrefRect.Right + Preferences.Elements[i].Rect.Right,
                     PrefRect.Top + Preferences.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( PrefRect.Left + Preferences.Elements[i].Rect.Left,
                        PrefRect.Top + Preferences.Elements[i].Rect.Top,
                        (BUTTON) Preferences.Elements[i].Object );
            break;

         case DIALOG_NUMBOX:
            DrawPromptBox( PrefRect.Left + Preferences.Elements[i].Rect.Left, 
                           PrefRect.Top + Preferences.Elements[i].Rect.Top,
                           *Preferences.Elements[i].PromptVal,
                           (PROMPT) Preferences.Elements[i].Object );
            break;
      }

   if (FastScreen && !PrefImageOK)
   {
      _getimage(PrefRect.Left, PrefRect.Top, PrefRect.Right, PrefRect.Bottom, PrefImageComplete);
      PrefImageOK = 1;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HidePreferencesWindow( )
{
   if (PrefImage)
   {
      HideCursor();

      _putimage( PrefRect.Left, PrefRect.Top, PrefImage, _GPSET );

      ShowCursor();

      free( PrefImage );

      PrefImage = 0;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [PREF.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
