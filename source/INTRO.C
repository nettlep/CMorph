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
  ±   [INTRO.C     ] - Source for intro                                      ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <dos.h>
#include <mem.h>
#include <string.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdio.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#define  END_FILE    (0 << 12)
#define  END_FRAME   (1 << 12)
#define  WRD_CHANGE  (2 << 12)
#define  STR_CHANGE  (3 << 12)
#define  SCR_CHANGE  (4 << 12)
#define  PAL_CHANGE  (5 << 12)

#define  PAL_SIZE    (256*3)
#define  FRAME_SIZE  64000
#define  FIRST       1
#define  NEXT        2
#define  LAST        3

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   PlayANM( char *FileName )
{
int                  Done = 0, i, Length, AnimHandle;
unsigned short int   *AnimBuffer;
char                 *Palette;
union REGS           regs, oregs;
struct SREGS         sregs;

/* OPEN THE ANIMATION FILE */

   AnimHandle = open( FileName, O_RDONLY | O_BINARY );   /* open file */

   if ( AnimHandle == -1 )
      return(0);

   Length = (unsigned long) filelength(AnimHandle);

/* MALLOC FRAME MEMORY */

   AnimBuffer = malloc( Length );

   if (!AnimBuffer)
      return(0);

   read( AnimHandle, AnimBuffer, Length );

   regs.w.ax = 0x13;
   int86( 0x10, &regs, &regs );

   Palette = DMalloc( 256*3 );

   for( i = 0; i < 256; i++ )
   {
      Palette[i*3+0] = 0; //(unsigned char) (i>>2);
      Palette[i*3+1] = 0; //(unsigned char) (i>>2);
      Palette[i*3+2] = (unsigned char) (i>>2);
   }

   regs.w.ax = 0x1012;
   regs.w.bx = 0;
   regs.w.cx = 256;
   regs.w.dx = PROT_OFF(Palette);
   sregs.es = PROT_SEG(Palette);
   int86x( VIDEO_INT, &regs, &oregs, &sregs );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±                           MAIN LOOP OF PROGRAM                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

   while( 1 )
   {
      Done = AnimateFrame( AnimBuffer );

      if (Done == -2 || Done == -1)
         break;

      delay(30);

      AnimBuffer = &AnimBuffer[Done];
   }

   free( AnimBuffer );

   return( 1 );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [INTRO.C     ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
