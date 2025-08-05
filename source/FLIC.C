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
  ±   [FLIC.C      ] - Flic handling routines                                ±
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

#ifndef NOREG
static   char  *FlicImage;
static   char  *FlicImageComplete;
static   int   FlicImageOK = 0;
#endif

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  Flics = {
   {"Flics"},
   {
      {{ 10, 20,  0,  0}, DIALOG_HEADING,    0,              0, "Output animation format:"},
      {{ 15, 32,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicFLIButton},
      {{ 70, 32,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicFLCButton},
      {{125, 32,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicFLXButton},
      {{180, 32,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicFLHButton},
      {{235, 32,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicFLTButton},

      {{  5, 58, -5,  0}, DIALOG_HLINE,      0,              0, ""},
      {{ 10, 63,  0,  0}, DIALOG_HEADING,    0,              0, "Flic Settings:"},
      {{ 15, 75,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicPingButton},
      {{125, 75,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicOnOffButton},
      {{180, 75,  0,  0}, DIALOG_BUTTON,     0,              0, &FlicColorButton},

      {{ 15,102,  0,  0}, DIALOG_HEADING,    0,              0, "Animation Name:"},
      {{133, 97,  0,  0}, DIALOG_TEXTBOX,    0,   FlicFileName, &FlicFilePrompt},
      {{ 15,124,  0,  0}, DIALOG_HEADING,    0,              0, "Flic Speed:"},
      {{101,119,  0,  0}, DIALOG_NUMBOX,     &FlicSpeed,     0, &FlicSpeedPrompt},

      {{  0,  0,  0,  0}, DIALOG_END,        0,              0, ""}
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawFlicButton( )
{
#ifndef NOREG
   DrawButton( ProcButton.Rect.Left + 100,
               ProcButton.Rect.Top,
               &FlicButton );
#endif
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowFlicWindow( )
{
#ifndef NOREG
int   i;
_RECT TRect;

   UpdateButtonStates( );

   FlicRect.Left = GI->CenX - FLIC_WIDTH/2;
   FlicRect.Right = FlicRect.Left + FLIC_WIDTH;
   FlicRect.Top  = GI->CenY - FLIC_HEIGHT/2;
   FlicRect.Bottom = FlicRect.Top + FLIC_HEIGHT;

   FlicImage = malloc(_imagesize( FlicRect.Left, FlicRect.Top, FlicRect.Right, FlicRect.Bottom ));

   if (!FlicImage)
      return( E_NOMEMMENU );

   _getimage(FlicRect.Left, FlicRect.Top, FlicRect.Right, FlicRect.Bottom, FlicImage);
   
   if (UpdateFastFlic && FastScreen && FlicImageOK)
   {
      free( FlicImageComplete );
      FlicImageOK = 0;
      UpdateFastFlic = 0;
   }

   if (FastScreen && !FlicImageOK)
   {
      FlicImageComplete = malloc(_imagesize( FlicRect.Left, FlicRect.Top, FlicRect.Right, FlicRect.Bottom ));

      if (!FlicImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && FlicImageOK)
   {
      _putimage( FlicRect.Left, FlicRect.Top, FlicImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &FlicRect, FLIC_WINDOW_COLOR );

   TRect = FlicRect;
   TRect.Bottom = FlicRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, FLIC_TITLE_COLOR );

   DrawOutRect( FlicRect.Left, FlicRect.Top, FlicRect.Right, FlicRect.Bottom );

   _setcolor(FLIC_TITLE_TEXT_COLOR);
   _moveto( FlicRect.Left + (FlicRect.Right - FlicRect.Left) / 2 - 
            strlen(Flics.Title) * TEXT_WIDTH / 2, FlicRect.Top + 5 );
   _outgtext( Flics.Title );

   for (i = 0; Flics.Elements[i].Type != DIALOG_END; i++)
      switch( Flics.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(FLIC_DB_HEAD_COLOR);
            _moveto( FlicRect.Left + Flics.Elements[i].Rect.Left,
                     FlicRect.Top + Flics.Elements[i].Rect.Top );
            _outgtext( Flics.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(FLIC_DB_SHEAD_COLOR);
            _moveto( FlicRect.Left + Flics.Elements[i].Rect.Left,
                     FlicRect.Top + Flics.Elements[i].Rect.Top );
            _outgtext( Flics.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(FLIC_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( FlicRect.Left + Flics.Elements[i].Rect.Left,
                     FlicRect.Top + Flics.Elements[i].Rect.Top );
            _lineto( FlicRect.Right + Flics.Elements[i].Rect.Right,
                     FlicRect.Top + Flics.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( FlicRect.Left + Flics.Elements[i].Rect.Left,
                        FlicRect.Top + Flics.Elements[i].Rect.Top,
                        (BUTTON) Flics.Elements[i].Object );
            break;

         case DIALOG_TEXTBOX:
            DrawPromptTBox( FlicRect.Left + Flics.Elements[i].Rect.Left, 
                            FlicRect.Top + Flics.Elements[i].Rect.Top,
                            Flics.Elements[i].PromptString,
                            (PROMPT) Flics.Elements[i].Object );
            break;

         case DIALOG_NUMBOX:
            DrawPromptBox( FlicRect.Left + Flics.Elements[i].Rect.Left, 
                           FlicRect.Top + Flics.Elements[i].Rect.Top,
                           *Flics.Elements[i].PromptVal,
                           (PROMPT) Flics.Elements[i].Object );
            break;
      }

   if (FastScreen && !FlicImageOK)
   {
      _getimage(FlicRect.Left, FlicRect.Top, FlicRect.Right, FlicRect.Bottom, FlicImageComplete);
      FlicImageOK = 1;
   }
#endif

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HideFlicWindow( )
{
#ifndef NOREG
   if (FlicImage)
   {
      HideCursor();

      _putimage( FlicRect.Left, FlicRect.Top, FlicImage, _GPSET );

      ShowCursor();

      free( FlicImage );

      FlicImage = 0;
   }
#endif
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [FLIC.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/


