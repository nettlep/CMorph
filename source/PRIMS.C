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
  ±   [PRIMS.C     ] - Graphics primitive routines                           ±
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

void  Circle( int X, int Y )
{
   _setcolor(CursColor);
   _setplotaction(_GXOR);

   _ellipse( _GFILLINTERIOR, X-4, Y-4, X+4, Y+4 );

   _setplotaction(_GPSET);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawOutRect( int Left, int Top, int Right, int Bottom )
{
   // DRAW BORDER

   _setcolor(SHADOW_COLOR);

   _rectangle( _GBORDER, Left, Top, Right, Bottom);
   _rectangle( _GBORDER, Left+1, Top+1, Right-1, Bottom-1);
   _rectangle( _GBORDER, Left+2, Top+2, Right-2, Bottom-2);

   // NOW OVERLAY WITH 3-D LOOKING HIGHLIGHTS

   _setcolor(HIGHLIGHT_COLOR);

   // TOP SIDE
   _moveto( Left, Top );
   _lineto( Right, Top+0 );
   _moveto( Left, Top+1 );
   _lineto( Right-1, Top+1 );
   _moveto( Left, Top+2 );
   _lineto( Right-2, Top+2 );

   // LEFT SIDE
   _moveto( Left, Top );
   _lineto( Left, Bottom);
   _moveto( Left+1, Top );
   _lineto( Left+1, Bottom-1);
   _moveto( Left+2, Top );
   _lineto( Left+2, Bottom-2);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawInRect( int Left, int Top, int Right, int Bottom )
{
   // DRAW BORDER

   _setcolor(HIGHLIGHT_COLOR);

   _rectangle( _GBORDER, Left, Top, Right, Bottom);
   _rectangle( _GBORDER, Left+1, Top+1, Right-1, Bottom-1);
   _rectangle( _GBORDER, Left+2, Top+2, Right-2, Bottom-2);

   // NOW OVERLAY WITH 3-D LOOKING SHADOWS

   _setcolor(SHADOW_COLOR);

   // TOP SIDE
   _moveto( Left, Top );
   _lineto( Right, Top+0 );
   _moveto( Left, Top+1 );
   _lineto( Right-1, Top+1 );
   _moveto( Left, Top+2 );
   _lineto( Right-2, Top+2 );

   // LEFT SIDE
   _moveto( Left, Top );
   _lineto( Left, Bottom);
   _moveto( Left+1, Top );
   _lineto( Left+1, Bottom-1);
   _moveto( Left+2, Top );
   _lineto( Left+2, Bottom-2);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawInGroove( int Left, int Top, int Right, int Bottom )
{
   _setcolor(SHADOW_COLOR);
   _rectangle( _GBORDER, Left, Top, Right-1, Bottom-1);

   _setcolor(HIGHLIGHT_COLOR);
   _rectangle( _GBORDER, Left+1, Top+1, Right, Bottom);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawOutGroove( int Left, int Top, int Right, int Bottom )
{
   _setcolor(SHADOW_COLOR);
   _rectangle( _GBORDER, Left, Top, Right-1, Bottom-1);

   _setcolor(HIGHLIGHT_COLOR);
   _rectangle( _GBORDER, Left+1, Top+1, Right, Bottom);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ShadeRect( RECT Rect, int ColorStart )
{
int   i, Width, Height;
#define  ShadeRange (40.0 * 2)

   if (VMode == VGA)
   {
      _setcolor( ColorStart );
      _rectangle( _GFILLINTERIOR, Rect->Left, Rect->Top, Rect->Right, Rect->Bottom );
      return;
   }

   Width  = Rect->Right - Rect->Left;
   Height = Rect->Bottom - Rect->Top;

   for( i = 0; i < Width + Height; i++ )
   {
      _setcolor( ColorStart + LIGHT_GREY - (int)(ShadeRange * 
                 (float) i / (float)(Width+Height)));

      if (Height < Width)
      {
         if (i < Height)
         {
            _moveto( Rect->Left+i, Rect->Top );
            _lineto( Rect->Left, Rect->Top+i );
         }
         else if (i < (Height+Width)/2)
         {
            _moveto( Rect->Left+i, Rect->Top );
            _lineto( Rect->Left-Height+i, Rect->Bottom );
         }
         else
         {
            _setcolor( ColorStart + LIGHT_GREY-ShadeRange/2 + (int)(ShadeRange * 
                     (float)(i-((Height+Width)/2)) / (float)(Width+Height)));

            if (i < Width)
            {
               _moveto( Rect->Left+i, Rect->Top );
               _lineto( Rect->Left-Height+i, Rect->Bottom );
            }
            else
            {
               _moveto( Rect->Right, Rect->Top-Width+i );
               _lineto( Rect->Left-Height+i, Rect->Bottom );
            }
         }
      }
      else
      {
         if (i < Width)
         {
            _moveto( Rect->Left+i, Rect->Top );
            _lineto( Rect->Left, Rect->Top+i );
         }
         else if (i < (Height+Width)/2)
         {
            _moveto( Rect->Right, Rect->Top+i-Width );
            _lineto( Rect->Left, Rect->Top+i );
         }
         else
         {
            _setcolor( ColorStart + LIGHT_GREY-ShadeRange/2 + (int)(ShadeRange * 
                     (float)(i-((Height+Width)/2)) / (float)(Width+Height)));

            if (i < Height)
            {
               _moveto( Rect->Right, Rect->Top+i-Width );
               _lineto( Rect->Left, Rect->Top+i );
            }
            else
            {
               _moveto( Rect->Right, Rect->Top+i-Width );
               _lineto( Rect->Left+i-Height, Rect->Bottom );
            }
         }
      }
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  FadeRect( RECT Rect, int ColorStart )
{
int   i, Width, Height;

   if (VMode == VGA)
   {
      _setcolor( ColorStart );
      _rectangle( _GFILLINTERIOR, Rect->Left, Rect->Top, Rect->Right, Rect->Bottom );
      return;
   }

   Width  = Rect->Right - Rect->Left;
   Height = Rect->Bottom - Rect->Top;

   for( i = 0; i < Width + Height; i++ )
   {
      if (i)
         _setcolor( (int)((float)(LIGHT_GREY / (((float)Width+(float)Height) / (float)i))) + ColorStart );
      else
         _setcolor( ColorStart );

      if (i < Height)
      {
         _moveto( Rect->Left+i, Rect->Top );
         _lineto( Rect->Left, Rect->Top+i );
      }
      else if (i < Width)
      {
         _moveto( Rect->Left+i, Rect->Top );
         _lineto( Rect->Left-Height+i, Rect->Bottom );
      }
      else
      {
         _moveto( Rect->Right, Rect->Top-Width+i );
         _lineto( Rect->Left-Height+i, Rect->Bottom );
      }
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [PRIMS.C     ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*//*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/


