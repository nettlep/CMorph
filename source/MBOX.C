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
  ±   [MBOX.C      ] - Message box routines                                  ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   _RECT MBoxRect;
static   char  *MessBoxImage = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  MBox( char *String )
{
   DrawMessBox( "Information", String, STATUS_COLOR );
   Beep( );
   GetKey( );
   EraseMessBox( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawMessBox( char *Title, char *String, int Color )
{
int   Longest, Lines = 1;
int   i, j, ctr = 0;
static   char  TempStr[132], TotalTempStr[5000];
_RECT TRect;

   if (MessBoxImage)
      EraseMessBox( );

   strcpy( TotalTempStr, String );
   strcat( TotalTempStr, "\n" );

   Longest = strlen(Title);

   for( i = 0; TotalTempStr[i]; i++ )
   {
      if (TotalTempStr[i] != 0xa)
      {
         ctr++;
      }
      else
      {
         if (ctr > Longest)
            Longest=ctr;

         ctr = 0;
         Lines++;
      }
   }

   MBoxRect.Left = GI->CenX - Longest * TEXT_WIDTH / 2 - 7;
   MBoxRect.Top = GI->CenY - Lines * (TEXT_HEIGHT + 2) / 2 - 7;
   MBoxRect.Right = GI->CenX + Longest * TEXT_WIDTH / 2 + 7;
   MBoxRect.Bottom = GI->CenY + Lines * (TEXT_HEIGHT + 2) / 2 + 7;

   MessBoxImage = malloc(_imagesize( MBoxRect.Left, MBoxRect.Top, 
                                     MBoxRect.Right, MBoxRect.Bottom ));

   if (!MessBoxImage)
      FatalError( "Cannnot open message box", E_NOMEMMENU );

   _setplotaction(_GPSET);

   _getimage(MBoxRect.Left, MBoxRect.Top, MBoxRect.Right, MBoxRect.Bottom, MessBoxImage);

   TRect = MBoxRect;

   // DRAW WINDOW TITLE
   TRect.Bottom = TRect.Top + 7 + TEXT_HEIGHT;
   ShadeRect( &TRect, Color );

   // DRAW WINDOW BULK
   MBoxRect.Top += 7+TEXT_HEIGHT;
   ShadeRect( &MBoxRect, MBOX_WINDOW_COLOR );

   MBoxRect.Top -= 7+TEXT_HEIGHT;
   DrawInRect( MBoxRect.Left, MBoxRect.Top, MBoxRect.Right, MBoxRect.Bottom );

   _setcolor(MBOX_TITLE_TEXT_COLOR);
   _moveto( MBoxRect.Left + 7, MBoxRect.Top + 5 );
   strcpy( TempStr, Title );
   strcat( TempStr, ":" );
   _outgtext( TempStr );
   _setcolor(MBOX_TITLE_TEXT_COLOR);
   _moveto( MBoxRect.Left + 3, MBoxRect.Top + 7 + TEXT_HEIGHT );
   _lineto( MBoxRect.Right - 3, MBoxRect.Top + 7 + TEXT_HEIGHT );

   _setcolor(MBOX_TEXT_COLOR);

   for( Lines = 1, j = 0, i = 0; TotalTempStr[i]; i++ )
   {
      if (TotalTempStr[i] != 0xa)
      {
         TempStr[j] = TotalTempStr[i];
         j++;
      }
      else
      {
         TempStr[j] = 0;

         _moveto( MBoxRect.Left + 7, MBoxRect.Top + 10 + Lines * (TEXT_HEIGHT+2) );
         _outgtext( TempStr );

         j = 0;
         Lines++;
      }
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  EraseMessBox( )
{
   if (MessBoxImage)
   {
      _putimage( MBoxRect.Left, MBoxRect.Top, MessBoxImage, _GPSET );
      free( MessBoxImage );
      MessBoxImage = 0;
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [MBOX.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
