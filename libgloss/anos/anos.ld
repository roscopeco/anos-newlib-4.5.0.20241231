/*
 * ANOS system (usermode manager)
 * Link Script
 *
 * (c)2023 Ross Bamford & Contributors
 * See LICENSE.md (spoiler: GPLv2)
 *
 * TODO this is forcing us into an mcmodel=large situation
 * for building the C lib, we should change that (it'll need
 * the unreasonably-large memory regions sorting out, which
 * will need a change in SYSTEM's initial mapping too...)
 */

OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

MEMORY {
    CODE    : org = 0x0000000002000000,  l = 0x0040000000     /* 1GiB for server code */
    DATA    : org = 0x0000000080000000,  l = 0x4000000000     /* 256GiB for bss, stack goes below this */
}

SECTIONS
{
  .text :
  {
    _code = .;
    *(.text.init)
    . = ALIGN(16);
    *(.text*)
    . = ALIGN(16);

       _ctors        = .;
    KEEP(*(.ctors))           /* Run in reverse order, so non-priority ones go first  */
    KEEP(*(SORT(.ctors.*)))   /* followed by ctors with priority */
      _ctors_end     = .;

      _dtors         = .;
    KEEP(*(SORT(.dtors.*)))   /* dtors with priority go first */
    KEEP(*(.dtors))           /* Followed by non-priority ones */
      _dtors_end     = .;

    . = ALIGN(16);
    *(.rodata .rodata.*)

    . = ALIGN(16);
    KEEP(*(.gcc_except_table))

    . = ALIGN(16);
    KEEP(*(.tm_clone_table*))

    . = ALIGN(16);
    KEEP(*(.init*))     /* maybe we should keep these in their sections 🤔... */
    KEEP(*(.fini*))

    . = ALIGN(4096);
    *(.rodata*)
    _code_end = .;
  } > CODE
  
  .bss (NOLOAD) :
  {
    _bss_start = .;
    . = ALIGN(4096);
    *(.bss*)
    *(COMMON)
    . = ALIGN(4096);
    _bss_end = .;
  } > DATA AT > CODE

  .data : 
  {
    _data_start = .;
    . = ALIGN(4096);
    *(.data)
    . = ALIGN(4096);
    _data_end = .;
  } > DATA

  _end = .;
}
