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
  ±   [MOUSE.C     ] - Mouse routines                                        ±
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

char  CursorShape1[17][17] = {
   {".                "},
   {" ..              "},
   {" .X..            "},
   {"  .XX..          "},
   {"  .XXXX..        "},
   {"   .XXXXX..      "},
   {"   .XXXXXXX..    "},
   {"    .XXXXXXXX..  "},
   {"    .XXXXXXXXXX. "},
   {"     .XXXXXX.... "},
   {"     .XXXX....   "},
   {"      .XXX...    "},
   {"      .XX...     "},
   {"       .X..      "},
   {"       .X.       "},
   {"        .        "},
   {"                 "}};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   char  BullImage[500];
static   int   MouseSensativity = 4;
static   int   DoubleSpeedThreshold = 10;
static   int   MouseRangeLeft = 0, MouseRangeRight = 0;
static   int   MouseRangeTop = 0, MouseRangeBottom = 0;
static   int   MouseX = 0, MouseY = 0;
static   int   OldMouseX = 0, OldMouseY = 0;
static   int   Left = 0; Right = 0;
static   int   LastCursDrawn, FirstCursDrawn;
static   int   OldNearX = -1, NearX, OldNearY = -1, NearY;
static   int   ocx = 0, ocy = 0, cx, cy;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   InitMouse( )
{
union REGS InRegs, OutRegs;

   FirstCursDrawn = 1;

   SetMouseRange( 0, GI->MaxX, 0, GI->MaxY );

   InRegs.w.ax = MOUSE_INIT;
   int86( MOUSE_INT, &InRegs, &OutRegs );

   if (OutRegs.w.ax == 0xffff)
      return 1;

   return 0;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SetMousePosition( int X, int Y )
{
   MouseX = X;
   MouseY = Y;
   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SetMouseRange( int Left, int Right, int Top, int Bottom )
{
   MouseRangeLeft = Left;
   MouseRangeRight = Right;
   MouseRangeTop = Top;
   MouseRangeBottom = Bottom;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SetMouseSensativity( unsigned int Val )
{
   if (Val > 6)
      Val = 6;

   MouseSensativity = Val;

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ReadMouse( int *Col, int *Row, int *BLeft, int *BRight, int Draw )
{
union    REGS InRegs, OutRegs;
short int MovedX, MovedY;
static   AccX = 0, AccY = 0;

   InRegs.w.ax = 0x000B;
   int86( MOUSE_INT, &InRegs, &OutRegs );

   MovedX = OutRegs.w.cx;
   MovedY = OutRegs.w.dx;

   if (MouseSensativity)
   {
      AccX += MovedX % MouseSensativity;
      AccY += MovedY % MouseSensativity;
      MovedX /= MouseSensativity;
      MovedY /= MouseSensativity;
   }

   if (AccX > MouseSensativity)
   {
      MovedX++;
      AccX -= MouseSensativity;
   }
   else if (AccX < -MouseSensativity)
   {
      MovedX--;
      AccX += MouseSensativity;
   }

   if (AccY > MouseSensativity)
   {
      MovedY++;
      AccY -= MouseSensativity;
   }
   else if (AccY < -MouseSensativity)
   {
      MovedY--;
      AccY += MouseSensativity;
   }

   if (ABS(MovedX) > DoubleSpeedThreshold)
      MovedX *= 2;

   if (ABS(MovedY) > DoubleSpeedThreshold)
      MovedY *= 2;

   MouseX += MovedX;
   MouseY += MovedY;

   if (MouseX < MouseRangeLeft)
      MouseX = MouseRangeLeft;

   if (MouseX > MouseRangeRight)
      MouseX = MouseRangeRight;

   if (MouseY < MouseRangeTop)
      MouseY = MouseRangeTop;

   if (MouseY > MouseRangeBottom)
      MouseY = MouseRangeBottom;

   *Col = MouseX;
   *Row = MouseY;

   InRegs.w.ax = MOUSE_READ_INF;
   int86( MOUSE_INT, &InRegs, &OutRegs );

   Left = OutRegs.w.bx & 1;
   Right = (OutRegs.w.bx >> 1) & 1;

   *BLeft = Left;
   *BRight = Right;

   if (Draw)
      if (MouseX != OldMouseX || MouseY != OldMouseY)
      {
         DrawCursor( MouseX, MouseY, OldMouseX, OldMouseY );
         OldMouseX = MouseX;
         OldMouseY = MouseY;
      }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UnInitMouse( )
{
   InitMouse( );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SaveCursor( int X, int Y )
{
   _getimage( X, Y, X+17, Y+17, BullImage );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  RestoreCursor( int X, int Y )
{
   if (FirstCursDrawn)
      return;

   if (LastCursDrawn == CROSS)
   {
      _setcolor(CursColor);
      _setplotaction(_GXOR);
      _moveto( X, 0 );
      _lineto( X, SourceImage->WindowBorderBottom );
      _moveto( 0, Y );
      _lineto( GI->MaxX, Y );

      if (X >= SourceImage->WindowBorderLeft && X <= SourceImage->WindowBorderRight &&
          Y >= SourceImage->WindowBorderTop  && Y <= SourceImage->WindowBorderBottom)
      {
         _moveto( X - SourceImage->WindowLeft + TargetImage->WindowLeft, 
                  Y - SourceImage->WindowTop  + TargetImage->WindowTop - 4);

         _lineto( X - SourceImage->WindowLeft + TargetImage->WindowLeft, 
                  Y - SourceImage->WindowTop  + TargetImage->WindowTop + 4);
      }
      if (X >= TargetImage->WindowBorderLeft && X <= TargetImage->WindowBorderRight &&
          Y >= TargetImage->WindowBorderTop  && Y <= TargetImage->WindowBorderBottom)
      {
         _moveto( X - TargetImage->WindowLeft + SourceImage->WindowLeft, 
                  Y - TargetImage->WindowTop  + SourceImage->WindowTop - 4);

         _lineto( X - TargetImage->WindowLeft + SourceImage->WindowLeft, 
                  Y - TargetImage->WindowTop  + SourceImage->WindowTop + 4);
      }
      
      if (OldNearX != -1 && OldNearY != -1)
      {
         Circle( OldNearX, OldNearY );
         Circle( ocx, ocy );
         OldNearX = -1;
         OldNearY = -1;
      }

      _setplotaction(_GPSET);
   }
   else
      _putimage( X, Y, BullImage, _GPSET );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawCursor( int X, int Y, int Ox, int Oy )
{
int   x, y;
int   h1, h2, v1, v2;

   // ON SOURCE IMAGE?
   if (X >= SourceImage->WindowBorderLeft && X <= SourceImage->WindowBorderRight &&
       Y >= SourceImage->WindowBorderTop  && Y <= SourceImage->WindowBorderBottom &&
       MenuMode == MAINMENU)
   {
      DrawImageInfo( &SourceInfoWindow, X, Y, SourceImage, "Source" );

      RestoreCursor( Ox, Oy );
      LastCursDrawn = CROSS;

      _setcolor(CursColor);
      _setplotaction(_GXOR);

      _moveto( X, 0 );
      _lineto( X, SourceImage->WindowBorderBottom );
      _moveto( 0, Y );
      _lineto( GI->MaxX, Y );

      _moveto( X - SourceImage->WindowLeft + TargetImage->WindowLeft, 
               Y - SourceImage->WindowTop  + TargetImage->WindowTop - 4);

      _lineto( X - SourceImage->WindowLeft + TargetImage->WindowLeft, 
               Y - SourceImage->WindowTop  + TargetImage->WindowTop + 4);

      _setplotaction(_GPSET);
   }

   // ON TARGET IMAGE?
   else if (X >= TargetImage->WindowBorderLeft && X <= TargetImage->WindowBorderRight &&
            Y >= TargetImage->WindowBorderTop  && Y <= TargetImage->WindowBorderBottom &&
            MenuMode == MAINMENU)
   {
      DrawImageInfo( &TargetInfoWindow, X, Y, TargetImage, "Target" );

      RestoreCursor( Ox, Oy );
      LastCursDrawn = CROSS;
      
      _setcolor(CursColor);
      _setplotaction(_GXOR);
      _moveto( X, 0 );
      _lineto( X, TargetImage->WindowBorderBottom );
      _moveto( 0, Y );
      _lineto( GI->MaxX, Y );

      _moveto( X - TargetImage->WindowLeft + SourceImage->WindowLeft, 
               Y - TargetImage->WindowTop  + SourceImage->WindowTop - 4);

      _lineto( X - TargetImage->WindowLeft + SourceImage->WindowLeft, 
               Y - TargetImage->WindowTop  + SourceImage->WindowTop + 4);

      _setplotaction(_GPSET);
   }

   // USE BULLSEYE
   else
   {
      RestoreCursor( Ox, Oy );

      LastCursDrawn = BULLSEYE;

      SaveCursor( X, Y );
      
      _setcolor(BULLSEYE_OUTLINE_COLOR);

      for( y = 0; y < 17; y++)
         for( x = 0; x < 17; x++)
         if (CursorShape1[y][x] == '.')
            _setpixel( X+x, Y+y );

      _setcolor(CursColor);

      for( y = 0; y < 17; y++)
         for( x = 0; x < 17; x++)
         if (CursorShape1[y][x] == 'X')
            _setpixel( X+x, Y+y );
   }

// Now draw the highlighted point
   if (MouseX >= SourceImage->WindowLeft &&
       MouseX <= SourceImage->WindowRight &&
       MouseY >= SourceImage->WindowTop &&
       MouseY <= SourceImage->WindowBottom &&
       MenuMode == MAINMENU)
   {
      NearestMeshPoint( SourceMesh, SourceImage, MouseX, MouseY, &NearX, &NearY, &h1, &h2, &v1, &v2 );

      cx = (int) ((float) (TargetImage->WindowRight - TargetImage->WindowLeft) * TargetMesh->VertList[v1][v2]) + TargetImage->WindowLeft;
      cy = (int) ((float) (TargetImage->WindowBottom - TargetImage->WindowTop) * TargetMesh->HorizList[h1][h2]) + TargetImage->WindowTop;

      if (NearX != OldNearX || NearY != OldNearY)
      {
         if (OldNearX != -1 && OldNearY != -1)
            Circle( OldNearX, OldNearY );

         Circle( NearX, NearY );

         // DRAW THE MIRROR HIGHLIGHT
         Circle( cx, cy );

         OldNearX = NearX;
         OldNearY = NearY;
         ocx = cx; ocy = cy;
      }
   }
   else if (MouseX >= TargetImage->WindowLeft &&
            MouseX <= TargetImage->WindowRight &&
            MouseY >= TargetImage->WindowTop &&
            MouseY <= TargetImage->WindowBottom &&
            MenuMode == MAINMENU)
   {
      NearestMeshPoint( TargetMesh, TargetImage, MouseX, MouseY, &NearX, &NearY, &h1, &h2, &v1, &v2  );

      cx = (int) ((float) (SourceImage->WindowRight - SourceImage->WindowLeft) * SourceMesh->VertList[v1][v2]) + SourceImage->WindowLeft;
      cy = (int) ((float) (SourceImage->WindowBottom - SourceImage->WindowTop) * SourceMesh->HorizList[h1][h2]) + SourceImage->WindowTop;

      if (NearX != OldNearX || NearY != OldNearY)
      {
         if (OldNearX != -1 && OldNearY != -1)
            Circle( OldNearX, OldNearY );

         Circle( NearX, NearY );

         // DRAW THE MIRROR HIGHLIGHT
         Circle( cx, cy );

         OldNearX = NearX;
         OldNearY = NearY;
         ocx = cx; ocy = cy;
      }
   }

   FirstCursDrawn = 0;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HideCursor( )
{
   RestoreCursor( OldMouseX, OldMouseY );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ShowCursor( )
{
   FirstCursDrawn = 1;
   DrawCursor( MouseX, MouseY, OldMouseX, OldMouseY );
   OldMouseX = MouseX;
   OldMouseY = MouseY;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   InRect( int X, int Y, RECT Rect )
{
   if (X >= Rect->Left && X <= Rect->Right && 
       Y >= Rect->Top  && Y <= Rect->Bottom)
      return 1;

   return 0;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [MOUSE.C     ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
