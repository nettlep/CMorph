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
  ±   [MOVEMESH.C  ] - Mesh movement routines                                ±
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

void  MoveMesh( int MouseX, int MouseY, int Left, int Right )
{
int   NearX, NearY, h1, h2, v1, v2;
int   OldMouseX = 0, OldMouseY = 0;

   if (MouseX >= SourceImage->WindowLeft && 
       MouseX <= SourceImage->WindowRight &&
       MouseY >= SourceImage->WindowTop && 
       MouseY <= SourceImage->WindowBottom)
   {
      NearestMeshPoint( SourceMesh, SourceImage, MouseX, MouseY, &NearX, &NearY, &h1, &h2, &v1, &v2 );
   
      // CLICK ON A CORNER? WAIT FOR NO CLICK AND GET OUT!
      if ((!v1 && !h1) || 
         (!v1 && h1 == SourceMesh->HorizRes-1) ||
         (!h1 && v1 == SourceMesh->VertRes-1) ||
         (v1 == SourceMesh->VertRes-1 && h1 == SourceMesh->HorizRes-1))
      {
         Beep( );

         while( Left )
            ReadMouse( &MouseX, &MouseY, &Left, &Right, 0 );

         return;
      }

      SetMousePosition((int) ((float)(SourceMesh->VertList[v1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft,
                     (int) ((float)(SourceMesh->HorizList[h1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop);

      // TOP BORDER?
      if (!h1)
      {
         SetMouseRange((int) ((float) (SourceMesh->VertList[v1-1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft + 1,
                     (int) ((float) (SourceMesh->VertList[v1+1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft - 1,
                     SourceImage->WindowTop, SourceImage->WindowTop);
      }
      // LEFT BORDER?
      else if (!v1)
      {
         SetMouseRange(SourceImage->WindowLeft, SourceImage->WindowLeft,
                     (int) ((float) (SourceMesh->HorizList[h1-1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop + 1,
                     (int) ((float) (SourceMesh->HorizList[h1+1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop - 1);
                    
      }
      // BOTTOM BORDER?
      else if (h1 == SourceMesh->HorizRes-1)
      {
         SetMouseRange((int) ((float) (SourceMesh->VertList[v1-1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft + 1,
                     (int) ((float) (SourceMesh->VertList[v1+1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft - 1,
                     SourceImage->WindowBottom, SourceImage->WindowBottom);
                    
      }
      // RIGHT BORDER?
      else if (v1 == SourceMesh->VertRes-1)
      {
         SetMouseRange(SourceImage->WindowRight, SourceImage->WindowRight,
                     (int) ((float) (SourceMesh->HorizList[h1-1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop + 1,
                     (int) ((float) (SourceMesh->HorizList[h1+1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop - 1);
                    
      }
      else
      {
         SetMouseRange((int) ((float) (SourceMesh->VertList[v1-1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft + 1,
                     (int) ((float) (SourceMesh->VertList[v1+1][v2]  * (float) (SourceImage->WindowRight - SourceImage->WindowLeft))) + SourceImage->WindowLeft - 1,
                     (int) ((float) (SourceMesh->HorizList[h1-1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop + 1,
                     (int) ((float) (SourceMesh->HorizList[h1+1][h2] * (float) (SourceImage->WindowBottom - SourceImage->WindowTop))) + SourceImage->WindowTop - 1);
      }

      while( Left )
      {
         ReadMouse( &MouseX, &MouseY, &Left, &Right, 0 );

         if (OldMouseX != MouseX || OldMouseY != MouseY)
         {
            if (OldMouseX && OldMouseY)
               Circle( OldMouseX, OldMouseY );

            OldMouseX = MouseX;
            OldMouseY = MouseY;
            Circle( MouseX, MouseY );

            DrawImageInfo( &SourceInfoWindow, MouseX, MouseY, SourceImage, "Source" );
         }
      }

      // ERASE THE CIRCLE
      Circle( OldMouseX, OldMouseY );

      // SET THE NEW COORDS
      SourceMesh->VertList[v1][v2] = (float) (MouseX - SourceImage->WindowLeft) / (float) (SourceImage->WindowRight - SourceImage->WindowLeft);
      SourceMesh->HorizList[h1][h2] = (float) (MouseY - SourceImage->WindowTop) / (float) (SourceImage->WindowBottom - SourceImage->WindowTop);

      // RESET THE MOUSE RANGE
      SetMouseRange( 0, GI->MaxX, 0, GI->MaxY );

      // NEED TO RESET THESE SO NEXT TIME IN IT KNOWS TO DRAW FIRST CIRCLE
      OldMouseX = 0;
      OldMouseY = 0;

      RestoreImage( SourceImage );
      DrawMesh(SourceMesh, SourceImage);
   }
   else if (MouseX >= TargetImage->WindowLeft && 
            MouseX <= TargetImage->WindowRight &&
            MouseY >= TargetImage->WindowTop && 
            MouseY <= TargetImage->WindowBottom)
   {
      NearestMeshPoint( TargetMesh, TargetImage, MouseX, MouseY, &NearX, &NearY, &h1, &h2, &v1, &v2 );
   
      // CLICK ON A CORNER? WAIT FOR NO CLICK AND GET OUT!
      if ((!v1 && !h1) || 
         (!v1 && h1 == TargetMesh->HorizRes-1) ||
         (!h1 && v1 == TargetMesh->VertRes-1) ||
         (v1 == TargetMesh->VertRes-1 && h1 == TargetMesh->HorizRes-1))
      {
         Beep( );

         while( Left )
            ReadMouse( &MouseX, &MouseY, &Left, &Right, 0 );

         return;
      }

      SetMousePosition((int) ((float)(TargetMesh->VertList[v1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft,
                     (int) ((float)(TargetMesh->HorizList[h1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop);

      // TOP BORDER?
      if (!h1)
      {
         SetMouseRange((int) ((float) (TargetMesh->VertList[v1-1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft + 1,
                     (int) ((float) (TargetMesh->VertList[v1+1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft - 1,
                     TargetImage->WindowTop, TargetImage->WindowTop);
      }
      // LEFT BORDER?
      else if (!v1)
      {
         SetMouseRange(TargetImage->WindowLeft, TargetImage->WindowLeft,
                     (int) ((float) (TargetMesh->HorizList[h1-1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop + 1,
                     (int) ((float) (TargetMesh->HorizList[h1+1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop - 1);
                    
      }
      // BOTTOM BORDER?
      else if (h1 == TargetMesh->HorizRes-1)
      {
         SetMouseRange((int) ((float) (TargetMesh->VertList[v1-1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft + 1,
                     (int) ((float) (TargetMesh->VertList[v1+1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft - 1,
                     TargetImage->WindowBottom, TargetImage->WindowBottom);
                    
      }
      // RIGHT BORDER?
      else if (v1 == TargetMesh->VertRes-1)
      {
         SetMouseRange(TargetImage->WindowRight, TargetImage->WindowRight,
                     (int) ((float) (TargetMesh->HorizList[h1-1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop + 1,
                     (int) ((float) (TargetMesh->HorizList[h1+1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop - 1);
                    
      }
      else
      {
         SetMouseRange((int) ((float) (TargetMesh->VertList[v1-1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft + 1,
                     (int) ((float) (TargetMesh->VertList[v1+1][v2]  * (float) (TargetImage->WindowRight - TargetImage->WindowLeft))) + TargetImage->WindowLeft - 1,
                     (int) ((float) (TargetMesh->HorizList[h1-1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop + 1,
                     (int) ((float) (TargetMesh->HorizList[h1+1][h2] * (float) (TargetImage->WindowBottom - TargetImage->WindowTop))) + TargetImage->WindowTop - 1);
      }

      while( Left )
      {
         ReadMouse( &MouseX, &MouseY, &Left, &Right, 0 );

         if (OldMouseX != MouseX || OldMouseY != MouseY)
         {
            if (OldMouseX && OldMouseY)
               Circle( OldMouseX, OldMouseY );

            OldMouseX = MouseX;
            OldMouseY = MouseY;
            Circle( MouseX, MouseY );

            DrawImageInfo( &TargetInfoWindow, MouseX, MouseY, TargetImage, "Target" );
         }
      }

      // ERASE THE CIRCLE
      Circle( OldMouseX, OldMouseY );

      // SET THE NEW COORDS
      TargetMesh->VertList[v1][v2] = (float) (MouseX - TargetImage->WindowLeft) / (float) (TargetImage->WindowRight - TargetImage->WindowLeft);
      TargetMesh->HorizList[h1][h2] = (float) (MouseY - TargetImage->WindowTop) / (float) (TargetImage->WindowBottom - TargetImage->WindowTop);

      // RESET THE MOUSE RANGE
      SetMouseRange( 0, GI->MaxX, 0, GI->MaxY );

      // NEED TO RESET THESE SO NEXT TIME IN IT KNOWS TO DRAW FIRST CIRCLE
      OldMouseX = 0;
      OldMouseY = 0;

      RestoreImage( TargetImage );
      DrawMesh(TargetMesh, TargetImage);
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [MOVEMESH.C  ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
