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
  ±   [CMORPH.C    ] - The Complete Morph main program file                  ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <io.h>
#include <time.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <graph.h>
#include <time.h>
#include "cmorph.h"

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   ZoomOn         = 1;
int   TotalFrames    = 30;
int   StartFrame     = 1;
int   EndFrame       = 30;
int   WarpTightness  = 50;
int   InterpTightness = 50;
int   Fade50         = 50;
int   FStart         = 0;
int   FEnd           = 100;
int   Warp50         = 50;
int   WStart         = 0;
int   WEnd           = 100;
int   Prev50         = 50;
int   OutputFileType = TGA;
int   MeshType       = CATMULL;
int   InterpMeshType = CATMULL;
int   MorphWarpFlag  = MORPH;
int   AutoMorph      = 0;
int   AutoExit       = 0;
int   CurvesOn       = 1;
int   QuietMode      = 0;
int   DebugMode      = 0;
int   BackDoor       = 0;
int   MenuMode       = MAINMENU;
int   GraphMode      = 0;
int   FastScreen     = 1;
int   UpdateFastPrev = 0;
int   UpdateFastPref = 0;
int   UpdateFastFile = 0;
int   UpdateFastProc = 0;
int   UpdateFastFlic = 0;
int   FrameOffset    = 0;
int   FlicType       = FLI;
int   FlicOnOff      = FLIC_ON;
int   FlicColorMono  = COLOR;
int   FlicAutoRew    = NO_AUTOREW;
int   FlicSpeed      = 0;
int   PrevColor      = COLOR;
int   OverlayThresholdL = 1;
int   OverlayThresholdH = 255;
int   BackThresholdL = 0;
int   BackThresholdH = 0;
int   ArithType      = ARITH_OFF;
int   Background     = BACK_OFF;
int   ShowBetweensEnable = 0;
int   FastMorphMode = 0;
int   UpdateImages = 0;
int   VMode;
int   TGAUpsideDownFlag = TOP_DOWN;
int   MeshColor;
int   CursColor;

int   BypassNotice = 0;

char  FlicFileName[180] = {DEFAULT_FLIC_FILE};
char  FilterFileName[180] = {DEFAULT_FILTER_FILE};
char  SourceFiltName[180] = {"BLUR"};
char  TargetFiltName[180] = {"BLUR"};
char  ProgramPath[80];

int   BUTTON_FACE_COLOR;
int   PREF_TITLE_COLOR;
int   PREV_TITLE_COLOR;
int   FLIC_TITLE_COLOR;
int   PROC_TITLE_COLOR;
int   EDIT_TITLE_COLOR;
int   EDIT_WINDOW_COLOR;
int   FILE_WINDOW_COLOR;
int   FILE_TITLE_COLOR;
int   MBOX_WINDOW_COLOR;
int   OPER_WINDOW_COLOR;
int   OPER_TITLE_COLOR;
int   PROMPT_COLOR;
int   BUTTON_TEXT_COLOR;
int   RBUTTON_TEXT_COLOR;
int   ENTER_TEXT_COLOR;
int   TEXT_CURSOR_COLOR;
int   OPER_TITLE_TEXT_COLOR;
int   PROMPT_TEXT_COLOR;
int   EDIT_TITLE_TEXT_COLOR;
int   INFO_TEXT_COLOR;
int   DEMO_TEXT_COLOR;
int   ZOOM_CURSOR_COLOR;
int   SHADOW_COLOR;
int   HIGHLIGHT_COLOR;
int   MBOX_TITLE_TEXT_COLOR;
int   MBOX_TEXT_COLOR;
int   BULLSEYE_OUTLINE_COLOR;
int   R_MESH_COLOR;
int   G_MESH_COLOR;
int   B_MESH_COLOR;
int   X_MESH_COLOR;
int   R_CURS_COLOR;
int   G_CURS_COLOR;
int   B_CURS_COLOR;
int   X_CURS_COLOR;
int   PREF_TITLE_TEXT_COLOR;
int   PREV_TITLE_TEXT_COLOR;
int   FLIC_TITLE_TEXT_COLOR;
int   PROC_TITLE_TEXT_COLOR;
int   TEXT_COLOR;
int   FILE_TITLE_TEXT_COLOR;
int   BACK_COLOR;
int   TITLE_COLOR;
int   CURSOR_COLOR;
int   ERROR_BOX_COLOR;
int   HELP_BOX_COLOR;
int   STATUS_COLOR;
int   PREF_WINDOW_COLOR;
int   PREV_WINDOW_COLOR;
int   FLIC_WINDOW_COLOR;
int   PROC_WINDOW_COLOR;
int   DEFAULT_MESH_COLOR;
int   DEFAULT_CURSOR_COLOR;
int   BOTTOM_COLOR;
int   PREF_DB_HEAD_COLOR;
int   PREF_DB_SHEAD_COLOR;
int   PREF_DB_HLINE_COLOR;
int   PREV_DB_HEAD_COLOR;
int   PREV_DB_SHEAD_COLOR;
int   PREV_DB_HLINE_COLOR;
int   FLIC_DB_HEAD_COLOR;
int   FLIC_DB_SHEAD_COLOR;
int   FLIC_DB_HLINE_COLOR;
int   PROC_DB_HEAD_COLOR;
int   PROC_DB_SHEAD_COLOR;
int   PROC_DB_HLINE_COLOR;
int   OPER_DB_HEAD_COLOR;
int   OPER_DB_SHEAD_COLOR;
int   OPER_DB_HLINE_COLOR;
int   FILE_DB_HEAD_COLOR;
int   FILE_DB_SHEAD_COLOR;
int   FILE_DB_HLINE_COLOR;
int   EDIT_DB_HEAD_COLOR;
int   EDIT_DB_SHEAD_COLOR;
int   EDIT_DB_HLINE_COLOR;

char  RegisteryDate[100] = {"ERIAMJH\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
char  *PName;

char	SourceImageFile[80] = {"\0"};
char	TargetImageFile[80] = {"\0"};

char  Copyright[100];

_RECT OperRect;
_RECT PrefRect;
_RECT EditRect;
_RECT FileRect;
_RECT ProcRect;
_RECT PrevRect;
_RECT FlicRect;

char	MeshFile[80]   = {""};
char  OutBaseName[80] = {"FRAME"};
char  OutArithFile[80] = {""};
char  BackgroundFile[80] = {""};

IMAGE SourceImage = 0, TargetImage = 0;
MESH  SourceMesh = 0, TargetMesh = 0;

_BUTTON  PerfButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Pref Menu"};
_BUTTON  OperButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " GO Menu "};
_BUTTON  EditButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Edit Menu"};
_BUTTON  FileButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "File Menu"};
_BUTTON  ProcButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Filters "};
_BUTTON  PrevButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Preview "};
_BUTTON  FlicButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "  Flics  "};

_BUTTON  ExitButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "   Leave   "};
_BUTTON  GoButton            = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "    GO!    "};
_BUTTON  ShellButton         = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " DOS Shell "};
_BUTTON  HelpButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "   HELP!   "};
_BUTTON  AboutButton         = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "   About   "};

_BUTTON  MeshSwapButton      = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "   Swap    "};
_BUTTON  CopySDMButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Copy S->D "};
_BUTTON  HFlipSMButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Horz Flip "};
_BUTTON  VFlipSMButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Vert Flip "};
_BUTTON  CopyDSMButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Copy S<-D "};
_BUTTON  HFlipDMButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Horz Flip "};
_BUTTON  VFlipDMButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Vert Flip "};
_BUTTON  ImageSwapButton     = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "   Swap    "};
_BUTTON  HFlipSIButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Horz Flip "};
_BUTTON  VFlipSIButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Vert Flip "};
_BUTTON  HFlipDIButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Horz Flip "};
_BUTTON  VFlipDIButton       = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Vert Flip "};


_BUTTON  MeshCATButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Catmull"};
_BUTTON  MeshSPLButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Bi-cubic"};
_BUTTON  MeshSTRButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Straight"};
_BUTTON  InterpMeshCATButton = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Catmull"};
_BUTTON  InterpMeshSPLButton = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Bi-cubic"};
_BUTTON  InterpMeshSTRButton = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Straight"};
_BUTTON  CurvesButton        = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Display Curves"};
_BUTTON  ZoomButton          = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Zoom"};
_BUTTON  CursRButton         = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Red"};
_BUTTON  CursGButton         = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Green"};
_BUTTON  CursBButton         = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Blue"};
_BUTTON  CursXButton         = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Negative"};
_BUTTON  MColorRButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Red"};
_BUTTON  MColorGButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Green"};
_BUTTON  MColorBButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Blue"};
_BUTTON  MColorXButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Negative"};
_BUTTON  OutputTGAButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " TGA"};
_BUTTON  OutputIPIButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " IPI"};
_BUTTON  OutputIMGButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " IMG"};
_BUTTON  OutputGIFButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " GIF"};
_BUTTON  OutputBMPButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " BMP"};
_BUTTON  OutputPCXButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " PCX"};
_BUTTON  MorphButton         = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Morph"};
_BUTTON  WarpButton          = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Warp"};
_BUTTON  FadeButton          = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Fade"};
_BUTTON  TGAUpsideDownButton = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " TGAs Bottom-Up"};

_BUTTON  FlicFLIButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " FLI"};
_BUTTON  FlicFLCButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " FLC"};
_BUTTON  FlicFLXButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " FLX"};
_BUTTON  FlicFLHButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " FLH"};
_BUTTON  FlicFLTButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " FLT"};
_BUTTON  FlicPingButton    = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " PingPong"};
_BUTTON  FlicOnOffButton   = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " On"};
_BUTTON  FlicColorButton   = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Monochrome"};
_PROMPT  FlicFilePrompt    = {{0,0,0,0},19};
_PROMPT  FlicSpeedPrompt   = {{0,0,0,0},2};

_BUTTON  KwikScreenButton    = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Kwik-screen"};
_BUTTON  SoundEnableButton   = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Sounds"};
_BUTTON  ShowBetweensButton  = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Show inbetweens"};
_BUTTON  FastMorphButton     = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Fast-morph"};

_PROMPT  FilterFileNamePrompt   = {{0,0,0,0},60};
_PROMPT  SourceFilterNamePrompt = {{0,0,0,0},25};
_PROMPT  TargetFilterNamePrompt = {{0,0,0,0},25};
_BUTTON  DoSourceFilterButton   = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Do Filter "};
_BUTTON  DoTargetFilterButton   = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, " Do Filter "};

_PROMPT  MeshNamePrompt        = {{0,0,0,0},48};
_PROMPT  SrcImagePrompt        = {{0,0,0,0},48};
_PROMPT  DstImagePrompt        = {{0,0,0,0},48};
_PROMPT  OutCharsPrompt        = {{0,0,0,0},48};

_BUTTON  LoadMeshButton        = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Load"};
_BUTTON  SaveMeshButton        = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Save"};

_BUTTON  ImpMeshButton         = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Import"};
_BUTTON  ExpMeshButton         = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Export"};

_BUTTON  LoadSourceImageButton = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Load"};
_BUTTON  LoadTargetImageButton = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Load"};
_BUTTON  SaveSourceImageButton = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Save"};
_BUTTON  SaveTargetImageButton = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Save"};

_BUTTON  OffOutButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Off"};
_BUTTON  AddOutButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Add"};
_BUTTON  SubOutButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Sub"};
_BUTTON  AveOutButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Ave"};
_BUTTON  OvrOutButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Ovr"};
_PROMPT  OvrThreshPromptL   = {{0,0,0,0},3};
_PROMPT  OvrThreshPromptH   = {{0,0,0,0},3};
_PROMPT  OutArithFilePrompt = {{0,0,0,0},41};

_BUTTON  BackOffButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Off"};
_BUTTON  BackOnButton        = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " On"};
_PROMPT  BackThreshPromptL   = {{0,0,0,0},3};
_PROMPT  BackThreshPromptH   = {{0,0,0,0},3};
_PROMPT  BackFilePrompt = {{0,0,0,0},41};

_PROMPT  FrameOffPrompt        = {{0,0,0,0},5};
_PROMPT  FramesPrompt          = {{0,0,0,0},5};
_PROMPT  StartFramePrompt      = {{0,0,0,0},5};
_PROMPT  EndFramePrompt        = {{0,0,0,0},5};
_PROMPT  InterpTightnessPrompt = {{0,0,0,0},3};
_PROMPT  WarpTightnessPrompt   = {{0,0,0,0},3};
_PROMPT  Fade50Prompt          = {{0,0,0,0},3};
_PROMPT  FadeStartPrompt       = {{0,0,0,0},3};
_PROMPT  FadeEndPrompt         = {{0,0,0,0},3};
_PROMPT  Warp50Prompt          = {{0,0,0,0},3};
_PROMPT  WarpStartPrompt       = {{0,0,0,0},3};
_PROMPT  WarpEndPrompt         = {{0,0,0,0},3};

_BUTTON  PrevGoButton          = {{0,0,0,0},STAND_BUTTON, BUTTON_UP, "Show Preview"};
_BUTTON  PrevColorButton       = {{0,0,0,0},RADIO_BUTTON, BUTTON_UP, " Color"};
_PROMPT  PrevFramePrompt       = {{0,0,0,0},3};

_RECT ZoomWindow;
_RECT SourceInfoWindow;
_RECT TargetInfoWindow;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

GINFO    GI;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#ifndef NOREG
static   char  FlicNamesList[3000][80];
#endif
static   int   NamesFound;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

static   void  PrintUsage( char *ProgName );
static   void  PrintCopyright( );
static   void  UpdateMorphStatusWindow( int CurrentFrame );
static   void  UpdateFlicStatusWindow( int CurrentFrame, int EndFrame );
static   void  ShowStatisticsWindow( double DiffTime );
#ifndef NOREG
static   void  Convert24To8( IMAGE Image );
static   void  MakeImageMono( IMAGE Image );
static   int   FinishFlicOutput( int ResX, int ResY );
#endif
static   int   OutputAnImage( char *FileName, IMAGE Image );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  main(int argc, char *argv[])
{
int   i, RetCode, Color16Mode = 0;
char  *ptr, TempStr[80];

#ifdef NOREG
sprintf( Copyright, "The Complete Morph v%d.%02dub Copyright 1993/1994 Paul Nettle. All rights reserved.", VERSION_MS, VERSION_LS );
#endif

#ifdef REG
sprintf( Copyright, "The Complete Morph v%d.%02drb Copyright 1993/1994 Paul Nettle. All rights reserved.", VERSION_MS, VERSION_LS );
#endif

#ifdef PRO
sprintf( Copyright, "The Complete Morph Pro v%d.%02db Copyright 1993/1994 Paul Nettle,All rights reserved", VERSION_MS, VERSION_LS );
#endif

   PName = argv[0];
   
   setbuf(stdout, 0);

   PrintCopyright( );

#ifdef DEMO
   printf( "THIS IS A DEMO COPY...NOT FOR RESALE\n" );
   printf( "THIS IS A DEMO COPY...NOT FOR RESALE\n" );
   printf( "THIS IS A DEMO COPY...NOT FOR RESALE\n" );
   printf( "THIS IS A DEMO COPY...NOT FOR RESALE\n" );
   printf( "THIS IS A DEMO COPY...NOT FOR RESALE\n\n" );
   printf( "----------- Press any key ----------\n\n" );
   getch();
#endif

   strcpy( ProgramPath, argv[0] );
   ptr = strrchr( ProgramPath, '\\' );
   *ptr = '\0';

   for( i = 1; i < argc; i++ )
   {
      if (argv[i][0] == '/' || argv[i][0] == '-')
      {
         switch( toupper (argv[i][1]) )
         {
            case '?':
            case 'H':
               PrintUsage( argv[0] );
               ExitSystem(1);

            case 'B':
               AutoMorph = 1;
               if (toupper(argv[i][2]) == 'X')
                  AutoExit = 1;
               break;

            case 'C':
               Color16Mode = 1;
               break;

            case 'G':
               GraphMode = atoi( &argv[i][2] );
               break;

            case 'J':
               if (!strcmp(&argv[i][2], "HGFDSA"))
                  BackDoor = 1;
               break;

            case 'M':
               strcpy( MeshFile, &argv[i][2] );
               break;

            case 'P':
               if (!strcmp(&argv[i][2], "DNDEBUG"))
                  DebugMode = 1;
               break;

            case 'S':
               if (!strcmp(&argv[i][2], "KIPNOTICE"))
               {
                  BypassNotice = 1;
                  break;
               }

               SetMouseSensativity( atoi(&argv[i][2]) );
               break;

            default:
               printf( "Unknown parameter:  \"%s\"\n", argv[i] );
               PrintUsage( argv[0] );
               ExitSystem(1);
         }
      }
      else
      {
         if (!SourceImageFile[0])
            strcpy( SourceImageFile, argv[i] );
         else if (!TargetImageFile[0])
            strcpy( TargetImageFile, argv[i] );
         else if (!MeshFile[0])
            strcpy( MeshFile, argv[i] );
         else
         {
            printf( "Unknown parameter:  \"%s\"\n", argv[i] );
            PrintUsage( argv[0] );
            ExitSystem(1);
         }
      }
   }

   if(argc < 3)
   {
      PrintUsage( argv[0] );
      ExitSystem(1);
   }

   if (Color16Mode)
   {
      if (GraphMode == 400)
      {
         printf( "Unable to set 16 color 640x400 mode\n" );
         PrintUsage( argv[0] );
         ExitSystem(1);
      }
      else
      {
         GraphMode += 1000;
      }
   }

   GI = SetGraphicsMode( GraphMode );

   /* Show intro animation */
   sprintf( TempStr, "%s\\INTRO.ANM", ProgramPath );
   i = PlayANM( TempStr );

   if (!i)
   {
      RestoreMode( );
      printf( "Loading [Source] image...\r" );
   }

   /* Create small image buffers since ReadImage expects there to already
      be one created */
   SourceImage = CreateImage( 2, 2 );
   TargetImage = CreateImage( 2, 2 );

   RetCode = ReadImage(SourceImageFile, &SourceImage);

   if (RetCode != E_NONE)
      FatalError( "Unable to load source image", RetCode);

   if (!i)
      printf( "Loading [Destintion] image...\n" );

   RetCode = ReadImage(TargetImageFile, &TargetImage);

   if (RetCode != E_NONE)
      FatalError( "Unable to load target image", RetCode);

   if (SourceImage->ResX != TargetImage->ResX ||
       SourceImage->ResY != TargetImage->ResY )
      FatalError( "Bad images", E_BADRESOLUTION );

   GI = SetGraphicsMode( GraphMode );
   InitScreen( SourceImage, TargetImage );

   // PUT THE IMAGES IN THE WINDOWS FOR THE FIRST TIME
   DrawImage( SourceImage );
   RestoreImage( SourceImage );

   DrawImage( TargetImage );
   RestoreImage( TargetImage );

   if (MeshFile[0])
   {
      LoadMesh( );
   }
   else
   {
      sprintf( MeshFile, "%s", DEFAULT_MESH );
      CreateMesh( &SourceMesh );
      CreateMesh( &TargetMesh );
   }

   if (AutoMorph)
   {
      RetCode = Morph( SourceImage, SourceMesh, TargetImage, TargetMesh, NO_SINGLE );

      if (RetCode != E_NONE)
         FatalError( "Unable to morph", RetCode );
   }

   if (!AutoExit)
   {
#ifdef NOREG
      /* Clear out keyboard buffer */

      while(kbhit()) if (!getch()) getch();
#endif

      DoAbout( );
      Input( );
   }

   UnInitSystem( );

   ExitSystem( 0 );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UnInitSystem( )
{
   RestoreMode( );

   UnInitMouse( );

   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ExitSystem( int ExitCode )
{
   printf( "Thank you for using CMorph, The Complete Morph!\n" );

#ifdef NOREG
{
int   Handle, overdue = 0, bytesread, found = 0;
long  located = 0;
static   char  buf[8000], *temp;
time_t start_time, end_time;

   Handle = open( PName, O_RDWR | O_BINARY, S_IREAD | S_IWRITE );

   if (Handle > 1)
   {
      while(1)
      {
         /* REMEMBER WHERE THIS CHUNK IS LOCATED */
         located = lseek( Handle, 0, SEEK_CUR );

         memset( buf, 0, 8000 );

         /* READ IN A CHUNK */
         bytesread = read( Handle, buf, 4000);

         /* THE END? */
         if (bytesread < 1)
            break;

         /* LOOK THROUGH IT FOR THE STRING */
         temp = buf;

         while( 1 )
         {
            temp = memchr( temp, 'E', bytesread );

            if (temp == NULL)
               break;

            if (!strncmp(temp, "ERIA", 4) && !strncmp(&temp[4], "MJH", 3))
            {
               found = 1;
               break;
            }

            temp = &temp[1];
         }

         /* NOT FOUND? MOVE ON... */
         if (!found)
            continue;

         /* FOUND!  GET THE TIME */
         temp = &temp[7];
         start_time = *(time_t *)temp;

         /* BYPASS THE NOTICE? */
         if (BypassNotice)
         {
            printf( "Resetting notice...Press any key to continue%c\n", 7 );
            getch();
            lseek( Handle, located, SEEK_SET );
            temp[0] = 0;
            temp[1] = 0;
            temp[2] = 0;
            temp[3] = 0;
            temp[4] = 0;
            temp[5] = 0;
            temp[6] = 0;
            temp[7] = 0;
            temp[8] = 0;
            temp[9] = 0;
            write( Handle, buf, 4000);
         }
         /* IS THIS THE FIRST TIME WE'RE RUN? */
         else if (!start_time)
         {
            lseek( Handle, located, SEEK_SET );
            *(time_t *)temp = time( NULL );
            write( Handle, buf, 4000);
         }
         /* HAVE WE NOT ALREADY BEEN MARKED AS OVERDUE? */
         else if (start_time != 1)
         {
            end_time = time( NULL );

            /* OVERDUE? */
            if (difftime(end_time, start_time) > 30.0*24.0*60.0*60.0)
            {
               /* OVERDUE!  MAKE SURE WE MARK OURSELVES PERMANENTLY */
               overdue = 1;
               lseek( Handle, located, SEEK_SET );
               *(time_t *)temp = 1;
               write( Handle, buf, 4000);
            }
         }
         /* ALREADY MARKED AS OVERDUE */
         else
            overdue = 1;

         break;
      }
      close( Handle );
   }

   if (overdue)
   {
      printf( "%c\n", 7);
      printf( "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€                               DID YOU KNOW?                               €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ€€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€    It's been more than 30 days since you've first run this program and    €€\n");
      printf( "€€    this is an EVALUATION copy.                                            €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€    Shareware depends on people like you to register.  PLEASE register.    €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€    This message will continue to appear, but the program will still run   €€\n");
      printf( "€€    fine, just like it has for the last 30 days.                           €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€    <If you have had this program for less than 30 days, you did not get   €€\n");
      printf( "€€    an original copy.  You may contact the author for an original copy.>   €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€\n");
   }

   printf( "\nIf you like using CMorph, please register it.  Thanks!\n" );
}
#endif

#ifdef PROTEMP
{
int   Handle, overdue = 0, bytesread, found = 0;
long  located = 0;
static   char  buf[8000], *temp;
time_t start_time, end_time;

   Handle = open( PName, O_RDWR | O_BINARY, S_IREAD | S_IWRITE );

   if (Handle > 1)
   {
      while(1)
      {
         /* REMEMBER WHERE THIS CHUNK IS LOCATED */
         located = lseek( Handle, 0, SEEK_CUR );

         memset( buf, 0, 8000 );

         /* READ IN A CHUNK */
         bytesread = read( Handle, buf, 4000);

         /* THE END? */
         if (bytesread < 1)
            break;

         /* LOOK THROUGH IT FOR THE STRING */
         temp = buf;

         while( 1 )
         {
            temp = memchr( temp, 'E', bytesread );

            if (temp == NULL)
               break;

            if (!strncmp(temp, "ERIA", 4) && !strncmp(&temp[4], "MJH", 3))
            {
               found = 1;
               break;
            }

            temp = &temp[1];
         }

         /* NOT FOUND? MOVE ON... */
         if (!found)
            continue;

         /* FOUND!  GET THE TIME */
         temp = &temp[7];
         start_time = *(time_t *)temp;

         /* BYPASS THE NOTICE? */
         if (BypassNotice)
         {
            printf( "Resetting notice...Press any key to continue%c\n", 7 );
            getch();
            lseek( Handle, located, SEEK_SET );
            temp[0] = 0;
            temp[1] = 0;
            temp[2] = 0;
            temp[3] = 0;
            temp[4] = 0;
            temp[5] = 0;
            temp[6] = 0;
            temp[7] = 0;
            temp[8] = 0;
            temp[9] = 0;
            write( Handle, buf, 4000);
         }
         /* IS THIS THE FIRST TIME WE'RE RUN? */
         else if (!start_time)
         {
            lseek( Handle, located, SEEK_SET );
            *(time_t *)temp = time( NULL );
            write( Handle, buf, 4000);
         }
         /* HAVE WE NOT ALREADY BEEN MARKED AS OVERDUE? */
         else if (start_time != 1)
         {
            end_time = time( NULL );

            /* OVERDUE? */
            if (difftime(end_time, start_time) > 60.0*24.0*60.0*60.0)
            {
               /* OVERDUE!  MAKE SURE WE MARK OURSELVES PERMANENTLY */
               overdue = 1;
               lseek( Handle, located, SEEK_SET );
               *(time_t *)temp = 1;
               write( Handle, buf, 4000);
            }
         }
         /* ALREADY MARKED AS OVERDUE */
         else
            overdue = 1;

         break;
      }
      close( Handle );
   }

   if (overdue)
   {
      printf( "%c\n", 7);
      printf( "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€                               DID YOU KNOW?                               €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ€€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€    This software has not been paid for.  Please contact the author for    €€\n");
      printf( "€€    payment information.                                                   €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€    This message will continue to appear, but the program will still run   €€\n");
      printf( "€€    fine, just like it has for the last 60 days.                           €€\n");
      printf( "€€                                                                           €€\n");
      printf( "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€\n");
   }

   printf( "\nThis is an evaluation copy of the PRO software for Pearl, Inc. ONLY.\n" );
}
#endif

   exit(ExitCode);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  PrintUsage( char *ProgName )
{
   printf( "Usage:  %s [options] <source> <target>\n", ProgName );
   printf( "\n" );
   printf( "  [Options]:\n" );
   printf( "    [-? | -H]˙˙˙˙˙˙List this help information\n" );
   printf( "\n" );
   printf( "    [-B[X]]˙˙˙˙˙˙˙˙Batch mode (auto morph on startup) [with auto eXit]\n" );
   printf( "\n" );
   printf( "    [-C]˙˙˙˙˙˙˙˙˙˙˙Force graphics mode to 16 colors (muse be used with -G)\n" );
   printf( "                     Default˙˙˙˙˙˙˙˙256 colors\n" );
   printf( "\n" );
   printf( "    [-Gxxx]˙˙˙˙˙˙˙˙Force vertical resolution to xxx scanlines\n" );
   printf( "                     Valid values˙˙˙400/480/600/768\n" );
   printf( "                     Default˙˙˙˙˙˙˙˙auto-detect\n" );
   printf( "\n" );
   printf( "    [-Mfilename]˙˙˙Specify mesh filename\n" );
   printf( "\n" );
   printf( "    [-Sx]˙˙˙˙˙˙˙˙˙˙Set mouse sensativity (0=fastest -or- 6=slowest)\n" );
   printf( "                     Default˙˙˙˙˙˙˙˙4\n" );
   printf( "\n" );
   printf( "-- MORE --\r" );
   if (!getch())getch();
   printf( "    source˙˙˙˙˙˙˙˙˙Source image (IPI/TGA/BMP/IMG/GIF/PCX)\n" );
   printf( "\n" );
   printf( "    target˙˙˙˙˙˙˙˙˙Target image (IPI/TGA/BMP/IMG/GIF/PCX)\n" );
   printf( "\n" );
   printf( "\n" );
   printf( "-------------------------------------------------------------------------\n" );
   printf( "    NOTE:  [options] may be specified before, after or between the image\n" );
   printf( "           names and may be specified by either the '-' or the '/' chars.\n" );
   printf( "-------------------------------------------------------------------------\n" );
   printf( "\n" );
   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  PrintCopyright( )
{
   printf( "%s\n\n", Copyright );
   return;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int      Morph(IMAGE SourceImage, MESH SourceMesh, IMAGE TargetImage, MESH TargetMesh, int SingleFrame)
{
int      i, j, k, key = 0, aborted = 0, RetCode;
char     OutFile[80];
double   CenterWarp, Warp1, Warp2, CenterFade, Fade1, Fade2;
double   FFirst, FLast, WFirst, WLast;
double   DiffTime;
IMAGE    DestImage, WarpedImage1, WarpedImage2, TempImage1, TempImage2;
IMAGE    ArithImage = 0, BackImage = 0;
MESH     WarpedMesh, TempMesh;
time_t   StartTime, EndTime;

   if (FlicOnOff == FLIC_ON)
   {
      NamesFound = 0;
      KillTree( &GlobOctTree );

      memset(GlobPalette,'\0',sizeof(GlobPalette));

      GlobSize = 0;                                    /* initialize variables */
      GlobReduceLevel = MAXDEPTH;
      GlobLeafLevel = GlobReduceLevel + 1;
      GlobOctTree = NULL;
   }

   if (ArithType != ARITH_OFF)
   {
      ArithImage = CreateImage( 2, 2 );

      DrawMessBox( "Information", "Patience...loading arithmetic image", ERROR_BOX_COLOR );
      RetCode = ReadImage( OutArithFile, &ArithImage );
      EraseMessBox( );
      
      if (RetCode != E_NONE)
         return( E_BADARITH );
   }

   if (Background != BACK_OFF)
   {
      BackImage = CreateImage( 2, 2 );

      DrawMessBox( "Information", "Patience...loading background arithmetic image", ERROR_BOX_COLOR );
      RetCode = ReadImage( BackgroundFile, &BackImage );
      EraseMessBox( );
      
      if (RetCode != E_NONE)
         return( E_BADBACK );
   }

   StartTime = time(NULL);

// CREATE TEMPORARY STORAGE FOR IMAGES AND MESH
   DestImage = CreateImage( SourceImage->ResX, SourceImage->ResY );

   if (!DestImage)
      return(IE_NOMEMIMG);

   TempImage1 = CreateImage( SourceImage->ResX, SourceImage->ResY );

   if (!TempImage1)
   {
      DestroyImage( &DestImage );
      return(IE_NOMEMIMG);
   }

   TempImage2 = CreateImage( SourceImage->ResX, SourceImage->ResY );

   if (!TempImage2)
   {
      DestroyImage( &DestImage );
      DestroyImage( &TempImage1 );
      return(IE_NOMEMIMG);
   }

   WarpedImage1 = CreateImage( SourceImage->ResX, SourceImage->ResY );

   if (!WarpedImage1)
   {
      DestroyImage( &DestImage );
      DestroyImage( &TempImage1 );
      DestroyImage( &TempImage2 );
      return(IE_NOMEMIMG);
   }

   WarpedImage2 = CreateImage( SourceImage->ResX, SourceImage->ResY );

   if (!WarpedImage2)
   {
      DestroyImage( &DestImage );
      DestroyImage( &TempImage1 );
      DestroyImage( &TempImage2 );
      DestroyImage( &WarpedImage1 );
      return(IE_NOMEMIMG);
   }

   WarpedImage1->WindowBorderLeft = SourceImage->WindowBorderLeft;
   WarpedImage1->WindowBorderTop = SourceImage->WindowBorderTop;
   WarpedImage1->WindowBorderRight = SourceImage->WindowBorderRight;
   WarpedImage1->WindowBorderBottom = SourceImage->WindowBorderBottom;
   WarpedImage1->WindowLeft = SourceImage->WindowLeft;
   WarpedImage1->WindowTop = SourceImage->WindowTop;
   WarpedImage1->WindowRight = SourceImage->WindowRight;
   WarpedImage1->WindowBottom = SourceImage->WindowBottom;

   WarpedImage2->WindowBorderLeft = TargetImage->WindowBorderLeft;
   WarpedImage2->WindowBorderTop = TargetImage->WindowBorderTop;
   WarpedImage2->WindowBorderRight = TargetImage->WindowBorderRight;
   WarpedImage2->WindowBorderBottom = TargetImage->WindowBorderBottom;
   WarpedImage2->WindowLeft = TargetImage->WindowLeft;
   WarpedImage2->WindowTop = TargetImage->WindowTop;
   WarpedImage2->WindowRight = TargetImage->WindowRight;
   WarpedImage2->WindowBottom = TargetImage->WindowBottom;

   CreateMesh( &WarpedMesh );
   memcpy( WarpedMesh, TargetMesh, sizeof(_MESH));

   CreateMesh( &TempMesh );
   memcpy( TempMesh, TargetMesh, sizeof(_MESH));

   WFirst     = ((double)TotalFrames-1) * ((double)WStart/100.0);
   WLast      = ((double)TotalFrames-1) * ((double)WEnd  /100.0);
   FFirst     = ((double)TotalFrames-1) * ((double)FStart/100.0);
   FLast      = ((double)TotalFrames-1) * ((double)FEnd  /100.0);
   CenterWarp = ((double)WLast-WFirst)  * ((double)Warp50/100.0) + WFirst;
   CenterFade = ((double)FLast-FFirst)  * ((double)Fade50/100.0) + FFirst;

   for( i = 0; i < TotalFrames; i++)
   {
      if (SingleFrame == SINGLE)
         i = (int) (((double)TotalFrames-1) * ((double)Prev50/100.0));

      if (i < StartFrame-1 || i > EndFrame-1)
         continue;

      if ((double) i < WFirst)
         Warp2 = 0.0;
      else if ((double)i > WLast)
         Warp2 = 1.0;
      else if ((double)i < CenterWarp)
         Warp2 = 0.5 / (CenterWarp-WFirst) * ((double)i - WFirst);
      else if ((double)i > CenterWarp)
         Warp2 = 0.5 + 0.5 / (WLast - CenterWarp) * ((double) i - CenterWarp);
      else
         Warp2 = 0.5;

      Warp1 = 1.0 - Warp2;

      if ((double) i < FFirst)
         Fade2 = 0.0;
      else if ((double)i > FLast)
         Fade2 = 1.0;
      else if ((double)i < CenterFade)
         Fade2 = 0.5 / (CenterFade-FFirst) * ((double)i - FFirst);
      else if ((double)i > CenterFade)
         Fade2 = 0.5 + 0.5 / (FLast - CenterFade) * ((double) i - CenterFade);
      else
         Fade2 = 0.5;

      Fade1 = 1.0 - Fade2;

      EraseMessBox( );

      if (kbhit())
      {
         key = getch();
         if (key == 27)
         {
            while( kbhit())
               getch();

            DrawMessBox( "Question", "Abort processing [y/N]?", ERROR_BOX_COLOR );
            Beep();
            key = GetKey( );
            if (toupper(key) == 'Y')
            {
               aborted = 1;
               AutoExit = 0;
               break;
            }
         }
         else if (!key)
         {
            key = getch();

            if (key == ALT_C)
               if (DebugMode)
                  DoScreenCapture( );
               else
                  Beep();
         }
      }

      UpdateMorphStatusWindow( i+1 );

      if (Fade2 == 0.0 && Warp2 == 0.0 && SingleFrame != SINGLE)
      {
         GetFrameName(OutFile, i+1);

         if (ArithType != ARITH_OFF || Background != BACK_OFF)
         {
            memcpy( TempImage1->Buffer, SourceImage->Buffer, TempImage1->ResX * TempImage1->ResY * 3);
            CalculateArith( TempImage1, ArithImage, BackImage );
            RetCode = OutputAnImage(OutFile, TempImage1 );

            if (RetCode != E_NONE)
               return( RetCode );
         }
         else
         {
            RetCode = OutputAnImage(OutFile, SourceImage );

            if (RetCode != E_NONE)
               return( RetCode );
         }
      }
      else if (Fade2 == 1.0 && Warp2 == 1.0 && SingleFrame != SINGLE)
      {
         GetFrameName(OutFile, i+1);

         if (ArithType != ARITH_OFF || Background != BACK_OFF)
         {
            memcpy( TempImage1->Buffer, TargetImage->Buffer, TempImage1->ResX * TempImage1->ResY * 3);
            CalculateArith( TempImage1, ArithImage, BackImage );
            RetCode = OutputAnImage(OutFile, TempImage1 );

            if (RetCode != E_NONE)
               return( RetCode );
         }
         else
         {
            RetCode = OutputAnImage(OutFile, TargetImage );

            if (RetCode != E_NONE)
               return( RetCode );
         }
      }
      else
      {
         if (MorphWarpFlag == FADE)
         {
            memcpy( WarpedImage1->Buffer, SourceImage->Buffer, SourceImage->ResX * SourceImage->ResY * 3);
            memcpy( WarpedImage2->Buffer, TargetImage->Buffer, TargetImage->ResX * TargetImage->ResY * 3);
         }
         else
         {
            // SCALE THE MESHES TO THE MORPH FRAME NUMBER
            for( j = 0; j < WarpedMesh->HorizRes; j++ )
               for( k = 0; k < WarpedMesh->VertRes; k++ )
               {
                  TempMesh->HorizList[j][k] = SourceMesh->HorizList[j][k];

                  TempMesh->VertList[k][j] =
                  SourceMesh->VertList[k][j] * Warp1 +
                  TargetMesh->VertList[k][j] * Warp2;
               }

            RetCode = WarpX(SourceImage, SourceMesh, TempImage1, TempMesh);

            if (RetCode != E_NONE )
            {
               DestroyImage( &TempImage1 );
               DestroyImage( &TempImage2 );
               DestroyImage( &DestImage );
               DestroyImage( &WarpedImage1 );
               DestroyImage( &WarpedImage2 );
               DestroyMesh( &WarpedMesh );
               DestroyMesh( &TempMesh );

               if (ArithType != ARITH_OFF)
                  DestroyImage( &ArithImage );

               if (Background != BACK_OFF)
                  DestroyImage( &BackImage );

               return( RetCode );
            }

            for( j = 0; j < WarpedMesh->HorizRes; j++ )
               for( k = 0; k < WarpedMesh->VertRes; k++ )
               {
                  WarpedMesh->HorizList[j][k] =
                  SourceMesh->HorizList[j][k] * Warp1 +
                  TargetMesh->HorizList[j][k] * Warp2;

                  WarpedMesh->VertList[k][j] =
                  SourceMesh->VertList[k][j] * Warp1 +
                  TargetMesh->VertList[k][j] * Warp2;
               }

            RetCode = WarpY(TempImage1, TempMesh, WarpedImage1, WarpedMesh);

            if (RetCode != E_NONE )
            {
               DestroyImage( &TempImage1 );
               DestroyImage( &TempImage2 );
               DestroyImage( &DestImage );
               DestroyImage( &WarpedImage1 );
               DestroyImage( &WarpedImage2 );
               DestroyMesh( &WarpedMesh );
               DestroyMesh( &TempMesh );

               if (ArithType != ARITH_OFF)
                  DestroyImage( &ArithImage );

               if (Background != BACK_OFF)
                  DestroyImage( &BackImage );

               return( RetCode );
            }

            for( j = 0; j < WarpedMesh->HorizRes; j++ )
               for( k = 0; k < WarpedMesh->VertRes; k++ )
               {
                  TempMesh->HorizList[j][k] = TargetMesh->HorizList[j][k];

                  TempMesh->VertList[k][j] =
                  SourceMesh->VertList[k][j] * Warp1 +
                  TargetMesh->VertList[k][j] * Warp2;
               }

            if (ShowBetweensEnable && SingleFrame != SINGLE)
            {
               EraseMessBox( );
               ShowCursor( );
               DrawImage( WarpedImage1 );
               RestoreImage( WarpedImage1 );
               HideCursor( );
               UpdateMorphStatusWindow( i+1 );
            }

            if (MorphWarpFlag == MORPH)
            {
               RetCode = WarpX(TargetImage, TargetMesh, TempImage2, TempMesh);

               if (RetCode != E_NONE )
               {
                  DestroyImage( &TempImage1 );
                  DestroyImage( &TempImage2 );
                  DestroyImage( &DestImage );
                  DestroyImage( &WarpedImage1 );
                  DestroyImage( &WarpedImage2 );
                  DestroyMesh( &WarpedMesh );
                  DestroyMesh( &TempMesh );

                  if (ArithType != ARITH_OFF)
                     DestroyImage( &ArithImage );

                  if (Background != BACK_OFF)
                     DestroyImage( &BackImage );

                  return( RetCode );
               }

               for( j = 0; j < WarpedMesh->HorizRes; j++ )
                  for( k = 0; k < WarpedMesh->VertRes; k++ )
                  {
                     WarpedMesh->HorizList[j][k] =
                     SourceMesh->HorizList[j][k] * Warp1 +
                     TargetMesh->HorizList[j][k] * Warp2;

                     WarpedMesh->VertList[k][j] =
                     SourceMesh->VertList[k][j] * Warp1 +
                     TargetMesh->VertList[k][j] * Warp2;
                  }

               RetCode = WarpY(TempImage2, TempMesh, WarpedImage2, WarpedMesh);

               if (RetCode != E_NONE )
               {
                  DestroyImage( &TempImage1 );
                  DestroyImage( &TempImage2 );
                  DestroyImage( &DestImage );
                  DestroyImage( &WarpedImage1 );
                  DestroyImage( &WarpedImage2 );
                  DestroyMesh( &WarpedMesh );
                  DestroyMesh( &TempMesh );

                  if (ArithType != ARITH_OFF)
                     DestroyImage( &ArithImage );

                  if (Background != BACK_OFF)
                     DestroyImage( &BackImage );

                  return( RetCode );
               }

               if (ShowBetweensEnable && SingleFrame != SINGLE)
               {
                  EraseMessBox( );
                  ShowCursor( );
                  DrawImage( WarpedImage2 );
                  RestoreImage( WarpedImage2 );
                  HideCursor( );
               }
            }
         }

         // CROSS FADE THE IMAGES
         if (MorphWarpFlag == WARP)
         {
            memcpy( DestImage->Buffer, WarpedImage1->Buffer, SourceImage->ResX * SourceImage->ResY * 3 );
         }
         else
         {
            for(j = 0; j < (SourceImage->ResX * SourceImage->ResY * 3); j += 3)
            {
               DestImage->Buffer[j+0] = (int) ((double) ((double) WarpedImage1->Buffer[j+0] * Fade1 + (double) WarpedImage2->Buffer[j+0] * Fade2));
               DestImage->Buffer[j+1] = (int) ((double) ((double) WarpedImage1->Buffer[j+1] * Fade1 + (double) WarpedImage2->Buffer[j+1] * Fade2));
               DestImage->Buffer[j+2] = (int) ((double) ((double) WarpedImage1->Buffer[j+2] * Fade1 + (double) WarpedImage2->Buffer[j+2] * Fade2));
            }
         }

         if (ArithType != ARITH_OFF || Background != BACK_OFF)
            CalculateArith( DestImage, ArithImage, BackImage );

         if (SingleFrame == NO_SINGLE)
         {
            // SAVE THIS NEXT FRAME IN THE ANIMATION
            GetFrameName(OutFile, i+1);
            RetCode = OutputAnImage(OutFile, DestImage );

            if (RetCode != E_NONE)
               return( RetCode );
         }
         else
         {
            break;
         }
      }
   }

   EraseMessBox( );

   if (SingleFrame == SINGLE)
   {
      RetCode = ShowPreviewImage( DestImage );

      if  (RetCode != E_NONE)
         ErrorBox( "Unable to show preview image", RetCode );
   }

   DestroyImage( &TempImage2 );
   DestroyImage( &DestImage );
   DestroyImage( &WarpedImage1 );
   DestroyImage( &WarpedImage2 );
   DestroyMesh( &WarpedMesh );
   DestroyMesh( &TempMesh );

#ifndef NOREG
   if (SingleFrame != SINGLE)
   {
      RetCode = FinishFlicOutput( TempImage1->ResX, TempImage1->ResY );

      if (RetCode != E_NONE)
         return( RetCode );
   }
#endif

   DestroyImage( &TempImage1 );

   if (ShowBetweensEnable && SingleFrame != SINGLE)
   {
      ShowCursor( );
      RestoreImage( SourceImage );
      RestoreImage( TargetImage );
      DrawMesh( SourceMesh, SourceImage);
      DrawMesh( TargetMesh, TargetImage);
      HideCursor();
   }

   if (ArithType != ARITH_OFF)
      DestroyImage( &ArithImage );

   if (Background != BACK_OFF)
      DestroyImage( &BackImage );

   if (SingleFrame != SINGLE)
   {
      EndTime = time(NULL);
      DiffTime = difftime( EndTime, StartTime );

      Beep();

      if (!AutoExit)
         ShowStatisticsWindow( DiffTime );
   }

   return(E_NONE);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

int   OutputAnImage( char *FileName, IMAGE Image )
{
int   RetCode;

   RetCode = WriteImage( FileName, Image, TGAUpsideDownFlag );

#ifndef NOREG
   if (RetCode != E_NONE)
      return(RetCode);

   /* If not flic output, then exit now */
   if (FlicOnOff == FLIC_OFF)
      return( E_NONE );

   /* Add the flic name to the list */
   strcpy( FlicNamesList[NamesFound], FileName );
   NamesFound++;

   /* Add the colors */
   if (FlicColorMono == COLOR)
      return (GenOctree(Image));
#endif

   return( E_NONE );
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#ifndef NOREG
int   FinishFlicOutput( int ResX, int ResY )
{
int   i, j, RetCode;
char  Palette256[256*3], TempFileName[80];
FILE  *FlicFile;
IMAGE From, To, TempImg;

   /* If not flic output, then exit now */
   if (FlicOnOff == FLIC_OFF)
      return( E_NONE );

   From = CreateImage( 2, 2 );

   if (!From)
      return( IE_NOMEMIMG );

   To = CreateImage( 2, 2 );

   if (!To)
      return( IE_NOMEMIMG );

   strcpy( TempFileName, FlicFileName );

   switch (FlicType)
   {
      case FLI:
         strcat( TempFileName, ".FLI" );
         UpdateFastFlic = 1;
         break;

      case FLC:
         strcat( TempFileName, ".FLC" );
         UpdateFastFlic = 1;
         break;

      case FLX:
         FlicColorMono = 0;
         strcat( TempFileName, ".FLX" );
         UpdateFastFlic = 1;
         break;

      case FLH:
         FlicColorMono = 0;
         strcat( TempFileName, ".FLH" );
         UpdateFastFlic = 1;
         break;

      case FLT:
         FlicColorMono = 0;
         strcat( TempFileName, ".FLT" );
         UpdateFastFlic = 1;
         break;

      default:
         if (ResX > 320 || ResY > 200)
         {
            strcat( TempFileName, ".FLC" );
            FlicType = FLC;
            UpdateFastFlic = 1;
         }
         else
         {
            strcat( TempFileName, ".FLI" );
            FlicType = FLI;
            UpdateFastFlic = 1;
         }
            
         break;
   }

   if (FlicType == FLI || FlicType == FLC)
   {
      if (FlicColorMono == COLOR)
      {
         RetCode = 1;
         InitPalette( GlobOctTree, (USHORT *) &RetCode );

         for (i = 0; i < 256; i++)
         {
            Palette256[i*3+0] = GlobPalette[i].Red;
            Palette256[i*3+1] = GlobPalette[i].Green;
            Palette256[i*3+2] = GlobPalette[i].Blue;
         }
      }
      else
      {
         for (i = 0; i < 256; i++)
            Palette256[i*3] = Palette256[i*3+1] = Palette256[i*3+2] = i;
      }
   }

   if (FlicAutoRew && NamesFound > 2)
   {
      for( i = 0, j = NamesFound-2; j >= 0; i++, j--)
         strcpy( FlicNamesList[NamesFound+i], FlicNamesList[j] );

      NamesFound = NamesFound * 2 - 1;
   }
   else
   {
      strcpy( FlicNamesList[NamesFound], FlicNamesList[0] );
      NamesFound++;
   }

   RetCode = FlicCreateFile( TempFileName, FlicType, NamesFound-1, ResX, ResY, FlicSpeed, &FlicFile );

   if (RetCode != FE_NONE)
      return( RetCode );

   for( i = 0; i < NamesFound; i++ )
   {
      UpdateFlicStatusWindow( i+1, NamesFound );

      RetCode = ReadImage( FlicNamesList[i], &To );

      if (RetCode != FE_NONE)
         return( RetCode );

      if (FlicType == FLI || FlicType == FLC)
      {
         if (FlicColorMono == MONOCHROME)
            MakeImageMono(To);
         else
            Convert24To8(To);
      }

      RetCode = FlicAddFrame( From, To, Palette256, FlicType, i == 0, i == 1, FlicFile );

      if (RetCode != FE_NONE)
         return( RetCode );

      TempImg = From;
      From = To;
      To = TempImg;
   }

   EraseMessBox( );

   DestroyImage( &From );
   DestroyImage( &To );

   RetCode = FlicCloseFile( FlicFile );
   
   if (RetCode != FE_NONE )
      return( RetCode );

   return( E_NONE );
}
#endif

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#ifndef NOREG
void  MakeImageMono( IMAGE Image )
{
int   x, y, t;

   t = Image->ResX * 3;

   for( y = 0; y < Image->ResY; y++ )
      for( x = 0; x < Image->ResX; x++ )
         Image->Buffer[y * Image->ResX + x] =
         MAX(Image->Buffer[y * t + (x * 3) + 0],
             MAX(Image->Buffer[y * t + (x * 3) + 1],
                 Image->Buffer[y * t + (x * 3) + 2])) & 0xfc;
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  Convert24To8( IMAGE Image )
{
int   x, y, t;

   t = Image->ResX * 3;

   for( y = 0; y < Image->ResY; y++ )
      for( x = 0; x < Image->ResX; x++ )
         Image->Buffer[y * Image->ResX + x] = GetClosestColor(Image->Buffer[y * t + (x * 3) + 0],
                                                              Image->Buffer[y * t + (x * 3) + 1],
                                                              Image->Buffer[y * t + (x * 3) + 2]);
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateFlicStatusWindow( int CurrentFrame, int EndFrame )
{
int   i, Percent;
char  TempStr[80], StatusString[512], PString[30], PString2[80];

   Percent = (int) ((float)(CurrentFrame+1) / (float)(EndFrame+1) * 100.0);

   strcpy( PString, "                         \n" );
   strcpy( PString2, "                         \n" );

   for( i = 0; i < (Percent >> 2); i++ )
      PString[i] = '€';

   sprintf( TempStr, "[%d%%]", Percent );

   for (i = 0; i < strlen(TempStr); i++ )
      PString2[11+i] = TempStr[i];

   sprintf( TempStr, " Current frame:  %d\n", CurrentFrame );
   strcpy( StatusString, TempStr );
   strcat( StatusString, PString );
   strcat( StatusString, PString2 );
   strcat( StatusString, " [Press ESCAPE to abort] " );

   DrawMessBox( "Flic Status", StatusString, STATUS_COLOR );

}
#endif

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  UpdateMorphStatusWindow( int CurrentFrame )
{
int   i, Percent;
char  TempStr[80], StatusString[512], PString[30], PString2[80];

   Percent = (int) ((float)(CurrentFrame-StartFrame+1) / (float)(EndFrame-StartFrame+1) * 100.0);

   strcpy( PString, "                         \n" );
   strcpy( PString2, "                         \n" );

   for( i = 0; i < (Percent >> 2); i++ )
      PString[i] = '€';

   sprintf( TempStr, "[%d%%]", Percent );

   for (i = 0; i < strlen(TempStr); i++ )
      PString2[11+i] = TempStr[i];

   sprintf( TempStr, " Current frame:  %d\n", CurrentFrame );
   strcpy( StatusString, TempStr );
   strcat( StatusString, PString );
   strcat( StatusString, PString2 );
   strcat( StatusString, " [Press ESCAPE to abort] " );

   sprintf( PString, "%sing status", MorphWarpFlag == MORPH ? "Morph": MorphWarpFlag == WARP ? "Warp" : "Fad" );
   DrawMessBox( PString, StatusString, STATUS_COLOR );

}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

void  ShowStatisticsWindow( double DiffTime )
{
int   Hours, Minutes, Seconds;
char  TempStr[80], StatusString[512], PString[30];

   Hours = (int) ((double)DiffTime / (double)60.0 / (double)60.0);
   DiffTime -= Hours * 60 * 60;
   Minutes = (int) ((double)DiffTime / (double)60.0);
   DiffTime -= Minutes * 60;
   Seconds = DiffTime;

   sprintf( TempStr, "      Total frames:  %d\n", TotalFrames );
   strcpy( StatusString, TempStr );
   sprintf( TempStr, "    Starting frame:  %d\n", StartFrame );
   strcat( StatusString, TempStr );
   sprintf( TempStr, "      Ending frame:  %d\n", EndFrame );
   strcat( StatusString, TempStr );
   sprintf( TempStr, "      Frame offset:  %d\n", FrameOffset );
   strcat( StatusString, TempStr );
   sprintf( TempStr, "Interpolation type:  %s\n", FastMorphMode==0 ? "Full":"Quick" );
   strcat( StatusString, TempStr );
   sprintf( TempStr, "              Task:  %s\n", MorphWarpFlag == MORPH ? "Morph": MorphWarpFlag == WARP ? "Warp" : "Fade" );
   strcat( StatusString, TempStr );
   strcat( StatusString, "ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ\n" );
   sprintf( TempStr, "        %s time:  %02d:%02d:%02d\n\n", MorphWarpFlag == MORPH ? "Morph": MorphWarpFlag == WARP ? " Warp" : " Fade", Hours, Minutes, Seconds );
   strcat( StatusString, TempStr );
   strcat( StatusString, "  [Press any key to return]" );

   sprintf( PString, "%sing statistics", MorphWarpFlag == MORPH ? "Morph": MorphWarpFlag == WARP ? "Warp" : "Fad" );
   DrawMessBox( PString, StatusString, STATUS_COLOR );

   GetKey();

   EraseMessBox();
}

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [CMORPH.C    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
