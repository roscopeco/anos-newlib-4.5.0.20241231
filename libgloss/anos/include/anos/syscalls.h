/*
 * Anos system call interface for user-mode code
 * anos - An Operating System
 *
 * Copyright (c) 2024 Ross Bamford
 */

// clang-format Language: C

#ifndef __ANOS_ANOS_SYSCALLS_H
#define __ANOS_ANOS_SYSCALLS_H

#include <assert.h>

#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

#include "anos/system.h"
#include "anos/types.h"

#define MAX_IPC_BUFFER_SIZE ((0x1000))

// NOTE these must be kept in-step with the kernel!
typedef enum {
    SYSCALL_ID_INVALID = 0,
    SYSCALL_ID_DEBUG_PRINT,
    SYSCALL_ID_DEBUG_CHAR,
    SYSCALL_ID_CREATE_THREAD,
    SYSCALL_ID_MEMSTATS,
    SYSCALL_ID_SLEEP,
    SYSCALL_ID_CREATE_PROCESS,
    SYSCALL_ID_MAP_VIRTUAL,
    SYSCALL_ID_SEND_MESSAGE,
    SYSCALL_ID_RECV_MESSAGE,
    SYSCALL_ID_REPLY_MESSAGE,
    SYSCALL_ID_CREATE_CHANNEL,
    SYSCALL_ID_DESTROY_CHANNEL,
    SYSCALL_ID_REGISTER_NAMED_CHANNEL,
    SYSCALL_ID_DEREGISTER_NAMED_CHANNEL,
    SYSCALL_ID_FIND_NAMED_CHANNEL,
    SYSCALL_ID_KILL_CURRENT_TASK,
    SYSCALL_ID_UNMAP_VIRTUAL,
    SYSCALL_ID_CREATE_REGION,
    SYSCALL_ID_DESTROY_REGION,
    SYSCALL_ID_MAP_FIRMWARE_TABLES,
    SYSCALL_ID_MAP_PHYSICAL,
    SYSCALL_ID_ALLOC_PHYSICAL_PAGES,
    SYSCALL_ID_ALLOC_INTERRUPT_VECTOR,
    SYSCALL_ID_WAIT_INTERRUPT,

    // sentinel
    SYSCALL_ID_END,
} __attribute__((packed)) SyscallId;

typedef enum {
    SYSCALL_OK = 0ULL,
    SYSCALL_FAILURE = -1ULL,
    SYSCALL_BAD_NUMBER = -2ULL,
    SYSCALL_NOT_IMPL = -3ULL,
    SYSCALL_BADARGS = -4ULL,
    SYSCALL_BAD_NAME = -5ULL,

    /* reserved */
    SYSCALL_INCAPABLE = -254ULL
} SyscallResultType;

typedef struct {
    SyscallResultType result;
    uint8_t value;
} SyscallResultU8;

typedef struct {
    SyscallResultType result;
    uint64_t value;
} SyscallResultU64;

typedef struct {
    SyscallResultType result;
    int64_t value;
} SyscallResultI64;

typedef struct {
    SyscallResultType result;
    void* value;
} SyscallResultP;

typedef struct {
    SyscallResultType result;
    uintptr_t value;
} SyscallResultA;

typedef SyscallResultU64 SyscallResult;

static_assert(sizeof(SyscallResultU8) == 16, "SyscallResultU8 must be 16 bytes");
static_assert(sizeof(SyscallResultU64) == 16, "SyscallResultU64 must be 16 bytes");
static_assert(sizeof(SyscallResultI64) == 16, "SyscallResultI64 must be 16 bytes");
static_assert(sizeof(SyscallResultP) == 16, "SyscallResultP must be 16 bytes");
static_assert(sizeof(SyscallResultA) == 16, "SyscallResultA must be 16 bytes");
static_assert(sizeof(SyscallResult) == 16, "SyscallResult must be 16 bytes");

#define REGION_FLAG_AUTOMAP ((1))

#ifdef DEBUG_INT_SYSCALLS
#define anos_kprint anos_kprint_int
#define anos_kputchar anos_kputchar_int
#define anos_create_thread anos_create_thread_int
#define anos_get_mem_info anos_get_mem_info_int
#define anos_task_sleep_current anos_task_sleep_current_int
#define anos_create_process anos_create_process_int
#define anos_map_virtual anos_map_virtual_int
#define anos_send_message anos_send_message_int
#define anos_recv_message anos_recv_message_int
#define anos_reply_message anos_reply_message_int
#define anos_create_channel anos_create_channel_int
#define anos_destroy_channel anos_destroy_channel_int
#define anos_register_channel_name anos_register_channel_name_int
#define anos_remove_channel_name anos_remove_channel_name_int
#define anos_find_named_channel anos_find_named_channel_int
#define anos_kill_current_task anos_kill_current_task_int
#define anos_unmap_virtual anos_unmap_virtual_int
#define anos_create_region anos_create_region_int
#define anos_destroy_region anos_destroy_region_int
#define anos_map_firmware_tables anos_map_firmware_tables_int
#define anos_map_physical anos_map_physical_int
#define anos_alloc_physical_pages anos_alloc_physical_pages_int
#define anos_allocate_interrupt_vector anos_allocate_interrupt_vector_int
#define anos_wait_interrupt anos_wait_interrupt_int
#else
#define anos_kprint anos_kprint_syscall
#define anos_kputchar anos_kputchar_syscall
#define anos_create_thread anos_create_thread_syscall
#define anos_get_mem_info anos_get_mem_info_syscall
#define anos_task_sleep_current anos_task_sleep_current_syscall
#define anos_create_process anos_create_process_syscall
#define anos_map_virtual anos_map_virtual_syscall
#define anos_send_message anos_send_message_syscall
#define anos_recv_message anos_recv_message_syscall
#define anos_reply_message anos_reply_message_syscall
#define anos_create_channel anos_create_channel_syscall
#define anos_destroy_channel anos_destroy_channel_syscall
#define anos_register_channel_name anos_register_channel_name_syscall
#define anos_remove_channel_name anos_remove_channel_name_syscall
#define anos_find_named_channel anos_find_named_channel_syscall
#define anos_kill_current_task anos_kill_current_task_syscall
#define anos_unmap_virtual anos_unmap_virtual_syscall
#define anos_create_region anos_create_region_syscall
#define anos_destroy_region anos_destroy_region_syscall
#define anos_map_firmware_tables anos_map_firmware_tables_syscall
#define anos_map_physical anos_map_physical_syscall
#define anos_alloc_physical_pages anos_alloc_physical_pages_syscall
#define anos_allocate_interrupt_vector anos_allocate_interrupt_vector_syscall
#define anos_wait_interrupt anos_wait_interrupt_syscall
#endif

#define ANOS_MAP_VIRTUAL_FLAG_READ      ((0x1))
#define ANOS_MAP_VIRTUAL_FLAG_WRITE     ((0x2))
#define ANOS_MAP_VIRTUAL_FLAG_EXEC      ((0x4))
#define ANOS_MAP_VIRTUAL_FLAG_NOCACHE   ((0x8))

#define ANOS_MAP_PHYSICAL_FLAG_READ     ((0x1))
#define ANOS_MAP_PHYSICAL_FLAG_WRITE    ((0x2))
#define ANOS_MAP_PHYSICAL_FLAG_EXEC     ((0x4))
#define ANOS_MAP_PHYSICAL_FLAG_NOCACHE  ((0x8))

SyscallResult anos_kprint_int(const char *msg);
SyscallResult anos_kprint_syscall(const char *msg);

SyscallResult anos_kputchar_int(char chr);
SyscallResult anos_kputchar_syscall(char chr);

SyscallResult anos_create_thread_int(ThreadFunc func, uintptr_t stack_pointer);
SyscallResult anos_create_thread_syscall(ThreadFunc func, uintptr_t stack_pointer);

SyscallResult anos_get_mem_info_int(AnosMemInfo *meminfo);
SyscallResult anos_get_mem_info_syscall(AnosMemInfo *meminfo);

SyscallResult anos_task_sleep_current_syscall(uint64_t ticks);
SyscallResult anos_task_sleep_current_int(uint64_t ticks);

SyscallResultI64 anos_create_process_syscall(ProcessCreateParams *params);
SyscallResultI64 anos_create_process_int(ProcessCreateParams *params);

SyscallResultP anos_map_virtual_syscall(uint64_t size, uintptr_t base_address, uint64_t flags);
SyscallResultP anos_map_virtual_int(uint64_t size, uintptr_t base_address, uint64_t flags);

SyscallResult anos_send_message_syscall(uint64_t channel_cookie, uint64_t tag,
                                   size_t buffer_size, void *buffer);
SyscallResult anos_send_message_int(uint64_t channel_cookie, uint64_t tag,
                               size_t buffer_size, void *buffer);

SyscallResult anos_recv_message_syscall(uint64_t channel_cookie, uint64_t *tag,
                                   size_t *buffer_size, void *buffer);
SyscallResult anos_recv_message_int(uint64_t channel_cookie, uint64_t *tag,
                               size_t *buffer_size, void *buffer);

SyscallResult anos_reply_message_syscall(uint64_t message_cookie, uint64_t reply);
SyscallResult anos_reply_message_int(uint64_t message_cookie, uint64_t reply);

SyscallResult anos_create_channel_syscall(void);
SyscallResult anos_create_channel_int(void);

SyscallResult anos_destroy_channel_syscall(uint64_t cookie);
SyscallResult anos_destroy_channel_int(uint64_t cookie);

SyscallResult anos_register_channel_name_syscall(uint64_t cookie, char *name);
SyscallResult anos_register_channel_name_int(uint64_t cookie, char *name);

SyscallResult anos_remove_channel_name_syscall(char *name);
SyscallResult anos_remove_channel_name_int(char *name);

SyscallResult anos_find_named_channel_syscall(char *name);
SyscallResult anos_find_named_channel_int(char *name);

noreturn SyscallResult anos_kill_current_task_syscall();
noreturn SyscallResult anos_kill_current_task_int();

SyscallResult anos_unmap_virtual_syscall(uint64_t size, uintptr_t base_address);
SyscallResult anos_unmap_virtual_int(uint64_t size, uintptr_t base_address);

SyscallResult anos_create_region_syscall(uintptr_t start, uintptr_t end, uint64_t flags);
SyscallResult anos_create_region_int(uintptr_t start, uintptr_t end, uint64_t flags);

SyscallResult anos_destroy_region_syscall(uintptr_t start);
SyscallResult anos_destroy_region_int(uintptr_t start);

SyscallResult anos_map_firmware_tables_syscall(uintptr_t start);
SyscallResult anos_map_firmware_tables_int(uintptr_t start);

SyscallResult anos_map_physical_syscall(uintptr_t start_phys, void *start_virt, size_t size, uint64_t flags);
SyscallResult anos_map_physical_int(uintptr_t start_phys, void *start_virt, size_t size, uint64_t flags);

SyscallResultA anos_alloc_physical_pages_syscall(size_t size);
SyscallResultA anos_alloc_physical_pages_int(size_t size);

SyscallResultU8 anos_allocate_interrupt_vector_syscall(uint32_t bus_device_func, uint64_t *msi_address, uint32_t *msi_data);
SyscallResultU8 anos_allocate_interrupt_vector_int(uint32_t bus_device_func, uint64_t *msi_address, uint32_t *msi_data);

SyscallResult anos_wait_interrupt_syscall(uint8_t vector, uint32_t *event_data);
SyscallResult anos_wait_interrupt_int(uint8_t vector, uint32_t *event_data);

#endif //__ANOS_ANOS_SYSCALLS_H
