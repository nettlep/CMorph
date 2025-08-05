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
  ±   [PROC.C      ] - Image processing menu routines                        ±
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

static   char  *ProcImage;
static   char  *ProcImageComplete;
static   int   ProcImageOK = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  Processes = {
   {"Image processing filters Menu"},
   {
      {{ 10, 20,  0,  0}, DIALOG_HEADING,    0,              0, "General:"},

      {{ 15, 37,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Filter File:"},
      {{111, 32,  0,  0}, DIALOG_TEXTBOX,    0, FilterFileName, &FilterFileNamePrompt},

      {{ 10, 58,  0,  0}, DIALOG_HEADING,    0,              0, "Source Image:"},

      {{ 15, 75,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Filter Name:"},
      {{111, 70,  0,  0}, DIALOG_TEXTBOX,    0, SourceFiltName, &SourceFilterNamePrompt},
      {{411, 70,  0,  0}, DIALOG_BUTTON,     0, 0,              &DoSourceFilterButton},

      {{ 10, 96,  0,  0}, DIALOG_HEADING,    0,              0, "Target Image:"},

      {{ 15,113,  0,  0}, DIALOG_SUBHEADING, 0,              0, "Filter Name:"},
      {{111,108,  0,  0}, DIALOG_TEXTBOX,    0, TargetFiltName, &TargetFilterNamePrompt},
      {{411,108,  0,  0}, DIALOG_BUTTON,     0, 0,              &DoTargetFilterButton},

      {{  0,  0,  0,  0}, DIALOG_END,        0,              0, ""}
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawProcessesButton( )
{
   DrawButton( EditButton.Rect.Left + 100,
               EditButton.Rect.Top,
               &ProcButton );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowProcessesWindow( )
{
int   i;
_RECT TRect;

   UpdateButtonStates( );

   ProcRect.Left = GI->CenX - PROC_WIDTH/2;
   ProcRect.Right = ProcRect.Left + PROC_WIDTH;
   ProcRect.Top  = GI->CenY - PROC_HEIGHT/2;
   ProcRect.Bottom = ProcRect.Top + PROC_HEIGHT;

   ProcImage = malloc(_imagesize( ProcRect.Left, ProcRect.Top, ProcRect.Right, ProcRect.Bottom ));

   if (!ProcImage)
      return( E_NOMEMMENU );

   _getimage(ProcRect.Left, ProcRect.Top, ProcRect.Right, ProcRect.Bottom, ProcImage);
   
   if (UpdateFastProc && FastScreen && ProcImageOK)
   {
      free( ProcImageComplete );
      ProcImageOK = 0;
      UpdateFastProc = 0;
   }

   if (FastScreen && !ProcImageOK)
   {
      ProcImageComplete = malloc(_imagesize( ProcRect.Left, ProcRect.Top, ProcRect.Right, ProcRect.Bottom ));

      if (!ProcImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && ProcImageOK)
   {
      _putimage( ProcRect.Left, ProcRect.Top, ProcImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &ProcRect, PROC_WINDOW_COLOR );

   TRect = ProcRect;
   TRect.Bottom = ProcRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, PROC_TITLE_COLOR );

   DrawOutRect( ProcRect.Left, ProcRect.Top, ProcRect.Right, ProcRect.Bottom );

   _setcolor(PROC_TITLE_TEXT_COLOR);
   _moveto( ProcRect.Left + (ProcRect.Right - ProcRect.Left) / 2 - 
            strlen(Processes.Title) * TEXT_WIDTH / 2, ProcRect.Top + 5 );
   _outgtext( Processes.Title );

   for (i = 0; Processes.Elements[i].Type != DIALOG_END; i++)
      switch( Processes.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(PROC_DB_HEAD_COLOR);
            _moveto( ProcRect.Left + Processes.Elements[i].Rect.Left,
                     ProcRect.Top + Processes.Elements[i].Rect.Top );
            _outgtext( Processes.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(PROC_DB_SHEAD_COLOR);
            _moveto( ProcRect.Left + Processes.Elements[i].Rect.Left,
                     ProcRect.Top + Processes.Elements[i].Rect.Top );
            _outgtext( Processes.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(PROC_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( ProcRect.Left + Processes.Elements[i].Rect.Left,
                     ProcRect.Top + Processes.Elements[i].Rect.Top );
            _lineto( ProcRect.Right + Processes.Elements[i].Rect.Right,
                     ProcRect.Top + Processes.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( ProcRect.Left + Processes.Elements[i].Rect.Left,
                        ProcRect.Top + Processes.Elements[i].Rect.Top,
                        (BUTTON) Processes.Elements[i].Object );
            break;

         case DIALOG_TEXTBOX:
            DrawPromptTBox( ProcRect.Left + Processes.Elements[i].Rect.Left, 
                            ProcRect.Top + Processes.Elements[i].Rect.Top,
                            Processes.Elements[i].PromptString,
                            (PROMPT) Processes.Elements[i].Object );
            break;
      }

   if (FastScreen && !ProcImageOK)
   {
      _getimage(ProcRect.Left, ProcRect.Top, ProcRect.Right, ProcRect.Bottom, ProcImageComplete);
      ProcImageOK = 1;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HideProcessesWindow( )
{
   if (ProcImage)
   {
      HideCursor();

      _putimage( ProcRect.Left, ProcRect.Top, ProcImage, _GPSET );

      ShowCursor();

      free( ProcImage );

      ProcImage = 0;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [PROC.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

