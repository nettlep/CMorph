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
  ±   [WARP.C  ] - Warping routines                                          ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmorph.h"

//#define Debug

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   WarpX(IMAGE SourceImage, MESH SourceMesh, IMAGE TargetImage, MESH TargetMesh)
{
int	   i, j, off, RetCode;
int      IBiggest, MBiggest;
int      ImageWidth, ImageHeight, MeshWidth, MeshHeight;
UCHAR	   *src, *dst;
float    *HorizRow, *VertCol, *Index, *SplinePoints;
static   float *TempMeshSrc[MESH_SIZE], *TempMeshDst[MESH_SIZE];

   ImageWidth = SourceImage->ResX;
   ImageHeight = SourceImage->ResY;
   MeshWidth = SourceMesh->VertRes;
   MeshHeight = SourceMesh->HorizRes;

// ----------- ALLOCATE ENOUGH MEMORY FOR SCANLINE ALONG LONGEST AXIS ----------

   IBiggest = MAX(ImageWidth, ImageHeight);
   MBiggest = MAX(MeshWidth, MeshHeight);

   Index = malloc( IBiggest * sizeof(float));
   if (!Index)
      return( E_NOMEMMSH );

   HorizRow = malloc( IBiggest * sizeof(float));
   if (!HorizRow)
   {
      free(Index);
      return( E_NOMEMMSH );
   }

   VertCol = malloc( IBiggest * sizeof(float));
   if (!VertCol)
   {
      free(Index);
      free(HorizRow);
      return( E_NOMEMMSH );
   }

   SplinePoints = malloc( IBiggest * sizeof(float));
   if (!SplinePoints)
   {
      free(Index);
      free(HorizRow);
      free(VertCol);
      return( E_NOMEMMSH );
   }

// ---- ALLOCATE ENOUGH MEMORY FOR SCANLINE MESHES (ALSO ALONG LONGEST AXIS) ---
   
   for (i = 0; i < MBiggest; i++)
   {
      TempMeshSrc[i] = malloc( IBiggest * sizeof(float) );
      if (!TempMeshSrc[i])
      {
         free(Index);
         free(HorizRow);
         free(VertCol);
         free(SplinePoints);
         return( E_NOMEMMSH );
      }

      TempMeshDst[i] = malloc( IBiggest * sizeof(float) );
      if (!TempMeshDst[i])
      {
         free(Index);
         free(HorizRow);
         free(VertCol);
         free(SplinePoints);
         return( E_NOMEMMSH );
      }
   }

// ---------- GET THE VERTICAL SPLINE VALUES --------------

   for( i = 0; i < ImageHeight; i++) Index[i] = i;

   for( i = 0; i < MeshWidth; i++)
   {
      for( j = 0; j < MeshHeight; j++ )
      {
         VertCol[j]  = SourceMesh->HorizList[j][i] * ImageHeight;
         HorizRow[j] = SourceMesh->VertList[i][j] * ImageWidth;
      }

      RetCode = Splines(VertCol, HorizRow, MeshHeight, Index, SplinePoints, ImageHeight, SP_WARP);

      if (RetCode != E_NONE )
      {
         for (i = 0; i < MBiggest; i++)
         {
            free( TempMeshSrc[i] );
            free( TempMeshDst[i] );
         }

         free((char *) Index);
         free((char *) HorizRow);
         free((char *) VertCol);
         free((char *) SplinePoints);
         return( RetCode );
      }

      for( j = 0; j < ImageHeight; j++)
         TempMeshSrc[i][j] = SplinePoints[j];

      // --------------------------------------------------

      for( j = 0; j < MeshHeight; j++ )
      {
         VertCol[j]  = TargetMesh->HorizList[j][i] * ImageHeight;
         HorizRow[j] = TargetMesh->VertList[i][j] * ImageWidth;
      }

      RetCode = Splines(VertCol, HorizRow, MeshHeight, Index, SplinePoints, ImageHeight, SP_WARP);

      if (RetCode != E_NONE )
      {
         for (i = 0; i < MBiggest; i++)
         {
            free( TempMeshSrc[i] );
            free( TempMeshDst[i] );
         }

         free((char *) Index);
         free((char *) HorizRow);
         free((char *) VertCol);
         free((char *) SplinePoints);
         return( RetCode );
      }

      for( j = 0; j < ImageHeight; j++)
         TempMeshDst[i][j] = SplinePoints[j];
   }

// ------- NOW DO THE WARPING IN THE X DIRECTION ----------

   for( i = 0; i < ImageWidth; i++) Index[i] = i;

   src = SourceImage->Buffer;
   dst = TargetImage->Buffer;

   for( i = 0; i < ImageHeight; i++ )
   {
      for( j = 0; j < MeshWidth; j++ )
      {
         HorizRow[j] = TempMeshSrc[j][i];
         VertCol[j] = TempMeshDst[j][i];
      }

      off = i * ImageWidth * 3;

      RetCode = Splines(HorizRow, VertCol, MeshWidth, Index, SplinePoints, ImageWidth, SP_INTRP);

      if (RetCode != E_NONE )
      {
         for (i = 0; i < MBiggest; i++)
         {
            free( TempMeshSrc[i] );
            free( TempMeshDst[i] );
         }

         free((char *) Index);
         free((char *) HorizRow);
         free((char *) VertCol);
         free((char *) SplinePoints);
         return( RetCode );
      }

      resample(&src[off+0], &dst[off+0], ImageWidth, 3, SplinePoints);
   }

// --------------------- NOW RELEASE ALL ALLOCATED MEMORY ----------------------

   for (i = 0; i < MBiggest; i++)
   {
      free( TempMeshSrc[i] );
      free( TempMeshDst[i] );
   }

   free((char *) Index);
   free((char *) HorizRow);
   free((char *) VertCol);
   free((char *) SplinePoints);

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   WarpY(IMAGE SourceImage, MESH SourceMesh, IMAGE TargetImage, MESH TargetMesh)
{
int	   i, j, off, RetCode;
int      IBiggest, MBiggest;
int      ImageWidth, ImageHeight, MeshWidth, MeshHeight;
UCHAR	   *src, *dst;
float    *HorizRow, *VertCol, *Index, *SplinePoints;
static   float *TempMeshSrc[MESH_SIZE], *TempMeshDst[MESH_SIZE];

#ifdef Debug
   memcpy( TargetImage, SourceImage, sizeof(_IMAGE) );
   memcpy( TargetImage->Buffer, SourceImage->Buffer, SourceImage->ResX * SourceImage->ResY * 3 );
   return;
#endif

   ImageWidth = SourceImage->ResX;
   ImageHeight = SourceImage->ResY;
   MeshWidth = SourceMesh->VertRes;
   MeshHeight = SourceMesh->HorizRes;

// ----------- ALLOCATE ENOUGH MEMORY FOR SCANLINE ALONG LONGEST AXIS ----------

   IBiggest = MAX(ImageWidth, ImageHeight);
   MBiggest = MAX(MeshWidth, MeshHeight);

   Index = malloc( IBiggest * sizeof(float));
   if (!Index)
      return( E_NOMEMMSH );

   HorizRow = malloc( IBiggest * sizeof(float));
   if (!HorizRow)
   {
      free(Index);
      return( E_NOMEMMSH );
   }

   VertCol = malloc( IBiggest * sizeof(float));
   if (!VertCol)
   {
      free(Index);
      free(HorizRow);
      return( E_NOMEMMSH );
   }

   SplinePoints = malloc( IBiggest * sizeof(float));
   if (!SplinePoints)
   {
      free(Index);
      free(HorizRow);
      free(VertCol);
      return( E_NOMEMMSH );
   }

// ---- ALLOCATE ENOUGH MEMORY FOR SCANLINE MESHES (ALSO ALONG LONGEST AXIS) ---
   
   for (i = 0; i < MBiggest; i++)
   {
      TempMeshSrc[i] = malloc( IBiggest * sizeof(float) );
      if (!TempMeshSrc[i])
      {
         free(Index);
         free(HorizRow);
         free(VertCol);
         return( E_NOMEMMSH );
      }

      TempMeshDst[i] = malloc( IBiggest * sizeof(float) );
      if (!TempMeshDst[i])
      {
         free(Index);
         free(HorizRow);
         free(VertCol);
         return( E_NOMEMMSH );
      }
   }

// ---------- GET THE HORIZONTAL SPLINE VALUES --------------

   for( i = 0; i < ImageWidth; i++) Index[i] = i;

   for( i = 0; i < MeshHeight; i++)
   {
      for( j = 0; j < MeshWidth; j++ )
      {
         HorizRow[j] = SourceMesh->VertList[j][i] * ImageWidth;
         VertCol[j]  = SourceMesh->HorizList[i][j] * ImageHeight;
      }

      RetCode = Splines(HorizRow, VertCol, MeshWidth, Index, SplinePoints, ImageWidth, SP_WARP);

      if (RetCode != E_NONE )
      {
         for (i = 0; i < MBiggest; i++)
         {
            free( TempMeshSrc[i] );
            free( TempMeshDst[i] );
         }

         free((char *) Index);
         free((char *) HorizRow);
         free((char *) VertCol);
         free((char *) SplinePoints);
         return( RetCode );
      }

      for( j = 0; j < ImageWidth; j++)
         TempMeshSrc[i][j] = SplinePoints[j];

      for( j = 0; j < MeshWidth; j++ )
      {
         HorizRow[j] = TargetMesh->VertList[j][i] * ImageWidth;
         VertCol[j]  = TargetMesh->HorizList[i][j] * ImageHeight;
      }

      RetCode = Splines(HorizRow, VertCol, MeshWidth, Index, SplinePoints, ImageWidth, SP_WARP);

      if (RetCode != E_NONE )
      {
         for (i = 0; i < MBiggest; i++)
         {
            free( TempMeshSrc[i] );
            free( TempMeshDst[i] );
         }

         free((char *) Index);
         free((char *) HorizRow);
         free((char *) VertCol);
         free((char *) SplinePoints);
         return( RetCode );
      }

      for( j = 0; j < ImageWidth; j++)
         TempMeshDst[i][j] = SplinePoints[j];
   }

// ------------------- NOW DO THE WARPING IN THE Y DIRECTION -------------------

   for( i = 0; i < ImageHeight; i++) Index[i] = i;

   src = SourceImage->Buffer;
   dst = TargetImage->Buffer;

   for( i = 0; i < ImageWidth; i++ )
   {
      for( j = 0; j < MeshHeight; j++ )
      {
         HorizRow[j] = TempMeshSrc[j][i];
         VertCol[j] = TempMeshDst[j][i];
      }

      off = i * 3;

      RetCode = Splines(HorizRow, VertCol, MeshHeight, Index, SplinePoints, ImageHeight, SP_INTRP);

      if (RetCode != E_NONE )
      {
         for (i = 0; i < MBiggest; i++)
         {
            free( TempMeshSrc[i] );
            free( TempMeshDst[i] );
         }

         free((char *) Index);
         free((char *) HorizRow);
         free((char *) VertCol);
         free((char *) SplinePoints);
         return( RetCode );
      }

      resample(&src[off+0], &dst[off+0], ImageHeight, ImageWidth * 3, SplinePoints);
   }

// --------------------- NOW RELEASE ALL ALLOCATED MEMORY ----------------------

   for (i = 0; i < MBiggest; i++)
   {
      free( TempMeshSrc[i] );
      free( TempMeshDst[i] );
   }

   free((char *) Index);
   free((char *) HorizRow);
   free((char *) VertCol);
   free((char *) SplinePoints);

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [WARP.C  ] - End Of File                                               ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
