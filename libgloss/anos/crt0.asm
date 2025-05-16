;
; Stdlib - Pre-main startup code
; anos - An Operating System
;
; Copyright (c) 2025 Ross Bamford
; This is the entry-point for user programs.
;
; There's still a lot TODO in here, it needs to run init
; for static constructors, set up IO, and so on...
;
 
bits 64
global _start

extern main,_init,_fini
extern _bss_start, _bss_end               ; Linker defined symbols
extern _anos_init_capabilities

section .text.init                        ; Linker needs to make sure this goes in first...

; Initialize C-land: Zero BSS, sort out arguments and call main
_start:
;   mov   rcx,_bss_end                        ; Get end of .bss section (VMA)
;   mov   rax,_bss_start                      ; Get start of .bss section (VMA)
;   sub   rcx,rax                             ; Compute length of .bss (bytes) in RCX
;   shr   rcx,0x3                             ; Divide by 8 (we're zeroing qwords)

;   test  rcx,rcx                             ; Do we have zero-size .bss?
;   jz    .done                               ; We're done if so...

;   mov   rbx,_bss_start                      ; bss start (VMA) into rbx
; .zero_bss_loop:
;   mov   qword [rbx],0x0                     ; Clear one qword
;   add   rbx,0x8                             ; Increment write pointer
;   dec   rcx                                 ; Decrement loop counter
;   jnz   .zero_bss_loop                      ; Loop until CX is zero

; .done:
  ; Pop capability count and pointer for cap init
  pop   rdi                                 ; ... count
  pop   rsi                                 ; ... pointer

  ; Pop argc / argv for main (later)
  pop   r12                                 ; ... argc
  pop   r13                                 ; ... argv

  ; Push a NULL frame pointer (and misalign by 8) here.
  ;
  xor   rbp, rbp                            ; Zero RBP
  push  rbp                                 ; Push NULL frame pointer (and misalign by 8)
  sub   rsp,0x8                             ; Realign stack for before C calls

  call  _anos_init_capabilities             ; Capability init
  call  _init                               ; GCC constructors

  mov   rdi,r12                             ; Move argc into first C arg....
  mov   rsi,r13                             ; ... and argv into second.
  call  main                                ; Let's do some C...  
  call  _fini                               ; GCC destructors

  ; TODO we'll go bang here, we need an exit syscall!

EMPTY_ARGS:
    dq  0