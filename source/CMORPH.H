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
  ±   [CMORPH.H    ] - Header file for The Complete Morph                    ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

#include "..\INCLUDE\IMAGE.H"
#include "..\INCLUDE\INT86.H"
#include "..\INCLUDE\FLIC.H"
#include "CMDEFS.H"
#include "CMTYPES.H"
#include "CMPROT.H"
#include "CMMACRO.H"

extern   char     Copyright[];
extern   int      ZoomOn;
extern   int      GraphMode;
extern   int      LastCursDrawn, FirstCursDrawn;
extern   int      TotalFrames;
extern   int      StartFrame;
extern   int      EndFrame;
extern   int      WarpTightness;
extern   int      InterpTightness;
extern   int      Fade50;
extern   int      FStart;
extern   int      FEnd;
extern   int      Warp50;
extern   int      WStart;
extern   int      WEnd;
extern   int      Prev50;
extern   int      OutputFileType;
extern   int      MeshType;
extern   int      InterpMeshType;
extern   int      MorphWarpFlag;
extern   int      CurvesOn;
extern   int      MeshColor;
extern   int      CursColor;
extern   int      AutoMorph;
extern   int      AutoExit;
extern   int      QuietMode;
extern   int      DebugMode;
extern   int      BackDoor;
extern   int      TGAUpsideDownFlag;
extern   int      MenuMode;
extern   int      FastScreen;
extern   int      FrameOffset;
extern   int      FlicType;
extern   int      FlicOnOff;
extern   int      FlicColorMono;
extern   int      FlicAutoRew;
extern   int      FlicSpeed;
extern   int      UpdateFastPref;
extern   int      UpdateFastFile;
extern   int      UpdateFastProc;
extern   int      UpdateFastPrev;
extern   int      UpdateFastFlic;
extern   int      VMode;
extern   int      ShowBetweensEnable;
extern   int      FastMorphMode;
extern   int      ArithType;
extern   int      OverlayThresholdL;
extern   int      OverlayThresholdH;
extern   int      BackThresholdL;
extern   int      BackThresholdH;
extern   int      Background;
extern   int      UpdateImages;
extern   int      PrevColor;

extern   char     FlicFileName[180];
extern   char     FilterFileName[180];
extern   char     SourceFiltName[180];
extern   char     TargetFiltName[180];
extern   char     ProgramPath[80];

extern   int      BUTTON_FACE_COLOR;
extern   int      PREF_TITLE_COLOR;
extern   int      PREV_TITLE_COLOR;
extern   int      FLIC_TITLE_COLOR;
extern   int      PROC_TITLE_COLOR;
extern   int      EDIT_TITLE_COLOR;
extern   int      EDIT_WINDOW_COLOR;
extern   int      FILE_WINDOW_COLOR;
extern   int      FILE_TITLE_COLOR;
extern   int      MBOX_WINDOW_COLOR;
extern   int      OPER_WINDOW_COLOR;
extern   int      OPER_TITLE_COLOR;
extern   int      PROMPT_COLOR;
extern   int      BUTTON_TEXT_COLOR;
extern   int      RBUTTON_TEXT_COLOR;
extern   int      ENTER_TEXT_COLOR;
extern   int      TEXT_CURSOR_COLOR;
extern   int      OPER_TITLE_TEXT_COLOR;
extern   int      PROMPT_TEXT_COLOR;
extern   int      EDIT_TITLE_TEXT_COLOR;
extern   int      INFO_TEXT_COLOR;
extern   int      DEMO_TEXT_COLOR;
extern   int      ZOOM_CURSOR_COLOR;
extern   int      SHADOW_COLOR;
extern   int      HIGHLIGHT_COLOR;
extern   int      MBOX_TITLE_TEXT_COLOR;
extern   int      MBOX_TEXT_COLOR;
extern   int      BULLSEYE_OUTLINE_COLOR;
extern   int      R_MESH_COLOR;
extern   int      G_MESH_COLOR;
extern   int      B_MESH_COLOR;
extern   int      X_MESH_COLOR;
extern   int      R_CURS_COLOR;
extern   int      G_CURS_COLOR;
extern   int      B_CURS_COLOR;
extern   int      X_CURS_COLOR;
extern   int      PREF_TITLE_TEXT_COLOR;
extern   int      PREV_TITLE_TEXT_COLOR;
extern   int      FLIC_TITLE_TEXT_COLOR;
extern   int      PROC_TITLE_TEXT_COLOR;
extern   int      TEXT_COLOR;
extern   int      FILE_TITLE_TEXT_COLOR;
extern   int      DTEXT_COLOR;
extern   int      BACK_COLOR;
extern   int      TITLE_COLOR;
extern   int      CURSOR_COLOR;
extern   int      ERROR_BOX_COLOR;
extern   int      HELP_BOX_COLOR;
extern   int      STATUS_COLOR;
extern   int      PREF_WINDOW_COLOR;
extern   int      PREV_WINDOW_COLOR;
extern   int      FLIC_WINDOW_COLOR;
extern   int      PROC_WINDOW_COLOR;
extern   int      DEFAULT_MESH_COLOR;
extern   int      DEFAULT_CURSOR_COLOR;
extern   int      BOTTOM_COLOR;
extern   int      PREF_DB_HEAD_COLOR;
extern   int      PREF_DB_SHEAD_COLOR;
extern   int      PREF_DB_HLINE_COLOR;
extern   int      PREV_DB_HEAD_COLOR;
extern   int      PREV_DB_SHEAD_COLOR;
extern   int      PREV_DB_HLINE_COLOR;
extern   int      FLIC_DB_HEAD_COLOR;
extern   int      FLIC_DB_SHEAD_COLOR;
extern   int      FLIC_DB_HLINE_COLOR;
extern   int      PROC_DB_HEAD_COLOR;
extern   int      PROC_DB_SHEAD_COLOR;
extern   int      PROC_DB_HLINE_COLOR;
extern   int      OPER_DB_HEAD_COLOR;
extern   int      OPER_DB_SHEAD_COLOR;
extern   int      OPER_DB_HLINE_COLOR;
extern   int      FILE_DB_HEAD_COLOR;
extern   int      FILE_DB_SHEAD_COLOR;
extern   int      FILE_DB_HLINE_COLOR;
extern   int      EDIT_DB_HEAD_COLOR;
extern   int      EDIT_DB_SHEAD_COLOR;
extern   int      EDIT_DB_HLINE_COLOR;

extern   char     SourceImageFile[80];
extern   char     TargetImageFile[80];

extern   _RECT    OperRect;
extern   _RECT    PrefRect;
extern   _RECT    EditRect;
extern   _RECT    FileRect;
extern   _RECT    ProcRect;
extern   _RECT    PrevRect;
extern   _RECT    FlicRect;

extern   char     MeshFile[];
extern   char     OutBaseName[];
extern   char     OutArithFile[];
extern   char     BackgroundFile[];

extern   IMAGE    SourceImage, TargetImage;
extern   MESH     SourceMesh, TargetMesh;

extern   _RECT    ZoomWindow;
extern   _RECT    SourceInfoWindow;
extern   _RECT    TargetInfoWindow;

extern   _BUTTON  PerfButton;
extern   _BUTTON  OperButton;
extern   _BUTTON  EditButton;
extern   _BUTTON  FileButton;
extern   _BUTTON  ProcButton;
extern   _BUTTON  PrevButton;
extern   _BUTTON  FlicButton;

extern   _BUTTON  ExitButton;
extern   _BUTTON  GoButton;
extern   _BUTTON  ShellButton;
extern   _BUTTON  HelpButton;
extern   _BUTTON  AboutButton;

extern   _BUTTON  MeshSwapButton;
extern   _BUTTON  CopySDMButton;
extern   _BUTTON  HFlipSMButton;
extern   _BUTTON  VFlipSMButton;
extern   _BUTTON  CopyDSMButton;
extern   _BUTTON  HFlipDMButton;
extern   _BUTTON  VFlipDMButton;
extern   _BUTTON  ImageSwapButton;
extern   _BUTTON  HFlipSIButton;
extern   _BUTTON  VFlipSIButton;
extern   _BUTTON  HFlipDIButton;
extern   _BUTTON  VFlipDIButton;

extern   _BUTTON  MeshCATButton;
extern   _BUTTON  MeshSPLButton;
extern   _BUTTON  MeshSTRButton;
extern   _BUTTON  InterpMeshCATButton;
extern   _BUTTON  InterpMeshSPLButton;
extern   _BUTTON  InterpMeshSTRButton;
extern   _BUTTON  CursRButton;
extern   _BUTTON  CursGButton;
extern   _BUTTON  CursBButton;
extern   _BUTTON  CursXButton;
extern   _BUTTON  MColorRButton;
extern   _BUTTON  MColorGButton;
extern   _BUTTON  MColorBButton;
extern   _BUTTON  MColorXButton;
extern   _BUTTON  ZoomButton;
extern   _BUTTON  CurvesButton;
extern   _BUTTON  MorphButton;
extern   _BUTTON  WarpButton;
extern   _BUTTON  FadeButton;
extern   _BUTTON  TGAUpsideDownButton;
extern   _BUTTON  OutputTGAButton;
extern   _BUTTON  OutputIPIButton;
extern   _BUTTON  OutputIMGButton;
extern   _BUTTON  OutputGIFButton;
extern   _BUTTON  OutputBMPButton;
extern   _BUTTON  OutputPCXButton;

extern   _BUTTON  FlicFLIButton;
extern   _BUTTON  FlicFLCButton;
extern   _BUTTON  FlicFLXButton;
extern   _BUTTON  FlicFLHButton;
extern   _BUTTON  FlicFLTButton;
extern   _BUTTON  FlicPingButton;
extern   _BUTTON  FlicOnOffButton;
extern   _BUTTON  FlicColorButton;
extern   _PROMPT  FlicFilePrompt;
extern   _PROMPT  FlicSpeedPrompt;

extern   _BUTTON  KwikScreenButton;
extern   _BUTTON  SoundEnableButton;
extern   _BUTTON  ShowBetweensButton;
extern   _BUTTON  FastMorphButton;

extern   _PROMPT  MeshNamePrompt;
extern   _PROMPT  SrcImagePrompt;
extern   _PROMPT  DstImagePrompt;
extern   _PROMPT  OutCharsPrompt;

extern   _BUTTON  LoadMeshButton;
extern   _BUTTON  SaveMeshButton;

extern   _BUTTON  ImpMeshButton;
extern   _BUTTON  ExpMeshButton;

extern   _PROMPT  FilterFileNamePrompt;
extern   _PROMPT  SourceFilterNamePrompt;
extern   _PROMPT  TargetFilterNamePrompt;
extern   _BUTTON  DoSourceFilterButton;
extern   _BUTTON  DoTargetFilterButton;

extern   _BUTTON  LoadSourceImageButton;
extern   _BUTTON  LoadTargetImageButton;
extern   _BUTTON  SaveSourceImageButton;
extern   _BUTTON  SaveTargetImageButton;

extern   _BUTTON  OffOutButton;
extern   _BUTTON  AddOutButton;
extern   _BUTTON  SubOutButton;
extern   _BUTTON  AveOutButton;
extern   _BUTTON  OvrOutButton;
extern   _PROMPT  OutArithFilePrompt;
extern   _PROMPT  OvrThreshPromptL;
extern   _PROMPT  OvrThreshPromptH;
extern   _PROMPT  BackThreshPromptL;
extern   _PROMPT  BackThreshPromptH;
extern   _PROMPT  BackFilePrompt;

extern   _BUTTON  BackOffButton;
extern   _BUTTON  BackOnButton;

extern   _PROMPT  FrameOffPrompt;
extern   _PROMPT  FramesPrompt;
extern   _PROMPT  StartFramePrompt;
extern   _PROMPT  EndFramePrompt;
extern   _PROMPT  WarpTightnessPrompt;
extern   _PROMPT  InterpTightnessPrompt;
extern   _PROMPT  Fade50Prompt;
extern   _PROMPT  FadeStartPrompt;
extern   _PROMPT  FadeEndPrompt;
extern   _PROMPT  Warp50Prompt;
extern   _PROMPT  WarpStartPrompt;
extern   _PROMPT  WarpEndPrompt;

extern   _BUTTON  PrevGoButton;
extern   _BUTTON  PrevColorButton;
extern   _PROMPT  PrevFramePrompt;

extern   GINFO    GI;

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [CMORPH.H    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
