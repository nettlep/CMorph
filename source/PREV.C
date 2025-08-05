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
  ±   [PREV.C      ] - Preferences routines                                  ±
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

static   char  *PrevImage;
static   char  *PrevImageComplete;
static   int   PrevImageOK = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   int   GetPrevPixel( int x, int y );
static   CLIST *GlobColorList;
static   IMAGE GlobBuffer;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  Preview = {
   {"Preview"},
   {
      {{ 52, 35,  0,  0}, DIALOG_HEADING,    0,              0, "Frame %:"},
      {{116, 30,  0,  0}, DIALOG_NUMBOX,     &Prev50,        0, &PrevFramePrompt},
      {{ 50, 50,  0,  0}, DIALOG_BUTTON,     0,              0, &PrevColorButton},
      {{ 50, 80,  0,  0}, DIALOG_BUTTON,     0,              0, &PrevGoButton},

      {{  0,  0,  0,  0}, DIALOG_END,        0,              0, ""}
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawPrevButton( )
{
   DrawButton( FileButton.Rect.Left + 100,
               FileButton.Rect.Top,
               &PrevButton );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowPrevWindow( )
{
int   i;
_RECT TRect;

   UpdateButtonStates( );

   PrevRect.Left = GI->CenX - PREV_WIDTH/2;
   PrevRect.Right = PrevRect.Left + PREV_WIDTH;
   PrevRect.Top  = GI->CenY - PREV_HEIGHT/2;
   PrevRect.Bottom = PrevRect.Top + PREV_HEIGHT;

   PrevImage = malloc(_imagesize( PrevRect.Left, PrevRect.Top, PrevRect.Right, PrevRect.Bottom ));

   if (!PrevImage)
      return( E_NOMEMMENU );

   _getimage(PrevRect.Left, PrevRect.Top, PrevRect.Right, PrevRect.Bottom, PrevImage);
   
   if (UpdateFastPrev && FastScreen && PrevImageOK)
   {
      free( PrevImageComplete );
      PrevImageOK = 0;
      UpdateFastPrev = 0;
   }

   if (FastScreen && !PrevImageOK)
   {
      PrevImageComplete = malloc(_imagesize( PrevRect.Left, PrevRect.Top, PrevRect.Right, PrevRect.Bottom ));

      if (!PrevImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && PrevImageOK)
   {
      _putimage( PrevRect.Left, PrevRect.Top, PrevImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &PrevRect, PREV_WINDOW_COLOR );

   TRect = PrevRect;
   TRect.Bottom = PrevRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, PREV_TITLE_COLOR );

   DrawOutRect( PrevRect.Left, PrevRect.Top, PrevRect.Right, PrevRect.Bottom );

   _setcolor(PREV_TITLE_TEXT_COLOR);
   _moveto( PrevRect.Left + (PrevRect.Right - PrevRect.Left) / 2 - 
            strlen(Preview.Title) * TEXT_WIDTH / 2, PrevRect.Top + 5 );
   _outgtext( Preview.Title );

   for (i = 0; Preview.Elements[i].Type != DIALOG_END; i++)
      switch( Preview.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(PREV_DB_HEAD_COLOR);
            _moveto( PrevRect.Left + Preview.Elements[i].Rect.Left,
                     PrevRect.Top + Preview.Elements[i].Rect.Top );
            _outgtext( Preview.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(PREV_DB_SHEAD_COLOR);
            _moveto( PrevRect.Left + Preview.Elements[i].Rect.Left,
                     PrevRect.Top + Preview.Elements[i].Rect.Top );
            _outgtext( Preview.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(PREV_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( PrevRect.Left + Preview.Elements[i].Rect.Left,
                     PrevRect.Top + Preview.Elements[i].Rect.Top );
            _lineto( PrevRect.Right + Preview.Elements[i].Rect.Right,
                     PrevRect.Top + Preview.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( PrevRect.Left + Preview.Elements[i].Rect.Left,
                        PrevRect.Top + Preview.Elements[i].Rect.Top,
                        (BUTTON) Preview.Elements[i].Object );
            break;

         case DIALOG_NUMBOX:
            DrawPromptBox( PrevRect.Left + Preview.Elements[i].Rect.Left, 
                           PrevRect.Top + Preview.Elements[i].Rect.Top,
                           *Preview.Elements[i].PromptVal,
                           (PROMPT) Preview.Elements[i].Object );
            break;
      }

   if (FastScreen && !PrevImageOK)
   {
      _getimage(PrevRect.Left, PrevRect.Top, PrevRect.Right, PrevRect.Bottom, PrevImageComplete);
      PrevImageOK = 1;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HidePrevWindow( )
{
   if (PrevImage)
   {
      HideCursor();

      _putimage( PrevRect.Left, PrevRect.Top, PrevImage, _GPSET );

      ShowCursor();

      free( PrevImage );

      PrevImage = 0;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowPreviewImage( IMAGE Image )
{
int   x, y, i, Left, Top, r, g, b;
char  *Palette, *PPalette;
SPAL  *SortedPalette;
CLIST *ColorList;
union    REGS InRegs, OutRegs;
struct   SREGS SegRegs;

   if (VMode == SVGA )
   {
      DrawMessBox( "Information", "Please wait...processing colors", STATUS_COLOR );

      if (PrevColor == COLOR)
      {
         PPalette = malloc( GI->Colors*3 );
         Palette = DMalloc( GI->Colors*3 );

         InRegs.w.ax = 0x1017;
         InRegs.w.bx = 0;
         InRegs.w.cx = GI->Colors;
         InRegs.w.dx = PROT_OFF(Palette);
         SegRegs.es = PROT_SEG(Palette);
         int86x( VIDEO_INT, &InRegs, &OutRegs, &SegRegs );
         memcpy( PPalette, Palette, GI->Colors*3 );

         ColorList = calloc( 256*256, sizeof( CLIST ) );

         if (!ColorList)
            return( IE_NOMEMPAL );

         SortedPalette = calloc( 256, sizeof(SPAL) );

         if (!SortedPalette)
         {
            free( ColorList );
            return( IE_NOMEMPAL );
         }

         ReducePalette( Image, 256, SortedPalette, ColorList );

         for( i = 0; i < GI->Colors; i++ )
         {
            Palette[i*3+0] = SortedPalette[i].r>>2;
            Palette[i*3+1] = SortedPalette[i].g>>2;
            Palette[i*3+2] = SortedPalette[i].b>>2;
         }
      }

      EraseMessBox( );
   }

   _setcolor( 0 );
   _rectangle( _GFILLINTERIOR, 0, 0, GI->MaxX, GI->MaxY );

   if (VMode == SVGA && PrevColor == COLOR)
   {
      InRegs.w.ax = 0x1012;
      InRegs.w.bx = 0;
      InRegs.w.cx = GI->Colors;
      InRegs.w.dx = PROT_OFF(Palette);
      SegRegs.es = PROT_SEG(Palette);
      int86x( VIDEO_INT, &InRegs, &OutRegs, &SegRegs );

      GlobBuffer = Image;
      GlobColorList = ColorList;
   }

   Left = GI->CenX - Image->ResX / 2;
   Top = GI->CenY - Image->ResY / 2;

   if (VMode == SVGA && PrevColor == COLOR )
   {
      for( y = 0; y < Image->ResY; y++ )
         for ( x = 0; x < Image->ResX; x++ )
         {
            _setcolor( GetPrevPixel( x, y ) );
            _setpixel( x+Left, y+Top );
         }

      FreeList(ColorList);
      free(SortedPalette);
      free(Palette);
   }
   else
   {
      if (VMode == SVGA)
      {
         for( y = 0; y < Image->ResY; y++ )
            for ( x = 0; x < Image->ResX; x++ )
            {
               r = Image->Buffer[y*Image->ResX*3+x*3+0] >> 2;
               g = Image->Buffer[y*Image->ResX*3+x*3+1] >> 2;
               b = Image->Buffer[y*Image->ResX*3+x*3+2] >> 2;

               _setcolor( MAX( r, MAX( g, b )) );
               _setpixel( x+Left, y+Top );
            }
      }
      else
      {
         for( y = 0; y < Image->ResY; y++ )
            for ( x = 0; x < Image->ResX; x++ )
            {
               r = (Image->Buffer[y*Image->ResX*3+x*3+0]) >> 4;
               g = (Image->Buffer[y*Image->ResX*3+x*3+1]) >> 4;
               b = (Image->Buffer[y*Image->ResX*3+x*3+2]) >> 4;

               _setcolor( MAX( r, MAX( g, b )) );
               _setpixel( x+Left, y+Top );
            }
      }
   }

   Beep();

   GetKey();

   _setcolor( 0 );
   _rectangle( _GFILLINTERIOR, 0, 0, GI->MaxX, GI->MaxY );

   if (VMode == SVGA && PrevColor == COLOR )
   {
      memcpy( Palette, PPalette, GI->Colors*3 );

      InRegs.w.ax = 0x1012;
      InRegs.w.bx = 0;
      InRegs.w.cx = GI->Colors;
      InRegs.w.dx = PROT_OFF(Palette);
      SegRegs.es = PROT_SEG(Palette);
      int86x( VIDEO_INT, &InRegs, &OutRegs, &SegRegs );
   }


   InitScreen( SourceImage, TargetImage );

   RestoreImage( SourceImage );
   RestoreImage( TargetImage );

   DrawMesh( SourceMesh, SourceImage);
   DrawMesh( TargetMesh, TargetImage);

   ShowCursor( );

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#ifdef   POPULARITY
int   GetPrevPixel( int x, int y )
{
int   r, g, b;
CLIST *TempList;

   r = GlobBuffer->Buffer[y*GlobBuffer->ResX*3+x*3+0] & 0xfc;
   g = GlobBuffer->Buffer[y*GlobBuffer->ResX*3+x*3+1] & 0xfc;
   b = GlobBuffer->Buffer[y*GlobBuffer->ResX*3+x*3+2] & 0xfc;

   TempList = &GlobColorList[r*256+g];

   while (TempList)
   {
      if (TempList->Blue == b)
         break;
      else
         TempList = TempList->Next;
   }

   return(TempList->Frequency);
}
#else
int   GetPrevPixel( int x, int y )
{
   return(GetClosestColor(GlobBuffer->Buffer[y*GlobBuffer->ResX*3+x*3+0],
                          GlobBuffer->Buffer[y*GlobBuffer->ResX*3+x*3+1],
                          GlobBuffer->Buffer[y*GlobBuffer->ResX*3+x*3+2]));
}
#endif

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [PREV.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

