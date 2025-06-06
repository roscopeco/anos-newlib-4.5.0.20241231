#
# Stdlib - Pre-main startup code (RISC-V)
# anos - An Operating System
#
# Copyright (c) 2025 Ross Bamford
# This is the entry-point for user programs.
#
# There's still stuff TODO, set up IO, and so on...
#
 
.section .text.init
.global _start
_start:
    # Pop capability args: count into a0, pointer into a1
    ld     a0, 0(sp)             # capability count
    ld     a1, 8(sp)             # capability pointer
    addi   sp, sp, 16            # advance the stack

    call   _anos_init_capabilities

    # Pop argc into t0, argv into t1
    ld     t0, 0(sp)             # argc
    ld     t1, 8(sp)             # argv

    # Push a null frame pointer, and realign
    li     s0, 0
    sd     s0, -8(sp)            # Push NULL frame pointer
    addi   sp, sp, -16           # Realign stack (8 + 8)
    
    # TODO _init_array!

    mv     a0, t0                # argc
    mv     a1, t1                # argv
    call   main
    # TODO _fini_array!

    # TODO: Proper syscall exit here
    j      .
