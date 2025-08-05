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
  ±   [PROMPTS.C   ] - Get and handle user input                             ±
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

void  SetFilterName( )
{
   HideCursor();

   GetFileName( FilterFileName, &FilterFileNamePrompt );

   DrawPromptTBox( FilterFileNamePrompt.Rect.Left, FilterFileNamePrompt.Rect.Top, FilterFileName, &FilterFileNamePrompt );

   ShowCursor();

   UpdateFastProc = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFlicName( )
{
   HideCursor();

   GetFileName( FlicFileName, &FlicFilePrompt );

   DrawPromptTBox( FlicFilePrompt.Rect.Left, FlicFilePrompt.Rect.Top, FlicFileName, &FlicFilePrompt );

   ShowCursor();

   UpdateFastFlic = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFlicSpeed( )
{
   HideCursor();

   GetNumericPrompt( 2, 0, 99, &FlicSpeedPrompt, &FlicSpeed );

   DrawPromptBox( FlicSpeedPrompt.Rect.Left, FlicSpeedPrompt.Rect.Top, FlicSpeed, &FlicSpeedPrompt );

   ShowCursor();

   UpdateFastFlic = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SourceFilter( )
{
   HideCursor();

   GetFileName( SourceFiltName, &SourceFilterNamePrompt );

   DrawPromptTBox( SourceFilterNamePrompt.Rect.Left, SourceFilterNamePrompt.Rect.Top, SourceFiltName, &SourceFilterNamePrompt );

   ShowCursor();

   UpdateFastProc = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  TargetFilter( )
{
   HideCursor();

   GetFileName( TargetFiltName, &TargetFilterNamePrompt );

   DrawPromptTBox( TargetFilterNamePrompt.Rect.Left, TargetFilterNamePrompt.Rect.Top, TargetFiltName, &TargetFilterNamePrompt );

   ShowCursor();

   UpdateFastProc = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetName( char *FileName, PROMPT Prompt )
{
   HideCursor();

   GetFileName( FileName, Prompt );

   DrawPromptTBox( Prompt->Rect.Left, Prompt->Rect.Top, FileName, Prompt );

   ShowCursor();

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFrameOff( )
{
   HideCursor();

   GetNumericPrompt( 3, 0, 999, &FrameOffPrompt, &FrameOffset );

   DrawPromptBox( FrameOffPrompt.Rect.Left, FrameOffPrompt.Rect.Top, FrameOffset, &FrameOffPrompt );

   ShowCursor();

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFrames( )
{
   HideCursor();

   GetNumericPrompt( 3, 1, 999, &FramesPrompt, &TotalFrames );

   DrawPromptBox( FramesPrompt.Rect.Left, FramesPrompt.Rect.Top, TotalFrames, &FramesPrompt );

   if (TotalFrames < EndFrame - StartFrame + 1)
   {
      WarningBox( "Adjusting the ending frame number" );
      EndFrame = StartFrame + TotalFrames - 1;
      DrawPromptBox( EndFramePrompt.Rect.Left, EndFramePrompt.Rect.Top, EndFrame, &EndFramePrompt );
   }

   ShowCursor();

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetStartFrame( )
{
   HideCursor();

   GetNumericPrompt( 3, 1, 999, &StartFramePrompt, &StartFrame );

   DrawPromptBox( StartFramePrompt.Rect.Left, StartFramePrompt.Rect.Top, StartFrame, &StartFramePrompt );

   if (StartFrame > EndFrame)
   {
      WarningBox( "Adjusting the ending frame number" );
      EndFrame = StartFrame;
      DrawPromptBox( EndFramePrompt.Rect.Left, EndFramePrompt.Rect.Top, EndFrame, &EndFramePrompt );
   }

   if (StartFrame < EndFrame - TotalFrames + 1)
   {
      WarningBox( "Adjusting the total frame number" );
      TotalFrames = EndFrame - StartFrame + 1;
      DrawPromptBox( FramesPrompt.Rect.Left, FramesPrompt.Rect.Top, TotalFrames, &FramesPrompt );
   }

   ShowCursor();

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetEndFrame( )
{
   HideCursor();

   GetNumericPrompt( 3, 1, 999, &EndFramePrompt, &EndFrame );

   DrawPromptBox( EndFramePrompt.Rect.Left, EndFramePrompt.Rect.Top, EndFrame, &EndFramePrompt );

   if (EndFrame < StartFrame)
   {
      WarningBox( "Adjusting the starting frame number" );
      StartFrame = EndFrame;
      DrawPromptBox( StartFramePrompt.Rect.Left, StartFramePrompt.Rect.Top, StartFrame, &StartFramePrompt );
   }

   if (EndFrame > StartFrame + TotalFrames - 1)
   {
      WarningBox( "Adjusting the total frame number" );
      TotalFrames = EndFrame - StartFrame + 1;
      DrawPromptBox( FramesPrompt.Rect.Left, FramesPrompt.Rect.Top, TotalFrames, &FramesPrompt );
   }

   ShowCursor();

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetWarpTightness( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, 100, &WarpTightnessPrompt, &WarpTightness );

   DrawPromptBox( WarpTightnessPrompt.Rect.Left, WarpTightnessPrompt.Rect.Top, WarpTightness, &WarpTightnessPrompt );
   
   ShowCursor( );

   UpdateImages = 1;

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetInterpTightness( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, 100, &InterpTightnessPrompt, &InterpTightness );

   DrawPromptBox( InterpTightnessPrompt.Rect.Left, InterpTightnessPrompt.Rect.Top, InterpTightness, &InterpTightnessPrompt );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetPrev50( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, 100, &PrevFramePrompt, &Prev50 );

   DrawPromptBox( PrevFramePrompt.Rect.Left, PrevFramePrompt.Rect.Top, Prev50, &PrevFramePrompt );

   ShowCursor( );

   UpdateFastPrev = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFade50( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, 100, &Fade50Prompt, &Fade50 );

   DrawPromptBox( Fade50Prompt.Rect.Left, Fade50Prompt.Rect.Top, Fade50, &Fade50Prompt );

   ShowCursor( );

   UpdateFastPref = 1;
}
 
/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFadeStart( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, FEnd, &FadeStartPrompt, &FStart );

   DrawPromptBox( FadeStartPrompt.Rect.Left, FadeStartPrompt.Rect.Top, FStart, &FadeStartPrompt );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetFadeEnd( )
{
   HideCursor( );

   GetNumericPrompt( 3, FStart, 100, &FadeEndPrompt, &FEnd );

   DrawPromptBox( FadeEndPrompt.Rect.Left, FadeEndPrompt.Rect.Top, FEnd, &FadeEndPrompt );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetWarp50( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, 100, &Warp50Prompt, &Warp50 );

   DrawPromptBox( Warp50Prompt.Rect.Left, Warp50Prompt.Rect.Top, Warp50, &Warp50Prompt );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetWarpStart( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, WEnd, &WarpStartPrompt, &WStart );

   DrawPromptBox( WarpStartPrompt.Rect.Left, WarpStartPrompt.Rect.Top, WStart, &WarpStartPrompt );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetWarpEnd( )
{
   HideCursor( );

   GetNumericPrompt( 3, WStart, 100, &WarpEndPrompt, &WEnd );

   DrawPromptBox( WarpEndPrompt.Rect.Left, WarpEndPrompt.Rect.Top, WEnd, &WarpEndPrompt );

   ShowCursor( );

   UpdateFastPref = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawPromptBox( int X, int Y, int CurVal, PROMPT Prompt )
{
char  TempStr[80];
int   Width, Height;

   Width = Prompt->WidthChars * TEXT_WIDTH + 4;
   Height = TEXT_HEIGHT + 4;

   Prompt->Rect.Left   = X;
   Prompt->Rect.Top    = Y;
   Prompt->Rect.Right  = X + Width;
   Prompt->Rect.Bottom = Y + Height + 3;

   ShadeRect( &Prompt->Rect, PROMPT_COLOR );
   DrawOutGroove( Prompt->Rect.Left, Prompt->Rect.Top, Prompt->Rect.Right, Prompt->Rect.Bottom );

   _setcolor(PROMPT_TEXT_COLOR);
   _moveto( Prompt->Rect.Left + 3, Prompt->Rect.Top + 5 );
   sprintf( TempStr, "%d", CurVal );
   _outgtext( TempStr );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawPromptTBox( int X, int Y, char *String, PROMPT Prompt )
{
char  TempStr[80];
int   Width, Height;

   Width = Prompt->WidthChars * TEXT_WIDTH + 4;
   Height = TEXT_HEIGHT + 4;

   Prompt->Rect.Left   = X;
   Prompt->Rect.Top    = Y;
   Prompt->Rect.Right  = X + Width;
   Prompt->Rect.Bottom = Y + Height + 3;

   ShadeRect( &Prompt->Rect, PROMPT_COLOR );
   DrawOutGroove( Prompt->Rect.Left, Prompt->Rect.Top, Prompt->Rect.Right, Prompt->Rect.Bottom );

   _setcolor(PROMPT_TEXT_COLOR);
   _moveto( Prompt->Rect.Left + 3, Prompt->Rect.Top + 5 );
   sprintf( TempStr, "%s", String );
   _outgtext( TempStr );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetOvrThreshL( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, OverlayThresholdH, &OvrThreshPromptL, &OverlayThresholdL );

   DrawPromptBox( OvrThreshPromptL.Rect.Left, OvrThreshPromptL.Rect.Top, OverlayThresholdL, &OvrThreshPromptL );
   
   ShowCursor( );

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetOvrThreshH( )
{
   HideCursor( );

   GetNumericPrompt( 3, OverlayThresholdL, 255, &OvrThreshPromptH, &OverlayThresholdH );

   DrawPromptBox( OvrThreshPromptH.Rect.Left, OvrThreshPromptH.Rect.Top, OverlayThresholdH, &OvrThreshPromptH );
   
   ShowCursor( );

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetBackThreshL( )
{
   HideCursor( );

   GetNumericPrompt( 3, 0, BackThresholdH, &BackThreshPromptL, &BackThresholdL );

   DrawPromptBox( BackThreshPromptL.Rect.Left, BackThreshPromptL.Rect.Top, BackThresholdL, &BackThreshPromptL );
   
   ShowCursor( );

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  GetBackThreshH( )
{
   HideCursor( );

   GetNumericPrompt( 3, BackThresholdL, 255, &BackThreshPromptH, &BackThresholdH );

   DrawPromptBox( BackThreshPromptH.Rect.Left, BackThreshPromptH.Rect.Top, BackThresholdH, &BackThreshPromptH );
   
   ShowCursor( );

   UpdateFastFile = 1;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [PROMPTS.C   ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
