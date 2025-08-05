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
  ±   Copyright (c) 1994 Paul D. Nettle.  All Rights Reserved.               ±
  ±                                                                          ±
  ±   [CMPROT.H    ] - CMorph Declarations                                   ±
  ±                                                                          ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// ADDMESH.C

void  AddMesh( int MouseX, int MouseY );
void  AddHorizPointToMesh( int Y, int Side, MESH Mesh, MESH OtherMesh, IMAGE Image );
void  AddVertPointToMesh( int X, int Side, MESH Mesh, MESH OtherMesh, IMAGE Image );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// ANIMA.ASM

int   AnimateFrame( unsigned short int *Buffer );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// ARITH.C

void  CalculateArith( IMAGE Dest, IMAGE Source, IMAGE Back );
void  AddBackground( IMAGE Dest, IMAGE Source );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// BICUBIC.C

int   Bicubic(float *x1, float *y1, int len1, float *x2, float *y2, int len2, int Tightness);

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// BUPDATE.C

void  DoTargetFilter( void );
void  DoSourceFilter( void );
void  UpdateSoundsEnable( void );
void  UpdateKwikScreen( void );
void  UpdateShowBetweens( void );
void  UpdateFastMorph( void );
void  UpdateUpsideDown( void );
void  UpdateCurves( void );
void  UpdateMorph( int State );
void  UpdateMeshType( int Type );
void  UpdateInterpType( int Type );
void  UpdateFileType( int Type );
void  UpdateFlicType( int Type );
void  UpdateFlicColor( void );
void  UpdateFlicOnOff( void );
void  UpdateFlicPing( void );
void  UpdateArithType( int Type );
void  UpdateBackground( int State );
void  UpdateZoomStatus( void );
void  UpdateMeshColor( int Color );
void  UpdateCursColor( int Color );
void  UpdatePrevColor( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// BUTTONS.C

void  DrawButton( int X, int Y, BUTTON Button );
void  UpdateButtons( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// CATMULL.C

int   Catmull(float *x1, float *y1, int len1, float *x2, float *y2, int len2);

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// CMORPH.C

void  UnInitSystem( void );
int   Morph(IMAGE SourceImage, MESH SourceMesh, IMAGE TargetImage, MESH TargetMesh, int SingleFrame);
void  ExitSystem( int ExitCode );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// CONV.C

int   Convolution( char *FilterName, IMAGE Image );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
//  EDIT.C

void  DrawEditButton( void );
int   ShowEditWindow( void );
void  HideEditWindow( void );
void  SwapMesh( MESH Mesh1, MESH Mesh2 );
void  CopyMesh( MESH TargetMesh, MESH SourceMesh );
void  HorizFlipMesh( MESH FlipMesh  );
void  VertFlipMesh( MESH FlipMesh );
void  SwapImage( IMAGE Image1, IMAGE Image2 );
void  HorizFlipImage( IMAGE FlipMesh  );
void  VertFlipImage( IMAGE FlipMesh );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// ERROR.C

void  WarningBox( char *String );
void  ErrorBox( char *String, int ErrorNumber );
void  FatalError( char *ErrString, int ErrorNumber );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
//  EXPORT.C

void  ExportMesh( void );
int   WriteMesh( char *FileName, MESH SourceMesh, MESH TargetMesh );
int   WriteOldMesh( char *FileName, MESH SourceMesh, MESH TargetMesh );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
//  FILE.C

void  DrawFileButton( void );
int   ShowFileWindow( void );
void  HideFileWindow( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// FLIC.C

void  DrawFlicButton( void );
int   ShowFlicWindow( void );
void  HideFlicWindow( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// IMPORT.C

void  ImportMesh( void );
int   ReadMesh( char *FileName, MESH *SourceMesh, MESH *TargetMesh );
int   ReadOldMesh( char *FileName, MESH *SourceMesh, MESH *TargetMesh );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// INPUT.C

void  Input( void );
void  DosShell( void );
void  DoMorph( void );
void  DoHelp( void );
void  DoAbout( void );
void  DoBackDoor( void );
void  DoScreenCapture( void );
void  DoPreview( void );
int   VerifyExit( void );
void  GetNumericPrompt( int Chars, int Min, int Max, PROMPT Prompt, int *Default );
int   GetAlphaNumericPrompt( int Chars, PROMPT Prompt, char *Default );
int   GetFileName( char *FileName, PROMPT Prompt );
void  ChangeMenu( int Mode );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// INTRO.C

int   PlayANM( char *FileName );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// KEYS.C

int   HandleKeys( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// MBOX.C

void  MBox( char *String );
void  DrawMessBox( char *Title, char *String, int Color );
void  EraseMessBox( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// MESH.C

void  CreateMesh( MESH *Mesh );
void  DestroyMesh( MESH *Mesh );
void  DrawMesh( MESH Mesh, IMAGE Image );
void  NearestMeshPoint( MESH Mesh, IMAGE Image, int X, int Y, int *Cx, int *Cy, int *H1, int *H2, int *V1, int *V2 );
void  LoadMesh( void );
void  SaveMesh( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// MOUSE.C

int   InitMouse( void );
void  SetMousePosition( int X, int Y );
void  SetMouseSensativity( unsigned int Val );
void  SetMouseRange( int Left, int Right, int Top, int Bottom );
void  ReadMouse( int *Col, int *Row, int *BLeft, int *BRight, int Draw );
void  SaveCursor( int X, int Y );
void  RestoreCursor( int X, int Y );
void  UnInitMouse( void );
void  DrawCursor( int X, int Y, int Ox, int Oy );
void  ShowCursor( void );
void  HideCursor( void );
int   InRect( int X, int Y, RECT Rect );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// MOVEMESH.C

void  MoveMesh( int MouseX, int MouseY, int Left, int Right );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// OPER.C

void  DrawOperButton( void );
int   ShowOperationsWindow( void );
void  HideOperationsWindow( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// PREF.C

void  DrawPerfButton( void );
int   ShowPreferencesWindow( void );
void  HidePreferencesWindow( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// PREV.C

void  DrawPrevButton( void );
int   ShowPrevWindow( void );
void  HidePrevWindow( void );
int   ShowPreviewImage( IMAGE Image );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// PRIMS.C

void  Circle( int X, int Y );
void  DrawOutRect( int Left, int Top, int Right, int Bottom );
void  DrawInRect( int Left, int Top, int Right, int Bottom );
void  DrawInGroove( int Left, int Top, int Right, int Bottom );
void  DrawOutGroove( int Left, int Top, int Right, int Bottom );
void  ShadeRect( RECT Rect, int ColorStart );
void  FadeRect( RECT Rect, int ColorStart );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// PROC.C

void  DrawProcessesButton( void );
int   ShowProcessesWindow( void );
void  HideProcessesWindow( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// PROMPTS.C

void  SetFilterName( void );
void  GetFlicSpeed( void );
void  GetFlicName( void );
void  SourceFilter( void );
void  TargetFilter( void );
void  GetName( char *FileName, PROMPT Prompt );
void  GetFrameOff( void );
void  GetFrames( void );
void  GetStartFrame( void );
void  GetEndFrame( void );
void  GetWarpTightness( void );
void  GetInterpTightness( void );
void  GetPrev50( void );
void  GetFade50( void );
void  GetFadeStart( void );
void  GetFadeEnd( void );
void  GetWarp50( void );
void  GetWarpStart( void );
void  GetWarpEnd( void );
void  DrawPromptBox( int X, int Y, int CurVal, PROMPT Prompt );
void  DrawPromptTBox( int X, int Y, char *String, PROMPT Prompt );
void  GetOvrThreshL( void );
void  GetOvrThreshH( void );
void  GetBackThreshL( void );
void  GetBackThreshH( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// RESAMPLE.C

void  resample(UCHAR *src, UCHAR *dst, int len, int offst, float *xmap );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// SCREEN.C

GINFO SetGraphicsMode( int Mode );
void  RestoreMode( void );
void  InitScreen( IMAGE SourceImage, IMAGE TargetImage );
void  InitBottomHalf( IMAGE SourceImage, IMAGE TargetImage );
void  DrawImage( IMAGE SourceImage );
void  RestoreImage( IMAGE SourceImage );
void  DrawImageInfo( RECT Rect, int X, int Y, IMAGE Image, char *Title );
void  UpdateZoomWindow( int X, int Y, IMAGE Image );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// STRAIGHT.C

int   Straight(float *x1, float *y1, int len1, float *x2, float *y2, int len2);

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
// UTILS.C

void  LoadImage( char *FileName, IMAGE Image );
void  SaveImage( char *FileName, IMAGE Image );
void  Beep( void );
void  GetFrameName(char *OutFile, int Val);
void  WriteWord( int Val, FILE *FilePointer );
int   ReadWord( FILE *FilePointer );
int   GetKey( void );
int   Splines(float *x1, float *y1, int len1, float *x2, float *y2, int len2, int InterpWarpFlag);
void  UpdateButtonStates( void );

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
//  WARP.C

int   WarpX(IMAGE SourceImage, MESH SourceMesh, IMAGE TargetImage, MESH TargetMesh);
int   WarpY(IMAGE SourceImage, MESH SourceMesh, IMAGE TargetImage, MESH TargetMesh);

/*±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ±   [CMPROT.H    ] - End Of File                                           ±
  ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±*/
