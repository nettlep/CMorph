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
  ±   [CATMULL.C   ] - Interpolating spline function                         ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

/* Catmull:
 *
 * Compute a Catmull-Rom spline passing through the len1 points in arrays
 * x1, y1, where y1 = f(x1)
 * len2 positions on the spline are to be computed. Their positions are
 * given in x2. The spline values are stored in y2.
 */

int   Catmull(float *x1, float *y1, int len1, float *x2, float *y2, int len2)
{
int      i,   j,   dir, j1,  j2;
double   x,   dx1, dx2;
double   dx,  dy,  yd1, yd2, p1, p2, p3;
double   a0y, a1y, a2y, a3y;


   /* find direction of monotonic x1; skip ends */

   if(x1[0] < x1[1])                            /* increasing */
   {
      if(x2[0]<x1[0] || x2[len2-1]>x1[len1-1])
         dir=0;
      else
         dir = 1;
   }
   else                                         /* decreasing */
   {
      if(x2[0]>x1[0] || x2[len2-1]<=x1[len1-1])
         dir=0;
      else
         dir = -1;
   }
   if(dir == 0)                                 /* error */
   {
      return( E_CORRUPTMESH );
   }

   /* p1 is first endpoint of interval
    * p2 is resampling position
    * p3 is second endpoint of interval
    * j  is input index for current interval
    */

   /* force coefficient initialization */

   if(dir==1)
      p3 = x2[0] - 1;
   else
      p3 = x2[0] + 1;

   for(i=0; i<len2; i++)
   {
      /* check if in new interval */

      p2 = x2[i];

      if((dir==1 && p2>p3) || (dir== -1 && p2<p3))
      {
         /* find the interval which contains p2 */

         if(dir)
         {
            for(j=0; j<len1 && p2>x1[j]; j++);

            if(p2 < x1[j])
               j--;
         }
         else
         {
            for(j=0; j<len1 && p2<x1[j]; j++);

            if(p2 > x1[j])
               j--;
         }

         p1 = x1[j];                            /* update 1st endpt */
         p3 = x1[j+1];		                     /* update 2nd endpt */

         /* clamp indices for endpoint interpolation */

         j1 = MAX(j-1, 0);
         j2 = MIN(j+2, len1-1);
            
         /* compute spline coefficients */

         dx  = 1.0 / (p3 - p1);
         dx1 = 1.0 / (p3 - x1[j1]);
         dx2 = 1.0 / (x1[j2] - p1);

         dy  = (y1[j+1] - y1[ j ]) * dx;
         yd1 = (y1[j+1] - y1[ j1]) * dx1;
         yd2 = (y1[j2 ] - y1[ j ]) * dx2;
         a0y =  y1[j];
         a1y =  yd1;
         a2y =  dx *  ( 3*dy - 2*yd1 - yd2);
         a3y =  dx*dx*(-2*dy +   yd1 + yd2);
      }

      /* use Horner's rule to calculate cubic polynomial */

      x = p2 - p1;

      y2[i] = ((a3y*x + a2y)*x + a1y)*x + a0y;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [CATMULL.C   ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
