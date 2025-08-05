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
  ±   [ADDMESH.C   ] - Routines to add mesh lins                             ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  AddMesh( int MouseX, int MouseY )
{
   // ADDING HORIZONTAL MESH LINE TO SOURCE IMAGE?
   if (MouseX >= SourceImage->WindowBorderLeft && 
      MouseX <= SourceImage->WindowLeft &&
      MouseY >= SourceImage->WindowTop && 
      MouseY <= SourceImage->WindowBottom)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddHorizPointToMesh( MouseY, LEFT, SourceMesh, TargetMesh, SourceImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }
   else if (MouseX <= SourceImage->WindowBorderRight && 
            MouseX >= SourceImage->WindowRight &&
            MouseY >= SourceImage->WindowTop && 
            MouseY <= SourceImage->WindowBottom)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddHorizPointToMesh( MouseY, RIGHT, SourceMesh, TargetMesh, SourceImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }

   //----------------------------------------------
   // ADDING HORIZONTAL MESH LINE TO TARGET IMAGE?
   //----------------------------------------------

   else if (MouseX >= TargetImage->WindowBorderLeft && 
            MouseX <= TargetImage->WindowLeft &&
            MouseY >= TargetImage->WindowTop && 
            MouseY <= TargetImage->WindowBottom)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddHorizPointToMesh( MouseY, LEFT, TargetMesh, SourceMesh, TargetImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }
   else if (MouseX <= TargetImage->WindowBorderRight && 
            MouseX >= TargetImage->WindowRight &&
            MouseY >= TargetImage->WindowTop && 
            MouseY <= TargetImage->WindowBottom)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddHorizPointToMesh( MouseY, RIGHT, TargetMesh, SourceMesh, TargetImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }

   //--------------------------------------------
   // ADDING VERTICAL MESH LINE TO SOURCE IMAGE?
   //--------------------------------------------

   else if (MouseY >= SourceImage->WindowBorderTop &&
            MouseY <= SourceImage->WindowTop &&
            MouseX >= SourceImage->WindowLeft &&
            MouseX <= SourceImage->WindowRight)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddVertPointToMesh( MouseX, TOP, SourceMesh, TargetMesh, SourceImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }
   else if (MouseY <= SourceImage->WindowBorderBottom && 
            MouseY >= SourceImage->WindowBottom &&
            MouseX >= SourceImage->WindowLeft && 
            MouseX <= SourceImage->WindowRight)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddVertPointToMesh( MouseX, BOTTOM, SourceMesh, TargetMesh, SourceImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }

   //--------------------------------------------
   // ADDING VERTICAL MESH LINE TO TARGET IMAGE?
   //--------------------------------------------

   else if (MouseY >= TargetImage->WindowBorderTop && 
            MouseY <= TargetImage->WindowTop &&
            MouseX >= TargetImage->WindowLeft && 
            MouseX <= TargetImage->WindowRight)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddVertPointToMesh( MouseX, TOP, TargetMesh, SourceMesh, TargetImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }
   else if (MouseY <= TargetImage->WindowBorderBottom && 
            MouseY >= TargetImage->WindowBottom &&
            MouseX >= TargetImage->WindowLeft && 
            MouseX <= TargetImage->WindowRight)
   {
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      AddVertPointToMesh( MouseX, BOTTOM, TargetMesh, SourceMesh, TargetImage );
      DrawMesh(SourceMesh, SourceImage);
      DrawMesh(TargetMesh, TargetImage);
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  AddHorizPointToMesh( int Y, int Side, MESH Mesh, MESH OtherMesh, IMAGE Image )
{
int   i, HorizPlace = MESH_SIZE + 1;
char  TempStr[80];
float FloatY;

   if (Side == LEFT)
      Side = 0;
   else
      Side = Mesh->VertRes-1;

   if (Mesh->HorizRes >= MESH_SIZE || OtherMesh->HorizRes >= MESH_SIZE)
   {
      sprintf( TempStr, "No room for mesh line, max is:  %d", MESH_SIZE );
      WarningBox( TempStr );
      return;
   }

// MAKE THE Y COORDINATE RELATIVE THE WINDOW
   Y -= Image->WindowTop;
   FloatY = (float) ((float) Y / (float) ((float) Image->WindowBottom - (float) Image->WindowTop));

// FIND WHERE THE NEW MESH LINE GOES
   for(i = 0; i < Mesh->HorizRes; i++ )
   {
      if (Mesh->HorizList[i][Side] == FloatY)
      {
         WarningBox( "Cannot add mesh line, already exists" );
         return;
      }
      else if (Mesh->HorizList[i][Side] > FloatY)
      {
         HorizPlace = i;
         break;
      }
   }

   if (HorizPlace == MESH_SIZE+1)
   {
      WarningBox( "Unable to add mesh line" );
      return;
   }

// MAKE ROOM FOR THE NEW MESH LINE (IN THE HORIZONTAL MESH LIST)
   memmove( Mesh->HorizList[HorizPlace+1], Mesh->HorizList[HorizPlace],
            MESH_SIZE * sizeof(float) * (MESH_SIZE-HorizPlace-1) );

// MAKE ROOM FOR THE NEW MESH LINE (IN THE VERTICAL MESH LIST)
   for (i = 0; i < Mesh->VertRes; i++)
   {
      memmove( &Mesh->VertList[i][HorizPlace+1], &Mesh->VertList[i][HorizPlace],
                sizeof(float) * (MESH_SIZE-HorizPlace-1) );
   }

// ADD THE NEW MESH LINE TO THE RESOLUTION
   Mesh->HorizRes++;

   for (i = 0; i < Mesh->VertRes; i++)
   {

// ADD THE NEW MESH LINE TO THE HORIZONTAL LIST
      // DOES THE LINE NEED TO BE BENT?
      if (!i)
      {
         Mesh->HorizList[HorizPlace][i] = FloatY;
      }
      else
      {
      // LINE NEEDS TO BE BENT, SO PROPORTIONALLY PLACE THE CURRENT POINT
      // ACCORDING TO WHERE THE PRIVIOUS ONE WAS
         Mesh->HorizList[HorizPlace][i] = Mesh->HorizList[HorizPlace-1][i] +
                                          (
                                           (
                                            Mesh->HorizList[HorizPlace+0][i-1]-
                                            Mesh->HorizList[HorizPlace-1][i-1]
                                           ) /
                                           (
                                            Mesh->HorizList[HorizPlace+1][i-1]-
                                            Mesh->HorizList[HorizPlace-1][i-1]
                                           ) *
                                           (
                                            Mesh->HorizList[HorizPlace+1][i]-
                                            Mesh->HorizList[HorizPlace-1][i]
                                           )
                                          );
      }

// ADD THE NEW MESH LINE TO THE VERTICAL LIST
      Mesh->VertList[i][HorizPlace] = Mesh->VertList[i][HorizPlace+1] -
                                      (
                                       (
                                        Mesh->VertList[i][HorizPlace+1]-
                                        Mesh->VertList[i][HorizPlace-1]
                                       ) /
                                       (
                                        Mesh->HorizList[HorizPlace+1][i]-
                                        Mesh->HorizList[HorizPlace-1][i]
                                       ) *
                                       (
                                        Mesh->HorizList[HorizPlace+1][i]-
                                        Mesh->HorizList[HorizPlace+0][i]
                                       )
                                      );
   }

// ----------------------------------------------------------------------------
// NOW IT'S TIME TO DO IT ALL OVER AGAIN FOR THE "OTHERMESH"
// ----------------------------------------------------------------------------

// MAKE ROOM FOR THE NEW MESH LINE (IN THE HORIZONTAL MESH LIST)
   memmove( OtherMesh->HorizList[HorizPlace+1], OtherMesh->HorizList[HorizPlace],
            MESH_SIZE * sizeof(float) * (MESH_SIZE-HorizPlace-1) );

// MAKE ROOM FOR THE NEW MESH LINE (IN THE VERTICAL MESH LIST)
   for (i = 0; i < OtherMesh->VertRes; i++)
   {
      memmove( &OtherMesh->VertList[i][HorizPlace+1], &OtherMesh->VertList[i][HorizPlace],
                sizeof(float) * (MESH_SIZE-HorizPlace-1) );
   }

// ADD THE NEW MESH LINE TO THE RESOLUTION
   OtherMesh->HorizRes++;

   if (FloatY <= OtherMesh->HorizList[HorizPlace-1][0] ||
       FloatY >= OtherMesh->HorizList[HorizPlace+1][0])
   {   
      FloatY = 
            OtherMesh->HorizList[HorizPlace-1][0] +
            (
             (
              Mesh->HorizList[HorizPlace+0][0]-
              Mesh->HorizList[HorizPlace-1][0]
             ) /
             (
              Mesh->HorizList[HorizPlace+1][0]-
              Mesh->HorizList[HorizPlace-1][0]
             ) *
             (
              OtherMesh->HorizList[HorizPlace+1][0]-
              OtherMesh->HorizList[HorizPlace-1][0]
             )
            );
   }

   for (i = 0; i < OtherMesh->VertRes; i++)
   {

// ADD THE NEW MESH LINE TO THE HORIZONTAL LIST
      // DOES THE LINE NEED TO BE BENT?
      if (!i)
      {
         OtherMesh->HorizList[HorizPlace][i] = FloatY;
      }
      else
      {
      // LINE NEEDS TO BE BENT, SO PROPORTIONALLY PLACE THE CURRENT POINT
      // ACCORDING TO WHERE THE PRIVIOUS ONE WAS
         OtherMesh->HorizList[HorizPlace][i] =
            OtherMesh->HorizList[HorizPlace-1][i] +
            (
             (
              OtherMesh->HorizList[HorizPlace+0][i-1]-
              OtherMesh->HorizList[HorizPlace-1][i-1]
             ) /
             (
              OtherMesh->HorizList[HorizPlace+1][i-1]-
              OtherMesh->HorizList[HorizPlace-1][i-1]
             ) *
             (
              OtherMesh->HorizList[HorizPlace+1][i]-
              OtherMesh->HorizList[HorizPlace-1][i]
             )
            );
      }

// ADD THE NEW MESH LINE TO THE VERTICAL LIST
      OtherMesh->VertList[i][HorizPlace] =
         OtherMesh->VertList[i][HorizPlace+1] -
         (
          (
           OtherMesh->VertList[i][HorizPlace+1]-
           OtherMesh->VertList[i][HorizPlace-1]
          ) /
          (
           OtherMesh->HorizList[HorizPlace+1][i]-
           OtherMesh->HorizList[HorizPlace-1][i]
          ) *
          (
           OtherMesh->HorizList[HorizPlace+1][i]-
           OtherMesh->HorizList[HorizPlace+0][i]
          )
         );
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  AddVertPointToMesh( int X, int Side, MESH Mesh, MESH OtherMesh, IMAGE Image )
{
int   i, VertPlace = MESH_SIZE + 1;
char  TempStr[80];
float FloatX;

   if (Side == TOP)
      Side = 0;
   else
      Side = Mesh->HorizRes-1;

   if (Mesh->VertRes >= MESH_SIZE || OtherMesh->VertRes >= MESH_SIZE)
   {
      sprintf( TempStr, "No room for mesh line, max is:  %d", MESH_SIZE );
      WarningBox( TempStr );
      return;
   }

// MAKE THE X COORDINATE RELATIVE THE WINDOW
   X -= Image->WindowLeft;
   FloatX = (float) ((float) X / (float) ((float) Image->WindowRight - (float) Image->WindowLeft));

// FIND WHERE THE NEW MESH LINE GOES
   for(i = 0; i < Mesh->VertRes; i++ )
   {
      if (Mesh->VertList[i][Side] == FloatX)
      {
         WarningBox( "Unable to add mesh line, already exists" );
         return;
      }
      else if (Mesh->VertList[i][Side] > FloatX)
      {
         VertPlace = i;
         break;
      }
   }

   if (VertPlace == MESH_SIZE+1)
   {
      WarningBox( "Unable to add mesh line" );
      return;
   }

// MAKE ROOM FOR THE NEW MESH LINE (IN THE HORIZONTAL MESH LIST)
   memmove( Mesh->VertList[VertPlace+1], Mesh->VertList[VertPlace],
            MESH_SIZE * sizeof(float) * (MESH_SIZE-VertPlace-1) );

// MAKE ROOM FOR THE NEW MESH LINE (IN THE VERTICAL MESH LIST)
   for (i = 0; i < Mesh->HorizRes; i++)
   {
      memmove( &Mesh->HorizList[i][VertPlace+1], &Mesh->HorizList[i][VertPlace],
                sizeof(float) * (MESH_SIZE-VertPlace-1) );
   }

// ADD THE NEW MESH LINE TO THE RESOLUTION
   Mesh->VertRes++;

   for (i = 0; i < Mesh->HorizRes; i++)
   {

// ADD THE NEW MESH LINE TO THE HORIZONTAL LIST
      // DOES THE LINE NEED TO BE BENT?
      if (!i)
      {
         Mesh->VertList[VertPlace][i] = FloatX;
      }
     else
      {
      // LINE NEEDS TO BE BENT, SO PROPORTIONALLY PLACE THE CURRENT POINT
      // ACCORDING TO WHERE THE PRIVIOUS ONE WAS
         Mesh->VertList[VertPlace][i] = Mesh->VertList[VertPlace-1][i] +
                                        (
                                         (
                                          Mesh->VertList[VertPlace+0][i-1]-
                                          Mesh->VertList[VertPlace-1][i-1]
                                         ) /
                                         (
                                          Mesh->VertList[VertPlace+1][i-1]-
                                          Mesh->VertList[VertPlace-1][i-1]
                                         ) *
                                         (
                                          Mesh->VertList[VertPlace+1][i]-
                                          Mesh->VertList[VertPlace-1][i]
                                         )
                                        );
      }

// ADD THE NEW MESH LINE TO THE VERTICAL LIST
      Mesh->HorizList[i][VertPlace] = Mesh->HorizList[i][VertPlace+1] -
                                      (
                                       (
                                        Mesh->HorizList[i][VertPlace+1]-
                                        Mesh->HorizList[i][VertPlace-1]
                                       ) /
                                       (
                                        Mesh->VertList[VertPlace+1][i]-
                                        Mesh->VertList[VertPlace-1][i]
                                       ) *
                                       (
                                        Mesh->VertList[VertPlace+1][i]-
                                        Mesh->VertList[VertPlace+0][i]
                                       )
                                      );
   }

// ----------------------------------------------------------------------------
// NOW IT'S TIME TO DO IT ALL OVER AGAIN FOR THE "OTHERMESH"
// ----------------------------------------------------------------------------

// MAKE ROOM FOR THE NEW MESH LINE (IN THE HORIZONTAL MESH LIST)
   memmove( OtherMesh->VertList[VertPlace+1], OtherMesh->VertList[VertPlace],
            MESH_SIZE * sizeof(float) * (MESH_SIZE-VertPlace-1) );

// MAKE ROOM FOR THE NEW MESH LINE (IN THE VERTICAL MESH LIST)
   for (i = 0; i < OtherMesh->HorizRes; i++)
   {
      memmove( &OtherMesh->HorizList[i][VertPlace+1], &OtherMesh->HorizList[i][VertPlace],
                sizeof(float) * (MESH_SIZE-VertPlace-1) );
   }

// ADD THE NEW MESH LINE TO THE RESOLUTION
   OtherMesh->VertRes++;

   if (FloatX <= OtherMesh->VertList[VertPlace-1][0] ||
       FloatX >= OtherMesh->VertList[VertPlace+1][0])
   {   
      FloatX = 
            OtherMesh->VertList[VertPlace-1][0] +
            (
             (
              Mesh->VertList[VertPlace+0][0]-
              Mesh->VertList[VertPlace-1][0]
             ) /
             (
              Mesh->VertList[VertPlace+1][0]-
              Mesh->VertList[VertPlace-1][0]
             ) *
             (
              OtherMesh->VertList[VertPlace+1][0]-
              OtherMesh->VertList[VertPlace-1][0]
             )
            );
   }

   for (i = 0; i < OtherMesh->HorizRes; i++)
   {

// ADD THE NEW MESH LINE TO THE HORIZONTAL LIST
      // DOES THE LINE NEED TO BE BENT?
      if (!i)
      {
         OtherMesh->VertList[VertPlace][i] = FloatX;
      }
      else
      {
      // LINE NEEDS TO BE BENT, SO PROPORTIONALLY PLACE THE CURRENT POINT
      // ACCORDING TO WHERE THE PRIVIOUS ONE WAS
         OtherMesh->VertList[VertPlace][i] =
            OtherMesh->VertList[VertPlace-1][i] +
            (
             (
              OtherMesh->VertList[VertPlace+0][i-1]-
              OtherMesh->VertList[VertPlace-1][i-1]
             ) /
             (
              OtherMesh->VertList[VertPlace+1][i-1]-
              OtherMesh->VertList[VertPlace-1][i-1]
             ) *
             (
              OtherMesh->VertList[VertPlace+1][i]-
              OtherMesh->VertList[VertPlace-1][i]
             )
            );
      }

// ADD THE NEW MESH LINE TO THE VERTICAL LIST
      OtherMesh->HorizList[i][VertPlace] =
         OtherMesh->HorizList[i][VertPlace+1] -
         (
          (
           OtherMesh->HorizList[i][VertPlace+1]-
           OtherMesh->HorizList[i][VertPlace-1]
          ) /
          (
           OtherMesh->VertList[VertPlace+1][i]-
           OtherMesh->VertList[VertPlace-1][i]
          ) *
          (
           OtherMesh->VertList[VertPlace+1][i]-
           OtherMesh->VertList[VertPlace+0][i]
          )
         );
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [ADDMESH.C   ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

