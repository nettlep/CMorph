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
  ±   [BUPDATE.C   ] - Button update routines                                ±
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

void  DoSourceFilter( )
{
int   RetCode;

   HideCursor( );
   RetCode = Convolution( SourceFiltName, SourceImage );
   ShowCursor( );

   if (RetCode != E_NONE)
   {
      ErrorBox( "Unable to perform filter", RetCode );
      return;
   }

   ChangeMenu( MAINMENU );

   if (VMode == SVGA)
      DrawMessBox( "Information", "One moment, replacing image", STATUS_COLOR );

   DrawImage( SourceImage );

   if (VMode == SVGA)
   {
      EraseMessBox( );
      RestoreImage( SourceImage );
   }

   DrawMesh( SourceMesh, SourceImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoTargetFilter( )
{
   HideCursor( );
   Convolution( TargetFiltName, TargetImage );
   ShowCursor( );

   ChangeMenu( MAINMENU );

   if (VMode == SVGA)
      DrawMessBox( "Information", "One moment, replacing image", STATUS_COLOR );
   
   DrawImage( TargetImage );

   if (VMode == SVGA)
   {
      EraseMessBox( );
      RestoreImage( TargetImage );
   }

   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateSoundsEnable( )
{
   HideCursor( );

   if (QuietMode)
   {
      QuietMode = 0;
   }
   else
   {
      QuietMode = 1;
   }

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( SoundEnableButton.Rect.Left, SoundEnableButton.Rect.Top, &SoundEnableButton );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateKwikScreen( )
{
   HideCursor( );

   if (FastScreen)
   {
      FastScreen = 0;
   }
   else
   {
      FastScreen = 1;
   }

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( KwikScreenButton.Rect.Left, KwikScreenButton.Rect.Top, &KwikScreenButton );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateShowBetweens( )
{
   HideCursor( );

   if (ShowBetweensEnable)
   {
      ShowBetweensEnable = 0;
   }
   else
   {
      ShowBetweensEnable = 1;
   }

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( ShowBetweensButton.Rect.Left, ShowBetweensButton.Rect.Top, &ShowBetweensButton );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFastMorph( )
{
   HideCursor( );

   if (FastMorphMode)
   {
      FastMorphMode = 0;
   }
   else
   {
      FastMorphMode = 1;
   }

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( FastMorphButton.Rect.Left, FastMorphButton.Rect.Top, &FastMorphButton );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateCurves( )
{
   HideCursor( );

   if (CurvesOn)
   {
      CurvesOn = 0;
   }
   else
   {
      CurvesOn = 1;
   }

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( CurvesButton.Rect.Left, CurvesButton.Rect.Top, &CurvesButton );

   ShowCursor( );

   UpdateImages = 1;

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateMorph( int State )
{
   HideCursor( );

   if (MorphWarpFlag != State )
   {
      MorphWarpFlag = State;

      UpdateButtonStates( );

      if (MenuMode == PERFMENU)
      {
         DrawButton( MorphButton.Rect.Left, MorphButton.Rect.Top, &MorphButton );
         DrawButton( WarpButton.Rect.Left, WarpButton.Rect.Top, &WarpButton );
         DrawButton( FadeButton.Rect.Left, FadeButton.Rect.Top, &FadeButton );
      }
   }

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateMeshType( int Type )
{
   HideCursor( );

   if (MeshType != Type)
   {
      MeshType = Type;

      UpdateButtonStates( );

      if (MenuMode == PERFMENU)
      {
         DrawButton( MeshCATButton.Rect.Left, MeshCATButton.Rect.Top, &MeshCATButton );
         DrawButton( MeshSPLButton.Rect.Left, MeshSPLButton.Rect.Top, &MeshSPLButton );
         DrawButton( MeshSTRButton.Rect.Left, MeshSTRButton.Rect.Top, &MeshSTRButton );
      }
   }

   ShowCursor( );

   UpdateImages = 1;

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateInterpType( int Type )
{
   HideCursor( );

   if (InterpMeshType != Type)
   {
      InterpMeshType = Type;

      UpdateButtonStates( );

      if (MenuMode == PERFMENU)
      {
         DrawButton( InterpMeshSPLButton.Rect.Left, InterpMeshSPLButton.Rect.Top, &InterpMeshSPLButton );
         DrawButton( InterpMeshCATButton.Rect.Left, InterpMeshCATButton.Rect.Top, &InterpMeshCATButton );
         DrawButton( InterpMeshSTRButton.Rect.Left, InterpMeshSTRButton.Rect.Top, &InterpMeshSTRButton );
      }
   }

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateArithType( int Type )
{
   HideCursor( );

   if (ArithType != Type)
   {
      ArithType = Type;

      UpdateButtonStates( );

      if (MenuMode == FILEMENU)
      {
         DrawButton( OffOutButton.Rect.Left, OffOutButton.Rect.Top, &OffOutButton );
         DrawButton( AveOutButton.Rect.Left, AveOutButton.Rect.Top, &AveOutButton );
         DrawButton( AddOutButton.Rect.Left, AddOutButton.Rect.Top, &AddOutButton );
         DrawButton( SubOutButton.Rect.Left, SubOutButton.Rect.Top, &SubOutButton );
         DrawButton( OvrOutButton.Rect.Left, OvrOutButton.Rect.Top, &OvrOutButton );
      }
   }

   ShowCursor( );

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateBackground( int State )
{
   HideCursor( );

   if (Background != State)
   {
      Background = State;

      UpdateButtonStates( );

      if (MenuMode == FILEMENU)
      {
         DrawButton( BackOffButton.Rect.Left, BackOffButton.Rect.Top, &BackOffButton );
         DrawButton( BackOnButton.Rect.Left, BackOnButton.Rect.Top, &BackOnButton );
      }
   }

   ShowCursor( );

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFileType( int Type )
{
   HideCursor( );

   if (OutputFileType != Type)
   {
      OutputFileType = Type;

      UpdateButtonStates( );

      if (MenuMode == PERFMENU)
      {
         DrawButton( OutputTGAButton.Rect.Left, OutputTGAButton.Rect.Top, &OutputTGAButton );
         DrawButton( OutputIMGButton.Rect.Left, OutputIMGButton.Rect.Top, &OutputIMGButton );
         DrawButton( OutputGIFButton.Rect.Left, OutputGIFButton.Rect.Top, &OutputGIFButton );
         DrawButton( OutputIPIButton.Rect.Left, OutputIPIButton.Rect.Top, &OutputIPIButton );
         DrawButton( OutputBMPButton.Rect.Left, OutputBMPButton.Rect.Top, &OutputBMPButton );
         DrawButton( OutputPCXButton.Rect.Left, OutputPCXButton.Rect.Top, &OutputPCXButton );
      }
   }

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFlicType( int Type )
{
   HideCursor( );

   if (FlicType != Type)
   {
      FlicType = Type;

      UpdateButtonStates( );

      if (MenuMode == FLICMENU)
      {
         DrawButton( FlicFLIButton.Rect.Left, FlicFLIButton.Rect.Top, &FlicFLIButton );
         DrawButton( FlicFLCButton.Rect.Left, FlicFLCButton.Rect.Top, &FlicFLCButton );
         DrawButton( FlicFLXButton.Rect.Left, FlicFLXButton.Rect.Top, &FlicFLXButton );
         DrawButton( FlicFLHButton.Rect.Left, FlicFLHButton.Rect.Top, &FlicFLHButton );
         DrawButton( FlicFLTButton.Rect.Left, FlicFLTButton.Rect.Top, &FlicFLTButton );
      }
   }

   ShowCursor( );

   UpdateFastFlic = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFlicPing( )
{
   HideCursor( );

   if (FlicAutoRew == NO_AUTOREW)
      FlicAutoRew = AUTOREW;
   else
      FlicAutoRew = NO_AUTOREW;

   UpdateButtonStates( );

   if (MenuMode == FLICMENU)
      DrawButton( FlicPingButton.Rect.Left, FlicPingButton.Rect.Top, &FlicPingButton );

   ShowCursor( );

   UpdateFastFlic = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFlicOnOff( )
{
   HideCursor( );

   if (FlicOnOff == FLIC_ON)
      FlicOnOff = FLIC_OFF;
   else
      FlicOnOff = FLIC_ON;

   UpdateButtonStates( );

   if (MenuMode == FLICMENU)
      DrawButton( FlicOnOffButton.Rect.Left, FlicOnOffButton.Rect.Top, &FlicOnOffButton );

   ShowCursor( );

   UpdateFastFlic = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFlicColor( )
{
   HideCursor( );

   if (FlicColorMono == COLOR)
      FlicColorMono = MONOCHROME;
   else
      FlicColorMono = COLOR;

   UpdateButtonStates( );

   if (MenuMode == FLICMENU)
      DrawButton( FlicColorButton.Rect.Left, FlicColorButton.Rect.Top, &FlicColorButton );

   ShowCursor( );

   UpdateFastFlic = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateUpsideDown( )
{
   HideCursor( );

   if (TGAUpsideDownFlag == TOP_DOWN)
      TGAUpsideDownFlag = BOTTOM_UP;
   else
      TGAUpsideDownFlag = TOP_DOWN;

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( TGAUpsideDownButton.Rect.Left, TGAUpsideDownButton.Rect.Top, &TGAUpsideDownButton );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdatePrevColor( )
{
   HideCursor( );

   if (PrevColor == COLOR)
      PrevColor = MONOCHROME;
   else
      PrevColor = COLOR;

   UpdateButtonStates( );

   if (MenuMode == PREVMENU)
      DrawButton( PrevColorButton.Rect.Left, PrevColorButton.Rect.Top, &PrevColorButton );

   ShowCursor( );

   UpdateFastPrev = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateCursColor( int Color )
{
   HideCursor( );

   if (CursColor != Color)
   {
      CursColor = Color;

      UpdateButtonStates( );

      if (MenuMode == PERFMENU)
      {
         DrawButton( CursRButton.Rect.Left, CursRButton.Rect.Top, &CursRButton );
         DrawButton( CursGButton.Rect.Left, CursGButton.Rect.Top, &CursGButton );
         DrawButton( CursBButton.Rect.Left, CursBButton.Rect.Top, &CursBButton );
         DrawButton( CursXButton.Rect.Left, CursXButton.Rect.Top, &CursXButton );
      }
   }

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateMeshColor( int Color )
{
   HideCursor( );

   if (MeshColor != Color)
   {
      MeshColor = Color;

      UpdateButtonStates( );

      if (MenuMode == PERFMENU)
      {
         DrawButton( MColorRButton.Rect.Left, MColorRButton.Rect.Top, &MColorRButton );
         DrawButton( MColorGButton.Rect.Left, MColorGButton.Rect.Top, &MColorGButton );
         DrawButton( MColorBButton.Rect.Left, MColorBButton.Rect.Top, &MColorBButton );
         DrawButton( MColorXButton.Rect.Left, MColorXButton.Rect.Top, &MColorXButton );
      }
   }

   ShowCursor( );

   UpdateImages = 1;

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateZoomStatus( )
{
   HideCursor( );

   if (ZoomOn)
   {
      ZoomOn = 0;
   }
   else
   {
      ZoomOn = 1;
   }

   UpdateButtonStates( );

   if (MenuMode == PERFMENU)
      DrawButton( ZoomButton.Rect.Left, ZoomButton.Rect.Top, &ZoomButton );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [BUPDATE.C   ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
