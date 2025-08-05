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
  ±   [BICUBIC.C   ] - Bi-cubic spline routines                              ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   void  Spline( float yp1, float ypn, int n );
static   int   Splint( int n, int Tightness );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   float xa[1280], ya[1280], y2[1280], newy, xc;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   Bicubic(float *x1, float *y1, int len1, float *x2, float *y2, int len2, int Tightness)
{
int   i, RetCode;

   for(i = 1; i <= len1; i++)
   {
      ya[i] = y1[i-1];
      xa[i] = x1[i-1];
   }

   // ---------------------------
   Spline((ya[2] - ya[1]) /
            (xa[2] - xa[1]),
         (ya[len1] - ya[len1-1]) /
            (xa[len1] - xa[len1-1]),
         len1);

   for(i = 0; i < len2; i++)
   {
      xc = x2[i];
      RetCode = Splint(len1, Tightness);

      if (RetCode != E_NONE)
         return(RetCode);

      y2[i] = newy;
   }

   return(E_NONE);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   void  Spline( float yp1, float ypn, int n )
{
int	i, k;
static float p, qn, sig, un, u[999];

	if ( yp1 > 0.99e30 )
   {
      y2[1] = u[1] = 0.0;
   }
	else
	{
      y2[1] = -0.5;
      u[1] = (3.0 / (xa[2] - xa[1])) * ((ya[2] - ya[1]) / (xa[2] - xa[1]) - yp1);
	}

	for(i = 2; i < n; i++)
	{
      sig = (xa[i] - xa[i-1]) / (xa[i+1] - xa[i-1]);
      p = sig * y2[i-1] + 2.0;
      y2[i] = (sig - 1.0) / p;
      u[i] = (ya[i+1] - ya[i]) / (xa[i+1] - xa[i]) - (ya[i] - ya[i-1]) / (xa[i] - xa[i-1]);
      u[i] = (6.0 * u[i] / (xa[i+1] - xa[i-1]) - sig * u[i-1]) / p;
	}

   if (ypn > 0.99e30)
   {
      qn = un = 0.0;
   }
   else
   {
      qn = 0.5;
      un = (3.0 / (xa[n] - xa[n-1])) * (ypn - (ya[n] - ya[n-1]) / (xa[n] - xa[n-1]));
   }

   y2[n] = (un - qn * u[n-1]) / (qn * y2[n-1] + 1.0);

   for(k = n-1; k >= 1; k--)
      y2[k] = y2[k] * y2[k+1] + u[k];
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   Splint( int n, int Tightness )
{
int   klo, khi, k;
float h, b, a, tight;

   klo = 1;
   khi = n;

   while (khi-klo > 1)
   {
      k = (khi + klo) >> 1;
      xa[k] > xc ? khi = k : klo = k;
   }

   h = xa[khi] - xa[klo];

   if (h == 0.0)
      return( E_BADX );

   a = (xa[khi] - xc) / h;
   b = (xc - xa[klo]) / h;

   tight = (float)Tightness / 100 * 34.0 + 6.0;

   newy = a * ya[klo] + b * ya[khi] + ((a * a * a - a) * y2[klo] + (b * b * b - b) * y2[khi]) * (h * h) / tight;

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [BICUBIC.C   ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
