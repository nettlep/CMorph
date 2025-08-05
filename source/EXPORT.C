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
  ±   [EXPORT.C    ] - Routines to export meshes                             ±
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

void  ExportMesh( )
{
int   RetCode;

   HideCursor();

   RetCode = WriteOldMesh( MeshFile, SourceMesh, TargetMesh );

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

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   WriteMesh( char *FileName, MESH SourceMesh, MESH TargetMesh )
{
int   i;
FILE  *FilePointer;

   if (!SourceMesh)
      return( E_INVALIDMESH );

   if (!TargetMesh)
      return( E_INVALIDMESH );

   FilePointer = fopen( FileName, "w+b" );

   if (FilePointer)
   {
   // WRITE THE HEADER
      fwrite( FILE_HEADER, strlen(FILE_HEADER), 1, FilePointer );
      fputc( 0x1a, FilePointer );

   // WRITE THE VERSION
      WriteWord( VERSION_MS,           FilePointer );
      WriteWord( VERSION_LS,           FilePointer );

   // WRITE THE SOURCE MESH SIZE
      WriteWord( SourceMesh->HorizRes, FilePointer );
      WriteWord( SourceMesh->VertRes,  FilePointer );

   // WRITE THE TARGET MESH SIZE
      WriteWord( TargetMesh->HorizRes, FilePointer );
      WriteWord( TargetMesh->VertRes,  FilePointer );

   // WRITE THE SPLINE PREFERENCES
      WriteWord( MeshType,             FilePointer );
      WriteWord( WarpTightness,        FilePointer );
      WriteWord( InterpMeshType,       FilePointer );
      WriteWord( InterpTightness,      FilePointer );

   // WRITE THE SCREEN PREFERENCES
      WriteWord( CurvesOn,             FilePointer );
      WriteWord( ZoomOn,               FilePointer );
      WriteWord( FastScreen,           FilePointer );
      WriteWord( QuietMode,            FilePointer );
      WriteWord( CursColor,            FilePointer );
      WriteWord( MeshColor,            FilePointer );

   // WRITE THE OUTPUT PREFERENCES
      WriteWord( OutputFileType,       FilePointer );
      WriteWord( TGAUpsideDownFlag,    FilePointer );

   // WRITE THE OPERATION PREFERENCES
      WriteWord( MorphWarpFlag,        FilePointer );
      WriteWord( FastMorphMode,        FilePointer );
      WriteWord( Fade50,               FilePointer );
      WriteWord( FStart,               FilePointer );
      WriteWord( FEnd,                 FilePointer );
      WriteWord( Warp50,               FilePointer );
      WriteWord( WStart,               FilePointer );
      WriteWord( WEnd,                 FilePointer );
      WriteWord( ShowBetweensEnable,   FilePointer );

   // WRITE THE FRAMES PREFERENCES
      WriteWord( TotalFrames,          FilePointer );
      WriteWord( StartFrame,           FilePointer );
      WriteWord( EndFrame,             FilePointer );
      WriteWord( FrameOffset,          FilePointer );

   // WRITE THE FILE PREFERENCES
      WriteWord( ArithType,            FilePointer );
      WriteWord( OverlayThresholdL,    FilePointer );
      WriteWord( OverlayThresholdH,    FilePointer );
      WriteWord( Background,           FilePointer );
      WriteWord( BackThresholdL,       FilePointer );
      WriteWord( BackThresholdH,       FilePointer );

   // WRITE THE PREVIEW PREFERENCES
      WriteWord( Prev50,               FilePointer );

   // WRITE THE FILENAME PREFERENCES
      fwrite(SourceImageFile, 1, 80, FilePointer);
      fwrite(TargetImageFile, 1, 80, FilePointer);
      fwrite(OutBaseName,     1, 80, FilePointer);
      fwrite(OutArithFile,    1, 80, FilePointer);
      fwrite(BackgroundFile,  1, 80, FilePointer);
      fwrite(SourceFiltName,  1, 80, FilePointer);
      fwrite(TargetFiltName,  1, 80, FilePointer);
      fwrite(FilterFileName,  1, 80, FilePointer);

   // SAVE ROOM FOR FUTURE MODIFICATIONS...
      for (i = 0; i < REMAINING_RFU; i++)
         WriteWord( 0, FilePointer );

   // READ THE PREVIEW MONO/COLOR FLAG
      WriteWord( PrevColor, FilePointer );

   // WRITE THE FLIC MONO/COLOR FLAG
      WriteWord( FlicColorMono, FilePointer );

   // WRITE THE FLIC ON/OFF FLAG
      WriteWord( FlicOnOff, FilePointer );

   // WRITE THE FLIC TYPE
      WriteWord( FlicType, FilePointer );

   // WRITE THE FLIC PING-PONG FLAG
      WriteWord( FlicAutoRew, FilePointer );

   // WRITE THE FLIC SPEED
      WriteWord( FlicSpeed, FilePointer );

   // WRITE THE FLIC nAME
      fwrite(FlicFileName, 1, 80,    FilePointer);

      for( i = 0; i < SourceMesh->HorizRes; i++ )
         if (fwrite(SourceMesh->HorizList[i], sizeof(float), SourceMesh->VertRes, FilePointer ) != SourceMesh->VertRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      for( i = 0; i < SourceMesh->VertRes; i++ )
         if (fwrite(SourceMesh->VertList[i], sizeof(float), SourceMesh->HorizRes, FilePointer ) != SourceMesh->HorizRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      for( i = 0; i < TargetMesh->HorizRes; i++ )
         if (fwrite(TargetMesh->HorizList[i], sizeof(float), TargetMesh->VertRes, FilePointer ) != TargetMesh->VertRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      for( i = 0; i < TargetMesh->VertRes; i++ )
         if (fwrite(TargetMesh->VertList[i], sizeof(float), TargetMesh->HorizRes, FilePointer ) != TargetMesh->HorizRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      fclose(FilePointer);
   }
   else
   {
      return( E_NOOPEN );
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   WriteOldMesh( char *FileName, MESH SourceMesh, MESH TargetMesh )
{
int   i;
FILE  *FilePointer;

   if (!SourceMesh)
      return( E_INVALIDMESH );

   if (!TargetMesh)
      return( E_INVALIDMESH );

   FilePointer = fopen( FileName, "w+b" );

   if (FilePointer)
   {
      fwrite( FILE_HEADER, strlen(FILE_HEADER), 1, FilePointer );
      fputc( '\0', FilePointer );
      fputc( 0x1a, FilePointer );

      WriteWord( SourceMesh->HorizRes, FilePointer );
      WriteWord( SourceMesh->VertRes,  FilePointer );

      WriteWord( TargetMesh->HorizRes, FilePointer );
      WriteWord( TargetMesh->VertRes,  FilePointer );

      WriteWord( ZoomOn,               FilePointer );
      WriteWord( TotalFrames,          FilePointer );
      WriteWord( StartFrame,           FilePointer );
      WriteWord( EndFrame,             FilePointer );
      WriteWord( WarpTightness,        FilePointer );
      WriteWord( Fade50,               FilePointer );
      WriteWord( Warp50,               FilePointer );
      WriteWord( OutputFileType,       FilePointer );
      WriteWord( MeshType,             FilePointer );
      WriteWord( MorphWarpFlag,        FilePointer );
      WriteWord( CurvesOn,             FilePointer );
      WriteWord( InterpMeshType,       FilePointer );
      WriteWord( TGAUpsideDownFlag,    FilePointer );
      WriteWord( InterpTightness,      FilePointer );
      WriteWord( ShowBetweensEnable,   FilePointer );
      WriteWord( FastMorphMode,        FilePointer );
      WriteWord( QuietMode,            FilePointer );
      WriteWord( FastScreen,           FilePointer );

      // Save room for future modifications...
      for (i = 0; i < 1019; i++)
         WriteWord( 0, FilePointer );

      fwrite(OutBaseName, 1, 80, FilePointer);

      for( i = 0; i < SourceMesh->HorizRes; i++ )
         if (fwrite(SourceMesh->HorizList[i], sizeof(float), SourceMesh->VertRes, FilePointer ) != SourceMesh->VertRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      for( i = 0; i < SourceMesh->VertRes; i++ )
         if (fwrite(SourceMesh->VertList[i], sizeof(float), SourceMesh->HorizRes, FilePointer ) != SourceMesh->HorizRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      for( i = 0; i < TargetMesh->HorizRes; i++ )
         if (fwrite(TargetMesh->HorizList[i], sizeof(float), TargetMesh->VertRes, FilePointer ) != TargetMesh->VertRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      for( i = 0; i < TargetMesh->VertRes; i++ )
         if (fwrite(TargetMesh->VertList[i], sizeof(float), TargetMesh->HorizRes, FilePointer ) != TargetMesh->HorizRes)
         {
            fclose( FilePointer );
            return( E_NOWRITE );
         }

      fclose(FilePointer);
   }
   else
   {
      return( E_NOOPEN );
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [EXPORT.C    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
