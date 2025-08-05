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
  ±   [FILE.C      ] - Operations routines                                   ±
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

static   char  *FileImage;
static   char  *FileImageComplete;
static   int   FileImageOK = 0;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

_DIALOG  File = {
   {"Files Menu"},
   {
      {{ 10, 20,  0,  0}, DIALOG_HEADING,    0, 0,                  "Meshes:"},

      {{ 15, 37,  0,  0}, DIALOG_SUBHEADING, 0, 0,                  "    Filename:"},
      {{119, 32,  0,  0}, DIALOG_TEXTBOX,    0, MeshFile,           &MeshNamePrompt},

      {{514, 22,  0,  0}, DIALOG_BUTTON,     0, 0,                  &LoadMeshButton},
      {{514, 43,  0,  0}, DIALOG_BUTTON,     0, 0,                  &SaveMeshButton},
      {{562, 22,  0,  0}, DIALOG_BUTTON,     0, 0,                  &ImpMeshButton},
      {{562, 43,  0,  0}, DIALOG_BUTTON,     0, 0,                  &ExpMeshButton},

      {{ 10, 74,  0,  0}, DIALOG_HEADING,    0, 0,                  "Images:"},

      {{ 15, 91,  0,  0}, DIALOG_SUBHEADING, 0, 0,                  "Source Image:"},
      {{119, 86,  0,  0}, DIALOG_TEXTBOX,    0, SourceImageFile,    &SrcImagePrompt},
      {{514, 86,  0,  0}, DIALOG_BUTTON,     0, 0,                  &LoadSourceImageButton},
      {{562, 86,  0,  0}, DIALOG_BUTTON,     0, 0,                  &SaveSourceImageButton},

      {{ 15,112,  0,  0}, DIALOG_SUBHEADING, 0, 0,                  "  Dest Image:"},
      {{119,108,  0,  0}, DIALOG_TEXTBOX,    0, TargetImageFile,    &DstImagePrompt},
      {{514,108,  0,  0}, DIALOG_BUTTON,     0, 0,                  &LoadTargetImageButton},
      {{562,108,  0,  0}, DIALOG_BUTTON,     0, 0,                  &SaveTargetImageButton},

      {{ 10,128,  0,  0}, DIALOG_HEADING,    0, 0,                  "Output:"},
      {{ 15,145,  0,  0}, DIALOG_SUBHEADING, 0, 0,                  "   Base name:"},
      {{119,140,  0,  0}, DIALOG_TEXTBOX,    0, OutBaseName,        &OutCharsPrompt},

      {{ 15,165,  0,  0}, DIALOG_SUBHEADING, 0, 0,                  "Arithmetic Filename:"},
      {{175,160,  0,  0}, DIALOG_TEXTBOX,    0, OutArithFile,       &OutArithFilePrompt},

      {{175,190,  0,  0}, DIALOG_BUTTON,     0, 0,                  &OffOutButton},
      {{231,190,  0,  0}, DIALOG_BUTTON,     0, 0,                  &AveOutButton},
      {{287,190,  0,  0}, DIALOG_BUTTON,     0, 0,                  &AddOutButton},
      {{343,190,  0,  0}, DIALOG_BUTTON,     0, 0,                  &SubOutButton},
      {{420,190,  0,  0}, DIALOG_BUTTON,     0, 0,                  &OvrOutButton},
      {{476,180,  0,  0}, DIALOG_NUMBOX,     &OverlayThresholdL, 0, &OvrThreshPromptL},
      {{476,200,  0,  0}, DIALOG_NUMBOX,     &OverlayThresholdH, 0, &OvrThreshPromptH},

      {{ 15,225,  0,  0}, DIALOG_SUBHEADING, 0, 0,                  "Background Filename:"},
      {{175,220,  0,  0}, DIALOG_TEXTBOX,    0, BackgroundFile,     &BackFilePrompt},

      {{175,250,  0,  0}, DIALOG_BUTTON,     0, 0,                  &BackOffButton},
      {{231,250,  0,  0}, DIALOG_BUTTON,     0, 0,                  &BackOnButton},
      {{287,240,  0,  0}, DIALOG_NUMBOX,     &BackThresholdL,    0, &BackThreshPromptL},
      {{287,260,  0,  0}, DIALOG_NUMBOX,     &BackThresholdH,    0, &BackThreshPromptH},

      {{  0,  0,  0,  0}, DIALOG_END,        0, 0,               ""}
   }
};

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  DrawFileButton( )
{
   DrawButton( OperButton.Rect.Left + 100,
               OperButton.Rect.Top,
               &FileButton );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ShowFileWindow( )
{
int   i;
_RECT TRect;

   UpdateButtonStates( );

   FileRect.Left = GI->CenX - FILE_WIDTH/2;
   FileRect.Right = FileRect.Left + FILE_WIDTH;
   FileRect.Top  = GI->CenY - FILE_HEIGHT/2;
   FileRect.Bottom = FileRect.Top + FILE_HEIGHT;

   FileImage = malloc(_imagesize( FileRect.Left, FileRect.Top, FileRect.Right, FileRect.Bottom ));

   if (!FileImage)
      return( E_NOMEMMENU );

   _getimage(FileRect.Left, FileRect.Top, FileRect.Right, FileRect.Bottom, FileImage);
   
   if (UpdateFastFile && FastScreen && FileImageOK)
   {
      free( FileImageComplete );
      FileImageOK = 0;
      UpdateFastFile = 0;
   }

   if (FastScreen && !FileImageOK)
   {
      FileImageComplete = malloc(_imagesize( FileRect.Left, FileRect.Top, FileRect.Right, FileRect.Bottom ));

      if (!FileImageComplete)
         FastScreen = 0;
   }
   else if (FastScreen && FileImageOK)
   {
      _putimage( FileRect.Left, FileRect.Top, FileImageComplete, _GPSET );
      return( E_NONE );
   }

   ShadeRect( &FileRect, FILE_WINDOW_COLOR );

   TRect = FileRect;
   TRect.Bottom = FileRect.Top + TEXT_HEIGHT + 6;
   ShadeRect( &TRect, FILE_TITLE_COLOR );

   DrawOutRect( FileRect.Left, FileRect.Top, FileRect.Right, FileRect.Bottom );

   _setcolor(FILE_TITLE_TEXT_COLOR);
   _moveto( FileRect.Left + (FileRect.Right - FileRect.Left) / 2 - 
            strlen(File.Title) * TEXT_WIDTH / 2, FileRect.Top + 5 );
   _outgtext( File.Title );

   for (i = 0; File.Elements[i].Type != DIALOG_END; i++)
      switch( File.Elements[i].Type )
      {
         case DIALOG_HEADING:
            _setcolor(FILE_DB_HEAD_COLOR);
            _moveto( FileRect.Left + File.Elements[i].Rect.Left,
                     FileRect.Top + File.Elements[i].Rect.Top );
            _outgtext( File.Elements[i].Object );
            break;

         case DIALOG_SUBHEADING:
            _setcolor(FILE_DB_SHEAD_COLOR);
            _moveto( FileRect.Left + File.Elements[i].Rect.Left,
                     FileRect.Top + File.Elements[i].Rect.Top );
            _outgtext( File.Elements[i].Object );
            break;

         case DIALOG_HLINE:
            _setcolor(FILE_DB_HLINE_COLOR);
            _setplotaction(_GXOR);
            _moveto( FileRect.Left + File.Elements[i].Rect.Left,
                     FileRect.Top + File.Elements[i].Rect.Top );
            _lineto( FileRect.Right + File.Elements[i].Rect.Right,
                     FileRect.Top + File.Elements[i].Rect.Top );
            _setplotaction(_GPSET);
            break;

         case DIALOG_BUTTON:
            DrawButton( FileRect.Left + File.Elements[i].Rect.Left,
                        FileRect.Top + File.Elements[i].Rect.Top,
                        (BUTTON) File.Elements[i].Object );
            break;

         case DIALOG_TEXTBOX:
            DrawPromptTBox( FileRect.Left + File.Elements[i].Rect.Left, 
                            FileRect.Top + File.Elements[i].Rect.Top,
                            File.Elements[i].PromptString,
                            (PROMPT) File.Elements[i].Object );
            break;

         case DIALOG_NUMBOX:
            DrawPromptBox( FileRect.Left + File.Elements[i].Rect.Left, 
                           FileRect.Top + File.Elements[i].Rect.Top,
                           *File.Elements[i].PromptVal,
                           (PROMPT) File.Elements[i].Object );
            break;
      }

   if (FastScreen && !FileImageOK)
   {
      _getimage(FileRect.Left, FileRect.Top, FileRect.Right, FileRect.Bottom, FileImageComplete);
      FileImageOK = 1;
   }

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  HideFileWindow( )
{
   if (FileImage)
   {
      HideCursor( );

      _putimage( FileRect.Left, FileRect.Top, FileImage, _GPSET );

      ShowCursor( );

      free( FileImage );

      FileImage = 0;
   }

}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [FILE.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/


