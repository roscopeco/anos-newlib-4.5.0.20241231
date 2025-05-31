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
    # Clear BSS
    la     t0, _bss_start        # t0 = start of .bss
    la     t1, _bss_end          # t1 = end of .bss
    bge    t0, t1, 1f            # Skip if size <= 0

0:  sd     zero, 0(t0)           # Store zero
    addi   t0, t0, 8             # Increment pointer
    blt    t0, t1, 0b            # Loop if t0 < t1

1:
    # Pop capability args: count into a0, pointer into a1
    ld     a0, 0(sp)             # capability count
    ld     a1, 8(sp)             # capability pointer
    addi   sp, sp, 16            # advance the stack

    # Pop argc into t0, argv into t1
    ld     t0, 0(sp)             # argc
    ld     t1, 8(sp)             # argv
    addi   sp, sp, 16

    # Push a null frame pointer, and realign
    li     s0, 0
    sd     s0, -8(sp)            # Push NULL frame pointer
    addi   sp, sp, -16           # Realign stack (8 + 8)
    
    call   _anos_init_capabilities
    # TODO _init_array!

    mv     a0, t0                # argc
    mv     a1, t1                # argv
    call   main
    # TODO _fini_array!

    # TODO: Proper syscall exit here
    j      .
