; -----------------------------------------------------------------------------
;   _____ __  __                  _
;  / ____|  \/  |                | |
; | |    | \  / | ___  _ __ _ __ | |__
; | |    | |\/| |/ _ \| '__| '_ \| '_ \
; | |____| |  | | (_) | |  | |_) | | | |
;  \_____|_|  |_|\___/|_|  | .__/|_| |_|
;                          | |                    
;                          |_|                    
;
; The Complete Morph - public source release
; -----------------------------------------------------------------------------
;
; Originally released under a custom license.
; This historical re-release is provided under the MIT License.
; See the LICENSE file in the repo root for details.
;
; https://github.com/nettlep
;
; -----------------------------------------------------------------------------
;
; Notes about this software:
;
; CMorph generates high quality (even by today's standards) two-dimensional
; morphs from two static input images. It was written for DOS a number of years
; ago, using the Watcom compiler, back before C++ was the rage.
;
; Some of this source is ugly, but it should all be fairly readable.
;
; You will not be able to simply compile this program into an EXE because not
; all components of the program exist (there are some other libraries that are
; required, but are unavailable for public release at this time.) These missing
; libraries are the Int86 lib (DOS software interrupts), FLIC lib (animations)
; and the Image lib (loading/saving of image files.)
;
; This source code is best viewed with 4-character tabs.
;
; -----------------------------------------------------------------------------
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;±                                                                             ±
;±   (c) Copyright 1992, 1993, 1994 Paul D. Nettle, All Rights Reserved        ±
;±                                                                             ±
;±   Anima.asm - Program to play animations                                    ±
;±                                                                             ±
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

.386p

      Public   AnimateFrame_

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

END_FILE   EQU 0000000000000000b                ;Equates for type of update
END_FRAME  EQU 0001000000000000b                ;
WRD_CHANGE EQU 0010000000000000b                ;
STR_CHANGE EQU 0011000000000000b                ;
SCR_CHANGE EQU 0100000000000000b                ;
PAL_CHANGE EQU 0101000000000000b                ;

PAL_SIZE   EQU (256*3)                          ;Size defines...
FRAME_SIZE EQU 64000                            ;

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

ARG1            EQU     6                       ;Standard C stack frame args
ARG2            EQU     8                       ;

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

Code  Segment  Public   Byte  "Code"
      Assume CS:Code

AnimateFrame_ Proc NEAR

   push  esi                                    ;Preparation...
   push  edi                                    ;
   push  ebp                                    ;

   xor   ebp,ebp

   mov   esi,eax                                ;Get the address of the string
   push  esi                                    ;Save this for later

   mov   edi,0a0000h                            ;Get screen area

   mov   dx,3dah                                ;Wait for Vertical Blank...
                                                ;
WaitForRetrace:                                 ;
   in    al,dx                                  ;
   test  al,1000b                               ;
   jz    WaitForRetrace                         ;

TopOfAnimLoop:
   cmp   Word Ptr [esi],WRD_CHANGE              ;Test for word change
   je    WordChangeHandler                      ;

   cmp   Word Ptr [esi],STR_CHANGE              ;Test for string change
   je    StringChangeHandler                    ;

   cmp   Word Ptr [esi],SCR_CHANGE              ;Test for screen change
   je    ScreenChangeHandler                    ;

   cmp   Word Ptr [esi],PAL_CHANGE              ;Test for palette change
   je    PalChangeHandler                       ;

   cmp   Word Ptr [esi],END_FILE                ;Test for end of file
   je    EndFileHandler                         ;

   cmp   Word Ptr [esi],END_FRAME               ;Test for end of frame
   je    EndFrameHandler                        ;

   mov   eax,-1                                 ;Bad command!
   pop   esi                                    ;
   jmp   ExitRoutine                            ;

StringChangeHandler:
   add   esi,6                                  ;Point to String data

   mov   edi,0A0000h                            ;Get the Offset
   mov   di,[esi-4]                             ;
   shl   di,1

   xor   ecx,ecx                                ;Get the word count
   mov   cx,[esi-2]                             ;

   add   ebp,ecx
   add   ebp,ecx
   add   ebp,6

   rep   movsw                                  ;Write the words

   jmp   TopOfAnimLoop                          ;Next command...

WordChangeHandler:
   add   esi,6                                  ;Point all the way past current

   mov   edi,0A0000h                            ;Get the Offset
   mov   di,[esi-4]                             ;
   shl   di,1

   mov   ax,[esi-2]                             ;Get the word

   mov   [edi],ax                               ;Write the word

   add   ebp,6
   jmp   TopOfAnimLoop                          ;Next command...

ScreenChangeHandler:
   add   esi,2                                  ;Point to screen data

   mov   edi,0A0000h                            ;ES:DI = A000:0000

   mov   ecx,64000/4                            ;Number of words per screen

   rep   movsd                                  ;Write new screen info

   add   ebp,64002
   jmp   TopOfAnimLoop                          ;Next command...

PalChangeHandler:
   add   esi,2                                  ;Point to palette data

   mov   ax,1012h                               ;Setup for palette update
   mov   bx,0                                   ;
   mov   cx,256                                 ;
   mov   dx,si                                  ;

   ;int   10h                                    ;Call int 10h for palette update

   add   esi,256*3                              ;Next command...

   add   ebp,770

   jmp   TopOfAnimLoop                          ;

EndFileHandler:
   mov   eax,-2                                 ;End of file...
   pop   esi                                    ;
   jmp   ExitRoutine                            ;

EndFrameHandler:
   pop   eax                                    ;End of frame...
   mov   eax,ebp                                ;
   add   eax,2                                  ;
   shr   eax,1                                  ;

ExitRoutine:
   pop   ebp                                    ;Cleanup and return
   pop   edi                                    ;
   pop   esi                                    ;
   ret                                          ;

AnimateFrame_ EndP

Code  EndS

End

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;± End of Anima.asm                                                            ±
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
