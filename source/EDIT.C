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
  ±   [EDIT.C      ] - Operations routines                                   ±
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

static   char  *EditImage;
static   char  *EditImageComplete;
static   int   EditImageOK = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  Edit = {
   {"Editing Menu"},
   {
      {{ 10, 20,  0,  0}, DIALOG_HEADING,    0, 0, "Meshes:"},

      {{ 20, 32,  0,  0}, DIALOG_BUTTON,     0, 0, &MeshSwapButton},

      {{ 15, 58,  0,  0}, DIALOG_SUBHEADING, 0, 0, "Source:"},

      {{ 20, 70,  0,  0}, DIALOG_BUTTON,     0, 0, &HFlipSMButton},
      {{ 20, 90,  0,  0}, DIALOG_BUTTON,     0, 0, &VFlipSMButton},
      {{ 20,110,  0,  0}, DIALOG_BUTTON,     0, 0, &CopySDMButton},

      {{ 15,136,  0,  0}, DIALOG_SUBHEADING, 0, 0, "Destination:"},

      {{ 20,148,  0,  0}, DIALOG_BUTTON,     0, 0, &HFlipDMButton},
      {{ 20,168,  0,  0}, DIALOG_BUTTON,     0, 0, &VFlipDMButton},
      {{ 20,188,  0,  0}, DIALOG_BUTTON,     0, 0, &CopyDSMButton},

      {{160, 20,  0,  0}, DIALOG_HEADING,    0, 0, "Images:"},

      {{170, 32,  0,  0}, DIALOG_BUTTON,     0, 0, &ImageSwapButton},

      {{165, 58,  0,  0}, DIALOG_SUBHEADING, 0, 0, "Source:"},

      {{170, 70,  0,  0}, DIALOG_BUTTON,     0, 0, &HFlipSIButton},
      {{170, 90,  0,  0}, DIALOG_BUTTON,     0, 0, &VFlipSIButton},

      {{165,136,  0,  0}, DIALOG_SUBHEADING, 0, 0, "Desination:"},

      {{170,148,  0,  0}, DIALOG_BUTTON,     0, 0, &HFlipDIButton},
      {{170,168,  0,  0}, DIALOG_BUTTON,     0, 0, &VFlipDIButton},

      {{  0,  0,  0,  0}, DIALOG_END,        0, 0, "" }
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawEditButton( )
{
   DrawButton( PerfButton.Rect.Left + 100,
               PerfButton.Rect.Top,
               &EditButton );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowEditWindow( )
{
int   i;
_RECT TRect;

   EditRect.Left = GI->CenX - EDIT_WIDTH/2;
   EditRect.Right = EditRect.Left + EDIT_WIDTH;
   EditRect.Top  = GI->CenY - EDIT_HEIGHT/2;
   EditRect.Bottom = EditRect.Top + EDIT_HEIGHT;

   EditImage = malloc(_imagesize( EditRect.Left, EditRect.Top, EditRect.Right, EditRect.Bottom ));

   if (!EditImage)
      return( E_NOMEMMENU );

   _getimage(EditRect.Left, EditRect.Top, EditRect.Right, EditRect.Bottom, EditImage);
   
   if (FastScreen && !EditImageOK)
   {
      EditImageComplete = malloc(_imagesize( EditRect.Left, EditRect.Top, EditRect.Right, EditRect.Bottom ));

      if (!EditImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && EditImageOK)
   {
      _putimage( EditRect.Left, EditRect.Top, EditImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &EditRect, EDIT_WINDOW_COLOR );

   TRect = EditRect;
   TRect.Bottom = EditRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, EDIT_TITLE_COLOR );

   DrawOutRect( EditRect.Left, EditRect.Top, EditRect.Right, EditRect.Bottom );

   _setcolor(EDIT_TITLE_TEXT_COLOR);
   _moveto( EditRect.Left + (EditRect.Right - EditRect.Left) / 2 - 
            strlen(Edit.Title) * TEXT_WIDTH / 2, EditRect.Top + 5 );
   _outgtext( Edit.Title );

   for (i = 0; Edit.Elements[i].Type != DIALOG_END; i++)
      switch( Edit.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(EDIT_DB_HEAD_COLOR);
            _moveto( EditRect.Left + Edit.Elements[i].Rect.Left,
                     EditRect.Top + Edit.Elements[i].Rect.Top );
            _outgtext( Edit.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(EDIT_DB_SHEAD_COLOR);
            _moveto( EditRect.Left + Edit.Elements[i].Rect.Left,
                     EditRect.Top + Edit.Elements[i].Rect.Top );
            _outgtext( Edit.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(EDIT_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( EditRect.Left + Edit.Elements[i].Rect.Left,
                     EditRect.Top + Edit.Elements[i].Rect.Top );
            _lineto( EditRect.Right + Edit.Elements[i].Rect.Right,
                     EditRect.Top + Edit.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( EditRect.Left + Edit.Elements[i].Rect.Left,
                        EditRect.Top + Edit.Elements[i].Rect.Top,
                        (BUTTON) Edit.Elements[i].Object );
            break;
      }

   if (FastScreen && !EditImageOK)
   {
      _getimage(EditRect.Left, EditRect.Top, EditRect.Right, EditRect.Bottom, EditImageComplete);
      EditImageOK = 1;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HideEditWindow( )
{
   if (EditImage)
   {
      HideCursor();

      _putimage( EditRect.Left, EditRect.Top, EditImage, _GPSET );

      ShowCursor();

      free( EditImage );

      EditImage = 0;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SwapMesh( MESH Mesh1, MESH Mesh2 )
{
MESH  TempMesh;

   CreateMesh( &TempMesh );

   *TempMesh = *Mesh1;
   *Mesh1 = *Mesh2;
   *Mesh2 = *TempMesh;

   DestroyMesh( &TempMesh );

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  CopyMesh( MESH TargetMesh, MESH SourceMesh )
{
   memcpy( TargetMesh, SourceMesh, sizeof(_MESH) );

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HorizFlipMesh( MESH FlipMesh  )
{
int   i, j;
MESH  TempMesh;

   CreateMesh( &TempMesh );

   memcpy( TempMesh, FlipMesh, sizeof(_MESH) );

   for( i = 0; i < FlipMesh->HorizRes; i++ )
      for( j = 0; j < FlipMesh->VertRes; j++ )
      {
         FlipMesh->HorizList[i][j] = TempMesh->HorizList[i][FlipMesh->VertRes-j-1];
         FlipMesh->VertList[j][i] = 1.0-TempMesh->VertList[FlipMesh->VertRes-j-1][i];
      }

   DestroyMesh( &TempMesh );

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  VertFlipMesh( MESH FlipMesh  )
{
int   i, j;
MESH  TempMesh;

   CreateMesh( &TempMesh );

   memcpy( TempMesh, FlipMesh, sizeof(_MESH) );

   for( i = 0; i < FlipMesh->HorizRes; i++ )
      for( j = 0; j < FlipMesh->VertRes; j++ )
      {
         FlipMesh->HorizList[i][j] = 1.0-TempMesh->HorizList[FlipMesh->HorizRes-i-1][j];
         FlipMesh->VertList[j][i] = TempMesh->VertList[j][FlipMesh->HorizRes-i-1];
      }

   DestroyMesh( &TempMesh );

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  SwapImage( IMAGE Image1, IMAGE Image2 )
{
_IMAGE  TempImage;

   TempImage.ResX   = Image2->ResX;
   TempImage.ResY   = Image2->ResY;
   TempImage.Buffer = Image2->Buffer;
   TempImage.Saved  = Image2->Saved;

   Image2->ResX     = Image1->ResX;
   Image2->ResY     = Image1->ResY;
   Image2->Buffer   = Image1->Buffer;
   Image2->Saved    = Image1->Saved;

   Image1->ResX     = TempImage.ResX;
   Image1->ResY     = TempImage.ResY;
   Image1->Buffer   = TempImage.Buffer;
   Image1->Saved    = TempImage.Saved;

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HorizFlipImage( IMAGE FlipImage )
{
int   x, y;
int   Width, Height;
int   YOff, XMax;
char  col;

   Width = FlipImage->ResX;
   Height = FlipImage->ResY;
   XMax = Width*3;

   for( y = 0; y < Height; y++ )
      for( x = 0; x < (Width>>1)*3; x+=3 )
      {
         YOff = y*Width*3;

         col = FlipImage->Buffer[YOff+x+0];
         FlipImage->Buffer[YOff+x+0] = FlipImage->Buffer[YOff+XMax-x-0];
         FlipImage->Buffer[YOff+XMax-x-0] = col;

         col = FlipImage->Buffer[YOff+x+1];
         FlipImage->Buffer[YOff+x+1] = FlipImage->Buffer[YOff+XMax-x-1];
         FlipImage->Buffer[YOff+XMax-x-1] = col;

         col = FlipImage->Buffer[YOff+x+2];
         FlipImage->Buffer[YOff+x+2] = FlipImage->Buffer[YOff+XMax-x-2];
         FlipImage->Buffer[YOff+XMax-x-2] = col;
      }

   Width = FlipImage->WindowRight - FlipImage->WindowLeft + 1;
   Height = FlipImage->WindowBottom - FlipImage->WindowTop + 1;
   XMax = Width;

   for( y = 0; y < Height; y++ )
      for( x = 0; x < (Width>>1); x++ )
      {
         YOff = y*Width;

         col = FlipImage->Saved[6+YOff+x];
         FlipImage->Saved[6+YOff+x] = FlipImage->Saved[6+YOff+XMax-x];
         FlipImage->Saved[6+YOff+XMax-x] = col;
      }

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  VertFlipImage( IMAGE FlipImage )
{
int   x, y;
int   Width, Height;
int   YOff, YMax;
char  col;

   Width = FlipImage->ResX;
   Height = FlipImage->ResY;
   YMax = (Height-1)*Width*3;

   for( y = 0; y < Height>>1; y++ )
      for( x = 0; x < Width*3; x+=3 )
      {
         YOff = y*Width*3;

         col = FlipImage->Buffer[YOff+x+0];
         FlipImage->Buffer[YOff+x+0] = FlipImage->Buffer[YMax-YOff+x+0];
         FlipImage->Buffer[YMax-YOff+x+0] = col;

         col = FlipImage->Buffer[YOff+x+1];
         FlipImage->Buffer[YOff+x+1] = FlipImage->Buffer[YMax-YOff+x+1];
         FlipImage->Buffer[YMax-YOff+x+1] = col;

         col = FlipImage->Buffer[YOff+x+2];
         FlipImage->Buffer[YOff+x+2] = FlipImage->Buffer[YMax-YOff+x+2];
         FlipImage->Buffer[YMax-YOff+x+2] = col;
      }

   Width = FlipImage->WindowRight - FlipImage->WindowLeft + 1;
   Height = FlipImage->WindowBottom - FlipImage->WindowTop + 1;
   YMax = (Height-1)*Width;

   for( y = 0; y < Height>>1; y++ )
      for( x = 0; x < Width; x++ )
      {
         YOff = y*Width;

         col = FlipImage->Saved[6+YOff+x];
         FlipImage->Saved[6+YOff+x] = FlipImage->Saved[6+YMax-YOff+x];
         FlipImage->Saved[6+YMax-YOff+x] = col;
      }

   ChangeMenu( MAINMENU );

   RestoreImage( SourceImage );
   DrawMesh( SourceMesh, SourceImage );
   RestoreImage( TargetImage );
   DrawMesh( TargetMesh, TargetImage );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [EDIT.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

