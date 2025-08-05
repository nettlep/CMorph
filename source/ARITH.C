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
  ±   [ARITH.C     ] - Routines for arithmetic on output                     ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  CalculateArith( IMAGE Dest, IMAGE Source, IMAGE Back )
{
int   MinX, MinY, ColR, ColG, ColB;
int   x, y;

   if (Background == BACK_ON)
      AddBackground( Dest, Back );

   if (ArithType == ARITH_OFF)
      return;

   MinX = MIN( Dest->ResX, Source->ResX );
   MinY = MIN( Dest->ResY, Source->ResY );

   for( y = 0; y < MinY; y++ )
      for (x = 0; x < MinX * 3; x += 3)
      {
         if (ArithType == ARITH_AVE)
         {
            ColR = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 0] +
                 (int) Source->Buffer[y*Source->ResX*3 + x + 0];
            ColG = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 1] +
                 (int) Source->Buffer[y*Source->ResX*3 + x + 1];
            ColB = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 2] +
                 (int) Source->Buffer[y*Source->ResX*3 + x + 2];

            Dest->Buffer[y*Dest->ResX*3 + x + 0] = (char) (ColR >> 1);
            Dest->Buffer[y*Dest->ResX*3 + x + 1] = (char) (ColG >> 1);
            Dest->Buffer[y*Dest->ResX*3 + x + 2] = (char) (ColB >> 1);
         }
         else if (ArithType == ARITH_ADD)
         {
            ColR = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 0] +
                 (int) Source->Buffer[y*Source->ResX*3 + x + 0];
            ColG = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 1] +
                 (int) Source->Buffer[y*Source->ResX*3 + x + 1];
            ColB = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 2] +
                 (int) Source->Buffer[y*Source->ResX*3 + x + 2];

            Dest->Buffer[y*Dest->ResX*3 + x + 0] = (char) ((int) (ColR > 255 ? 255:ColR));
            Dest->Buffer[y*Dest->ResX*3 + x + 1] = (char) ((int) (ColG > 255 ? 255:ColG));
            Dest->Buffer[y*Dest->ResX*3 + x + 2] = (char) ((int) (ColB > 255 ? 255:ColB));
         }
         else if (ArithType == ARITH_SUB)
         {
            ColR = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 0] -
                 (int) Source->Buffer[y*Source->ResX*3 + x + 0];
            ColG = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 1] -
                 (int) Source->Buffer[y*Source->ResX*3 + x + 1];
            ColB = (int) Dest->Buffer[y*Dest->ResX  *3 + x + 2] -
                 (int) Source->Buffer[y*Source->ResX*3 + x + 2];

            Dest->Buffer[y*Dest->ResX*3 + x + 0] = (char) ((int) (ColR < 0 ? 0:ColR));
            Dest->Buffer[y*Dest->ResX*3 + x + 1] = (char) ((int) (ColG < 0 ? 0:ColG));
            Dest->Buffer[y*Dest->ResX*3 + x + 2] = (char) ((int) (ColB < 0 ? 0:ColB));
         }
         else if (ArithType == ARITH_OVR)
         {
            if (Source->Buffer[y*Source->ResX*3 + x + 0] >= OverlayThresholdL &&
                Source->Buffer[y*Source->ResX*3 + x + 0] <= OverlayThresholdH)
                  Dest->Buffer[y*Dest->ResX  *3 + x + 0] =
                Source->Buffer[y*Source->ResX*3 + x + 0];

            if (Source->Buffer[y*Source->ResX*3 + x + 1] >= OverlayThresholdL &&
                Source->Buffer[y*Source->ResX*3 + x + 1] <= OverlayThresholdH)
                  Dest->Buffer[y*Dest->ResX  *3 + x + 1] =
                Source->Buffer[y*Source->ResX*3 + x + 1];

            if (Source->Buffer[y*Source->ResX*3 + x + 2] >= OverlayThresholdL &&
                Source->Buffer[y*Source->ResX*3 + x + 2] <= OverlayThresholdH)
                  Dest->Buffer[y*Dest->ResX  *3 + x + 2] =
                Source->Buffer[y*Source->ResX*3 + x + 2];
         }
      }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  AddBackground( IMAGE Dest, IMAGE Source )
{
int   MinX, MinY;
int   x, y;

   MinX = MIN( Dest->ResX, Source->ResX );
   MinY = MIN( Dest->ResY, Source->ResY );

   if (Background == BACK_ON)
   {
      for( y = 0; y < MinY; y++ )
         for (x = 0; x < MinX * 3; x += 3)
         {
            if ( Dest->Buffer[y*Dest->ResX  *3 + x + 0] >= BackThresholdL &&
                 Dest->Buffer[y*Dest->ResX  *3 + x + 0] <= BackThresholdH)
                 Dest->Buffer[y*Dest->ResX  *3 + x + 0] =
               Source->Buffer[y*Source->ResX*3 + x + 0];

            if ( Dest->Buffer[y*Dest->ResX  *3 + x + 1] >= BackThresholdL &&
                 Dest->Buffer[y*Dest->ResX  *3 + x + 1] <= BackThresholdH)
                 Dest->Buffer[y*Dest->ResX  *3 + x + 1] =
               Source->Buffer[y*Source->ResX*3 + x + 1];

            if ( Dest->Buffer[y*Dest->ResX  *3 + x + 2] >= BackThresholdL &&
                 Dest->Buffer[y*Dest->ResX  *3 + x + 2] <= BackThresholdH)
                 Dest->Buffer[y*Dest->ResX  *3 + x + 2] =
               Source->Buffer[y*Source->ResX*3 + x + 2];
         }
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [ARITH.C     ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
