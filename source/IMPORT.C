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
  ±   [IMPORT.C    ] - Routines for importing mesh files                     ±
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

void  ImportMesh( )
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
   RetCode = ReadOldMesh( MeshFile, &TempSMesh, &TempTMesh );

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

int   ReadMesh( char *FileName, MESH *SourceMesh, MESH *TargetMesh )
{
int   i;
char  Temp[80];
FILE  *FilePointer;

   FilePointer = fopen( FileName, "rb" );

   if (FilePointer)
   {
   // READ THE HEADER
      fread( Temp, strlen(FILE_HEADER), 1, FilePointer );

      Temp[strlen(FILE_HEADER)] = '\0';

      if (strcmp( Temp, FILE_HEADER ))
      {
         fclose( FilePointer );
         return( E_BADMESHFILE );
      }

      if (fgetc( FilePointer ) != 0x1a)
      {
         fclose( FilePointer );
         return( E_BADMESHFILE );
      }

   // IGNORE THE VERSION
      ReadWord( FilePointer );
      ReadWord( FilePointer );

   // READ THE SOURCE MESH SIZE
      (*SourceMesh)->HorizRes = ReadWord( FilePointer );
      (*SourceMesh)->VertRes  = ReadWord( FilePointer );

   // READ THE TARGET MESH SIZE
      (*TargetMesh)->HorizRes = ReadWord( FilePointer );
      (*TargetMesh)->VertRes  = ReadWord( FilePointer );

   // READ THE SPLINE PREFERENCES
      MeshType             = ReadWord( FilePointer ); 
      WarpTightness        = ReadWord( FilePointer );
      InterpMeshType       = ReadWord( FilePointer );
      InterpTightness      = ReadWord( FilePointer );

   // READ THE SCREEN PREFERENCES
      CurvesOn             = ReadWord( FilePointer );
      ZoomOn               = ReadWord( FilePointer );
      FastScreen           = ReadWord( FilePointer );
      QuietMode            = ReadWord( FilePointer );
      CursColor            = ReadWord( FilePointer );
      MeshColor            = ReadWord( FilePointer );

   // READ THE OUTPUT PREFERENCES
      OutputFileType       = ReadWord( FilePointer );
      TGAUpsideDownFlag    = ReadWord( FilePointer );

   // READ THE OPERATION PREFERENCES
      MorphWarpFlag        = ReadWord( FilePointer );
      FastMorphMode        = ReadWord( FilePointer );
      Fade50               = ReadWord( FilePointer );
      FStart               = ReadWord( FilePointer );
      FEnd                 = ReadWord( FilePointer );
      Warp50               = ReadWord( FilePointer );
      WStart               = ReadWord( FilePointer );
      WEnd                 = ReadWord( FilePointer );
      ShowBetweensEnable   = ReadWord( FilePointer );

   // READ THE FRAMES PREFERENCES
      TotalFrames          = ReadWord( FilePointer );
      StartFrame           = ReadWord( FilePointer );
      EndFrame             = ReadWord( FilePointer );
      FrameOffset          = ReadWord( FilePointer );

   // READ THE FILE PREFERENCES
      ArithType            = ReadWord( FilePointer );
      OverlayThresholdL    = ReadWord( FilePointer );
      OverlayThresholdH    = ReadWord( FilePointer );
      Background           = ReadWord( FilePointer );
      BackThresholdL       = ReadWord( FilePointer );
      BackThresholdH       = ReadWord( FilePointer );

   // READ THE PREVIEW PREFERENCES
      Prev50               = ReadWord( FilePointer );

   // READ THE FILENAME PREFERENCES
      fread(SourceImageFile, 1, 80,    FilePointer);
      fread(TargetImageFile, 1, 80,    FilePointer);
      fread(OutBaseName,     1, 80,    FilePointer);
      fread(OutArithFile,    1, 80,    FilePointer);
      fread(BackgroundFile,  1, 80,    FilePointer);
      fread(SourceFiltName,  1, 80,    FilePointer);
      fread(TargetFiltName,  1, 80,    FilePointer);
      fread(FilterFileName,  1, 80,    FilePointer);

   // SAVE ROOM FOR FUTURE MODIFICATIONS...
      for (i = 0; i < REMAINING_RFU; i++)
         ReadWord( FilePointer );

   // READ THE PREVIEW MONO/COLOR FLAG
      PrevColor = ReadWord( FilePointer );

   // READ THE FLIC MONO/COLOR FLAG
      FlicColorMono = ReadWord( FilePointer );

   // READ THE FLIC ON/OFF FLAG
      FlicOnOff     = ReadWord( FilePointer );

   // READ THE FLIC TYPE
      FlicType      = ReadWord( FilePointer );

   // READ THE FLIC PING-PONG FLAG
      FlicAutoRew   = ReadWord( FilePointer );

   // READ THE FLIC SPEED
      FlicSpeed     = ReadWord( FilePointer );

   // READ THE FLIC SPEED
      fread(FlicFileName, 1, 80,    FilePointer);

      if ((*SourceMesh)->HorizRes > MESH_SIZE ||
          (*SourceMesh)->VertRes > MESH_SIZE ||
          (*TargetMesh)->HorizRes > MESH_SIZE ||
          (*TargetMesh)->VertRes > MESH_SIZE )
      {
         fclose( FilePointer );
         return( E_INVMESHFILE );
      }

      for( i = 0; i < (*SourceMesh)->HorizRes; i++ )
         if (fread((*SourceMesh)->HorizList[i], sizeof(float), (*SourceMesh)->VertRes, FilePointer ) != (*SourceMesh)->VertRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      for( i = 0; i < (*SourceMesh)->VertRes; i++ )
         if (fread((*SourceMesh)->VertList[i], sizeof(float), (*SourceMesh)->HorizRes, FilePointer ) != (*SourceMesh)->HorizRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      for( i = 0; i < (*TargetMesh)->HorizRes; i++ )
         if (fread((*TargetMesh)->HorizList[i], sizeof(float), (*TargetMesh)->VertRes, FilePointer ) != (*TargetMesh)->VertRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      for( i = 0; i < (*TargetMesh)->VertRes; i++ )
         if (fread((*TargetMesh)->VertList[i], sizeof(float), (*TargetMesh)->HorizRes, FilePointer ) != (*TargetMesh)->HorizRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      fclose(FilePointer);
   }
   else
   {
      return(E_NOOPEN);
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ReadOldMesh( char *FileName, MESH *SourceMesh, MESH *TargetMesh )
{
int   i;
char  TempStr[80];
FILE  *FilePointer;

   FilePointer = fopen( FileName, "rb" );

   if (FilePointer)
   {
      fread( TempStr, 1, strlen(FILE_HEADER) + 1, FilePointer );

      if (strcmp( TempStr, FILE_HEADER ))
      {
         fclose( FilePointer );
         return( E_BADMESHFILE );
      }

      fgetc( FilePointer );

      (*SourceMesh)->HorizRes = ReadWord( FilePointer );
      (*SourceMesh)->VertRes  = ReadWord( FilePointer );

      (*TargetMesh)->HorizRes = ReadWord( FilePointer );
      (*TargetMesh)->VertRes  = ReadWord( FilePointer );

      ZoomOn                  = ReadWord( FilePointer );
      TotalFrames             = ReadWord( FilePointer );
      StartFrame              = ReadWord( FilePointer );
      EndFrame                = ReadWord( FilePointer );
      WarpTightness           = ReadWord( FilePointer );
      Fade50                  = ReadWord( FilePointer );
      Warp50                  = ReadWord( FilePointer );
      OutputFileType          = ReadWord( FilePointer );
      MeshType                = ReadWord( FilePointer );
      MorphWarpFlag           = ReadWord( FilePointer );
      CurvesOn                = ReadWord( FilePointer );
      InterpMeshType          = ReadWord( FilePointer );
      TGAUpsideDownFlag       = ReadWord( FilePointer );
      InterpTightness         = ReadWord( FilePointer );
      ShowBetweensEnable      = ReadWord( FilePointer );
      FastMorphMode           = ReadWord( FilePointer );
      QuietMode               = ReadWord( FilePointer );
      FastScreen              = ReadWord( FilePointer );

      // Save room for future modifications...
      for (i = 0; i < 1019; i++)
         ReadWord( FilePointer );

      fread(OutBaseName, 1, 80, FilePointer);
      
      if ((*SourceMesh)->HorizRes > MESH_SIZE ||
          (*SourceMesh)->VertRes > MESH_SIZE ||
          (*TargetMesh)->HorizRes > MESH_SIZE ||
          (*TargetMesh)->VertRes > MESH_SIZE )
      {
         fclose( FilePointer );
         return( E_INVMESHFILE );
      }

      for( i = 0; i < (*SourceMesh)->HorizRes; i++ )
         if (fread((*SourceMesh)->HorizList[i], sizeof(float), (*SourceMesh)->VertRes, FilePointer ) != (*SourceMesh)->VertRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      for( i = 0; i < (*SourceMesh)->VertRes; i++ )
         if (fread((*SourceMesh)->VertList[i], sizeof(float), (*SourceMesh)->HorizRes, FilePointer ) != (*SourceMesh)->HorizRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      for( i = 0; i < (*TargetMesh)->HorizRes; i++ )
         if (fread((*TargetMesh)->HorizList[i], sizeof(float), (*TargetMesh)->VertRes, FilePointer ) != (*TargetMesh)->VertRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
         }

      for( i = 0; i < (*TargetMesh)->VertRes; i++ )
         if (fread((*TargetMesh)->VertList[i], sizeof(float), (*TargetMesh)->HorizRes, FilePointer ) != (*TargetMesh)->HorizRes)
         {
            fclose( FilePointer );
            return( E_INVMESHFILE );
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
  ±   [IMPORT.C    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
