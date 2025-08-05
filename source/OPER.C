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
  ±   [OPER.C      ] - Operations routines                                   ±
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

static   char  *OperImage;
static   char  *OperImageComplete;
static   int   OperImageOK = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  Operations = {
   {"Operations Menu"},
   {
      {{ 10, 20,  0,  0}, DIALOG_HEADING, 0, 0, "System:"},

      {{ 20, 34,  0,  0}, DIALOG_BUTTON,  0, 0, &ExitButton},
      {{ 20, 54,  0,  0}, DIALOG_BUTTON,  0, 0, &ShellButton},

      {{  5, 75, -5,  0}, DIALOG_HLINE,   0, 0, ""},
      {{ 10, 80,  0,  0}, DIALOG_HEADING, 0, 0, "Operations:"},

      {{ 20, 94,  0,  0}, DIALOG_BUTTON,  0, 0, &GoButton},

      {{  5,115, -5,  0}, DIALOG_HLINE,   0, 0, ""},
      {{ 10,120,  0,  0}, DIALOG_HEADING, 0, 0, "Other:"},

      {{ 20,134,  0,  0}, DIALOG_BUTTON,  0, 0, &HelpButton},

      {{  5,155, -5,  0}, DIALOG_HLINE,   0, 0, ""},
      {{ 20,195,  0,  0}, DIALOG_BUTTON,  0, 0, &AboutButton},

      {{  0,  0,  0,  0}, DIALOG_END,     0, 0, ""}
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawOperButton( )
{
   DrawButton( PerfButton.Rect.Left,
               PerfButton.Rect.Bottom + 5,
               &OperButton );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowOperationsWindow( )
{
int   i;
_RECT TRect;

   OperRect.Left = GI->CenX - OPER_WIDTH/2;
   OperRect.Right = OperRect.Left + OPER_WIDTH;
   OperRect.Top  = GI->CenY - OPER_HEIGHT/2;
   OperRect.Bottom = OperRect.Top + OPER_HEIGHT;

   OperImage = malloc(_imagesize( OperRect.Left, OperRect.Top, OperRect.Right, OperRect.Bottom ));

   if (!OperImage)
      return( E_NOMEMMENU );

   _getimage(OperRect.Left, OperRect.Top, OperRect.Right, OperRect.Bottom, OperImage);
   
   if (FastScreen && !OperImageOK)
   {
      OperImageComplete = malloc(_imagesize( OperRect.Left, OperRect.Top, OperRect.Right, OperRect.Bottom ));

      if (!OperImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && OperImageOK)
   {
      _putimage( OperRect.Left, OperRect.Top, OperImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &OperRect, OPER_WINDOW_COLOR );

   TRect = OperRect;
   TRect.Bottom = OperRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, OPER_TITLE_COLOR );

   DrawOutRect( OperRect.Left, OperRect.Top, OperRect.Right, OperRect.Bottom );

   _setcolor(OPER_TITLE_TEXT_COLOR);
   _moveto( OperRect.Left + (OperRect.Right - OperRect.Left) / 2 - 
            strlen(Operations.Title) * TEXT_WIDTH / 2, OperRect.Top + 5 );
   _outgtext( Operations.Title );

   for (i = 0; Operations.Elements[i].Type != DIALOG_END; i++)
      switch( Operations.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(OPER_DB_HEAD_COLOR);
            _moveto( OperRect.Left + Operations.Elements[i].Rect.Left,
                     OperRect.Top + Operations.Elements[i].Rect.Top );
            _outgtext( Operations.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(OPER_DB_SHEAD_COLOR);
            _moveto( OperRect.Left + Operations.Elements[i].Rect.Left,
                     OperRect.Top + Operations.Elements[i].Rect.Top );
            _outgtext( Operations.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(OPER_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( OperRect.Left + Operations.Elements[i].Rect.Left,
                     OperRect.Top + Operations.Elements[i].Rect.Top );
            _lineto( OperRect.Right + Operations.Elements[i].Rect.Right,
                     OperRect.Top + Operations.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( OperRect.Left + Operations.Elements[i].Rect.Left,
                        OperRect.Top + Operations.Elements[i].Rect.Top,
                        (BUTTON) Operations.Elements[i].Object );
            break;
      }

   if (FastScreen && !OperImageOK)
   {
      _getimage(OperRect.Left, OperRect.Top, OperRect.Right, OperRect.Bottom, OperImageComplete);
      OperImageOK = 1;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HideOperationsWindow( )
{
   if (OperImage)
   {
      HideCursor();

      _putimage( OperRect.Left, OperRect.Top, OperImage, _GPSET );

      ShowCursor();

      free( OperImage );

      OperImage = 0;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [OPER.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
