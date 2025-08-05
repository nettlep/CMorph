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
  ±   [MESH.C      ] - Mesh handling routines                                ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dos.h>
#include <conio.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  CreateMesh( MESH *Mesh )
{
   *Mesh = malloc(sizeof(_MESH));

   if (!*Mesh)
      FatalError( "Error creating mesh", E_NOMEMMSH );

   (*Mesh)->VertRes = 2;
   (*Mesh)->HorizRes = 2;

// FILL THE OUTER POINTS INTO THE MESH
   (*Mesh)->HorizList[0][0] = 0.0;
   (*Mesh)->VertList[0][0] = 0.0;

   (*Mesh)->HorizList[0][1] = 0.0;
   (*Mesh)->VertList[1][0] = 1.0;

   (*Mesh)->HorizList[1][0] = 1.0;
   (*Mesh)->VertList[0][1] = 0.0;

   (*Mesh)->HorizList[1][1] = 1.0;
   (*Mesh)->VertList[1][1] = 1.0;

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DestroyMesh( MESH *Mesh )
{
   if (*Mesh)
   {
      free(*Mesh);
      *Mesh = 0;
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawMesh( MESH Mesh, IMAGE Image )
{
int   i, j, RetCode;
int   XOffset, YOffset;
float Width, Height;
float *xrow, *yrow, *indx, *map;

   HideCursor( );

   /* GET LIMITS FOR MESH DRAWING */
   Width  = Image->WindowRight - Image->WindowLeft;
   XOffset = Image->WindowLeft;
   Height = Image->WindowBottom - Image->WindowTop;
   YOffset = Image->WindowTop;

   /* PREPARE GRAPHICS */
   _setcolor( MeshColor );
   _setplotaction(_GXOR);

   /* DRAW THE CURVED MESHES */
   if (CurvesOn)
   {
      indx = malloc(MAX(Width, Height) * sizeof(float));
      if (!indx)
         FatalError( "Unable to draw mesh", E_NOMEMMSH);

      xrow = malloc(MAX(Width, Height) * sizeof(float));
      if (!xrow)
         FatalError( "Unable to draw mesh", E_NOMEMMSH);

      yrow = malloc(MAX(Width, Height) * sizeof(float));
      if (!yrow)
         FatalError( "Unable to draw mesh", E_NOMEMMSH);

      map  = malloc(MAX(Width, Height) * sizeof(float));
      if (!map)
         FatalError( "Unable to draw mesh", E_NOMEMMSH);

      /* DRAW ALL HORIZONTAL MESH LINES */
      for( i = 0; i < Width; i++)
         indx[i] = i;

      for( i = 0; i < Mesh->HorizRes; i++ )
      {
         for( j = 0; j < Mesh->VertRes; j++ )
         {
            xrow[j] = Mesh->VertList[j][i] * Width;
            yrow[j] = Mesh->HorizList[i][j] * Height;
         }

         RetCode = Splines(xrow, yrow, Mesh->VertRes, indx, map, Width, SP_WARP);

         if (RetCode != E_NONE)
         {
            free((char *) indx);
            free((char *) xrow);
            free((char *) yrow);
            free((char *)  map);
            ErrorBox( "Unable to draw mesh", RetCode );
            return;
         }

         for( j = 0; j <= Width; j++)
            _setpixel( XOffset + indx[j], YOffset + map[j]);
      }

      /* DRAW ALL VERTICAL MESH LINES */
      for( i = 0; i < Height; i++)
         indx[i] = i;

      for( i = 0; i < Mesh->VertRes; i++ )
      {
         for( j = 0; j < Mesh->HorizRes; j++ )
         {
            xrow[j] = Mesh->HorizList[j][i] * Height;
            yrow[j] = Mesh->VertList[i][j] * Width;
         }

         RetCode = Splines(xrow, yrow, Mesh->HorizRes, indx, map, Height, SP_WARP);

         if (RetCode != E_NONE)
         {
            free((char *) indx);
            free((char *) xrow);
            free((char *) yrow);
            free((char *)  map);
            ErrorBox( "Unable to draw mesh", RetCode );
            return;
         }

         for( j = 0; j <= Height; j++)
            _setpixel( XOffset + map[j], YOffset + indx[j]);
      }

      /* FREE UP MEMORY */
      free((char *) indx);
      free((char *) xrow);
      free((char *) yrow);
      free((char *)  map);
   }
   else
   {
      /* DRAW ALL HORIZONTAL MESH LINES */
      for (i = 0; i < Mesh->HorizRes; i++)
         for (j = 0; j < Mesh->VertRes-1; j++)
         {
            _moveto( XOffset + (int) (Width  * Mesh->VertList[j][i]),
                     YOffset + (int) (Height * Mesh->HorizList[i][j] ));

            _lineto( XOffset + (int) (Width  * Mesh->VertList[j+1][i]),
                     YOffset + (int) (Height * Mesh->HorizList[i][j+1]));
         }

      /* DRAW ALL VERTICAL MESH LINES */
      for (i = 0; i < Mesh->VertRes; i++)
         for (j = 0; j < Mesh->HorizRes-1; j++)
         {
            _moveto( XOffset + (int) (Width  * Mesh->VertList[i][j]),
                     YOffset + (int) (Height * Mesh->HorizList[j][i]));

            _lineto( XOffset + (int) (Width  * Mesh->VertList[i][j+1]),
                     YOffset + (int) (Height * Mesh->HorizList[j+1][i]));
         }
   }

   /* RESTORE GRAPHICS STATE */
   _setplotaction(_GPSET);
   ShowCursor( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  NearestMeshPoint( MESH Mesh, IMAGE Image, int X, int Y, int *Cx, int *Cy, int *H1, int *H2, int *V1, int *V2 )
{
int   i, j;
float FloatX, FloatY, Dist, ClosestDist = 1.0, ClosestX, ClosestY;

   /* MAKE THE POINTS RELATIVE TO THE WINDOW */
   X -= Image->WindowLeft;
   Y -= Image->WindowTop;

   FloatX = (float)((float) X / ((float)Image->WindowRight - (float)Image->WindowLeft));
   FloatY = (float)((float) Y / ((float)Image->WindowBottom - (float)Image->WindowTop));

   /* DRAW ALL HORIZONTAL MESH LINES */
   for (i = 0; i < Mesh->HorizRes; i++)
      for (j = 0; j < Mesh->VertRes; j++)
      {
         Dist = sqrt(((FloatX - Mesh->VertList[j][i]) * (FloatX - Mesh->VertList[j][i])) +
                     ((FloatY - Mesh->HorizList[i][j]) * (FloatY - Mesh->HorizList[i][j])));

         if (Dist < ClosestDist)
         {
            ClosestDist = Dist;
            ClosestX = Mesh->VertList[j][i];
            ClosestY = Mesh->HorizList[i][j];
            *H1 = i; *H2 = j;
            *V1 = j; *V2 = i;
         }
      }

   /* LOCATE THE MESH POINT */
   *Cx = (int) (ClosestX * (Image->WindowRight - Image->WindowLeft)) + Image->WindowLeft;
   *Cy = (int) (ClosestY * (Image->WindowBottom - Image->WindowTop)) + Image->WindowTop;

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  LoadMesh( )
{
int   Key, RetCode;
MESH  TempSMesh, TempTMesh;

   HideCursor();

   if (!SourceMesh)
      CreateMesh( &SourceMesh );
   if (!TargetMesh)
      CreateMesh( &TargetMesh );

   /* DOES A MESH ALREADY EXIST? */
   if (SourceMesh->HorizRes > 2 || SourceMesh->VertRes > 2 ||
       TargetMesh->HorizRes > 2 || TargetMesh->VertRes > 2 )
   {
      DrawMessBox( "Question", "Mesh already loaded;  Replace current mesh [y/N]?", ERROR_BOX_COLOR );
      Beep( );
      Key = GetKey();
      EraseMessBox( );

      if (toupper(Key) != 'Y')
      {
         WarningBox( "Load aborted" );
         ShowCursor();
         return;
      }
   }

   /* CREATE TEMPORARY MESHES */
   CreateMesh( &TempSMesh );
   CreateMesh( &TempTMesh );

   /* LOAD UP THE TEMPORARY MESHES */
   RetCode = ReadMesh( MeshFile, &TempSMesh, &TempTMesh );

   /* DID THE LOAD WORK OK? */
   if (RetCode == E_NONE)
   {
      /* COPY OVER THE NEW MESHES IF EVERYTHING WORKED */
      DestroyMesh( &SourceMesh );
      DestroyMesh( &TargetMesh );
      SourceMesh = TempSMesh;
      TargetMesh = TempTMesh;

      /* CLEAN UP THE GRAPHICS */
      ShowCursor();
      ChangeMenu( MAINMENU );
      RestoreImage( SourceImage );
      DrawMesh( SourceMesh, SourceImage );
      RestoreImage( TargetImage );
      DrawMesh( TargetMesh, TargetImage );
   }
   else
   {
      ErrorBox( "Mesh load failure", RetCode );
      ShowCursor();
   }

   UpdateFastPref = 1;
   UpdateFastFile = 1;
   UpdateFastProc = 1;
   UpdateFastPrev = 1;

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SaveMesh( )
{
int   RetCode;

   HideCursor();

   RetCode = WriteMesh( MeshFile, SourceMesh, TargetMesh );

   /* DID THE SAVE WORK OK? */
   if (RetCode != E_NONE)
   {
      ErrorBox( "Mesh save failure", RetCode );
   }
   else
   {
      MBox( "Mesh saved ok" );
   }

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [MESH.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
