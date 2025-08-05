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
  ±   [ERROR.C     ] - Error handling routines                               ±
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

static   char  *ErrStr( int ErrorNumber );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

char  *ErrStr( int ErrorNumber )
{
   switch( ErrorNumber )
   {
      case E_NONE:
         return( "No error occured; you should not see this message" );
         break;

      case E_NOOPEN:
         return( "Unable to open/find file; check filename" );
         break;

      case E_NOREAD:
         return( "Unable to read from file; possibly corrupt file" );
         break;

      case E_NOWRITE:
         return( "Unable to write to file; possibly no room" );
         break;

      case E_BADMESHFILE:
         return( "Bad mesh file; file is corrupt" );
         break;

      case E_INVMESHFILE:
         return( "Invalid mesh in file; file is corrupt" );
         break;

      case E_INVALIDMESH:
         return( "Invalid mesh; try reloading mesh" );
         break;

      case E_BADRESOLUTION:
         return( "Image resolutions are not the same; convert or use other images" );
         break;

      case E_BADX:
         return( "Bad X input into routine SPLINT; mesh is corrupt" );
         break;

      case E_CORRUPTMESH:
         return( "Mesh is corrupt" );
         break;

      case E_BADARITH:
         return( "Unable to open arithmetic file; check filename" );
         break;

      case E_BADBACK:
         return( "Unable to open background file; check filename" );
         break;

      case E_NOFILTERFILE:
         return( "Unable to find filter file; check filename" );
         break;

      case E_NOFILTERNAME:
         return( "Unable to find filter in file; check filter name" );
         break;

      case E_INVFILTOPER:
         return( "Invalid filter operator" );
         break;

      case E_NOMEMMSH:
         return( "Not enough memory for mesh; try virtual memory" );
         break;

      case E_NOMEMMENU:
         return( "Not enough memory to open menu; try virtual memory" );
         break;

      case E_NOMEMGRF:
         return( "Not enough memory for video description information; try virtual memory" );
         break;

      case E_NOGRF:
         return( "Graphics card not compatible; try another mode with -G or -C parameters" );
         break;

      case E_NOMEMZM:
         return( "Not enough memory for zoom information; try virtual memory" );
         break;

      case E_NOMOUSE:
         return( "No mouse driver is installed; install a mouse driver before running CMorph" );
         break;

      case E_UNKNOWN:
         return( "An unknown error occured" );
         break;

// IMAGE ERRORS

      case IE_BADIMAGE:
         return( "Bad or missing image" );
         break;

      case IE_BADTGATYPE:
         return( "TarGA file is not type 2" );
         break;

      case IE_INVALIDFILETYPE:
         return( "Invalid file type; check the filename/extension" );
         break;

      case IE_INVALIDGIFFILE:
         return( "Not a valid GIF file" );
         break;

      case IE_NOCOMPBMP:
         return( "Compressed BMPs not allowed" );
         break;

      case IE_NOGIFINTERLACE:
         return( "Interlace not allowed in GIF files" );
         break;

      case IE_NOMEMIMG:
         return( "Not enough memory for image buffer; try virtual memory" );
         break;

      case IE_NOMEMPAL:
         return( "Not enough memory for palette buffer; try virtual memory" );
         break;

      case IE_NOOPEN:
         return( "Unable to open/find image file; check filename" );
         break;

      case IE_NOREAD:
         return( "Unable to read from image file; possibly corrupt file" );
         break;

      case IE_NOTBMP:
         return( "Not a valid BMP file" );
         break;

      case IE_NOWRITE:
         return( "Unable to write to image file; possibly no room" );
         break;

      case IE_BADCODE:
         return( "Bad code in GIF file" );
         break;

      case IE_NOTPCX:
         return( "This is not an understandable PCX file" );
         break;

      case IE_NOTPCX256:
         return( "PCX files must be 256 indexed or 24-bit true-color only" );
         break;

      case IE_NOTPCXENCODE:
         return( "PCX files must be RLE compressed" );
         break;

// FLIC ERRORS

      case FE_BADFLICRES:
         return( "Invalid resolution for FLIC" );
         break;

      case FE_BADFLICTYPE:
         return( "Bad FLIC type" );
         break;

      case FE_NOWRITE:
         return( "Unable to write to FLIC file" );
         break;

      case FE_NOCREATE:
         return( "Unable to create FLIC file" );
         break;

      case FE_NOMEMFLIC:
         return( "Out of memory for FLIC" );
         break;

      case FE_UNKNOWN:
      case IE_UNKNOWN:
         return( "An unknown image-related error occured" );
         break;

      default:
         return( "An unknown error occured" );
         break;
   }
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  WarningBox( char *String )
{
   DrawMessBox( "WARNING", String, ERROR_BOX_COLOR );
   Beep( );
   GetKey( );
   EraseMessBox( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ErrorBox( char *String, int ErrorNumber )
{
char  ErrorString[160];

   sprintf( ErrorString, "%s (%03d):\n\n%s", String, ErrorNumber, ErrStr(ErrorNumber) );

   DrawMessBox( "ERROR", ErrorString, ERROR_BOX_COLOR );
   Beep( );
   GetKey( );
   EraseMessBox( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  FatalError( char *ErrString, int ErrorNumber )
{
int   Key, RetCode;
char  FileName[80];

   _setvideomode( _DEFAULTMODE );

   printf( "FATAL ERROR:  %s\n(%03d) %s\n", ErrString, ErrorNumber, ErrStr(ErrorNumber) );

   if (SourceMesh && TargetMesh)
   {
      printf( "\n\nSave mesh now [Y/n]?  ");

      Key = toupper(getch());

      if (Key != 'N')
      {
         printf( "Y\n\nEnter file name to use for save:  " );
         FileName[0] = 75;  FileName[1] = '\0';
         cgets( FileName );
         printf( "\n" );

         RetCode = WriteMesh( &FileName[2], SourceMesh, TargetMesh );

         if (RetCode == E_NONE )
         {
            printf( "Mesh file saved.\n" );
         }
         else
         {
            printf( "Unable to save mesh (%03d):  %s\n", RetCode, ErrStr(RetCode) );
         }
      }
      else
      {
         printf( "N\nMesh file not saved.\n" );
      }
   }

   ExitSystem( 1 );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [ERROR.C     ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
