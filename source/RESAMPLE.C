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
  ±   [RESAMPLE.C] - Resampling routines                                     ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   void  quick_resample(UCHAR *Source, UCHAR *Dest, int Length, int JumpSize, float *Map );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  resample(UCHAR *src, UCHAR *dst, int len, int offst, float *xmap )
{
int      i, u, x, v0r, v1r, v0g, v1g, v0b, v1b;
static   float valr, valg, valb, sizfac, inseg, outseg, accr, accg, accb, inpos[1280];

/* CLEANUP THE MAPPING IN CASE THERE IS A PROBLEM */

   for (i = 1; i < len-1; i++)
   {
      if (xmap[i-1] > xmap[i])
         xmap[i] = xmap[i-1];

      xmap[i] = (int)xmap[i] > len ? len:xmap[i];
   }

   xmap[len-1] = len-1;
   xmap[0] = 0;

/* CHECK FOR FAST MODE */

   if (FastMorphMode)
   {
      quick_resample( src, dst, len, offst, xmap );
      return;
   }

/* PRECOMPUTE INPUT INDEX FOR EACH OUTPUT PIXEL */

   for(u=x=0; x<len; x++)
   {
      while(xmap[u+1]<x) u++;

      inpos[x] = u + (double) (x-xmap[u]) / (xmap[u+1]-xmap[u]);
   }

/* COPY FIRST AND LAST PIXELS, SINCE THE WARPING IS DONE BETWEEN THEM */

   dst[(len-1)*offst+0] = src[(len-1)*offst+0];
   dst[(len-1)*offst+1] = src[(len-1)*offst+1];
   dst[(len-1)*offst+2] = src[(len-1)*offst+2];
   dst[0] = src[0];
   dst[1] = src[1];
   dst[2] = src[2];

/* START THE RESAMPLING */

   inseg  = 1.0;
   outseg = inpos[1];
   sizfac = outseg;
   accr = 0.0;
   accg = 0.0;
   accb = 0.0;

   v0r = src[0];
   v0g = src[1];
   v0b = src[2];  src += offst;
   v1r = src[0];
   v1g = src[1];
   v1b = src[2];  src += offst;

   for(u=1; u<len; )
   {
      valr = inseg*v0r + (1-inseg)*v1r;
      valg = inseg*v0g + (1-inseg)*v1g;
      valb = inseg*v0b + (1-inseg)*v1b;

      if(inseg < outseg)
      {
         accr += (valr * inseg);
         accg += (valg * inseg);
         accb += (valb * inseg);
         outseg -= inseg;
         inseg = 1.0;
         v0r = v1r;
         v0g = v1g;
         v0b = v1b;
         v1r = src[0];
         v1g = src[1];
         v1b = src[2];
         src += offst;
      }
      else
      {
         accr += (valr * outseg);
         accr /= sizfac;

         accg += (valg * outseg);
         accg /= sizfac;

         accb += (valb * outseg);
         accb /= sizfac;

         dst[0] = (int) MIN(accr, 0xff);
         dst[1] = (int) MIN(accg, 0xff);
         dst[2] = (int) MIN(accb, 0xff);

         dst += offst;
         accr = 0.0;
         accg = 0.0;
         accb = 0.0;
         inseg -= outseg;
         outseg = inpos[u+1] - inpos[u];
         sizfac = outseg;
         u++;
      }
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  quick_resample(UCHAR *src, UCHAR *dst, int len, int offst, float *xmap )
{
int   i, j, soff, doff;
int   txmap[2048];

/* COPY MAPPING TO INTs */

   for (i = 0; i < len; i++)
      txmap[i] = xmap[i];

/* START THE RESAMPLING */

   dst[(len-1)*offst+0] = src[(len-1)*offst+0];
   dst[(len-1)*offst+1] = src[(len-1)*offst+1];
   dst[(len-1)*offst+2] = src[(len-1)*offst+2];

   dst[0] = src[0];
   dst[1] = src[1];
   dst[2] = src[2];

   for (i = 0; i < len-1; i++)
   {
      soff = i * offst;

      for( j = txmap[i]; j <= txmap[i+1]; j++ )
      {
         doff = j * offst;

         dst[doff + 0] = src[soff + 0];
         dst[doff + 1] = src[soff + 1];
         dst[doff + 2] = src[soff + 2];
      }
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [RESAMPLE.C] - End Of File                                             ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

