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
  ±   [BUTTONS.C     ] - Button handling routines                            ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <conio.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

char UpRadioButtonBitmap[] = 
{
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x63>>2, 0x7F>>2, 0x7B>>2, 0x66>>2, 0x3F>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x50>>2, 0x90>>2, 0xA4>>2, 0xA7>>2, 0xA4>>2, 0xA0>>2, 0x98>>2, 0x8D>>2, 0x7A>>2, 0x4D>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x8C>>2, 0xAB>>2, 0xB3>>2, 0xB1>>2, 0xAF>>2, 0xAD>>2, 0xA8>>2, 0xA0>>2, 0x99>>2, 0x8D>>2, 0x7E>>2, 0x65>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x87>>2, 0xB0>>2, 0xB6>>2, 0xB8>>2, 0xBA>>2, 0xB5>>2, 0xB2>>2, 0xAD>>2, 0xA7>>2, 0x9F>>2, 0x95>>2, 0x88>>2, 0x79>>2, 0x61>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x67>>2, 0xAF>>2, 0xB5>>2, 0xBC>>2, 0xBD>>2, 0xBA>>2, 0xB8>>2, 0xB4>>2, 0xAF>>2, 0xA7>>2, 0xA1>>2, 0x97>>2, 0x8B>>2, 0x7E>>2, 0x6D>>2, 0x4E>>2, 0x00>>2,
   0x00>>2, 0x9F>>2, 0xB3>>2, 0xB8>>2, 0xBD>>2, 0xBD>>2, 0xBB>>2, 0xB8>>2, 0xB4>>2, 0xAF>>2, 0xA8>>2, 0xA1>>2, 0x97>>2, 0x8C>>2, 0x81>>2, 0x72>>2, 0x5C>>2, 0x00>>2,
   0x64>>2, 0xA9>>2, 0xB5>>2, 0xB8>>2, 0xB9>>2, 0xBB>>2, 0xBB>>2, 0xC2>>2, 0xB2>>2, 0xAD>>2, 0xA7>>2, 0x9F>>2, 0x96>>2, 0x8A>>2, 0x7F>>2, 0x70>>2, 0x5E>>2, 0x3E>>2,
   0x97>>2, 0xA9>>2, 0xB3>>2, 0xB7>>2, 0xB9>>2, 0xB8>>2, 0xC1>>2, 0xCA>>2, 0xAF>>2, 0xAA>>2, 0xA4>>2, 0x9C>>2, 0x92>>2, 0x88>>2, 0x7C>>2, 0x6F>>2, 0x5D>>2, 0x46>>2,
   0x97>>2, 0xA4>>2, 0xAE>>2, 0xB2>>2, 0xB4>>2, 0xB3>>2, 0xB2>>2, 0xAE>>2, 0xAA>>2, 0xA2>>2, 0x9F>>2, 0x98>>2, 0x8F>>2, 0x85>>2, 0x79>>2, 0x6A>>2, 0x59>>2, 0x45>>2,
   0x93>>2, 0xA0>>2, 0xAA>>2, 0xAE>>2, 0xB0>>2, 0xAF>>2, 0xAD>>2, 0xA7>>2, 0xA6>>2, 0xA1>>2, 0x9B>>2, 0x93>>2, 0x8A>>2, 0x81>>2, 0x75>>2, 0x66>>2, 0x52>>2, 0x41>>2,
   0x88>>2, 0x99>>2, 0xA2>>2, 0xA6>>2, 0xA6>>2, 0xA8>>2, 0xA4>>2, 0xA3>>2, 0x9F>>2, 0x9A>>2, 0x93>>2, 0x8C>>2, 0x81>>2, 0x78>>2, 0x6D>>2, 0x5E>>2, 0x4E>>2, 0x3F>>2,
   0x64>>2, 0x8D>>2, 0x98>>2, 0x99>>2, 0xA0>>2, 0xA0>>2, 0x9E>>2, 0x9B>>2, 0x95>>2, 0x92>>2, 0x8B>>2, 0x84>>2, 0x7B>>2, 0x70>>2, 0x63>>2, 0x55>>2, 0x44>>2, 0x3A>>2,
   0x00>>2, 0x7F>>2, 0x8E>>2, 0x92>>2, 0x96>>2, 0x96>>2, 0x94>>2, 0x92>>2, 0x8E>>2, 0x89>>2, 0x82>>2, 0x7B>>2, 0x72>>2, 0x67>>2, 0x5A>>2, 0x4C>>2, 0x3F>>2, 0x00>>2,
   0x00>>2, 0x5A>>2, 0x7F>>2, 0x84>>2, 0x89>>2, 0x8A>>2, 0x8A>>2, 0x88>>2, 0x83>>2, 0x7F>>2, 0x78>>2, 0x71>>2, 0x68>>2, 0x5A>>2, 0x4E>>2, 0x3E>>2, 0x3D>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x65>>2, 0x74>>2, 0x7C>>2, 0x7D>>2, 0x7C>>2, 0x7B>>2, 0x77>>2, 0x72>>2, 0x6C>>2, 0x63>>2, 0x5A>>2, 0x4D>>2, 0x42>>2, 0x3E>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x59>>2, 0x68>>2, 0x6C>>2, 0x6D>>2, 0x6B>>2, 0x67>>2, 0x62>>2, 0x5A>>2, 0x54>>2, 0x47>>2, 0x3F>>2, 0x3C>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x43>>2, 0x54>>2, 0x58>>2, 0x57>>2, 0x55>>2, 0x51>>2, 0x4A>>2, 0x42>>2, 0x3E>>2, 0x3B>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x38>>2, 0x3D>>2, 0x39>>2, 0x36>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2
};

char DownRadioButtonBitmap[] = 
{
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x37>>2, 0x37>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x36>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x35>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x36>>2, 0x3B>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x40>>2, 0x42>>2, 0x47>>2, 0x43>>2, 0x38>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x31>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x41>>2, 0x46>>2, 0x4F>>2, 0x53>>2, 0x59>>2, 0x5B>>2, 0x59>>2, 0x3D>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x45>>2, 0x4D>>2, 0x55>>2, 0x5E>>2, 0x61>>2, 0x69>>2, 0x6B>>2, 0x6C>>2, 0x66>>2, 0x31>>2, 0x00>>2,
   0x00>>2, 0x3B>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x46>>2, 0x52>>2, 0x5B>>2, 0x64>>2, 0x6B>>2, 0x72>>2, 0x77>>2, 0x7B>>2, 0x7B>>2, 0x7A>>2, 0x6A>>2, 0x00>>2,
   0x00>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x46>>2, 0x52>>2, 0x5F>>2, 0x68>>2, 0x70>>2, 0x78>>2, 0x7C>>2, 0x84>>2, 0x88>>2, 0x89>>2, 0x87>>2, 0x82>>2, 0x39>>2,
   0x3C>>2, 0x3F>>2, 0x3F>>2, 0x3F>>2, 0x45>>2, 0x51>>2, 0x5E>>2, 0x6A>>2, 0x73>>2, 0x7A>>2, 0x84>>2, 0x89>>2, 0x8E>>2, 0x93>>2, 0x96>>2, 0x94>>2, 0x90>>2, 0x66>>2,
   0x3F>>2, 0x3F>>2, 0x3F>>2, 0x42>>2, 0x4F>>2, 0x5C>>2, 0x67>>2, 0x73>>2, 0x7D>>2, 0x86>>2, 0x8E>>2, 0x93>>2, 0x9A>>2, 0x9D>>2, 0xA0>>2, 0x9E>>2, 0x9B>>2, 0x79>>2,
   0x3F>>2, 0x3F>>2, 0x3F>>2, 0x47>>2, 0x57>>2, 0x65>>2, 0x71>>2, 0x7D>>2, 0x83>>2, 0x8E>>2, 0x95>>2, 0x9C>>2, 0xA2>>2, 0xA6>>2, 0xA9>>2, 0xA7>>2, 0xA4>>2, 0x83>>2,
   0x3D>>2, 0x3F>>2, 0x41>>2, 0x51>>2, 0x60>>2, 0x6C>>2, 0x79>>2, 0x84>>2, 0x8E>>2, 0x96>>2, 0x9E>>2, 0xA5>>2, 0xA8>>2, 0xAE>>2, 0xB0>>2, 0xAE>>2, 0xAA>>2, 0x7E>>2,
   0x31>>2, 0x3F>>2, 0x46>>2, 0x57>>2, 0x63>>2, 0x73>>2, 0x80>>2, 0x8B>>2, 0x92>>2, 0x9B>>2, 0xA5>>2, 0xA8>>2, 0xB1>>2, 0xB5>>2, 0xB6>>2, 0xB3>>2, 0xAC>>2, 0x64>>2,
   0x00>>2, 0x3D>>2, 0x4A>>2, 0x5C>>2, 0x6B>>2, 0x79>>2, 0x85>>2, 0x91>>2, 0x9A>>2, 0xA3>>2, 0xAB>>2, 0xB1>>2, 0xB6>>2, 0xB9>>2, 0xBA>>2, 0xB7>>2, 0xAB>>2, 0x00>>2,
   0x00>>2, 0x31>>2, 0x4C>>2, 0x5F>>2, 0x6F>>2, 0x7D>>2, 0x89>>2, 0x95>>2, 0x9C>>2, 0xA7>>2, 0xAF>>2, 0xB2>>2, 0xBA>>2, 0xBC>>2, 0xBB>>2, 0xB5>>2, 0x77>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x45>>2, 0x5F>>2, 0x6F>>2, 0x7F>>2, 0x8C>>2, 0x98>>2, 0xA1>>2, 0xAA>>2, 0xB0>>2, 0xB6>>2, 0xBB>>2, 0xBC>>2, 0xBA>>2, 0xA8>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x56>>2, 0x6D>>2, 0x7D>>2, 0x8A>>2, 0x97>>2, 0xA0>>2, 0xA8>>2, 0xB0>>2, 0xB4>>2, 0xB7>>2, 0xB8>>2, 0xAB>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x5C>>2, 0x76>>2, 0x87>>2, 0x94>>2, 0x9E>>2, 0xA5>>2, 0xAC>>2, 0xB0>>2, 0xB0>>2, 0x91>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2,
   0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x54>>2, 0x73>>2, 0x8A>>2, 0x91>>2, 0x83>>2, 0x6B>>2, 0x43>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2, 0x00>>2
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static void  DrawStandButton( int X, int Y, BUTTON Button );
static void  DrawRadioButton( int X, int Y, BUTTON Button );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateButtons( )
{
int   RetCode = E_NONE;

   if (MenuMode == PERFMENU)
   {
      HideOperationsWindow();
      HideEditWindow();
      HideFileWindow();
      HideProcessesWindow();
      HidePrevWindow();
      HideFlicWindow();
      
      RetCode = ShowPreferencesWindow();
   }
   else if (MenuMode == OPERMENU)
   {
      HidePreferencesWindow();
      HideEditWindow();
      HideFileWindow();
      HideProcessesWindow();
      HidePrevWindow();
      HideFlicWindow();
      
      RetCode = ShowOperationsWindow();
   }
   else if (MenuMode == EDITMENU)
   {
      HidePreferencesWindow();
      HideOperationsWindow();
      HideFileWindow();
      HideProcessesWindow();
      HidePrevWindow();
      HideFlicWindow();
      
      RetCode = ShowEditWindow();
   }
   else if (MenuMode == FILEMENU)
   {
      HidePreferencesWindow();
      HideOperationsWindow();
      HideEditWindow();
      HideProcessesWindow();
      HidePrevWindow();
      HideFlicWindow();

      RetCode = ShowFileWindow();
   }
   else if (MenuMode == PROCMENU)
   {
      HidePreferencesWindow();
      HideOperationsWindow();
      HideEditWindow();
      HideFileWindow();
      HidePrevWindow();
      HideFlicWindow();

      RetCode = ShowProcessesWindow();
   }
   else if (MenuMode == PREVMENU)
   {
      HidePreferencesWindow();
      HideOperationsWindow();
      HideEditWindow();
      HideFileWindow();
      HideProcessesWindow();
      HideFlicWindow();

      RetCode = ShowPrevWindow();
   }
   else if (MenuMode == FLICMENU)
   {
      HidePreferencesWindow();
      HideOperationsWindow();
      HideEditWindow();
      HideFileWindow();
      HideProcessesWindow();
      HidePrevWindow();

      RetCode = ShowFlicWindow();
   }

   if (RetCode != E_NONE)
   {
      MenuMode = MAINMENU;
      ErrorBox( "Unable to change menus", RetCode );
   }

   if (MenuMode == MAINMENU)
   {
      HidePreferencesWindow( );
      HideOperationsWindow( );
      HideEditWindow();
      HideFileWindow();
      HideProcessesWindow();
      HidePrevWindow();
      HideFlicWindow();

      DrawPerfButton( );
      DrawOperButton( );
      DrawEditButton( );
      DrawFileButton( );
      DrawProcessesButton( );
      DrawPrevButton( );
      DrawFlicButton( );
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawButton( int X, int Y, BUTTON Button )
{
   if (Button->Type == STAND_BUTTON)
      DrawStandButton(X, Y, Button);
   else if (Button->Type == RADIO_BUTTON)
      DrawRadioButton(X, Y, Button);

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static void  DrawStandButton( int X, int Y, BUTTON Button )
{
int   Width, Height;

   Width = strlen( Button->Title ) * TEXT_WIDTH + 2 + 2 + 6;
   Height = TEXT_HEIGHT + 2 + 2 + 6;

   Button->Rect.Left   = X;
   Button->Rect.Top    = Y;
   Button->Rect.Right  = X + Width;
   Button->Rect.Bottom = Y + Height;

   if (Button->State == BUTTON_UP)
   {
      ShadeRect( &Button->Rect, BUTTON_FACE_COLOR );
      DrawOutRect( Button->Rect.Left, Button->Rect.Top, Button->Rect.Right, Button->Rect.Bottom );

      _setcolor(BUTTON_TEXT_COLOR);
      _moveto( Button->Rect.Left + 6, Button->Rect.Top + 6 );
      _outgtext( Button->Title );
   }
   else
   {
      ShadeRect( &Button->Rect, BUTTON_FACE_COLOR );
      DrawInRect( Button->Rect.Left, Button->Rect.Top, Button->Rect.Right, Button->Rect.Bottom );

      _setcolor(BUTTON_TEXT_COLOR);
      _moveto( Button->Rect.Left + 7, Button->Rect.Top + 7 );
      _outgtext( Button->Title );
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static void  DrawRadioButton( int X, int Y, BUTTON Button )
{
int   Largest, x, y, yoff, Shiftval = 0;
int   Width, Height;

   if (VMode == VGA)
      Shiftval = 2;

   Width = strlen( Button->Title ) * TEXT_WIDTH + 2 + 2 + 6;
   Height = TEXT_HEIGHT + 2 + 2 + 6;

   Button->Rect.Left   = X;
   Button->Rect.Top    = Y;
   Button->Rect.Right  = X + TEXT_WIDTH + 2 + 2 + 6;
   Button->Rect.Bottom = Y + TEXT_HEIGHT + 2 + 2 + 6;

   Largest = (TEXT_WIDTH>TEXT_HEIGHT ? TEXT_WIDTH:TEXT_HEIGHT)+2+2+6;

   if (Button->State == BUTTON_UP)
   {
      for( y = 0; y < 18; y++)
      {
         yoff = y*18;

         for( x = 0; x < 18; x++)
         {
            if (UpRadioButtonBitmap[yoff+x])
            {
               _setcolor(UpRadioButtonBitmap[yoff+x]>>Shiftval);
               _setpixel(Button->Rect.Left + x, Button->Rect.Top + y);
            }
         }
      }
   }
   else
   {
      for( y = 0; y < 18; y++)
      {
         yoff = y*18;

         for( x = 0; x < 18; x++)
         {
            if (DownRadioButtonBitmap[yoff+x])
            {
               _setcolor(DownRadioButtonBitmap[yoff+x]>>Shiftval);
               _setpixel(Button->Rect.Left + x, Button->Rect.Top + y);
            }
         }
      }
   }

   _setcolor(RBUTTON_TEXT_COLOR);
   _moveto( Button->Rect.Left + TEXT_WIDTH*2, Button->Rect.Top + 6 );
   _outgtext( Button->Title );

   /* Need to modify the button rect for the selectable range */
   Button->Rect.Left   = X;
   Button->Rect.Top    = Y;
   Button->Rect.Right  = X + Width + TEXT_WIDTH;
   Button->Rect.Bottom = Y + Height;

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [BUTTONS.C   ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
