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
  ±   [INPUT.C     ] - Get and handle user input                             ±
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

static   int   GetCursorKey( int x, int y );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  Input( )
{
int   Done = 0;
int   MouseX = 0, MouseY = 0;
int   Left = 0, Right = 0, OldLeft = 0, OldRight = 0;
static   int   SourceHidden, TargetHidden;

//--------------------------------------------------------------------------
//
//                         KEYBOARD USAGE...
//
//--------------------------------------------------------------------------

   while( !Done )
   {
      Done = HandleKeys( );

      ReadMouse( &MouseX, &MouseY, &Left, &Right, 1 );

//--------------------------------------------------------------------------
//
//                         MAIN MENU...
//
//--------------------------------------------------------------------------

      if (MenuMode == MAINMENU)
      {
         //------------------------------------------
         // HANDLE HIDE MESHES
         //------------------------------------------

         if (Right && !OldRight)
         {
            // HIDE THE SOURCE MESH?
            if (InRect( MouseX, MouseY, &SourceInfoWindow ))
            {
               RestoreImage( SourceImage );
               SourceHidden = 1;
            }

            // HIDE THE TARGET MESH?
            if (InRect( MouseX, MouseY, &TargetInfoWindow ))
            {
               RestoreImage( TargetImage );
               TargetHidden = 1;
            }

            OldRight = Right;
            continue;
         }

         if (SourceHidden && !Right)
         {
            DrawMesh( SourceMesh, SourceImage );
            SourceHidden = 0;
            continue;
         }

         if (TargetHidden && !Right)
         {
            DrawMesh( TargetMesh, TargetImage );
            TargetHidden = 0;
            continue;
         }

         if (TargetHidden || SourceHidden)
            continue;

         //------------------------------------------
         // DID THE USER JUST PRESS THE LEFT BUTTON?
         //------------------------------------------

         if (Left && !OldLeft)
         {
            MoveMesh( MouseX, MouseY, Left, Right );
         }

         //----------------------------------------
         // DID THE USER RELEASE THE RIGHT BUTTON?
         //----------------------------------------


         if (!Right && OldRight)
         {
            AddMesh( MouseX, MouseY );
         }
      }

      //----------------
      // BACKUP A MENU?
      //----------------

      if (!Right && OldRight)
         ChangeMenu( MAINMENU );

      OldRight = Right;

      //---------------------------------------
      // DID THE USER RELEASE THE LEFT BUTTON?
      //---------------------------------------

      if (!Left && OldLeft)
      {
         // ---------------------------------
         // HANDLE BUTTONS ON THE WAY BACK UP
         // ---------------------------------

         if (MenuMode == MAINMENU)
         {
            if (PerfButton.State != BUTTON_UP)
            {
               PerfButton.State = BUTTON_UP;
               HideCursor( );
               DrawButton( PerfButton.Rect.Left, PerfButton.Rect.Top, &PerfButton );
               ShowCursor( );
            }
            else if (OperButton.State != BUTTON_UP)
            {
               OperButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( OperButton.Rect.Left, OperButton.Rect.Top, &OperButton );
               ShowCursor();
            }
            else if (EditButton.State != BUTTON_UP)
            {
               EditButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( EditButton.Rect.Left, EditButton.Rect.Top, &EditButton );
               ShowCursor();
            }
            else if (FileButton.State != BUTTON_UP)
            {
               FileButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( FileButton.Rect.Left, FileButton.Rect.Top, &FileButton );
               ShowCursor();
            }
            else if (ProcButton.State != BUTTON_UP)
            {
               ProcButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ProcButton.Rect.Left, ProcButton.Rect.Top, &ProcButton );
               ShowCursor();
            }
            else if (PrevButton.State != BUTTON_UP)
            {
               PrevButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( PrevButton.Rect.Left, PrevButton.Rect.Top, &PrevButton );
               ShowCursor();
            }
#ifndef NOREG
            else if (FlicButton.State != BUTTON_UP)
            {
               FlicButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( FlicButton.Rect.Left, FlicButton.Rect.Top, &FlicButton );
               ShowCursor();
            }
#endif
         }
         else if (MenuMode == PERFMENU)
         {
         }
         else if (MenuMode == OPERMENU)
         {
            if (ExitButton.State != BUTTON_UP)
            {
               ExitButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ExitButton.Rect.Left, ExitButton.Rect.Top, &ExitButton );
               ShowCursor();
            }
            else if (ShellButton.State != BUTTON_UP)
            {
               ShellButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ShellButton.Rect.Left, ShellButton.Rect.Top, &ShellButton );
               ShowCursor();
            }
            else if (GoButton.State != BUTTON_UP)
            {
               GoButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( GoButton.Rect.Left, GoButton.Rect.Top, &GoButton );
               ShowCursor();
            }
            else if (HelpButton.State != BUTTON_UP)
            {
               HelpButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HelpButton.Rect.Left, HelpButton.Rect.Top, &HelpButton );
               ShowCursor();
            }
            else if (AboutButton.State != BUTTON_UP)
            {
               AboutButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( AboutButton.Rect.Left, AboutButton.Rect.Top, &AboutButton );
               ShowCursor();
            }
         }
         else if (MenuMode == PREVMENU)
         {
            if (PrevGoButton.State != BUTTON_UP)
            {
               PrevGoButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( PrevGoButton.Rect.Left, PrevGoButton.Rect.Top, &PrevGoButton );
               ShowCursor();
            }
         }
         else if (MenuMode == EDITMENU)
         {
            if (MeshSwapButton.State != BUTTON_UP)
            {
               MeshSwapButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( MeshSwapButton.Rect.Left, MeshSwapButton.Rect.Top, &MeshSwapButton );
               ShowCursor();
            }
            else if (CopySDMButton.State != BUTTON_UP)
            {
               CopySDMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( CopySDMButton.Rect.Left, CopySDMButton.Rect.Top, &CopySDMButton );
               ShowCursor();
            }
            else if (HFlipSMButton.State != BUTTON_UP)
            {
               HFlipSMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipSMButton.Rect.Left, HFlipSMButton.Rect.Top, &HFlipSMButton );
               ShowCursor();
            }
            else if (VFlipSMButton.State != BUTTON_UP)
            {
               VFlipSMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipSMButton.Rect.Left, VFlipSMButton.Rect.Top, &VFlipSMButton );
               ShowCursor();
            }
            else if (CopyDSMButton.State != BUTTON_UP)
            {
               CopyDSMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( CopyDSMButton.Rect.Left, CopyDSMButton.Rect.Top, &CopyDSMButton );
               ShowCursor();
            }
            else if (HFlipDMButton.State != BUTTON_UP)
            {
               HFlipDMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipDMButton.Rect.Left, HFlipDMButton.Rect.Top, &HFlipDMButton );
               ShowCursor();
            }
            else if (VFlipDMButton.State != BUTTON_UP)
            {
               VFlipDMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipDMButton.Rect.Left, VFlipDMButton.Rect.Top, &VFlipDMButton );
               ShowCursor();
            }
            else if (ImageSwapButton.State != BUTTON_UP)
            {
               ImageSwapButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ImageSwapButton.Rect.Left, ImageSwapButton.Rect.Top, &ImageSwapButton );
               ShowCursor();
            }
            else if (HFlipSIButton.State != BUTTON_UP)
            {
               HFlipSIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipSIButton.Rect.Left, HFlipSIButton.Rect.Top, &HFlipSIButton );
               ShowCursor();
            }
            else if (VFlipSIButton.State != BUTTON_UP)
            {
               VFlipSIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipSIButton.Rect.Left, VFlipSIButton.Rect.Top, &VFlipSIButton );
               ShowCursor();
            }
            else if (HFlipDIButton.State != BUTTON_UP)
            {
               HFlipDIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipDIButton.Rect.Left, HFlipDIButton.Rect.Top, &HFlipDIButton );
               ShowCursor();
            }
            else if (VFlipDIButton.State != BUTTON_UP)
            {
               VFlipDIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipDIButton.Rect.Left, VFlipDIButton.Rect.Top, &VFlipDIButton );
               ShowCursor();
            }
         }
         else if (MenuMode == FILEMENU)
         {
            if (LoadMeshButton.State != BUTTON_UP)
            {
               LoadMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( LoadMeshButton.Rect.Left, LoadMeshButton.Rect.Top, &LoadMeshButton );
               ShowCursor();
            }
            else if (SaveMeshButton.State != BUTTON_UP)
            {
               SaveMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( SaveMeshButton.Rect.Left, SaveMeshButton.Rect.Top, &SaveMeshButton );
               ShowCursor();
            }
            else if (ImpMeshButton.State != BUTTON_UP)
            {
               ImpMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ImpMeshButton.Rect.Left, ImpMeshButton.Rect.Top, &ImpMeshButton );
               ShowCursor();
            }
            else if (ExpMeshButton.State != BUTTON_UP)
            {
               ExpMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ExpMeshButton.Rect.Left, ExpMeshButton.Rect.Top, &ExpMeshButton );
               ShowCursor();
            }
            else if (LoadSourceImageButton.State != BUTTON_UP)
            {
               LoadSourceImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( LoadSourceImageButton.Rect.Left, LoadSourceImageButton.Rect.Top, &LoadSourceImageButton );
               ShowCursor();
            }
            else if (SaveSourceImageButton.State != BUTTON_UP)
            {
               SaveSourceImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( SaveSourceImageButton.Rect.Left, SaveSourceImageButton.Rect.Top, &SaveSourceImageButton );
               ShowCursor();
            }
            else if (LoadTargetImageButton.State != BUTTON_UP)
            {
               LoadTargetImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( LoadTargetImageButton.Rect.Left, LoadTargetImageButton.Rect.Top, &LoadTargetImageButton );
               ShowCursor();
            }
            else if (SaveTargetImageButton.State != BUTTON_UP)
            {
               SaveTargetImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( SaveTargetImageButton.Rect.Left, SaveTargetImageButton.Rect.Top, &SaveTargetImageButton );
               ShowCursor();
            }
         }
         else if (MenuMode == PROCMENU)
         {
            if (DoSourceFilterButton.State != BUTTON_UP)
            {
               DoSourceFilterButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( DoSourceFilterButton.Rect.Left, DoSourceFilterButton.Rect.Top, &DoSourceFilterButton );
               ShowCursor();
            }
            else if (DoTargetFilterButton.State != BUTTON_UP)
            {
               DoTargetFilterButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( DoTargetFilterButton.Rect.Left, DoTargetFilterButton.Rect.Top, &DoTargetFilterButton );
               ShowCursor();
            }
         }

         // ---------------------------------
         // HANDLE BUTTONS ON THE WAY UP
         // ---------------------------------

         if (MenuMode == MAINMENU)
         {
            if (InRect( MouseX, MouseY, &PerfButton.Rect ))
            {
               PerfButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( PerfButton.Rect.Left, PerfButton.Rect.Top, &PerfButton );
               ShowCursor();
               ChangeMenu( PERFMENU );
            }
            else if (InRect( MouseX, MouseY, &OperButton.Rect ))
            {
               OperButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( OperButton.Rect.Left, OperButton.Rect.Top, &OperButton );
               ShowCursor();
               ChangeMenu( OPERMENU );
            }
            else if (InRect( MouseX, MouseY, &EditButton.Rect ))
            {
               EditButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( EditButton.Rect.Left, EditButton.Rect.Top, &EditButton );
               ShowCursor();
               ChangeMenu( EDITMENU );
            }
            else if (InRect( MouseX, MouseY, &FileButton.Rect ))
            {
               FileButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( FileButton.Rect.Left, FileButton.Rect.Top, &FileButton );
               ShowCursor();
               ChangeMenu( FILEMENU );
            }
            else if (InRect( MouseX, MouseY, &ProcButton.Rect ))
            {
               ProcButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ProcButton.Rect.Left, ProcButton.Rect.Top, &ProcButton );
               ShowCursor();
               ChangeMenu( PROCMENU );
            }
            else if (InRect( MouseX, MouseY, &PrevButton.Rect ))
            {
               PrevButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( PrevButton.Rect.Left, PrevButton.Rect.Top, &PrevButton );
               ShowCursor();
               ChangeMenu( PREVMENU );
            }
#ifndef NOREG
            else if (InRect( MouseX, MouseY, &FlicButton.Rect ))
            {
               FlicButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( FlicButton.Rect.Left, FlicButton.Rect.Top, &FlicButton );
               ShowCursor();
               ChangeMenu( FLICMENU );
            }
#endif
         }
         else if (MenuMode == PERFMENU)
         {
            if (InRect( MouseX, MouseY, &MeshCATButton.Rect ))
            {
               UpdateMeshType( CATMULL );
            }
            else if (InRect( MouseX, MouseY, &MeshSPLButton.Rect ))
            {
               UpdateMeshType( SPLINE );
            }
            else if (InRect( MouseX, MouseY, &MeshSTRButton.Rect ))
            {
               UpdateMeshType( STRAIGHT );
            }
            else if (InRect( MouseX, MouseY, &InterpMeshCATButton.Rect ))
            {
               UpdateInterpType( CATMULL );
            }
            else if (InRect( MouseX, MouseY, &InterpMeshSPLButton.Rect ))
            {
               UpdateInterpType( SPLINE );
            }
            else if (InRect( MouseX, MouseY, &InterpMeshSTRButton.Rect ))
            {
               UpdateInterpType( STRAIGHT );
            }
            else if (InRect( MouseX, MouseY, &MorphButton.Rect ))
            {
               UpdateMorph( MORPH );
            }
            else if (InRect( MouseX, MouseY, &WarpButton.Rect ))
            {
               UpdateMorph( WARP );
            }
            else if (InRect( MouseX, MouseY, &FadeButton.Rect ))
            {
               UpdateMorph( FADE );
            }
            else if (InRect( MouseX, MouseY, &FramesPrompt.Rect ))
            {
               GetFrames();
            }
            else if (InRect( MouseX, MouseY, &StartFramePrompt.Rect ))
            {
               GetStartFrame();
            }
            else if (InRect( MouseX, MouseY, &EndFramePrompt.Rect ))
            {
               GetEndFrame();
            }
            else if (InRect( MouseX, MouseY, &FrameOffPrompt.Rect ))
            {
               GetFrameOff();
            }
            else if (InRect( MouseX, MouseY, &WarpTightnessPrompt.Rect ))
            {
               GetWarpTightness();
            }
            else if (InRect( MouseX, MouseY, &InterpTightnessPrompt.Rect ))
            {
               GetInterpTightness();
            }
            else if (InRect( MouseX, MouseY, &Fade50Prompt.Rect ))
            {
               GetFade50();
            }
            else if (InRect( MouseX, MouseY, &FadeStartPrompt.Rect ))
            {
               GetFadeStart();
            }
            else if (InRect( MouseX, MouseY, &FadeEndPrompt.Rect ))
            {
               GetFadeEnd();
            }
            else if (InRect( MouseX, MouseY, &Warp50Prompt.Rect ))
            {
               GetWarp50();
            }
            else if (InRect( MouseX, MouseY, &WarpStartPrompt.Rect ))
            {
               GetWarpStart();
            }
            else if (InRect( MouseX, MouseY, &WarpEndPrompt.Rect ))
            {
               GetWarpEnd();
            }
            else if (InRect( MouseX, MouseY, &CursRButton.Rect ))
            {
               UpdateCursColor( R_CURS_COLOR );
            }
            else if (InRect( MouseX, MouseY, &CursGButton.Rect ))
            {
               UpdateCursColor( G_CURS_COLOR );
            }
            else if (InRect( MouseX, MouseY, &CursBButton.Rect ))
            {
               UpdateCursColor( B_CURS_COLOR );
            }
            else if (InRect( MouseX, MouseY, &CursXButton.Rect ))
            {
               UpdateCursColor( X_CURS_COLOR );
            }
            else if (InRect( MouseX, MouseY, &MColorRButton.Rect ))
            {
               UpdateMeshColor( R_MESH_COLOR );
            }
            else if (InRect( MouseX, MouseY, &MColorGButton.Rect ))
            {
               UpdateMeshColor( G_MESH_COLOR );
            }
            else if (InRect( MouseX, MouseY, &MColorBButton.Rect ))
            {
               UpdateMeshColor( B_MESH_COLOR );
            }
            else if (InRect( MouseX, MouseY, &MColorXButton.Rect ))
            {
               UpdateMeshColor( X_MESH_COLOR );
            }
            else if (InRect( MouseX, MouseY, &CurvesButton.Rect ))
            {
               UpdateCurves();
            }
            else if (InRect( MouseX, MouseY, &ZoomButton.Rect ))
            {
               UpdateZoomStatus();
            }
            else if (InRect( MouseX, MouseY, &OutputTGAButton.Rect ))
            {
               UpdateFileType( TGA );
            }
            else if (InRect( MouseX, MouseY, &OutputIPIButton.Rect ))
            {
               UpdateFileType( IPI );
            }
            else if (InRect( MouseX, MouseY, &OutputBMPButton.Rect ))
            {
               UpdateFileType( BMP );
            }
            else if (InRect( MouseX, MouseY, &OutputPCXButton.Rect ))
            {
               UpdateFileType( PCX );
            }
            else if (InRect( MouseX, MouseY, &OutputGIFButton.Rect ))
            {
               UpdateFileType( GIF );
            }
            else if (InRect( MouseX, MouseY, &OutputIMGButton.Rect ))
            {
               UpdateFileType( IMG );
            }
            else if (InRect( MouseX, MouseY, &TGAUpsideDownButton.Rect ))
            {
               UpdateUpsideDown();
            }
            else if (InRect( MouseX, MouseY, &SoundEnableButton.Rect ))
            {
               UpdateSoundsEnable();
            }
            else if (InRect( MouseX, MouseY, &KwikScreenButton.Rect ))
            {
               UpdateKwikScreen();
            }
            else if (InRect( MouseX, MouseY, &ShowBetweensButton.Rect ))
            {
               UpdateShowBetweens();
            }
            else if (InRect( MouseX, MouseY, &FastMorphButton.Rect ))
            {
               UpdateFastMorph();
            }
         }
         else if (MenuMode == OPERMENU)
         {
            if (InRect( MouseX, MouseY, &ExitButton.Rect ))
            {
               ExitButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ExitButton.Rect.Left, ExitButton.Rect.Top, &ExitButton );
               ShowCursor();
               Done = VerifyExit();
            }
            else if (InRect( MouseX, MouseY, &ShellButton.Rect ))
            {
               ShellButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ShellButton.Rect.Left, ShellButton.Rect.Top, &ShellButton );
               ShowCursor();
               DosShell();
            }
            else if (InRect( MouseX, MouseY, &GoButton.Rect ))
            {
               GoButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( GoButton.Rect.Left, GoButton.Rect.Top, &GoButton );
               ShowCursor();
               DoMorph();
            }
            else if (InRect( MouseX, MouseY, &HelpButton.Rect ))
            {
               HelpButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HelpButton.Rect.Left, HelpButton.Rect.Top, &HelpButton );
               ShowCursor();
               DoHelp();
            }
            else if (InRect( MouseX, MouseY, &AboutButton.Rect ))
            {
               AboutButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( AboutButton.Rect.Left, AboutButton.Rect.Top, &AboutButton );
               ShowCursor();
               DoAbout();
            }
         }
         else if (MenuMode == PREVMENU)
         {
            if (InRect( MouseX, MouseY, &PrevGoButton.Rect ))
            {
               PrevGoButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( PrevGoButton.Rect.Left, PrevGoButton.Rect.Top, &PrevGoButton );
               ShowCursor();
               DoPreview();
            }
            else if (InRect( MouseX, MouseY, &PrevFramePrompt.Rect ))
            {
               GetPrev50();
            }
            else if (InRect( MouseX, MouseY, &PrevColorButton.Rect ))
            {
               UpdatePrevColor( );
            }
         }
         else if (MenuMode == EDITMENU)
         {
            if (InRect( MouseX, MouseY, &MeshSwapButton.Rect ))
            {
               MeshSwapButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( MeshSwapButton.Rect.Left, MeshSwapButton.Rect.Top, &MeshSwapButton );
               ShowCursor();
               SwapMesh( SourceMesh, TargetMesh );
            }
            else if (InRect( MouseX, MouseY, &CopySDMButton.Rect ))
            {
               CopySDMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( CopySDMButton.Rect.Left, CopySDMButton.Rect.Top, &CopySDMButton );
               ShowCursor();
               CopyMesh( TargetMesh, SourceMesh );
            }
            else if (InRect( MouseX, MouseY, &HFlipSMButton.Rect ))
            {
               HFlipSMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipSMButton.Rect.Left, HFlipSMButton.Rect.Top, &HFlipSMButton );
               ShowCursor();
               HorizFlipMesh( SourceMesh );
            }
            else if (InRect( MouseX, MouseY, &VFlipSMButton.Rect ))
            {
               VFlipSMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipSMButton.Rect.Left, VFlipSMButton.Rect.Top, &VFlipSMButton );
               ShowCursor();
               VertFlipMesh( SourceMesh );
            }
            else if (InRect( MouseX, MouseY, &CopyDSMButton.Rect ))
            {
               CopyDSMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( CopyDSMButton.Rect.Left, CopyDSMButton.Rect.Top, &CopyDSMButton );
               ShowCursor();
               CopyMesh( SourceMesh, TargetMesh );
            }
            else if (InRect( MouseX, MouseY, &HFlipDMButton.Rect ))
            {
               HFlipDMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipDMButton.Rect.Left, HFlipDMButton.Rect.Top, &HFlipDMButton );
               ShowCursor();
               HorizFlipMesh( TargetMesh );
            }
            else if (InRect( MouseX, MouseY, &VFlipDMButton.Rect ))
            {
               VFlipDMButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipDMButton.Rect.Left, VFlipDMButton.Rect.Top, &VFlipDMButton );
               ShowCursor();
               VertFlipMesh( TargetMesh );
            }
            else if (InRect( MouseX, MouseY, &ImageSwapButton.Rect ))
            {
               ImageSwapButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ImageSwapButton.Rect.Left, ImageSwapButton.Rect.Top, &ImageSwapButton );
               ShowCursor();
               SwapImage( SourceImage, TargetImage );
            }
            else if (InRect( MouseX, MouseY, &HFlipSIButton.Rect ))
            {
               HFlipSIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipSIButton.Rect.Left, HFlipSIButton.Rect.Top, &HFlipSIButton );
               ShowCursor();
               HorizFlipImage( SourceImage );
            }
            else if (InRect( MouseX, MouseY, &VFlipSIButton.Rect ))
            {
               VFlipSIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipSIButton.Rect.Left, VFlipSIButton.Rect.Top, &VFlipSIButton );
               ShowCursor();
               VertFlipImage( SourceImage );
            }
            else if (InRect( MouseX, MouseY, &HFlipDIButton.Rect ))
            {
               HFlipDIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( HFlipDIButton.Rect.Left, HFlipDIButton.Rect.Top, &HFlipDIButton );
               ShowCursor();
               HorizFlipImage( TargetImage );
            }
            else if (InRect( MouseX, MouseY, &VFlipDIButton.Rect ))
            {
               VFlipDIButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( VFlipDIButton.Rect.Left, VFlipDIButton.Rect.Top, &VFlipDIButton );
               ShowCursor();
               VertFlipImage( TargetImage );
            }
         }
         else if (MenuMode == FILEMENU)
         {
            if (InRect( MouseX, MouseY, &MeshNamePrompt.Rect ))
            {
               GetName( MeshFile, &MeshNamePrompt );
            }
            else if (InRect( MouseX, MouseY, &LoadMeshButton.Rect ))
            {
               LoadMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( LoadMeshButton.Rect.Left, LoadMeshButton.Rect.Top, &LoadMeshButton );
               ShowCursor();
               LoadMesh();
            }
            else if (InRect( MouseX, MouseY, &SaveMeshButton.Rect ))
            {
               SaveMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( SaveMeshButton.Rect.Left, SaveMeshButton.Rect.Top, &SaveMeshButton );
               ShowCursor();
               SaveMesh();
            }
            else if (InRect( MouseX, MouseY, &ImpMeshButton.Rect ))
            {
               ImpMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ImpMeshButton.Rect.Left, ImpMeshButton.Rect.Top, &ImpMeshButton );
               ShowCursor();
               ImportMesh();
            }
            else if (InRect( MouseX, MouseY, &ExpMeshButton.Rect ))
            {
               ExpMeshButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( ExpMeshButton.Rect.Left, ExpMeshButton.Rect.Top, &ExpMeshButton );
               ShowCursor();
               ExportMesh();
            }
            else if (InRect( MouseX, MouseY, &SrcImagePrompt.Rect ))
            {
               GetName( SourceImageFile, &SrcImagePrompt );
            }
            else if (InRect( MouseX, MouseY, &LoadSourceImageButton.Rect ))
            {
               LoadSourceImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( LoadSourceImageButton.Rect.Left, LoadSourceImageButton.Rect.Top, &LoadSourceImageButton );
               ShowCursor();
               LoadImage( SourceImageFile, SourceImage );
            }
            else if (InRect( MouseX, MouseY, &SaveSourceImageButton.Rect ))
            {
               SaveSourceImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( SaveSourceImageButton.Rect.Left, SaveSourceImageButton.Rect.Top, &SaveSourceImageButton );
               ShowCursor();
               SaveImage( SourceImageFile, SourceImage );
            }
            else if (InRect( MouseX, MouseY, &DstImagePrompt.Rect ))
            {
               GetName( TargetImageFile, &DstImagePrompt );
            }
            else if (InRect( MouseX, MouseY, &LoadTargetImageButton.Rect ))
            {
               LoadTargetImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( LoadTargetImageButton.Rect.Left, LoadTargetImageButton.Rect.Top, &LoadTargetImageButton );
               ShowCursor();
               LoadImage( TargetImageFile, TargetImage );
            }
            else if (InRect( MouseX, MouseY, &SaveTargetImageButton.Rect ))
            {
               SaveTargetImageButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( SaveTargetImageButton.Rect.Left, SaveTargetImageButton.Rect.Top, &SaveTargetImageButton );
               ShowCursor();
               SaveImage( TargetImageFile, TargetImage );
            }
            else if (InRect( MouseX, MouseY, &OutCharsPrompt.Rect ))
            {
               GetName( OutBaseName, &OutCharsPrompt );
            }
            else if (InRect( MouseX, MouseY, &OutArithFilePrompt.Rect ))
            {
               GetName( OutArithFile, &OutArithFilePrompt );
            }
            else if (InRect( MouseX, MouseY, &OffOutButton.Rect ))
            {
               UpdateArithType( ARITH_OFF );
            }
            else if (InRect( MouseX, MouseY, &AveOutButton.Rect ))
            {
               UpdateArithType( ARITH_AVE );
            }
            else if (InRect( MouseX, MouseY, &AddOutButton.Rect ))
            {
               UpdateArithType( ARITH_ADD );
            }
            else if (InRect( MouseX, MouseY, &SubOutButton.Rect ))
            {
               UpdateArithType( ARITH_SUB );
            }
            else if (InRect( MouseX, MouseY, &OvrOutButton.Rect ))
            {
               UpdateArithType( ARITH_OVR );
            }
            else if (InRect( MouseX, MouseY, &OvrThreshPromptL.Rect ))
            {
               GetOvrThreshL();
            }
            else if (InRect( MouseX, MouseY, &OvrThreshPromptH.Rect ))
            {
               GetOvrThreshH();
            }
            else if (InRect( MouseX, MouseY, &BackOffButton.Rect ))
            {
               UpdateBackground( BACK_OFF );
            }
            else if (InRect( MouseX, MouseY, &BackOnButton.Rect ))
            {
               UpdateBackground( BACK_ON );
            }
            else if (InRect( MouseX, MouseY, &BackFilePrompt.Rect ))
            {
               GetName( BackgroundFile, &BackFilePrompt );
            }
            else if (InRect( MouseX, MouseY, &BackThreshPromptL.Rect ))
            {
               GetBackThreshL();
            }
            else if (InRect( MouseX, MouseY, &BackThreshPromptH.Rect ))
            {
               GetBackThreshH();
            }
         }
         else if (MenuMode == PROCMENU)
         {
            if (InRect( MouseX, MouseY, &FilterFileNamePrompt.Rect ))
            {
               SetFilterName();
            }
            else if (InRect( MouseX, MouseY, &SourceFilterNamePrompt.Rect ))
            {
               SourceFilter();
            }
            else if (InRect( MouseX, MouseY, &DoSourceFilterButton.Rect ))
            {
               DoSourceFilterButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( DoSourceFilterButton.Rect.Left, DoSourceFilterButton.Rect.Top, &DoSourceFilterButton );
               ShowCursor();
               DoSourceFilter();
            }
            else if (InRect( MouseX, MouseY, &TargetFilterNamePrompt.Rect ))
            {
               TargetFilter();
            }
            else if (InRect( MouseX, MouseY, &DoTargetFilterButton.Rect ))
            {
               DoTargetFilterButton.State = BUTTON_UP;
               HideCursor();
               DrawButton( DoTargetFilterButton.Rect.Left, DoTargetFilterButton.Rect.Top, &DoTargetFilterButton );
               ShowCursor();
               DoTargetFilter();
            }
         }
         else if (MenuMode == FLICMENU)
         {
            if (InRect( MouseX, MouseY, &FlicFLIButton.Rect ))
            {
               UpdateFlicType( FLI );
            }
            else if (InRect( MouseX, MouseY, &FlicFLCButton.Rect ))
            {
               UpdateFlicType( FLC );
            }
            else if (InRect( MouseX, MouseY, &FlicFLXButton.Rect ))
            {
               UpdateFlicType( FLX );
            }
            else if (InRect( MouseX, MouseY, &FlicFLHButton.Rect ))
            {
               UpdateFlicType( FLH );
            }
            else if (InRect( MouseX, MouseY, &FlicFLTButton.Rect ))
            {
               UpdateFlicType( FLT );
            }
            else if (InRect( MouseX, MouseY, &FlicPingButton.Rect ))
            {
               UpdateFlicPing( );
            }
            else if (InRect( MouseX, MouseY, &FlicOnOffButton.Rect ))
            {
               UpdateFlicOnOff( );
            }
            else if (InRect( MouseX, MouseY, &FlicColorButton.Rect ))
            {
               UpdateFlicColor( );
            }
            else if (InRect( MouseX, MouseY, &FlicFilePrompt.Rect ))
            {
               GetFlicName( );
            }
            else if (InRect( MouseX, MouseY, &FlicSpeedPrompt.Rect ))
            {
               GetFlicSpeed( );
            }
         }
      }
      else if (Left && !OldLeft)
      {
         if (MenuMode == MAINMENU)
         {
            if (InRect( MouseX, MouseY, &PerfButton.Rect ))
            {
               PerfButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( PerfButton.Rect.Left, PerfButton.Rect.Top, &PerfButton );
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &OperButton.Rect ))
            {
               OperButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( OperButton.Rect.Left, OperButton.Rect.Top, &OperButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &EditButton.Rect ))
            {
               EditButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( EditButton.Rect.Left, EditButton.Rect.Top, &EditButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &FileButton.Rect ))
            {
               FileButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( FileButton.Rect.Left, FileButton.Rect.Top, &FileButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &ProcButton.Rect ))
            {
               ProcButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( ProcButton.Rect.Left, ProcButton.Rect.Top, &ProcButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &PrevButton.Rect ))
            {
               PrevButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( PrevButton.Rect.Left, PrevButton.Rect.Top, &PrevButton);
               ShowCursor();
            }
#ifndef NOREG
            else if (InRect( MouseX, MouseY, &FlicButton.Rect ))
            {
               FlicButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( FlicButton.Rect.Left, FlicButton.Rect.Top, &FlicButton);
               ShowCursor();
            }
#endif
         }
         else if (MenuMode == PERFMENU)
         {
         }
         else if (MenuMode == OPERMENU)
         {
            if (InRect( MouseX, MouseY, &ExitButton.Rect ))
            {
               ExitButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( ExitButton.Rect.Left, ExitButton.Rect.Top, &ExitButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &ShellButton.Rect ))
            {
               ShellButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( ShellButton.Rect.Left, ShellButton.Rect.Top, &ShellButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &GoButton.Rect ))
            {
               GoButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( GoButton.Rect.Left, GoButton.Rect.Top, &GoButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &HelpButton.Rect ))
            {
               HelpButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( HelpButton.Rect.Left, HelpButton.Rect.Top, &HelpButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &AboutButton.Rect ))
            {
               AboutButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( AboutButton.Rect.Left, AboutButton.Rect.Top, &AboutButton);
               ShowCursor();
            }
         }
         else if (MenuMode == PREVMENU)
         {
            if (InRect( MouseX, MouseY, &PrevGoButton.Rect ))
            {
               PrevGoButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( PrevGoButton.Rect.Left, PrevGoButton.Rect.Top, &PrevGoButton);
               ShowCursor();
            }
         }
         else if (MenuMode == EDITMENU)
         {
            if (InRect( MouseX, MouseY, &MeshSwapButton.Rect ))
            {
               MeshSwapButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( MeshSwapButton.Rect.Left, MeshSwapButton.Rect.Top, &MeshSwapButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &CopySDMButton.Rect ))
            {
               CopySDMButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( CopySDMButton.Rect.Left, CopySDMButton.Rect.Top, &CopySDMButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &HFlipSMButton.Rect ))
            {
               HFlipSMButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( HFlipSMButton.Rect.Left, HFlipSMButton.Rect.Top, &HFlipSMButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &VFlipSMButton.Rect ))
            {
               VFlipSMButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( VFlipSMButton.Rect.Left, VFlipSMButton.Rect.Top, &VFlipSMButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &CopyDSMButton.Rect ))
            {
               CopyDSMButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( CopyDSMButton.Rect.Left, CopyDSMButton.Rect.Top, &CopyDSMButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &HFlipDMButton.Rect ))
            {
               HFlipDMButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( HFlipDMButton.Rect.Left, HFlipDMButton.Rect.Top, &HFlipDMButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &VFlipDMButton.Rect ))
            {
               VFlipDMButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( VFlipDMButton.Rect.Left, VFlipDMButton.Rect.Top, &VFlipDMButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &ImageSwapButton.Rect ))
            {
               ImageSwapButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( ImageSwapButton.Rect.Left, ImageSwapButton.Rect.Top, &ImageSwapButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &HFlipSIButton.Rect ))
            {
               HFlipSIButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( HFlipSIButton.Rect.Left, HFlipSIButton.Rect.Top, &HFlipSIButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &VFlipSIButton.Rect ))
            {
               VFlipSIButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( VFlipSIButton.Rect.Left, VFlipSIButton.Rect.Top, &VFlipSIButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &HFlipDIButton.Rect ))
            {
               HFlipDIButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( HFlipDIButton.Rect.Left, HFlipDIButton.Rect.Top, &HFlipDIButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &VFlipDIButton.Rect ))
            {
               VFlipDIButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( VFlipDIButton.Rect.Left, VFlipDIButton.Rect.Top, &VFlipDIButton);
               ShowCursor();
            }
         }
         else if (MenuMode == FILEMENU)
         {
            if (InRect( MouseX, MouseY, &LoadMeshButton.Rect ))
            {
               LoadMeshButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( LoadMeshButton.Rect.Left, LoadMeshButton.Rect.Top, &LoadMeshButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &SaveMeshButton.Rect ))
            {
               SaveMeshButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( SaveMeshButton.Rect.Left, SaveMeshButton.Rect.Top, &SaveMeshButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &ImpMeshButton.Rect ))
            {
               ImpMeshButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( ImpMeshButton.Rect.Left, ImpMeshButton.Rect.Top, &ImpMeshButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &ExpMeshButton.Rect ))
            {
               ExpMeshButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( ExpMeshButton.Rect.Left, ExpMeshButton.Rect.Top, &ExpMeshButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &LoadSourceImageButton.Rect ))
            {
               LoadSourceImageButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( LoadSourceImageButton.Rect.Left, LoadSourceImageButton.Rect.Top, &LoadSourceImageButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &SaveSourceImageButton.Rect ))
            {
               SaveSourceImageButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( SaveSourceImageButton.Rect.Left, SaveSourceImageButton.Rect.Top, &SaveSourceImageButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &LoadTargetImageButton.Rect ))
            {
               LoadTargetImageButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( LoadTargetImageButton.Rect.Left, LoadTargetImageButton.Rect.Top, &LoadTargetImageButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &SaveTargetImageButton.Rect ))
            {
               SaveTargetImageButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( SaveTargetImageButton.Rect.Left, SaveTargetImageButton.Rect.Top, &SaveTargetImageButton);
               ShowCursor();
            }
         }
         else if (MenuMode == PROCMENU)
         {
            if (InRect( MouseX, MouseY, &DoSourceFilterButton.Rect ))
            {
               DoSourceFilterButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( DoSourceFilterButton.Rect.Left, DoSourceFilterButton.Rect.Top, &DoSourceFilterButton);
               ShowCursor();
            }
            else if (InRect( MouseX, MouseY, &DoTargetFilterButton.Rect ))
            {
               DoTargetFilterButton.State = BUTTON_DOWN;
               HideCursor();
               DrawButton( DoTargetFilterButton.Rect.Left, DoTargetFilterButton.Rect.Top, &DoTargetFilterButton);
               ShowCursor();
            }
         }
      }

      OldLeft = Left;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetNumericPrompt( int Chars, int Min, int Max, PROMPT Prompt, int *Default )
{
int   CurVal, Done = 0, CurLen = 0, Key, CursorPos;
char  TempStr[80], WorkingString[80];

   sprintf( WorkingString, "%d", *Default );
   CurLen = strlen( WorkingString );

   CursorPos = (CurLen > Chars) ? Chars:CurLen;

   while( !Done )
   {
      _setcolor(BACK_COLOR);
      _rectangle( _GFILLINTERIOR, Prompt->Rect.Left+2, Prompt->Rect.Top+2, Prompt->Rect.Right-2, Prompt->Rect.Bottom-2 );

      _setcolor(ENTER_TEXT_COLOR);
      _moveto( Prompt->Rect.Left + 3, Prompt->Rect.Top + 5 );
      _outgtext( WorkingString );

      Key = GetCursorKey(Prompt->Rect.Left + 3 + CursorPos*TEXT_WIDTH, Prompt->Rect.Top + 5 + TEXT_HEIGHT);

      if (Key >= '0' && Key <= '9')
      {
         strcpy( TempStr, WorkingString );
         TempStr[CursorPos] = Key;
         strcpy(&TempStr[CursorPos+1], &WorkingString[CursorPos]);
         strcpy( WorkingString, TempStr );
         CurLen++;
         CursorPos++;
      }
      else
      {
         switch( Key )
         {
            case 8:                    // Backspace
               if (CursorPos)
               {
                  CurLen--;
                  CursorPos--;
                  strcpy( TempStr, WorkingString );
                  strcpy( &TempStr[CursorPos], &WorkingString[CursorPos+1] );
                  strcpy( WorkingString, TempStr );
               }
               break;

            case 13:                   // CR
               Done = 1;
               break;

            case 27:                   // Escape
               return;

            case 0:                    // Unused, don't accept remaining code
               switch( getch() )
               {
                  case 0x4b:           // Left arrow
                     CursorPos--;
                     break;

                  case 0x4d:           // Right arrow
                     CursorPos++;
                     break;

                  case 0x47:           // Home
                     CursorPos = 0;
                     break;

                  case 0x4f:           // End
                     CursorPos = Chars-1;
                     break;

                  case 0x53:           // Delete
                     if (CurLen)
                     {
                        CurLen--;
                        strcpy( TempStr, WorkingString );
                        strcpy( &TempStr[CursorPos], &WorkingString[CursorPos+1] );
                        strcpy( WorkingString, TempStr );
                     }
                     break;

                  case ALT_C:
                     if (DebugMode)
                        DoScreenCapture( );

                  /* Fall through! */

                  default:
                     Beep( );
                     break;
               }
               break;

            default:                   // Invalid input
               Beep( );
               break;
         }
      }

      if (CurLen > Chars)
         CurLen = Chars;

      if (CursorPos > Chars-1)
         CursorPos = Chars-1;

      if (CursorPos > CurLen)
         CursorPos = CurLen;

      if (CurLen < 0)
         CurLen = 0;

      if (CursorPos < 0)
         CursorPos = 0;

      WorkingString[CurLen] = '\0';
   }

   CurVal = atoi( WorkingString );

   if (CurVal > Max)
   {
      sprintf( TempStr, "The value you entered is too high (maximum is %d)", Max );
      WarningBox( TempStr );
   }
   else if (CurVal < Min)
   {
      sprintf( TempStr, "The value you entered is too low (minimum is %d)", Min );
      WarningBox( TempStr );
   }
   else
   {
      *Default = CurVal;
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   GetAlphaNumericPrompt( int Chars, PROMPT Prompt, char *Default )
{
int   Done = 0, CurLen = 0, Key, CursorPos;
char  TempStr[80], WorkingString[80];

   strcpy( WorkingString, Default );
   CurLen = strlen( WorkingString );

   CursorPos = (CurLen > Chars) ? Chars:CurLen;

   while( !Done )
   {
      _setcolor(BACK_COLOR);
      _rectangle( _GFILLINTERIOR, Prompt->Rect.Left+2, Prompt->Rect.Top+2, Prompt->Rect.Right-2, Prompt->Rect.Bottom-2 );

      _setcolor(ENTER_TEXT_COLOR);
      _moveto( Prompt->Rect.Left + 3, Prompt->Rect.Top + 5 );
      _outgtext( WorkingString );

      Key = GetCursorKey(Prompt->Rect.Left + 3 + CursorPos*TEXT_WIDTH, Prompt->Rect.Top + 5 + TEXT_HEIGHT);

      if ((Key >= '0' && Key <= '9') || 
          (Key >= 'a' && Key <= 'z') ||
          (Key >= 'A' && Key <= 'Z') ||
          Key == '.' || Key == '!' || Key == '\\' || Key == '@' ||
          Key == '#' || Key == '$' || Key == '%%' || Key == '^' ||
          Key == '&' || Key == '*' || Key == '-'  || Key == '_' ||
          Key == '+' || Key == '=' || Key == '?'  || Key == ':')
      {
         strcpy( TempStr, WorkingString );
         TempStr[CursorPos] = Key;
         strcpy(&TempStr[CursorPos+1], &WorkingString[CursorPos]);
         strcpy( WorkingString, TempStr );
         CurLen++;
         CursorPos++;
      }
      else
      {
         switch( Key )
         {
            case 8:                    // Backspace
               if (CursorPos)
               {
                  CurLen--;
                  CursorPos--;
                  strcpy( TempStr, WorkingString );
                  strcpy( &TempStr[CursorPos], &WorkingString[CursorPos+1] );
                  strcpy( WorkingString, TempStr );
               }
               break;

            case 13:                   // CR
               Done = 1;
               break;

            case 27:                   // Escape
               return(0);

            case 0:                    // Unused, don't accept remaining code
               switch( getch() )
               {
                  case 0x4b:           // Left arrow
                     CursorPos--;
                     break;

                  case 0x4d:           // Right arrow
                     CursorPos++;
                     break;

                  case 0x47:           // Home
                     CursorPos = 0;
                     break;

                  case 0x4f:           // End
                     CursorPos = Chars-1;
                     break;

                  case 0x53:           // Delete
                     if (CurLen)
                     {
                        CurLen--;
                        strcpy( TempStr, WorkingString );
                        strcpy( &TempStr[CursorPos], &WorkingString[CursorPos+1] );
                        strcpy( WorkingString, TempStr );
                     }
                     break;

                  case ALT_C:
                     if (DebugMode)
                        DoScreenCapture( );

                  /* Fall through! */

                  default:
                     Beep( );
                     break;
               }
               break;

            default:                   // Invalid input
               Beep( );
               break;
         }
      }

      if (CurLen > Chars)
         CurLen = Chars;

      if (CursorPos > Chars-1)
         CursorPos = Chars-1;

      if (CursorPos > CurLen)
         CursorPos = CurLen;

      if (CurLen < 0)
         CurLen = 0;

      if (CursorPos < 0)
         CursorPos = 0;

      WorkingString[CurLen] = '\0';
   }

   strcpy(Default, WorkingString);
   strupr(Default);

   return(1);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   GetCursorKey( int x, int y )
{
int   i, onflag = 0;

   _setplotaction(_GXOR);
   _setcolor(TEXT_CURSOR_COLOR);

   while( !kbhit() )
   {
      _moveto( x, y-1 );
      _lineto( x + TEXT_WIDTH-2, y-1 );
      _moveto( x, y );
      _lineto( x + TEXT_WIDTH-2, y );
      onflag = !onflag;

      for( i = 0; i < BLINK_10MSECS; i++)
      {
         delay( 10 );
         if (kbhit()) break;
      }
   }

   if (onflag)
   {
      _moveto( x, y-1 );
      _lineto( x + TEXT_WIDTH-2, y-1 );
      _moveto( x, y );
      _lineto( x + TEXT_WIDTH-2, y );
   }

   _setplotaction(_GPSET);

   return(getch());
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   VerifyExit( )
{
int   Key, i;

   HideCursor();

   DrawMessBox( "Question", "Are You Sure [y/N]?", ERROR_BOX_COLOR );

   if (!QuietMode)
   {
      for( i = 5000; i >= 1000; i-=100 )
      {
         sound(i);
         delay(1);
      }

      nosound();
   }
   Key = GetKey( );

   if (toupper(Key) == 'Y')
      return(1);

   EraseMessBox( );

   ShowCursor();

   return(0);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DosShell( )
{
   ChangeMenu( MAINMENU );

   HideCursor();

   RestoreMode( );

   printf( "ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ\n" );
   printf( "Û                                         Û\n" );                       
   printf( "Û To return to \"The Complete Morph\" type: Û\n" );
   printf( "Û     EXIT [RETURN]                       Û\n" );
   printf( "Û                                         Û\n" );                       
   printf( "ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ\n" );
   system( "COMMAND.COM" );

   SetGraphicsMode( GraphMode );

   InitScreen( SourceImage, TargetImage );

   RestoreImage( SourceImage );
   RestoreImage( TargetImage );

   DrawMesh( SourceMesh, SourceImage);
   DrawMesh( TargetMesh, TargetImage);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ChangeMenu( int Mode )
{
   if (MenuMode == Mode)
      return;

   if (MenuMode == PERFMENU)
      HidePreferencesWindow();

   if (MenuMode == OPERMENU)
      HideOperationsWindow();

   if (MenuMode == EDITMENU)
      HideEditWindow();

   if (MenuMode == FILEMENU)
      HideFileWindow();

   if (MenuMode == PROCMENU)
      HideProcessesWindow();

   if (MenuMode == PREVMENU)
      HidePrevWindow();

   if (MenuMode == FLICMENU)
      HideFlicWindow();

   MenuMode = Mode;

   HideCursor();
   UpdateButtons();
   ShowCursor();

   if (UpdateImages)
   {
      RestoreImage( SourceImage );
      DrawMesh( SourceMesh, SourceImage );
      RestoreImage( TargetImage );
      DrawMesh( TargetMesh, TargetImage );
      UpdateImages = 0;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoMorph( )
{
int   RetCode;

   ChangeMenu( MAINMENU );

   HideCursor();

   RetCode = Morph( SourceImage, SourceMesh, TargetImage, TargetMesh, NO_SINGLE );

   if (RetCode != E_NONE)
      ErrorBox( "Unable to morph", RetCode );

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoPreview( )
{
int   RetCode;

   ChangeMenu( MAINMENU );

   HideCursor();

   RetCode = Morph( SourceImage, SourceMesh, TargetImage, TargetMesh, SINGLE );

   if (RetCode != E_NONE)
      ErrorBox( "Unable to morph", RetCode );

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoHelp( )
{
static   char HelpString[4000];

   HideCursor();

   strcpy( HelpString, "Alt-Aúúúúúúúúúú About CMorph\n" );
   strcat( HelpString, "Alt-Dúúúúúúúúúú Dos shell\n" );
   strcat( HelpString, "Alt-Eúúúúúúúúúú Change to edit menu\n" );
   strcat( HelpString, "Alt-Fúúúúúúúúúú Change to file menu\n" );
   strcat( HelpString, "Alt-Gúúúúúúúúúú Change to go menu\n" );
   strcat( HelpString, "Alt-Iúúúúúúúúúú Change to filters menu\n" );
#ifndef NOREG
   strcat( HelpString, "Alt-Lúúúúúúúúúú Change to flic menu\n" );
#endif
   strcat( HelpString, "Alt-Púúúúúúúúúú Change to preferences menu\n" );
   strcat( HelpString, "Alt-Rúúúúúúúúúú Change to preview menu\n" );
   strcat( HelpString, "Alt-Súúúúúúúúúú Start morphing/warping/fading\n" );
   strcat( HelpString, "Alt-Vúúúúúúúúúú Start preview\n" );
   strcat( HelpString, "Alt-Xúúúúúúúúúú Exit\n" );
   strcat( HelpString, "Escapeúúúúúúúúú Backup a menu\n" );
   strcat( HelpString, "\n" );
   strcat( HelpString, "\n" );
   strcat( HelpString, "            Press any key to exit" );

   DrawMessBox( "Help", HelpString, HELP_BOX_COLOR );
   GetKey();

   EraseMessBox( );

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoAbout( )
{
static   char AboutString[4000], Note[80];

   HideCursor();

   strcpy( AboutString, "                                                 \n" );
   strcat( AboutString, "           Written by:  Paul D. Nettle           \n" );
   strcat( AboutString, "             (Registered ASP member)             \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "          Produced by: Hot Wax Software          \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "       Copyright 1993, 1994 Paul D. Nettle       \n" );
   strcat( AboutString, "                                                 \n" );

#ifdef REG
   strcat( AboutString, "     This copy of CMorph has been registered.    \n" );
   strcat( AboutString, "      Please do not distribute this software     \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "       AND THANKS FOR SUPPORTING SHAREWARE!      \n" );
#endif

#ifdef NOREG
   strcat( AboutString, "      This is an unregistered copy of CMorph     \n" );
   strcat( AboutString, "     If you like this program, please support    \n" );
   strcat( AboutString, "           it and the shareware concept          \n" );
   strcat( AboutString, "             by registering it today!            \n" );
   strcat( AboutString, "             (see REGISTER.DOC file)             \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "      You may regiser on-line via the SWREG      \n" );
   strcat( AboutString, "     forum on CompuServe, just use ID: #1629     \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "     ....or purchase your commercial version     \n" );
   strcat( AboutString, "     of CMorph PRO! from Pearl Agency today!     \n" );
#endif

#ifdef PRO
   strcat( AboutString, "           This is commercial software.          \n" );
   strcat( AboutString, "      Please do not distribute this software     \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "           AND THANKS FOR YOUR SUPPORT!          \n" );
#endif

   strcat( AboutString, "           ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ          \n" );
   strcat( AboutString, "                                                 \n" );
   strcat( AboutString, "     [Press any key or click mouse to return]    " );

#ifdef NOREG
   sprintf( Note,       " CMorph -- The Complete Morph!  version %d.%02dub", VERSION_MS, VERSION_LS );
#endif

#ifdef REG
   sprintf( Note,       " CMorph -- The Complete Morph!  version %d.%02drb", VERSION_MS, VERSION_LS );
#endif

#ifdef PRO
   sprintf( Note,       "CMorph -- The Complete Morph Pro! version %d.%02db", VERSION_MS, VERSION_LS );
#endif

   DrawMessBox( Note, AboutString, HELP_BOX_COLOR );

   GetKey();

   EraseMessBox( );

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoBackDoor( )
{
static   char BackDoorString[4000];

   HideCursor();

   strcpy( BackDoorString, "\n" );
   strcat( BackDoorString, "KA:  You stupid bastard, you've got no\n" );
   strcat( BackDoorString, "     left!\n" );
   strcat( BackDoorString, "\n" );
   strcat( BackDoorString, "BK:  Yes I have.\n" );
   strcat( BackDoorString, "\n" );
   strcat( BackDoorString, "KA:  Look!\n" );
   strcat( BackDoorString, "\n" );
   strcat( BackDoorString, "BK:  'Tis just a flesh wound.\n" );
   strcat( BackDoorString, "\n" );
   strcat( BackDoorString, "         Press any key to exit" );

   DrawMessBox(            "--------------> BackDoor <-------------", BackDoorString, HELP_BOX_COLOR );

   GetKey();

   EraseMessBox( );

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DoScreenCapture( )
{
int   x, y, col, colr, colg, colb;
static   int   CurOff = 0;
static   char  BackDoorString[4000], FileName[80];
FILE  *FilePointer;

   Beep();

   sprintf( FileName, "Screen%02d.tga", CurOff++ );

   FilePointer = fopen(FileName, "wb");

   if(!FilePointer)
   {
      Beep();
      return;
   }

   fputc(0, FilePointer);
   fputc(0, FilePointer);

   fputc(2, FilePointer);

   for(x = 3; x < 12; x++)
      fputc(0, FilePointer);

   fputc(GI->ResX & 0xff, FilePointer);
   fputc(GI->ResX >> 8, FilePointer);
   fputc(GI->ResY & 0xff, FilePointer);
   fputc(GI->ResY >> 8, FilePointer);

   fputc(24, FilePointer);

   if (TGAUpsideDownFlag == TOP_DOWN)
      fputc(32, FilePointer);
   else
      fputc(0, FilePointer);

/* ENCODE THIS IMAGE */

   if (TGAUpsideDownFlag == TOP_DOWN)
      for( y = 0; y < GI->ResY; y++ )
      {
         for( x = 0; x < GI->ResX; x++ )
         {
            col = _getpixel( x, y );

            if (col <= GREY_END)
            {
               colr = col<<2;
               colg = col<<2;
               colb = col<<2;
            }
            else if (col <= RED_END)
            {
               colr = col<<2;
               colg = 0;
               colb = 0;
            }
            else if (col <= GREEN_END)
            {
               colr = 0;
               colg = col<<2;
               colb = 0;
            }
            else if (col <= BLUE_END)
            {
               colr = 0;
               colg = 0;
               colb = col<<2;
            }

            fputc(colb, FilePointer);
            fputc(colg, FilePointer);
            fputc(colr, FilePointer);
         }
      }
   else
      for( y = GI->ResY-1; y >= 0; y-- )
      {
         for( x = 0; x < GI->ResX; x++ )
         {
            col = _getpixel( x, y );

            if (col <= GREY_END)
            {
               colr = col<<2;
               colg = col<<2;
               colb = col<<2;
            }
            else if (col <= RED_END)
            {
               colr = col<<2;
               colg = 0;
               colb = 0;
            }
            else if (col <= GREEN_END)
            {
               colr = 0;
               colg = col<<2;
               colb = 0;
            }
            else if (col <= BLUE_END)
            {
               colr = 0;
               colg = 0;
               colb = col<<2;
            }

            fputc(colb, FilePointer);
            fputc(colg, FilePointer);
            fputc(colr, FilePointer);
         }
      }
        
   fclose(FilePointer);

   HideCursor();

   strcpy( BackDoorString, "                                   \n" );
   strcat( BackDoorString, "         Capture complete!         \n" );
   strcat( BackDoorString, "                                   \n" );
   strcat( BackDoorString, "       Press any key to exit       " );

   DrawMessBox("---------> ScreenCapture <---------", BackDoorString, HELP_BOX_COLOR );

   GetKey();

   EraseMessBox( );

   ShowCursor();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   GetFileName( char *FileName, PROMPT Prompt )
{
int      Length, RetCode;
char     TempStr[80];

   Length = (Prompt->Rect.Right - Prompt->Rect.Left) / TEXT_WIDTH;

   strcpy( TempStr, FileName );

   RetCode = GetAlphaNumericPrompt( Length, Prompt, TempStr );

   if (RetCode)
      strcpy( FileName, TempStr );

   return(RetCode);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [INPUT.C     ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
