#pragma once
#define SHDRS 16
/*
.dynsym .dynstr .hash .rel.dyn .rel.plt
.plt .text .ARM.extab .ARM.exidx .fini_array
.init_array .dynamic .got .data
*/
#define NONE 0
#define DYNSYM 1
#define DYNSTR 2
#define HASH 3
#define RELDYN 4
#define RELPLT 5
#define PLT 6
#define TEXT_CODE 7
#define ARMEXIDX 8
#define FINIARRAY 9
#define INITARRAY 10
#define DYNAMIC 11
#define GOT 12
#define DATA 13
#define BSS 14
#define STRTAB 15

char* str = "..dynsym..dynstr..hash..rel.dyn..rel.plt..plt..text@.ARM.extab..ARM.exidx..fini_array..init_array..dynamic..got..data..bss..shstrtab\0";
char* str1 = "\0.dynsym\0.dynstr\0.hash\0.rel.dyn\0.rel.plt\0.plt\0.text\0.ARM.extab\0.ARM.exidx\0.fini_array\0.init_array\0.dynamic\0.got\0.data\0.bss\0.shstrtab\0";
Elf32_Shdr shdr[SHDRS] = { 0 };
