# system - anos syscalls interface for usermode (RISC-V)
# anos - An Operating System
#
# Copyright (c) 2025 Ross Bamford
#

.section .text
.global __syscall_capabilities
.extern __syscall_capabilities

.macro anos_find_call_cap idx
    # a7 = __syscall_capabilities[idx]
    la    t0, __syscall_capabilities
    ld    a7, \idx*8(t0)
.endm

.macro anos_syscall name, idx
    .global \name\()_syscall
\name\()_syscall:
    anos_find_call_cap \idx
    ecall
    ret
.endm

anos_syscall anos_kprint, 1
anos_syscall anos_kputchar, 2
anos_syscall anos_create_thread, 3
anos_syscall anos_get_mem_info, 4
anos_syscall anos_task_sleep_current, 5
anos_syscall anos_create_process, 6
anos_syscall anos_map_virtual, 7
anos_syscall anos_send_message, 8
anos_syscall anos_recv_message, 9
anos_syscall anos_reply_message, 10
anos_syscall anos_create_channel, 11
anos_syscall anos_destroy_channel, 12
anos_syscall anos_register_channel_name, 13
anos_syscall anos_remove_channel_name, 14
anos_syscall anos_find_named_channel, 15
anos_syscall anos_kill_current_task, 16
anos_syscall anos_unmap_virtual, 17
anos_syscall anos_create_region, 18
anos_syscall anos_destroy_region, 19
anos_syscall anos_map_firmware_tables, 20
anos_syscall anos_map_physical, 21
