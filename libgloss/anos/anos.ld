/*
 * ANOS usermode program link script
 *
 * (c)2023 Ross Bamford & Contributors
 * See LICENSE.md (spoiler: GPLv2)
 */

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

    /* maybe we should keep these in their sections 🤔... */
      __preinit_array_start = .;
    KEEP (*(SORT_BY_INIT_PRIORITY(.preinit_array)))
      __preinit_array_end = .;
    . = ALIGN(16);
      __init_array_start = .;
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array*))) /* init_array... */
    KEEP(*(SORT_BY_INIT_PRIORITY(.ctors.*)))   /* followed by ctors with priority */
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    KEEP (*(.init_array*))
      __init_array_end = .;
    . = ALIGN(16);
      __fini_array_start = .;
    KEEP(*(SORT_BY_INIT_PRIORITY(.dtors.*)))   /* dtors with priority go first */
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array*))) /* ...followed by fini_array */
      __fini_array_end = .;

    . = ALIGN(16);
    KEEP(*(.init*))
    KEEP(*(.fini*))

    . = ALIGN(16);

    . = ALIGN(16);
    KEEP(*(.tm_clone_table*))

    . = ALIGN(4096);
    *(.rodata .rodata* .rodata.*)
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
    *(.sbss*)
    *(COMMON)
    . = ALIGN(4096);
    _bss_end = .;
  } > DATA AT > CODE

  .data : 
  {
    _data_start = .;
    . = ALIGN(4096);
    *(.data*)
    *(.sdata*)
    . = ALIGN(4096);
    _data_end = .;
  } > DATA

  _end = .;
}
