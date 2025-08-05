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
  ±   [UTILS.C      ] - Misc. morphing utils                                 ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  LoadImage( char *FileName, IMAGE Image )
{
int   RetCode;
   HideCursor( );

   DrawMessBox( "Information", "Patience...loading image", ERROR_BOX_COLOR );
   RetCode = ReadImage(FileName, &Image);
   EraseMessBox( );

   if (RetCode != E_NONE)
   {
      ErrorBox( "Unable to load image", RetCode );
      ShowCursor( );
      return;
   }

   MBox( "Image loaded OK" );
   ShowCursor( );
   ChangeMenu( MAINMENU );

   if (VMode == SVGA)
      DrawMessBox( "Information", "One moment, replacing image", STATUS_COLOR );

   DrawImage( Image );

   if (VMode == SVGA)
   {
      EraseMessBox( );
      RestoreImage( SourceImage );
   }

   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SaveImage( char *FileName, IMAGE Image )
{
int   RetCode;

   HideCursor( );

   DrawMessBox( "Information", "Patience...saving image", ERROR_BOX_COLOR );
   RetCode = WriteImage(FileName, Image, TGAUpsideDownFlag);
   DrawImage( Image );
   EraseMessBox( );

   if (RetCode != E_NONE)
   {
      ErrorBox( "Unable to save image", RetCode );
      ShowCursor( );
      return;
   }

   MBox( "Image saved OK" );

   ShowCursor( );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  Beep( )
{
   if (!QuietMode)
   {
      sound(2000);
      delay(10);
      nosound( );
   }
}
/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFrameName(char *OutFile, int Val)
{
   sprintf( OutFile, "%s%03d.%s", OutBaseName, Val+FrameOffset, OutputFileType == IPI ? "IPI":OutputFileType == TGA ? "TGA":OutputFileType == BMP ? "BMP": OutputFileType == IMG ? "IMG": OutputFileType == GIF ? "GIF" : "PCX" );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  WriteWord( int Val, FILE *FilePointer )
{
   fputc( Val >> 8,   FilePointer );
   fputc( Val & 0xff, FilePointer );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ReadWord( FILE *FilePointer )
{
int   RetCode;

   RetCode = fgetc( FilePointer ) * 256;
   RetCode += fgetc( FilePointer );

   return( RetCode );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   GetKey( )
{
int   Key, x, y, left, right, ld = 0, rd = 0;

   while(1)
   {
      if (kbhit())
      {
         Key = getch();

         if (!Key)
         {
            Key = getch();

            if (Key == ALT_C)
               if (DebugMode)
                  DoScreenCapture( );
               else
                  Beep();
         }

         return( Key );
      }

      ReadMouse(&x, &y, &left, &right, 0);

      if ( left && !ld )
         ld = 1;

      if ( right && !rd )
         rd = 1;

      if ( ld && !left )
         return 'Y';

      if ( rd && !right )
         return 'N';
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   Splines(float *x1, float *y1, int len1, float *x2, float *y2, int len2, int InterpWarpFlag)
{
int   RetCode;

   if (InterpWarpFlag == SP_WARP)
   {
      if (MeshType == CATMULL)
         RetCode = Catmull( x1, y1, len1, x2, y2, len2 );
      else if (MeshType == STRAIGHT)
         RetCode = Straight( x1, y1, len1, x2, y2, len2 );
      else if (MeshType == SPLINE)
         RetCode = Bicubic( x1, y1, len1, x2, y2, len2, WarpTightness );
   }
   else if (InterpWarpFlag == SP_INTRP)
   {
      if (InterpMeshType == CATMULL)
         RetCode = Catmull( x1, y1, len1, x2, y2, len2 );
      else if (InterpMeshType == STRAIGHT)
         RetCode = Straight( x1, y1, len1, x2, y2, len2 );
      else if (InterpMeshType == SPLINE)
         RetCode = Bicubic( x1, y1, len1, x2, y2, len2, InterpTightness );
   }

   return( RetCode );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateButtonStates( void )
{
   MeshCATButton.State       = MeshType==CATMULL            ? BUTTON_DOWN:BUTTON_UP;
   MeshSPLButton.State       = MeshType==SPLINE             ? BUTTON_DOWN:BUTTON_UP;
   MeshSTRButton.State       = MeshType==STRAIGHT           ? BUTTON_DOWN:BUTTON_UP;
   InterpMeshCATButton.State = InterpMeshType==CATMULL      ? BUTTON_DOWN:BUTTON_UP;
   InterpMeshSPLButton.State = InterpMeshType==SPLINE       ? BUTTON_DOWN:BUTTON_UP;
   InterpMeshSTRButton.State = InterpMeshType==STRAIGHT     ? BUTTON_DOWN:BUTTON_UP;
   CurvesButton.State        = CurvesOn==1                  ? BUTTON_DOWN:BUTTON_UP;
   ZoomButton.State          = ZoomOn==1                    ? BUTTON_DOWN:BUTTON_UP;
   CursRButton.State         = CursColor==R_CURS_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   CursGButton.State         = CursColor==G_CURS_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   CursBButton.State         = CursColor==B_CURS_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   CursXButton.State         = CursColor==X_CURS_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   MColorRButton.State       = MeshColor==R_MESH_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   MColorGButton.State       = MeshColor==G_MESH_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   MColorBButton.State       = MeshColor==B_MESH_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   MColorXButton.State       = MeshColor==X_MESH_COLOR      ? BUTTON_DOWN:BUTTON_UP;
   OutputTGAButton.State     = OutputFileType==TGA          ? BUTTON_DOWN:BUTTON_UP;
   OutputIPIButton.State     = OutputFileType==IPI          ? BUTTON_DOWN:BUTTON_UP;
   OutputIMGButton.State     = OutputFileType==IMG          ? BUTTON_DOWN:BUTTON_UP;
   OutputGIFButton.State     = OutputFileType==GIF          ? BUTTON_DOWN:BUTTON_UP;
   OutputBMPButton.State     = OutputFileType==BMP          ? BUTTON_DOWN:BUTTON_UP;
   OutputPCXButton.State     = OutputFileType==PCX          ? BUTTON_DOWN:BUTTON_UP;
   MorphButton.State         = MorphWarpFlag==MORPH         ? BUTTON_DOWN:BUTTON_UP;
   WarpButton.State          = MorphWarpFlag==WARP          ? BUTTON_DOWN:BUTTON_UP;
   FadeButton.State          = MorphWarpFlag==FADE          ? BUTTON_DOWN:BUTTON_UP;
   TGAUpsideDownButton.State = TGAUpsideDownFlag==BOTTOM_UP ? BUTTON_DOWN:BUTTON_UP;
   KwikScreenButton.State    = FastScreen==1                ? BUTTON_DOWN:BUTTON_UP;
   SoundEnableButton.State   = QuietMode==0                 ? BUTTON_DOWN:BUTTON_UP;
   ShowBetweensButton.State  = ShowBetweensEnable==1        ? BUTTON_DOWN:BUTTON_UP;
   FastMorphButton.State     = FastMorphMode==1             ? BUTTON_DOWN:BUTTON_UP;
   OffOutButton.State        = ArithType==ARITH_OFF         ? BUTTON_DOWN:BUTTON_UP;
   AveOutButton.State        = ArithType==ARITH_AVE         ? BUTTON_DOWN:BUTTON_UP;
   AddOutButton.State        = ArithType==ARITH_ADD         ? BUTTON_DOWN:BUTTON_UP;
   SubOutButton.State        = ArithType==ARITH_SUB         ? BUTTON_DOWN:BUTTON_UP;
   OvrOutButton.State        = ArithType==ARITH_OVR         ? BUTTON_DOWN:BUTTON_UP;
   BackOffButton.State       = Background==BACK_OFF         ? BUTTON_DOWN:BUTTON_UP;
   BackOnButton.State        = Background==BACK_ON          ? BUTTON_DOWN:BUTTON_UP;
   PrevColorButton.State     = PrevColor==COLOR             ? BUTTON_DOWN:BUTTON_UP;
   FlicFLIButton.State       = FlicType==FLI                ? BUTTON_DOWN:BUTTON_UP;
   FlicFLCButton.State       = FlicType==FLC                ? BUTTON_DOWN:BUTTON_UP;
   FlicFLXButton.State       = FlicType==FLX                ? BUTTON_DOWN:BUTTON_UP;
   FlicFLHButton.State       = FlicType==FLH                ? BUTTON_DOWN:BUTTON_UP;
   FlicFLTButton.State       = FlicType==FLT                ? BUTTON_DOWN:BUTTON_UP;
   FlicPingButton.State      = FlicAutoRew==AUTOREW         ? BUTTON_DOWN:BUTTON_UP;
   FlicOnOffButton.State     = FlicOnOff==FLIC_ON           ? BUTTON_DOWN:BUTTON_UP;
   FlicColorButton.State     = FlicColorMono==MONOCHROME    ? BUTTON_DOWN:BUTTON_UP;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [UTILS.C      ] - End Of File                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
