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
  ±   [SCREEN.C    ] - Screen I/O graphical routines                         ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <graph.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   int   ZoomBoxSize;
static   char *ZoomImage = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

GINFO SetGraphicsMode( int Mode )
{
int   i;
char  *Palette;
GINFO GraphInfo;
struct videoconfig VideoConfig;
union    REGS InRegs, OutRegs;
struct   SREGS SegRegs;

   GraphInfo = malloc(sizeof(_GINFO));

   if (!GraphInfo)
      FatalError( "Unable to allocate graphics information", E_NOMEMGRF );

   switch( Mode )
   {
      case 400:
         _setvideomode( _VRES16COLOR );
         if (!_setvideomode( _URES256COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = SVGA;
         break;
          
      case 1480:
         if (!_setvideomode( _VRES16COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = VGA;
         break;

      case 480:
         _setvideomode( _VRES16COLOR );
         if (!_setvideomode( _VRES256COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = SVGA;
         break;

      case 1600:
         _setvideomode( _VRES16COLOR );
         if (!_setvideomode( _SVRES16COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = VGA;
         break;

      case 600:
         _setvideomode( _VRES16COLOR );
         if (!_setvideomode( _SVRES256COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = SVGA;
         break;

      case 1768:
         _setvideomode( _VRES16COLOR );
         if (!_setvideomode( _XRES16COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = VGA;
         break;

      case 768:
         _setvideomode( _VRES16COLOR );
         if (!_setvideomode( _XRES256COLOR ))
            FatalError( "Unable to set video mode", E_NOGRF );
         VMode = SVGA;
         break;

      default:
         VMode = SVGA;

         if (!_setvideomode( _XRES256COLOR ))
            if (!_setvideomode( _SVRES256COLOR ))
               if (!_setvideomode( _VRES256COLOR ))
                  if (!_setvideomode( _URES256COLOR ))
                  {
                     VMode = VGA;
                     if (!_setvideomode( _XRES16COLOR ))
                        if (!_setvideomode( _SVRES16COLOR ))
                           if (!_setvideomode( _VRES16COLOR ))
                              FatalError( "Unable to set video mode", E_NOGRF );
                  }

         break;
   }

   _getvideoconfig( &VideoConfig );

   GraphInfo->ResX = VideoConfig.numxpixels;
   GraphInfo->ResY = VideoConfig.numypixels;
   GraphInfo->MaxX = VideoConfig.numxpixels - 1;
   GraphInfo->MaxY = VideoConfig.numypixels - 1;
   GraphInfo->CenX = VideoConfig.numxpixels >> 1;
   GraphInfo->CenY = VideoConfig.numypixels >> 1;
   GraphInfo->Mode = VideoConfig.mode;
   GraphInfo->Colors = VideoConfig.numcolors;

   if (GraphInfo->ResX < 640 || GraphInfo->ResY < 400)
      FatalError( "Unable to find a mode with resolution >= 640x400", E_NOGRF );

   Palette = DMalloc( GraphInfo->Colors*3 );

   if (VMode == SVGA)
   {
      for( i = 0; i < 64; i++ )
      {
         Palette[i*3+0] = i;
         Palette[i*3+1] = i;
         Palette[i*3+2] = i;
      }

      for( ; i < 128; i++ )
      {
         Palette[i*3+0] = i;
         Palette[i*3+1] = 0;
         Palette[i*3+2] = 0;
      }

      for( ; i < 192; i++ )
      {
         Palette[i*3+0] = 0;
         Palette[i*3+1] = i;
         Palette[i*3+2] = 0;
      }

      for( ; i < 256; i++ )
      {
         Palette[i*3+0] = 0;
         Palette[i*3+1] = 0;
         Palette[i*3+2] = i;
      }
   }
   else if (VMode == VGA)
   {
      for( i = 0; i < 16; i++ )
      {
         Palette[i*3+0] = i*4;
         Palette[i*3+1] = i*4;
         Palette[i*3+2] = i*4;
      }
   }
   else
      FatalError( "Unable to use the selected vidio mode", E_NOGRF );

   InRegs.w.ax = 0x1012;
   InRegs.w.bx = 0;
   InRegs.w.cx = GraphInfo->Colors;
   InRegs.w.dx = PROT_OFF(Palette);
   SegRegs.es = PROT_SEG(Palette);
   int86x( VIDEO_INT, &InRegs, &OutRegs, &SegRegs );

   /* SET UP COLORS DEPENDING ON MODE */
   if (VMode == SVGA)
   {
      INFO_TEXT_COLOR        = LIGHT_GREY;
      DEMO_TEXT_COLOR        = LIGHT_RED;
      ZOOM_CURSOR_COLOR      = LIGHT_GREEN;

      BULLSEYE_OUTLINE_COLOR = DARK_GREY;

      R_MESH_COLOR           = LIGHT_RED;
      G_MESH_COLOR           = LIGHT_GREEN;
      B_MESH_COLOR           = LIGHT_BLUE;
      X_MESH_COLOR           = LIGHT_GREY;

      R_CURS_COLOR           = LIGHT_RED;
      G_CURS_COLOR           = LIGHT_GREEN;
      B_CURS_COLOR           = LIGHT_BLUE;
      X_CURS_COLOR           = LIGHT_GREY;

      BUTTON_FACE_COLOR      = GREY_START;
      BUTTON_TEXT_COLOR      = DARK_GREY;
      RBUTTON_TEXT_COLOR     = LIGHT_GREY;

      SHADOW_COLOR           = DARK_GREY+8;
      HIGHLIGHT_COLOR        = LIGHT_GREY-8;

      MBOX_TITLE_TEXT_COLOR  = LIGHT_GREY;
      MBOX_WINDOW_COLOR      = GREY_START;
      MBOX_TEXT_COLOR        = DARK_GREY;

      ENTER_TEXT_COLOR       = LIGHT_GREY;
      TEXT_COLOR             = LIGHT_BLUE;
      TEXT_CURSOR_COLOR      = LIGHT_BLUE;

      PREF_TITLE_COLOR       = GREEN_START;
      PREF_TITLE_TEXT_COLOR  = LIGHT_GREY;
      PREF_WINDOW_COLOR      = BLUE_START;
      PREF_DB_HEAD_COLOR     = LIGHT_GREY;
      PREF_DB_SHEAD_COLOR    = LIGHT_GREEN;
      PREF_DB_HLINE_COLOR    = LIGHT_BLUE;

      PREV_TITLE_COLOR       = GREEN_START;
      PREV_TITLE_TEXT_COLOR  = LIGHT_GREY;
      PREV_WINDOW_COLOR      = BLUE_START;
      PREV_DB_HEAD_COLOR     = LIGHT_GREY;
      PREV_DB_SHEAD_COLOR    = LIGHT_GREEN;
      PREV_DB_HLINE_COLOR    = LIGHT_BLUE;

      FLIC_TITLE_COLOR       = GREEN_START;
      FLIC_TITLE_TEXT_COLOR  = LIGHT_GREY;
      FLIC_WINDOW_COLOR      = BLUE_START;
      FLIC_DB_HEAD_COLOR     = LIGHT_GREY;
      FLIC_DB_SHEAD_COLOR    = LIGHT_GREEN;
      FLIC_DB_HLINE_COLOR    = LIGHT_BLUE;

      PROC_TITLE_COLOR       = GREEN_START;
      PROC_TITLE_TEXT_COLOR  = LIGHT_GREY;
      PROC_WINDOW_COLOR      = BLUE_START;
      PROC_DB_HEAD_COLOR     = LIGHT_GREY;
      PROC_DB_SHEAD_COLOR    = LIGHT_GREEN;
      PROC_DB_HLINE_COLOR    = LIGHT_BLUE;

      EDIT_TITLE_COLOR       = BLUE_START;
      EDIT_TITLE_TEXT_COLOR  = LIGHT_GREY;
      EDIT_WINDOW_COLOR      = GREY_START;
      EDIT_DB_HEAD_COLOR     = LIGHT_BLUE;
      EDIT_DB_SHEAD_COLOR    = LIGHT_GREY;
      EDIT_DB_HLINE_COLOR    = LIGHT_BLUE;

      FILE_TITLE_COLOR       = GREY_START;
      FILE_WINDOW_COLOR      = BLUE_START;
      FILE_TITLE_TEXT_COLOR  = LIGHT_GREY;
      FILE_DB_HEAD_COLOR     = LIGHT_GREEN;
      FILE_DB_SHEAD_COLOR    = LIGHT_GREY;
      FILE_DB_HLINE_COLOR    = LIGHT_BLUE;

      OPER_TITLE_COLOR       = BLUE_START;
      OPER_TITLE_TEXT_COLOR  = LIGHT_GREY;
      OPER_WINDOW_COLOR      = GREY_START;
      OPER_DB_HEAD_COLOR     = LIGHT_BLUE;
      OPER_DB_SHEAD_COLOR    = LIGHT_GREY;
      OPER_DB_HLINE_COLOR    = LIGHT_BLUE;

      PROMPT_COLOR           = BLUE_START;
      PROMPT_TEXT_COLOR      = LIGHT_GREY;

      BOTTOM_COLOR           = BLUE_START;
      BACK_COLOR             = GREY_START + 32;
      TITLE_COLOR            = GREY_END;
      CURSOR_COLOR           = GREEN_END;
      ERROR_BOX_COLOR        = GREY_END;
      HELP_BOX_COLOR         = BLUE_START;
      STATUS_COLOR           = RED_START;
   }
   else
   {
      INFO_TEXT_COLOR        = 15;
      DEMO_TEXT_COLOR        = 15;
      ZOOM_CURSOR_COLOR      = 15;

      BULLSEYE_OUTLINE_COLOR = 0;

      R_MESH_COLOR           = 10;
      G_MESH_COLOR           = 10;
      B_MESH_COLOR           = 15;
      X_MESH_COLOR           = 15;

      R_CURS_COLOR           = 10;
      G_CURS_COLOR           = 10;
      B_CURS_COLOR           = 15;
      X_CURS_COLOR           = 15;

      BUTTON_FACE_COLOR      = 8;
      BUTTON_TEXT_COLOR      = 2;
      RBUTTON_TEXT_COLOR     = 2;

      SHADOW_COLOR           = 4;
      HIGHLIGHT_COLOR        = 14;

      MBOX_TITLE_TEXT_COLOR  = 15;
      MBOX_WINDOW_COLOR      = 8;
      MBOX_TEXT_COLOR        = 0;
      ERROR_BOX_COLOR        = 6;
      HELP_BOX_COLOR         = 6;
      STATUS_COLOR           = 6;

      PROMPT_COLOR           = 8;
      PROMPT_TEXT_COLOR      = 15;

      ENTER_TEXT_COLOR       = 15;
      TEXT_COLOR             = 15;
      TEXT_CURSOR_COLOR      = 8;

      PREF_TITLE_COLOR       = 4;
      PREF_TITLE_TEXT_COLOR  = 15;
      PREF_WINDOW_COLOR      = 8;
      PREF_DB_HEAD_COLOR     = 15;
      PREF_DB_SHEAD_COLOR    = 12;
      PREF_DB_HLINE_COLOR    = 6;

      PREV_TITLE_COLOR       = 4;
      PREV_TITLE_TEXT_COLOR  = 15;
      PREV_WINDOW_COLOR      = 8;
      PREV_DB_HEAD_COLOR     = 15;
      PREV_DB_SHEAD_COLOR    = 12;
      PREV_DB_HLINE_COLOR    = 6;

      FLIC_TITLE_COLOR       = 4;
      FLIC_TITLE_TEXT_COLOR  = 15;
      FLIC_WINDOW_COLOR      = 8;
      FLIC_DB_HEAD_COLOR     = 15;
      FLIC_DB_SHEAD_COLOR    = 12;
      FLIC_DB_HLINE_COLOR    = 6;

      PROC_TITLE_COLOR       = 4;
      PROC_TITLE_TEXT_COLOR  = 15;
      PROC_WINDOW_COLOR      = 8;
      PROC_DB_HEAD_COLOR     = 15;
      PROC_DB_SHEAD_COLOR    = 12;
      PROC_DB_HLINE_COLOR    = 6;

      EDIT_TITLE_COLOR       = 4;
      EDIT_TITLE_TEXT_COLOR  = 15;
      EDIT_WINDOW_COLOR      = 8;
      EDIT_DB_HEAD_COLOR     = 15;
      EDIT_DB_SHEAD_COLOR    = 12;
      EDIT_DB_HLINE_COLOR    = 6;

      FILE_TITLE_COLOR       = 4;
      FILE_TITLE_TEXT_COLOR  = 15;
      FILE_WINDOW_COLOR      = 8;
      FILE_DB_HEAD_COLOR     = 15;
      FILE_DB_SHEAD_COLOR    = 12;
      FILE_DB_HLINE_COLOR    = 6;

      OPER_TITLE_COLOR       = 4;
      OPER_TITLE_TEXT_COLOR  = 15;
      OPER_WINDOW_COLOR      = 8;
      OPER_DB_HEAD_COLOR     = 15;
      OPER_DB_SHEAD_COLOR    = 12;
      OPER_DB_HLINE_COLOR    = 6;

      BOTTOM_COLOR           = 6;
      BACK_COLOR             = 8;
      TITLE_COLOR            = 15;
      CURSOR_COLOR           = 15;
   }

   DEFAULT_MESH_COLOR     = R_MESH_COLOR;
   DEFAULT_CURSOR_COLOR   = G_CURS_COLOR;
   MeshColor = DEFAULT_MESH_COLOR;
   CursColor = DEFAULT_CURSOR_COLOR;

   return GraphInfo;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  RestoreMode( )
{
   _setvideomode( _DEFAULTMODE );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  InitScreen( IMAGE SourceImage, IMAGE TargetImage )
{
char  TempStr[80];

   SourceImage->WindowBorderLeft   = 0;
   SourceImage->WindowBorderTop    = 0;
   SourceImage->WindowBorderRight  = GI->CenX - 2;
   SourceImage->WindowBorderBottom = GI->CenX - 2;

   TargetImage->WindowBorderLeft   = GI->CenX + 1;
   TargetImage->WindowBorderTop    = 0;
   TargetImage->WindowBorderRight  = GI->MaxX;
   TargetImage->WindowBorderBottom = GI->CenX - 2;

   SourceImage->WindowLeft   = SourceImage->WindowBorderLeft + 3;
   SourceImage->WindowTop    = SourceImage->WindowBorderTop + 3;
   SourceImage->WindowRight  = SourceImage->WindowBorderRight - 3;
   SourceImage->WindowBottom = SourceImage->WindowBorderBottom - 3;

   TargetImage->WindowLeft   = TargetImage->WindowBorderLeft + 3;
   TargetImage->WindowTop    = TargetImage->WindowBorderTop + 3;
   TargetImage->WindowRight  = TargetImage->WindowBorderRight - 3;
   TargetImage->WindowBottom = TargetImage->WindowBorderBottom - 3;

   SourceInfoWindow.Left   = SourceImage->WindowBorderLeft+2;
   SourceInfoWindow.Top    = SourceImage->WindowBorderBottom+2;
   SourceInfoWindow.Right  = SourceImage->WindowBorderRight-2;
   SourceInfoWindow.Bottom = SourceInfoWindow.Top+4+TEXT_HEIGHT+3;

   TargetInfoWindow.Left   = TargetImage->WindowBorderLeft+2;
   TargetInfoWindow.Top    = TargetImage->WindowBorderBottom+2;
   TargetInfoWindow.Right  = TargetImage->WindowBorderRight-2;
   TargetInfoWindow.Bottom = TargetInfoWindow.Top+4+TEXT_HEIGHT+3;

   ZoomBoxSize = (GI->MaxY - TEXT_HEIGHT - 2 - (TargetInfoWindow.Bottom + 2)) / (ZOOM_BLOCK_SIZE+1) - 1;
   
   // MAKE SURE ZOOM BOX HAS AN ODD NUMBER OF BLOCKS 
   if (!(ZoomBoxSize & 1))
      ZoomBoxSize--;

   ZoomWindow.Right  = TargetInfoWindow.Right;
   ZoomWindow.Top    = TargetInfoWindow.Bottom + 2;
   ZoomWindow.Left   = ZoomWindow.Right - ZoomBoxSize*(ZOOM_BLOCK_SIZE+1)-3;
   ZoomWindow.Bottom = ZoomWindow.Top + ZoomBoxSize*(ZOOM_BLOCK_SIZE+1)+3;

   // SET THE FONT
   sprintf( TempStr, "h%d w%d", TEXT_WIDTH, TEXT_HEIGHT );
   _setfont( TempStr );

   // DRAW IMAGE BORDERS
   DrawOutRect( SourceImage->WindowBorderLeft, SourceImage->WindowBorderTop,
                SourceImage->WindowBorderRight, SourceImage->WindowBorderBottom);
   DrawOutRect( TargetImage->WindowBorderLeft, TargetImage->WindowBorderTop,
                TargetImage->WindowBorderRight, TargetImage->WindowBorderBottom);

   _setcolor(TEXT_COLOR);
   strcpy( TempStr, "Patience, resizing image..." );

   _moveto( (SourceImage->WindowRight - SourceImage->WindowLeft) / 2 - strlen(TempStr) * TEXT_WIDTH / 2 + SourceImage->WindowLeft, 
            (SourceImage->WindowBottom - SourceImage->WindowTop) / 2 - TEXT_WIDTH / 2 + SourceImage->WindowTop);
   _outgtext( TempStr );
   _moveto( (TargetImage->WindowRight - TargetImage->WindowLeft) / 2 - strlen(TempStr) * TEXT_WIDTH / 2 + TargetImage->WindowLeft, 
            (TargetImage->WindowBottom - TargetImage->WindowTop) / 2 - TEXT_HEIGHT / 2 + TargetImage->WindowTop);
   _outgtext( TempStr );

   InitBottomHalf( SourceImage, TargetImage );

   if (!InitMouse())
      FatalError( "A mouse driver MUST be installed!", E_NOMOUSE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  InitBottomHalf(  IMAGE SourceImage, IMAGE TargetImage )
{
_RECT TempRect;
static   char  *BackImageComplete;
static   int   BackImageOK = 0;

   // FILL THE BACKGROUND
   TempRect.Left = 0;
   TempRect.Top = SourceImage->WindowBorderBottom;
   TempRect.Right = GI->MaxX;
   TempRect.Bottom = GI->MaxY;

   if (FastScreen && !BackImageOK)
   {
      BackImageComplete = malloc(_imagesize( TempRect.Left, TempRect.Top, TempRect.Right, TempRect.Bottom ));

      if (!BackImageComplete)
         FastScreen = 0;
      else
      {
         FadeRect( &TempRect, BOTTOM_COLOR );

         // DRAW THE TITLE
         _setcolor(BACK_COLOR);
         _moveto( GI->CenX - strlen( Copyright ) * TEXT_WIDTH / 2-2, GI->MaxY-TEXT_HEIGHT-2);
         _outgtext( Copyright );
         _moveto( GI->CenX - strlen( Copyright ) * TEXT_WIDTH / 2-1, GI->MaxY-TEXT_HEIGHT-1);
         _outgtext( Copyright );
         _setcolor( TITLE_COLOR );
         _moveto( GI->CenX - strlen( Copyright ) * TEXT_WIDTH / 2, GI->MaxY-TEXT_HEIGHT);
         _outgtext( Copyright );

         _getimage(TempRect.Left, TempRect.Top, TempRect.Right, TempRect.Bottom, BackImageComplete);
         BackImageOK = 1;
      }
   }
   else if (FastScreen && BackImageOK)
   {
      _putimage( TempRect.Left, TempRect.Top, BackImageComplete, _GPSET );
   }
   else
   {
      FadeRect( &TempRect, BOTTOM_COLOR );

      // DRAW THE TITLE
      _setcolor(BACK_COLOR);
      _moveto( GI->CenX - strlen( Copyright ) * TEXT_WIDTH / 2-2, GI->MaxY-TEXT_HEIGHT-2);
      _outgtext( Copyright );
      _moveto( GI->CenX - strlen( Copyright ) * TEXT_WIDTH / 2-1, GI->MaxY-TEXT_HEIGHT-1);
      _outgtext( Copyright );
      _setcolor( TITLE_COLOR );
      _moveto( GI->CenX - strlen( Copyright ) * TEXT_WIDTH / 2, GI->MaxY-TEXT_HEIGHT);
      _outgtext( Copyright );
   }

   // DRAW THE IMAGE INFORMATION BOXES
   DrawInGroove( SourceInfoWindow.Left,
                 SourceInfoWindow.Top,
                 SourceInfoWindow.Right,
                 SourceInfoWindow.Bottom);
   DrawImageInfo( &SourceInfoWindow, 0, 0, SourceImage, "Source" );

   DrawInGroove( TargetInfoWindow.Left,
                 TargetInfoWindow.Top,
                 TargetInfoWindow.Right,
                 TargetInfoWindow.Bottom);
   DrawImageInfo( &TargetInfoWindow, 0, 0, TargetImage, "Target" );

   // DRAW THE ZOOM BOX
   DrawOutGroove( ZoomWindow.Left, ZoomWindow.Top,
                  ZoomWindow.Right, ZoomWindow.Bottom );

   // DRAW THE BUTTONS
   UpdateButtons();

}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  RestoreImage(  IMAGE Image )
{
   if (Image->Saved)
   {
      HideCursor();
      _putimage( Image->WindowLeft, Image->WindowTop, Image->Saved, _GPSET );
      ShowCursor();
   }

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawImage(  IMAGE Image )
{
int   X, Y;
int   ColR, ColG, ColB, Index;
int   SWidth, IWidth, SHeight, IHeight;
float XDiv, YDiv;

   if( Image->Saved )
      free( Image->Saved );

   Image->Saved = malloc(_imagesize( Image->WindowLeft, Image->WindowTop,
                                     Image->WindowRight, Image->WindowBottom));

   memset( Image->Saved, 0, _imagesize( Image->WindowLeft, Image->WindowTop,
                                        Image->WindowRight, Image->WindowBottom));

   if (!Image->Saved)
      FatalError( "Unable to allocate image data", IE_NOMEMIMG );

   SWidth  = Image->WindowRight - Image->WindowLeft + 1;
   SHeight = Image->WindowBottom - Image->WindowTop + 1;
   IWidth  = Image->ResX;
   IHeight = Image->ResY;

   ((short *)Image->Saved)[0] = SWidth;
   ((short *)Image->Saved)[1] = SHeight;

   XDiv = (float)((float) IWidth / (float)SWidth);
   YDiv = (float)((float) IHeight / (float)SHeight);

   if (GI->Colors == 256)
   {
      ((short *)Image->Saved)[2] = 8;

      for( Y = 0; Y < SHeight; Y++)
         for( X = 0; X < SWidth; X++)
         {
            Index = (int) ((float)((float)Y * YDiv)) * IWidth * 3 + (int) ((float)((float)X * XDiv)) * 3;
            ColR = Image->Buffer[Index + 0];
            ColG = Image->Buffer[Index + 1];
            ColB = Image->Buffer[Index + 2];
            Image->Saved[6 + Y * SWidth + X] = MAX(ColR, MAX(ColG, ColB)) >> 2;
         }
   }
   else
   {
      HideCursor();
      ((short *)Image->Saved)[2] = 4;

      for( Y = 0; Y < SHeight; Y++)
         for( X = 0; X < SWidth; X++)
         {
            Index = (int) ((float)((float)Y * YDiv)) * IWidth * 3 + (int) ((float)((float)(X+0) * XDiv)) * 3;
            ColR = Image->Buffer[Index + 0];
            ColG = Image->Buffer[Index + 1];
            ColB = Image->Buffer[Index + 2];

            _setcolor( MAX(ColR, MAX(ColG, ColB)) >> 4 );
            _setpixel( X+Image->WindowLeft, Y+Image->WindowTop );
         }

      _getimage( Image->WindowLeft, Image->WindowTop, Image->WindowRight, Image->WindowBottom, Image->Saved );
      ShowCursor();
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawImageInfo( RECT Rect, int X, int Y, IMAGE Image, char *Title )
{
char  TempStr[80];

   sprintf( TempStr, "h%d w%d", TEXT_WIDTH, TEXT_HEIGHT );
   _setfont( TempStr );

   sprintf( TempStr, "%s:  [X:%03d] [Y:%03d]", Title, X - Image->WindowLeft, Y - Image->WindowTop );

   _setcolor( BACK_COLOR );
   _rectangle( _GFILLINTERIOR, Rect->Left+2, Rect->Top+2, Rect->Right-2, Rect->Bottom-2);

   DrawOutGroove( Rect->Left, Rect->Top, Rect->Right, Rect->Bottom );

   _setcolor(INFO_TEXT_COLOR);
   _moveto( Rect->Left+4, Rect->Top+4 );
   _outgtext( TempStr );

   UpdateZoomWindow( X, Y, Image );

#ifdef DEMO
   _setcharsize( 40, 40 );
   _settextalign( _LEFT, _TOP );

   _setcolor(DEMO_TEXT_COLOR);
   _grtext( Rect->Left+5, Rect->Bottom+4, "DEMO" );
   _grtext( Rect->Left+6, Rect->Bottom+4, "DEMO" );
   _grtext( Rect->Left+7, Rect->Bottom+4, "DEMO" );
   _grtext( Rect->Left+8, Rect->Bottom+4, "DEMO" );

   _setcolor(DEMO_TEXT_COLOR+16);
   _grtext( Rect->Left+5, Rect->Bottom+5, "DEMO" );
   _grtext( Rect->Left+6, Rect->Bottom+5, "DEMO" );
   _grtext( Rect->Left+7, Rect->Bottom+5, "DEMO" );
   _grtext( Rect->Left+8, Rect->Bottom+5, "DEMO" );

   _setcolor(DEMO_TEXT_COLOR + 32);
   _grtext( Rect->Left+5, Rect->Bottom+6, "DEMO" );
   _grtext( Rect->Left+6, Rect->Bottom+6, "DEMO" );
   _grtext( Rect->Left+7, Rect->Bottom+6, "DEMO" );
   _grtext( Rect->Left+8, Rect->Bottom+6, "DEMO" );

   _setcolor(DEMO_TEXT_COLOR + 64);
   _grtext( Rect->Left+5, Rect->Bottom+7, "DEMO" );
   _grtext( Rect->Left+6, Rect->Bottom+7, "DEMO" );
   _grtext( Rect->Left+7, Rect->Bottom+7, "DEMO" );
   _grtext( Rect->Left+8, Rect->Bottom+7, "DEMO" );
#endif

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateZoomWindow( int X, int Y, IMAGE Image )
{
int      x1, x2, x3, y1, y2, y3;
int      IWidth, IHeight, ZWidth, ZHeight;
int      XOff = 0, YOff = 0, zbs = (ZoomBoxSize>>1);
char     col;

   if (!ZoomOn || !Image->Saved)
      return;

   if (VMode == VGA)
   {
      for( y1 = 0; y1 < ZoomBoxSize; y1++ )
         for( x1 = 0; x1 < ZoomBoxSize; x1++ )
         {
            _setcolor(_getpixel( X-(ZoomBoxSize>>1)+x1, Y-(ZoomBoxSize>>1)+y1 ));
            _rectangle( _GFILLINTERIOR,
                        ZoomWindow.Left + 3 + x1 * (ZOOM_BLOCK_SIZE+1),
                        ZoomWindow.Top  + 3 + y1 * (ZOOM_BLOCK_SIZE+1),
                        ZoomWindow.Left + 3 + x1 * (ZOOM_BLOCK_SIZE+1) + ZOOM_BLOCK_SIZE - 1,
                        ZoomWindow.Top  + 3 + y1 * (ZOOM_BLOCK_SIZE+1) + ZOOM_BLOCK_SIZE - 1);
         }

      return;
   }

   IWidth  = Image->WindowRight  - Image->WindowLeft + 1;
   IHeight = Image->WindowBottom - Image->WindowTop + 1;

   ZWidth = ZoomWindow.Right - ZoomWindow.Left - 2;
   ZHeight = ZoomWindow.Bottom - ZoomWindow.Top - 2;

   if (X < Image->WindowLeft)
      X = Image->WindowLeft;
   if (Y < Image->WindowTop)
      Y = Image->WindowTop;
   if (X > Image->WindowRight)
      X = Image->WindowRight;
   if (Y > Image->WindowBottom)
      Y = Image->WindowBottom;

   X-=3, Y-=3;  /* Account for window borders...*/

   if (X < Image->WindowLeft-3+zbs)
   {
      XOff = (Image->WindowLeft-3+zbs-X) * (ZOOM_BLOCK_SIZE+1) * -1;
      X = Image->WindowLeft-3+zbs;
   }
   else if (X > Image->WindowRight-3-zbs)
   {
      XOff = (Image->WindowRight-3-zbs-X) * (ZOOM_BLOCK_SIZE+1) * -1;
      X = Image->WindowRight-3-zbs;
   }

   if (Y < Image->WindowTop-3+zbs)
   {
      YOff = (Image->WindowTop-3+zbs-Y) * (ZOOM_BLOCK_SIZE+1) * -1;
      Y = Image->WindowTop-3+zbs;
   }
   else if (Y > Image->WindowBottom-3-zbs)
   {
      YOff = (Image->WindowBottom-3-zbs-Y) * (ZOOM_BLOCK_SIZE+1) * -1;
      Y = Image->WindowBottom-3-zbs;
   }

   if (!ZoomImage)
   {
      ZoomImage = malloc( _imagesize( ZoomWindow.Left, ZoomWindow.Top,
                                      ZoomWindow.Right, ZoomWindow.Bottom));
      if (!ZoomImage)
      {
         ErrorBox( "Unable to allocate memory for Zoom window", E_NOMEMZM );
         ZoomOn = 0;
         return;
      }

      memset( ZoomImage, 0, _imagesize( ZoomWindow.Left, ZoomWindow.Top,
                                        ZoomWindow.Right, ZoomWindow.Bottom));

      ((short *)ZoomImage)[0] = ZWidth;
      ((short *)ZoomImage)[1] = ZHeight;
      ((short *)ZoomImage)[2] = 8;
   }

   for( y1 = 1, y2 = 0; y1 < ZWidth; y1 += ZOOM_BLOCK_SIZE+1, y2++ )
      for( x1 = 1, x2 = 0; x1 < ZWidth; x1 += ZOOM_BLOCK_SIZE+1, x2++ )
      {
         col = Image->Saved[6+(Y+3-Image->WindowTop-zbs+y2)*IWidth+(X+3-Image->WindowLeft-zbs+x2)];

         for( y3 = 0; y3 < ZOOM_BLOCK_SIZE; y3++ )
            for( x3 = 0; x3 < ZOOM_BLOCK_SIZE; x3++ )
               ZoomImage[6 + (y1 + y3) * ZWidth + x1 + x3] = col;
      }

   _putimage( ZoomWindow.Left+2, ZoomWindow.Top+2, ZoomImage, _GPSET );

   // DRAW THE ZOOM BOX'S CURSOR
   _setcolor(ZOOM_CURSOR_COLOR);
   _rectangle( _GBORDER,
               XOff + ZoomWindow.Left + 3 + zbs * (ZOOM_BLOCK_SIZE+1)-1,
               YOff + ZoomWindow.Top  + 3 + zbs * (ZOOM_BLOCK_SIZE+1)-1,
               XOff + ZoomWindow.Left + 3 + zbs * (ZOOM_BLOCK_SIZE+1) + ZOOM_BLOCK_SIZE,
               YOff + ZoomWindow.Top  + 3 + zbs * (ZOOM_BLOCK_SIZE+1) + ZOOM_BLOCK_SIZE);

}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [SCREEN.C    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
