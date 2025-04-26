/*
 * ANOS usermode program link script
 *
 * (c)2023 Ross Bamford & Contributors
 * See LICENSE.md (spoiler: GPLv2)
 */

OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

MEMORY {
    CODE    : org = 0x0000000002000000,  l = 0x003e000000     /* 992MiB for code segment, to 1GiB mark */
    /* stack goes here - 64MiB from 0x040000000 to 043ffffff */
    DATA    : org = 0x0000000044000000,  l = 0x007c000000     /* 1.9GiB for bss, 64MiB stack goes below this */
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
    KEEP(*(.tm_clone_table*))

    . = ALIGN(16);
    KEEP(*(.init*))     /* maybe we should keep these in their sections 🤔... */
    KEEP(*(.fini*))

    . = ALIGN(4096);
    *(.rodata*)
    _code_end = .;
  } > CODE
  
  .eh_frame_hdr : 
  { 
    KEEP(*(.eh_frame_hdr))
  } > CODE

  .eh_frame : ONLY_IF_RO 
  { 
    KEEP(*(.eh_frame)) 
  } > CODE

  .gcc_except_table : ONLY_IF_RO 
  { 
    *(.gcc_except_table .gcc_except_table.*) 
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
