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
  ±   [CONV.C      ] - Convolution filters                                   ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <math.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dos.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   void  Convolution3x3(IMAGE InBuffer, IMAGE OutBuffer, double *Grid, char *Operator, double OperValue, int Bias);
static   void  Convolution5x5(IMAGE InBuffer, IMAGE OutBuffer, double *Grid, char *Operator, double OperValue, int Bias);
static   void  Convolution7x7(IMAGE InBuffer, IMAGE OutBuffer, double *Grid, char *Operator, double OperValue, int Bias);
static   int   ReadLine( char *Line, FILE *FilePointer );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   Convolution( char *FilterName, IMAGE Image )
{
int      Bias = 0, ConvSize;
long     Found = 0;
char     FilterFile[180], LineIn[280], p1[40], p2[40], p3[40], Operator[40];
double   Grid[7*7], OperValue;
IMAGE    OutBuffer;
FILE     *FilePointer;

   strcpy( FilterFile, FilterFileName );
   strupr( FilterName );

// READ FILTER FILE

   FilePointer = fopen(FilterFile, "rb");

   if(!FilePointer)
   {
      sprintf( LineIn, "%s\\%s", ProgramPath, FilterFile );

      strcpy( FilterFile, LineIn );

      FilePointer = fopen(FilterFile, "rb");

      if(!FilePointer)
         return( E_NOFILTERFILE );
   }

   while( !Found )
   {
      if (!ReadLine( LineIn, FilePointer))
         break;

      sscanf( LineIn, "%s %s %s", p1, p2, p3 );

      strupr( p1 ); strupr( p2 ); strupr( p3 );

      if (!strcmp( p1, "FILTER" ) && !strcmp( p2, FilterName ) && 
          (!strcmp( p3, "3" ) || !strcmp( p3, "5" ) || !strcmp( p3, "7" )))
         Found = 1;
   }

   if (!Found)
   {
      fclose( FilePointer );
      return( E_NOFILTERNAME );
   }

   ConvSize = atoi(p3);

   switch( ConvSize )
   {
      case 3:
         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf", &Grid[0], &Grid[1], &Grid[2] );
                                             
         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf", &Grid[3], &Grid[4], &Grid[5] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf", &Grid[6], &Grid[7], &Grid[8] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%s %lf %d", Operator, &OperValue, &Bias );

         break;

      case 5:
         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf", &Grid[0], &Grid[1], &Grid[2], &Grid[3] , &Grid[4] );
                                             
         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf", &Grid[5], &Grid[6], &Grid[7], &Grid[8] , &Grid[9] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf", &Grid[10], &Grid[11], &Grid[12], &Grid[13] , &Grid[14] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf", &Grid[15], &Grid[16], &Grid[17], &Grid[18] , &Grid[19] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf", &Grid[20], &Grid[21], &Grid[22], &Grid[23] , &Grid[24] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%s %lf %d", Operator, &OperValue, &Bias );

         break;

      case 7:
         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[0], &Grid[1], &Grid[2], &Grid[3], &Grid[4], &Grid[5], &Grid[6] );
                                             
         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[7], &Grid[8], &Grid[9], &Grid[10], &Grid[11], &Grid[12], &Grid[13] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[14], &Grid[15], &Grid[16], &Grid[17], &Grid[18], &Grid[19], &Grid[20] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[21], &Grid[22], &Grid[23], &Grid[24], &Grid[25], &Grid[26], &Grid[27] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[28], &Grid[29], &Grid[30], &Grid[31], &Grid[32], &Grid[33], &Grid[34] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[35], &Grid[36], &Grid[37], &Grid[38], &Grid[39], &Grid[40], &Grid[41] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%lf %lf %lf %lf %lf %lf %lf", &Grid[42], &Grid[43], &Grid[44], &Grid[45], &Grid[46], &Grid[47], &Grid[48] );

         ReadLine( LineIn, FilePointer );
         sscanf( LineIn, "%s %lf %d", Operator, &OperValue, &Bias );

         break;

   }

   strupr(Operator);

   fclose( FilePointer );

   if (*Operator != '*' && *Operator != '/' && *Operator != '<' && *Operator != '>' &&
       *Operator != 'M')
      return( E_INVFILTOPER );

   if (!OperValue && Operator == '/')
      return( E_INVFILTOPER );

// GET BUFFER

   OutBuffer = malloc( sizeof(_IMAGE) );

   if (!OutBuffer)
      return( IE_NOMEMIMG );

   *OutBuffer = *Image;

   OutBuffer->Saved = 0;
   OutBuffer->Buffer = malloc( Image->ResX * Image->ResY * 3 );
   
   if (!OutBuffer->Buffer)
   {
      free( OutBuffer );
      return( IE_NOMEMIMG );
   }

   memset( OutBuffer->Buffer, 0, OutBuffer->ResX * OutBuffer->ResY * 3 );

// PERFORM FILTER

   switch( ConvSize )
   {
      case 3:
         Convolution3x3(Image, OutBuffer, Grid, Operator, OperValue, Bias);
         break;

      case 5:
         Convolution5x5(Image, OutBuffer, Grid, Operator, OperValue, Bias);
         break;

      case 7:
         Convolution7x7(Image, OutBuffer, Grid, Operator, OperValue, Bias);
         break;
   }

   memcpy( Image->Buffer, OutBuffer->Buffer, Image->ResX * Image->ResY * 3 );

   free( OutBuffer->Buffer );
   free( OutBuffer );

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  Convolution3x3(IMAGE InBuffer, IMAGE OutBuffer, double *Grid, char *Operator, double OperValue, int Bias)
{
int            i, j, k, l;
unsigned char  *InVideoBuffer, *OutVideoBuffer;
char           InfoString[1000], TempStr[500];
double         SortedGrid[3*3], Temp;

   strcpy( TempStr, "Performing 3x3 filter:\n\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf\n\n" );
   strcat( TempStr, "Operation(%c %lf) + Bias(%d)" );

   sprintf( InfoString, TempStr,
            Grid[0], Grid[1], Grid[2],
            Grid[3], Grid[4], Grid[5],
            Grid[6], Grid[7], Grid[8],
            *Operator, OperValue, Bias );
   DrawMessBox( "Information", InfoString, STATUS_COLOR );

   InVideoBuffer = InBuffer->Buffer;
   OutVideoBuffer = OutBuffer->Buffer;

   if (*Operator == 'M' || *Operator == '<' || *Operator == '>')
   {
      for( i = InBuffer->ResX * 3;
           i < (InBuffer->ResY-2) * InBuffer->ResX * 3;
           i += InBuffer->ResX * 3)
      {
         for( j = 3; j < (InBuffer->ResX-2) * 3; j++ )
         {
            SortedGrid[0] = InVideoBuffer[i+j-InBuffer->ResX*3-3] * Grid[0];
            SortedGrid[1] = InVideoBuffer[i+j-InBuffer->ResX*3+0] * Grid[1];
            SortedGrid[2] = InVideoBuffer[i+j-InBuffer->ResX*3+3] * Grid[2];
            SortedGrid[3] = InVideoBuffer[i+j-3]                  * Grid[3];
            SortedGrid[4] = InVideoBuffer[i+j+0]                  * Grid[4]; 
            SortedGrid[5] = InVideoBuffer[i+j+3]                  * Grid[5]; 
            SortedGrid[6] = InVideoBuffer[i+j+InBuffer->ResX*3-3] * Grid[6];
            SortedGrid[7] = InVideoBuffer[i+j+InBuffer->ResX*3+0] * Grid[7];
            SortedGrid[8] = InVideoBuffer[i+j+InBuffer->ResX*3+3] * Grid[8];

            for( k = 0; k < 8; k++ )
               for( l = 0; l < 8; l++ )
                  if (SortedGrid[l] > SortedGrid[l+1])
                  {
                     Temp = SortedGrid[l];
                     SortedGrid[l] = SortedGrid[l+1];
                     SortedGrid[l+1] = Temp;
                  }

            switch( *Operator )
            {
               case 'M':
                  SortedGrid[4] += Bias;
                  if ( SortedGrid[4] > 255 ) SortedGrid[4] = 255;
                  if ( SortedGrid[4] < 0 ) SortedGrid[4] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[4];
                  break;

               case '>':
                  SortedGrid[8] += Bias;
                  if ( SortedGrid[8] > 255 ) SortedGrid[8] = 255;
                  if ( SortedGrid[8] < 0 ) SortedGrid[8] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[8];
                  break;

               case '<':
                  SortedGrid[0] += Bias;
                  if ( SortedGrid[0] > 255 ) SortedGrid[0] = 255;
                  if ( SortedGrid[0] < 0 ) SortedGrid[0] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[0];
                  break;
            }
         }
      }
   }
   else if (*Operator == '*' || *Operator == '/')
   {
      for( i = InBuffer->ResX * 3;
           i < (InBuffer->ResY-2) * InBuffer->ResX * 3;
           i += InBuffer->ResX * 3)
      {
         for( j = 3; j < (InBuffer->ResX-2) * 3; j++ )
         {
            Temp = (double) InVideoBuffer[i+j-InBuffer->ResX*3-3] * Grid[0] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+0] * Grid[1] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+3] * Grid[2] +
                   (double) InVideoBuffer[i+j-3]                  * Grid[3] +
                   (double) InVideoBuffer[i+j+0]                  * Grid[4] +
                   (double) InVideoBuffer[i+j+3]                  * Grid[5] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3-3] * Grid[6] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+0] * Grid[7] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+3] * Grid[8];

            if (*Operator == '*')
               Temp = Temp * OperValue + Bias;
            else
               Temp = Temp / OperValue + Bias;
         
            if( Temp > 255 ) Temp = 255;
            if( Temp < 0 ) Temp = 0;
         
            OutVideoBuffer[i+j] = Temp;
         }
      }
   }
   EraseMessBox( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  Convolution5x5(IMAGE InBuffer, IMAGE OutBuffer, double *Grid, char *Operator, double OperValue, int Bias)
{
int            i, j, k, l;
unsigned char  *InVideoBuffer, *OutVideoBuffer;
char           InfoString[1000], TempStr[500];
double         SortedGrid[5*5], Temp;

   strcpy( TempStr, "Performing 5x5 filter:\n\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf\n\n" );
   strcat( TempStr, "Operation(%c %lf) + Bias(%d)" );

   sprintf( InfoString, TempStr, 
           Grid[0], Grid[1], Grid[2], Grid[3], Grid[4], 
           Grid[5], Grid[6], Grid[7], Grid[8], Grid[9],
           Grid[10], Grid[11], Grid[12], Grid[13], Grid[14],
           Grid[15], Grid[16], Grid[17], Grid[18], Grid[19],
           Grid[20], Grid[21], Grid[22], Grid[23], Grid[24],
           *Operator, OperValue, Bias );

   DrawMessBox( "Information", InfoString, STATUS_COLOR );

   InVideoBuffer = InBuffer->Buffer;
   OutVideoBuffer = OutBuffer->Buffer;

   if (*Operator == 'M' || *Operator == '<' || *Operator == '>')
   {
      for( i = InBuffer->ResX * 3 * 2;
           i < (InBuffer->ResY-4) * InBuffer->ResX * 3;
           i += InBuffer->ResX * 3)
      {
         for( j = 6; j < (InBuffer->ResX-4) * 3; j++ )
         {
            SortedGrid[ 0] = InVideoBuffer[i+j-InBuffer->ResX*3*2-6] * Grid[ 0];
            SortedGrid[ 1] = InVideoBuffer[i+j-InBuffer->ResX*3*2-3] * Grid[ 1];
            SortedGrid[ 2] = InVideoBuffer[i+j-InBuffer->ResX*3*2+0] * Grid[ 2];
            SortedGrid[ 3] = InVideoBuffer[i+j-InBuffer->ResX*3*2+3] * Grid[ 3];
            SortedGrid[ 4] = InVideoBuffer[i+j-InBuffer->ResX*3*2+6] * Grid[ 4];
            SortedGrid[ 5] = InVideoBuffer[i+j-InBuffer->ResX*3-6]   * Grid[ 5];
            SortedGrid[ 6] = InVideoBuffer[i+j-InBuffer->ResX*3-3]   * Grid[ 6];
            SortedGrid[ 7] = InVideoBuffer[i+j-InBuffer->ResX*3+0]   * Grid[ 7];
            SortedGrid[ 8] = InVideoBuffer[i+j-InBuffer->ResX*3+3]   * Grid[ 8];
            SortedGrid[ 9] = InVideoBuffer[i+j-InBuffer->ResX*3+6]   * Grid[ 9];
            SortedGrid[10] = InVideoBuffer[i+j-6]                    * Grid[10];
            SortedGrid[11] = InVideoBuffer[i+j-3]                    * Grid[11];
            SortedGrid[12] = InVideoBuffer[i+j+0]                    * Grid[12]; 
            SortedGrid[13] = InVideoBuffer[i+j+3]                    * Grid[13]; 
            SortedGrid[14] = InVideoBuffer[i+j+6]                    * Grid[14]; 
            SortedGrid[15] = InVideoBuffer[i+j+InBuffer->ResX*3-6]   * Grid[15];
            SortedGrid[16] = InVideoBuffer[i+j+InBuffer->ResX*3-3]   * Grid[16];
            SortedGrid[17] = InVideoBuffer[i+j+InBuffer->ResX*3+0]   * Grid[17];
            SortedGrid[18] = InVideoBuffer[i+j+InBuffer->ResX*3+3]   * Grid[18];
            SortedGrid[19] = InVideoBuffer[i+j+InBuffer->ResX*3+6]   * Grid[19];
            SortedGrid[20] = InVideoBuffer[i+j+InBuffer->ResX*3*2-6] * Grid[20];
            SortedGrid[21] = InVideoBuffer[i+j+InBuffer->ResX*3*2-3] * Grid[21];
            SortedGrid[22] = InVideoBuffer[i+j+InBuffer->ResX*3*2+0] * Grid[22];
            SortedGrid[23] = InVideoBuffer[i+j+InBuffer->ResX*3*2+3] * Grid[23];
            SortedGrid[24] = InVideoBuffer[i+j+InBuffer->ResX*3*2+6] * Grid[24];

            for( k = 0; k < 24; k++ )
               for( l = 0; l < 24; l++ )
                  if (SortedGrid[l] > SortedGrid[l+1])
                  {
                     Temp = SortedGrid[l];
                     SortedGrid[l] = SortedGrid[l+1];
                     SortedGrid[l+1] = Temp;
                  }

            switch( *Operator )
            {
               case 'M':
                  SortedGrid[12] += Bias;
                  if ( SortedGrid[12] > 255 ) SortedGrid[12] = 255;
                  if ( SortedGrid[12] < 0 ) SortedGrid[12] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[12];
                  break;

               case '>':
                  SortedGrid[24] += Bias;
                  if ( SortedGrid[24] > 255 ) SortedGrid[24] = 255;
                  if ( SortedGrid[24] < 0 ) SortedGrid[24] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[24];
                  break;

               case '<':
                  SortedGrid[0] += Bias;
                  if ( SortedGrid[0] > 255 ) SortedGrid[0] = 255;
                  if ( SortedGrid[0] < 0 ) SortedGrid[0] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[0];
                  break;
            }
         }
      }
   }
   else if (*Operator == '*' || *Operator == '/')
   {
      for( i = InBuffer->ResX * 3 * 2;
           i < (InBuffer->ResY-4) * InBuffer->ResX * 3;
           i += InBuffer->ResX * 3)
      {
         for( j = 6; j < (InBuffer->ResX-4) * 3; j++ )
         {
            Temp = (double) InVideoBuffer[i+j-InBuffer->ResX*3*2-6] * Grid[ 0] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2-3] * Grid[ 1] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+0] * Grid[ 2] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+3] * Grid[ 3] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+6] * Grid[ 4] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3-6]   * Grid[ 5] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3-3]   * Grid[ 6] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+0]   * Grid[ 7] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+3]   * Grid[ 8] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+6]   * Grid[ 9] +
                   (double) InVideoBuffer[i+j-6]                    * Grid[10] +
                   (double) InVideoBuffer[i+j-3]                    * Grid[11] +
                   (double) InVideoBuffer[i+j+0]                    * Grid[12] +
                   (double) InVideoBuffer[i+j+3]                    * Grid[13] +
                   (double) InVideoBuffer[i+j+6]                    * Grid[14] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3-6]   * Grid[15] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3-3]   * Grid[16] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+0]   * Grid[17] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+3]   * Grid[18] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+6]   * Grid[19] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2-6] * Grid[20] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2-3] * Grid[21] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+0] * Grid[22] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+3] * Grid[23] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+6] * Grid[24];

            if (*Operator == '*')
               Temp = Temp * OperValue + Bias;
            else
               Temp = Temp / OperValue + Bias;
         
            if( Temp > 255 ) Temp = 255;
            if( Temp < 0 ) Temp = 0;
         
            OutVideoBuffer[i+j] = Temp;
         }
      }
   }

   EraseMessBox( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  Convolution7x7(IMAGE InBuffer, IMAGE OutBuffer, double *Grid, char *Operator, double OperValue, int Bias)
{
int            i, j, k, l;
unsigned char  *InVideoBuffer, *OutVideoBuffer;
char           InfoString[1000], TempStr[500];
double         SortedGrid[7*7], Temp;

   strcpy( TempStr, "Performing 7x7 filter:\n\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n" ); 
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n" ); 
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n" ); 
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n" );
   strcat( TempStr, "%+10lf %+10lf %+10lf %+10lf %+10lf %+10lf %+10lf\n\n" );
   strcat( TempStr, "Operation(%c %lf) + Bias(%d)" );

   sprintf( InfoString, TempStr,
            Grid[ 0], Grid[ 1], Grid[ 2], Grid[ 3], Grid[ 4], Grid[ 5], Grid[ 6],
            Grid[ 7], Grid[ 8], Grid[ 9], Grid[10], Grid[11], Grid[12], Grid[13],
            Grid[14], Grid[15], Grid[16], Grid[17], Grid[18], Grid[19], Grid[20],
            Grid[21], Grid[22], Grid[23], Grid[24], Grid[25], Grid[26], Grid[27],
            Grid[28], Grid[29], Grid[30], Grid[31], Grid[32], Grid[33], Grid[34],
            Grid[35], Grid[36], Grid[37], Grid[38], Grid[39], Grid[40], Grid[41],
            Grid[42], Grid[43], Grid[44], Grid[45], Grid[46], Grid[47], Grid[48],
            *Operator, OperValue, Bias );

   DrawMessBox( "Information", InfoString, STATUS_COLOR );

   InVideoBuffer = InBuffer->Buffer;
   OutVideoBuffer = OutBuffer->Buffer;

   if (*Operator == 'M' || *Operator == '<' || *Operator == '>')
   {
      for( i = InBuffer->ResX * 3 * 3;
           i < (InBuffer->ResY-6) * InBuffer->ResX * 3;
           i += InBuffer->ResX * 3)
      {
         for( j = 9; j < (InBuffer->ResX-6) * 3; j++ )
         {
            SortedGrid[ 0] = InVideoBuffer[i+j-InBuffer->ResX*3*3-9] * Grid[ 0];
            SortedGrid[ 1] = InVideoBuffer[i+j-InBuffer->ResX*3*3-6] * Grid[ 1];
            SortedGrid[ 2] = InVideoBuffer[i+j-InBuffer->ResX*3*3-3] * Grid[ 2];
            SortedGrid[ 3] = InVideoBuffer[i+j-InBuffer->ResX*3*3+0] * Grid[ 3];
            SortedGrid[ 4] = InVideoBuffer[i+j-InBuffer->ResX*3*3+3] * Grid[ 4];
            SortedGrid[ 5] = InVideoBuffer[i+j-InBuffer->ResX*3*3+6] * Grid[ 5];
            SortedGrid[ 6] = InVideoBuffer[i+j-InBuffer->ResX*3*3+9] * Grid[ 6];
            SortedGrid[ 7] = InVideoBuffer[i+j-InBuffer->ResX*3*2-9] * Grid[ 7];
            SortedGrid[ 8] = InVideoBuffer[i+j-InBuffer->ResX*3*2-6] * Grid[ 8];
            SortedGrid[ 9] = InVideoBuffer[i+j-InBuffer->ResX*3*2-3] * Grid[ 9];
            SortedGrid[10] = InVideoBuffer[i+j-InBuffer->ResX*3*2+0] * Grid[10];
            SortedGrid[11] = InVideoBuffer[i+j-InBuffer->ResX*3*2+3] * Grid[11];
            SortedGrid[12] = InVideoBuffer[i+j-InBuffer->ResX*3*2+6] * Grid[12];
            SortedGrid[13] = InVideoBuffer[i+j-InBuffer->ResX*3*2+9] * Grid[13];
            SortedGrid[14] = InVideoBuffer[i+j-InBuffer->ResX*3-9]   * Grid[14];
            SortedGrid[15] = InVideoBuffer[i+j-InBuffer->ResX*3-6]   * Grid[15];
            SortedGrid[16] = InVideoBuffer[i+j-InBuffer->ResX*3-3]   * Grid[16];
            SortedGrid[17] = InVideoBuffer[i+j-InBuffer->ResX*3+0]   * Grid[17];
            SortedGrid[18] = InVideoBuffer[i+j-InBuffer->ResX*3+3]   * Grid[18];
            SortedGrid[19] = InVideoBuffer[i+j-InBuffer->ResX*3+6]   * Grid[19];
            SortedGrid[20] = InVideoBuffer[i+j-InBuffer->ResX*3+9]   * Grid[20];
            SortedGrid[21] = InVideoBuffer[i+j-9]                    * Grid[21];
            SortedGrid[22] = InVideoBuffer[i+j-6]                    * Grid[22];
            SortedGrid[23] = InVideoBuffer[i+j-3]                    * Grid[23];
            SortedGrid[24] = InVideoBuffer[i+j+0]                    * Grid[24]; 
            SortedGrid[25] = InVideoBuffer[i+j+3]                    * Grid[25]; 
            SortedGrid[26] = InVideoBuffer[i+j+6]                    * Grid[26]; 
            SortedGrid[27] = InVideoBuffer[i+j+9]                    * Grid[27]; 
            SortedGrid[28] = InVideoBuffer[i+j+InBuffer->ResX*3-9]   * Grid[28];
            SortedGrid[29] = InVideoBuffer[i+j+InBuffer->ResX*3-6]   * Grid[29];
            SortedGrid[30] = InVideoBuffer[i+j+InBuffer->ResX*3-3]   * Grid[30];
            SortedGrid[31] = InVideoBuffer[i+j+InBuffer->ResX*3+0]   * Grid[31];
            SortedGrid[32] = InVideoBuffer[i+j+InBuffer->ResX*3+3]   * Grid[32];
            SortedGrid[33] = InVideoBuffer[i+j+InBuffer->ResX*3+6]   * Grid[33];
            SortedGrid[34] = InVideoBuffer[i+j+InBuffer->ResX*3+9]   * Grid[34];
            SortedGrid[35] = InVideoBuffer[i+j+InBuffer->ResX*3*2-9] * Grid[35];
            SortedGrid[36] = InVideoBuffer[i+j+InBuffer->ResX*3*2-6] * Grid[36];
            SortedGrid[37] = InVideoBuffer[i+j+InBuffer->ResX*3*2-3] * Grid[37];
            SortedGrid[38] = InVideoBuffer[i+j+InBuffer->ResX*3*2+0] * Grid[38];
            SortedGrid[39] = InVideoBuffer[i+j+InBuffer->ResX*3*2+3] * Grid[39];
            SortedGrid[40] = InVideoBuffer[i+j+InBuffer->ResX*3*2+6] * Grid[40];
            SortedGrid[41] = InVideoBuffer[i+j+InBuffer->ResX*3*2+9] * Grid[41];
            SortedGrid[42] = InVideoBuffer[i+j+InBuffer->ResX*3*3-9] * Grid[42];
            SortedGrid[43] = InVideoBuffer[i+j+InBuffer->ResX*3*3-6] * Grid[43];
            SortedGrid[44] = InVideoBuffer[i+j+InBuffer->ResX*3*3-3] * Grid[44];
            SortedGrid[45] = InVideoBuffer[i+j+InBuffer->ResX*3*3+0] * Grid[45];
            SortedGrid[46] = InVideoBuffer[i+j+InBuffer->ResX*3*3+3] * Grid[46];
            SortedGrid[47] = InVideoBuffer[i+j+InBuffer->ResX*3*3+6] * Grid[47];
            SortedGrid[48] = InVideoBuffer[i+j+InBuffer->ResX*3*3+9] * Grid[48];

            for( k = 0; k < 48; k++ )
               for( l = 0; l < 48; l++ )
                  if (SortedGrid[l] > SortedGrid[l+1])
                  {
                     Temp = SortedGrid[l];
                     SortedGrid[l] = SortedGrid[l+1];
                     SortedGrid[l+1] = Temp;
                  }

            switch( *Operator )
            {
               case 'M':
                  SortedGrid[24] += Bias;
                  if ( SortedGrid[24] > 255 ) SortedGrid[24] = 255;
                  if ( SortedGrid[24] < 0 ) SortedGrid[24] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[24];
                  break;

               case '>':
                  SortedGrid[48] += Bias;
                  if ( SortedGrid[48] > 255 ) SortedGrid[48] = 255;
                  if ( SortedGrid[48] < 0 ) SortedGrid[48] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[48];
                  break;

               case '<':
                  SortedGrid[0] += Bias;
                  if ( SortedGrid[0] > 255 ) SortedGrid[0] = 255;
                  if ( SortedGrid[0] < 0 ) SortedGrid[0] = 0;
                  OutVideoBuffer[i+j] = SortedGrid[0];
                  break;
            }
         }
      }
   }
   else if (*Operator == '*' || *Operator == '/')
   {
      for( i = InBuffer->ResX * 3 * 3;
           i < (InBuffer->ResY-6) * InBuffer->ResX * 3;
           i += InBuffer->ResX * 3)
      {
         for( j = 9; j < (InBuffer->ResX-6) * 3; j++ )
         {
            Temp = (double) InVideoBuffer[i+j-InBuffer->ResX*3*3-9] * Grid[ 0] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*3-6] * Grid[ 1] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*3-3] * Grid[ 2] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*3+0] * Grid[ 3] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*3+3] * Grid[ 4] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*3+6] * Grid[ 5] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*3+9] * Grid[ 6] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2-9] * Grid[ 7] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2-6] * Grid[ 8] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2-3] * Grid[ 9] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+0] * Grid[10] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+3] * Grid[11] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+6] * Grid[12] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3*2+9] * Grid[13] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3-9]   * Grid[14] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3-6]   * Grid[15] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3-3]   * Grid[16] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+0]   * Grid[17] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+3]   * Grid[18] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+6]   * Grid[19] +
                   (double) InVideoBuffer[i+j-InBuffer->ResX*3+9]   * Grid[20] +
                   (double) InVideoBuffer[i+j-9]                    * Grid[21] +
                   (double) InVideoBuffer[i+j-6]                    * Grid[22] +
                   (double) InVideoBuffer[i+j-3]                    * Grid[23] +
                   (double) InVideoBuffer[i+j+0]                    * Grid[24] +
                   (double) InVideoBuffer[i+j+3]                    * Grid[25] +
                   (double) InVideoBuffer[i+j+6]                    * Grid[26] +
                   (double) InVideoBuffer[i+j+9]                    * Grid[27] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3-9]   * Grid[28] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3-6]   * Grid[29] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3-3]   * Grid[30] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+0]   * Grid[31] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+3]   * Grid[32] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+6]   * Grid[33] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3+9]   * Grid[34] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2-9] * Grid[35] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2-6] * Grid[36] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2-3] * Grid[37] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+0] * Grid[38] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+3] * Grid[39] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+6] * Grid[40] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*2+9] * Grid[41] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3-9] * Grid[42] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3-6] * Grid[43] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3-3] * Grid[44] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3+0] * Grid[45] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3+3] * Grid[46] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3+6] * Grid[47] +
                   (double) InVideoBuffer[i+j+InBuffer->ResX*3*3+9] * Grid[48];

            if (*Operator == '*')
               Temp = Temp * OperValue + Bias;
            else
               Temp = Temp / OperValue + Bias;
         
            if( Temp > 255 ) Temp = 255;
            if( Temp < 0 ) Temp = 0;
         
            OutVideoBuffer[i+j] = Temp;
         }
      }
   }
   EraseMessBox( );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   int   ReadLine( char *Line, FILE *FilePointer )
{
int   count = 0;
char  inchar;

   while(1)
   {
      inchar = fgetc( FilePointer );

      if (inchar == '\n' || inchar == '\r')
         if (count != 0)
         {
            Line[count] = '\0';
            return 1;
         }
         else
         {
            count = 0;
            continue;
         }

      if (feof( FilePointer ))
      {
         Line[count] = '\0';
         return 0;
      }
      else
      {
         Line[count++] = inchar;
      }
   }
}
 
/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [CONV.C      ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

