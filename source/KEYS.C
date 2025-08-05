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
  ±   [KEYS.C      ] - Keyboard handler                                      ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   HandleKeys( )
{
int   Key;

   if (kbhit())
   {
      Key = getch( );

      switch( toupper(Key) )
      {
         case 0:
            Key = getch( );

            switch( Key )
            {
               case CTRL_F9:
                  DebugMode = !DebugMode;
                  break;

               case ALT_A:
                  DoAbout( );
                  break;

               case ALT_B:
                  if (BackDoor && DebugMode)
                     DoBackDoor( );
                  else
                     Beep();
                  break;

               case ALT_C:
                  if (DebugMode)
                     DoScreenCapture( );
                  else
                     Beep();
                  break;

               case ALT_D:
                  ChangeMenu( MAINMENU );
                  DosShell();
                  break;

               case ALT_E:
                  ChangeMenu( EDITMENU );
                  break;

               case ALT_F:
                  ChangeMenu( FILEMENU );
                  break;

               case ALT_G:
                  ChangeMenu( OPERMENU );
                  break;

               case ALT_H:
               case F1:
                  DoHelp();
                  break;

               case ALT_I:
                  ChangeMenu( PROCMENU );
                  break;
#ifndef NOREG
               case ALT_L:
                  ChangeMenu( FLICMENU );
                  break;
#endif
               case ALT_P:
                  ChangeMenu( PERFMENU );
                  break;

               case ALT_R:
                  ChangeMenu( PREVMENU );
                  break;

               case ALT_S:
                  DoMorph();
                  break;

               case ALT_V:
                  DoPreview( );
                  break;

               case ALT_X:
                  return (VerifyExit());
                  break;

               default:
                  Beep( );
                  break;
            }

            break;

         case 27:
            ChangeMenu( MAINMENU );
            break;

         default:
            Beep( );
            break;
      }
   }

   return( 0 );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [KEYS.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
