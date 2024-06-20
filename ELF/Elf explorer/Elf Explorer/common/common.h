#pragma once
//#include "elf.h"
CString e_machineSz[] = {
	"No machine",   //=0
	"AT&T WE 32100", //=1
	"SPARC", //=2
	"Intel 80386", //=3
	"Motorola 68000", //=4
	"Motorola 88000", //=5
	"Reserved for future use (was EM_486)", //=6
	"Intel 80860", //=7
	"MIPS I Architecture", //=8
	"IBM System/370 Processor", //=9
	"MIPS RS3000 Little-endian", //=10
	"Reserved for future use", //=11
	"Reserved for future use", //=12
	"Reserved for future use", //=13
	"Reserved for future use", //=14
	"Hewlett-Packard PA-RISC", //=15
	"Reserved for future use", //=16
	"Fujitsu VPP500", //=17
	"Enhanced instruction set SPARC", //=18
	"Intel 80960", //=19
	"PowerPC", //=20
	"64-bit PowerPC", //=21
	"IBM System/390 Processor", //=22
	"Reserved for future use", //=23
	"Reserved for future use", //=24
	"Reserved for future use", //=25
	"Reserved for future use", //=26
	"Reserved for future use", //=27
	"Reserved for future use", //=28
	"Reserved for future use", //=29
	"Reserved for future use", //=30
	"Reserved for future use", //=31
	"Reserved for future use", //=32
	"Reserved for future use", //=33
	"Reserved for future use", //=34
	"Reserved for future use", //=35
	"NEC V800", //=36
	"Fujitsu FR20", //=37
	"TRW RH-32", //=38
	"Motorola RCE", //=39
	"Advanced RISC Machines ARM", //=40
	"Digital Alpha", //=41
	"Hitachi SH", //=42
	"SPARC Version 9", //=43
	"Siemens TriCore embedded processor", //=44
	"Argonaut RISC Core, Argonaut Technologies Inc.", //=45
	"Hitachi H8/300", //=46
	"Hitachi H8/300H", //=47
	"Hitachi H8S", //=48
	"Hitachi H8/500", //=49
	"Intel IA-64 processor architecture", //=50
	"Stanford MIPS-X", //=51
	"Motorola ColdFire", //=52
	"Motorola M68HC12", //=53
	"Fujitsu MMA Multimedia Accelerator", //=54
	"Siemens PCP", //=55
	"Sony nCPU embedded RISC processor", //=56
	"Denso NDR1 microprocessor", //=57
	"Motorola Star*Core processor", //=58
	"Toyota ME16 processor", //=59
	"STMicroelectronics ST100 processor", //=60
	"Advanced Logic Corp. TinyJ embedded processor family", //=61
	"AMD x86-64 architecture", //=62
	"Sony DSP Processor", //=63
	"Digital Equipment Corp. PDP-10", //=64
	"Digital Equipment Corp. PDP-11", //=65
	"Siemens FX66 microcontroller", //=66
	"STMicroelectronics ST9+ 8/16 bit microcontroller", //=67
	"STMicroelectronics ST7 8-bit microcontroller", //=68
	"Motorola MC68HC16 Microcontroller", //=69
	"Motorola MC68HC11 Microcontroller", //=70
	"Motorola MC68HC08 Microcontroller", //=71
	"Motorola MC68HC05 Microcontroller", //=72
	"Silicon Graphics SVx", //=73
	"",//74
	"Digital VAX", //=75
	"Axis Communications 32-bit embedded processor", //=76
	"Infineon Technologies 32-bit embedded processor", //=77
	"Element 14 64-bit DSP Processor", //=78
	"LSI Logic 16-bit DSP Processor", //=79
	"Donald Knuth's educational 64-bit processor", //=80
	"Harvard University machine-independent object files", //=81
	"SiTera Prism", //=82
	"Atmel AVR 8-bit microcontroller", //=83
	"Fujitsu FR30", //=84
	"Mitsubishi D10V", //=85
	"Mitsubishi D30V", //=86
	"NEC v850", //=87
	"Mitsubishi M32R", //=88
	"Matsushita MN10300", //=89
	"Matsushita MN10200", //=90
	"picoJava", //=91
	"OpenRISC 32-bit embedded processor", //=92
	"ARC Cores Tangent-A5", //=93
	"Tensilica Xtensa Architecture", //=94
	"Alphamosaic VideoCore processor", //=95
	"Thompson Multimedia General Purpose Processor", //=96
	"National Semiconductor 32000 series", //=97
	"Tenor Network TPC processor", //=98
	"Trebia SNP 1000 processor", //=99
	"STMicroelectronics (www.st.com) ST200 microcontroller", //=100
	"Ubicom IP2xxx microcontroller family", //=101
	"MAX Processor", //=102
	"National Semiconductor CompactRISC microprocessor", //=103
	"Fujitsu F2MC16", //=104
	"Texas Instruments embedded microcontroller msp430", //=105
	"Analog Devices Blackfin (DSP) processor", //=106
	"S1C33 Family of Seiko Epson processors", //=107
	"Sharp embedded microprocessor", //=108
	"Arca RISC Microprocessor", //=109
	"Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University",//110
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"EM_AARCH64" , // 183
};
//参考链接https://blog.csdn.net/lzheibai1/article/details/135775215
CString  ei_osabiSz[] = {
   "UNIX System V ABI",   //0						
   "HP-UX",							//1
   "NetBSD.",							//2
   "GNU/Linux."	,					//3
   "GNU/Hurd",					//4
   "860pen common IA32 ABI",							//5
   "Solaris.",							//6
   "AIX",						//7
	"IRIX",				//8
	"FreeBSD"	,		 //9
	"Compaq TRU64 UNIX.",//10
};




 CString  e_typeSz[] = {
	"No file type",
	"Relocatable file",
	"Executable file",
	"Shared object file",
	"Core file"
	"",
	"",
	"",
	""
};
 CString  e_versionSz[] = {
   "EV_NONE",
   "EV_CURRENT"
 };


CString st_infoSz[] = {
	"STB_LOCA",//0
		"STB_GOBA",
		"STB_WEAK",//2
		"STB_NUM",
		"",
		"",
		"",
		"",
		"",
		"",
		"STB_LOOS",//010
		"",
		"STB_HIOS",//012
		"STB_LOPROC",//013
		"",
		"STB_HIPROC"//015
};
CString sym_info_type_eSz[] = {
	"STT_NOTYPE",      //= 0x0,
	"STT_OBJECT",     // = 0x1,
	"STT_FUNC",      //  = 0x2,
	"STT_SECTION",   //  = 0x3,
	"STT_FILE",      //  = 0x4,
	"STT_COMMON",    //  = 0x5,
	"STT_TLS",      //   = 0x6,
	"STT_NUM",       //  = 0x7,
	"",
	"",
	"STT_LOOS",      //  = 0xA,
	"STT_GNU_IFUNC", //  = 0xA,
	"STT_HIOS",       // = 0xB,
	"STT_LOPROC",    //  = 0xC,
	"STT_HIPROC"     // = 0xD
};



#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
//#define ELFMAG "\177ELF"

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_PAD 8



// Program Header Types
typedef enum ProgramType {
	PT_NULL = 0x0,
	PT_LOAD = 0x1,
	PT_DYNAMIC = 0x2,
	PT_INERP = 0x3,
	PT_NOTE = 0x4,
	PT_SHLIB = 0x5,
	PT_PHDR = 0x6,
	PT_TLS = 0x7,
	PT_NUM = 0x8,
	PT_LOOS = 0x60000000,
	PT_GNU_EH_FRAME = 0x6474e550,
	PT_GNU_STACK = 0x6474e551,
	PT_GNU_RELRO = 0x6474e552,
	PT_LOSUNW = 0x6ffffffa,
	PT_SUNWBSS = 0x6ffffffa,
	PT_SUNWSTACK = 0x6ffffffb,
	PT_HISUNW = 0x6fffffff,
	PT_HIOS = 0x6fffffff,
	PT_LOPROC = 0x70000000,
	PT_HIPROC = 0x7fffffff,
	// ARM Sections
	PT_SHT_ARM_EXIDX = 0x70000001,
	PT_SHT_ARM_PREEMPTMAP = 0x70000002,
	PT_SHT_ARM_ATTRIBUTES = 0x70000003,
	PT_SHT_ARM_DEBUGOVERLAY = 0x70000004,
	PT_SHT_ARM_OVERLAYSECTION = 0x70000005
} p_type32_e;
//typedef p_type32_e p_type64_e;

typedef enum ProgramMemType {
	PF_None = 0x0,
	PF_Exec = 0x1,
	PF_Write = 0x2,
	PF_Write_Exec = 0x3,
	PF_Read = 0x4,
	PF_Read_Exec = 0x5,
	PF_Read_Write = 0x6,
	PF_Read_Write_Exec = 0x7
} p_flags32_e;
//typedef p_flags32_e p_flags64_e;

enum GetProgramType {
	None = 0x0,
	GetDefProgramType = 0x1,
	GetProgramType = 0x2,
	GetProgramMemType = 0x3,
	ProgramHeader32 = 0x4,
	ProgramHeader64 = 0x5,
	SectionHeader32 = 0x6,
	SectionHeader64 = 0x7,
	GetSectionType = 0x8,
	GetSectionMemType = 0x9,
	SET_STRTAB = 10,
	SET_DYNSYM = 11,
	SET_HASH = 12,
	SET_GNUHASH = 13,
	SET_SYMTAB = 14,
	SET_RELDYN = 15,
	SET_RELPLT = 16,
	GETSECTIONBIND = 17,
	GETSECTIONTYPE = 18,
	UNDEF  = 0xff,
};







//#define PT_NULL 0
//#define PT_LOAD 1
//#define PT_DYNAMIC 2
//#define PT_INTERP 3
//#define PT_NOTE 4
//#define PT_SHLIB 5
//#define PT_PHDR 6
//#define PT_TLS 7
//#define PT_LOOS 0x60000000
//#define PT_HIOS 0x6fffffff
//#define PT_LOPROC 0x70000000
//#define PT_HIPROC 0x7fffffff
//#define PT_GNU_EH_FRAME (PT_LOOS + 0x474e550)
//#define PT_GNU_STACK (PT_LOOS + 0x474e551)
//#define PT_GNU_RELRO (PT_LOOS + 0x474e552)
//#define PT_GNU_PROPERTY (PT_LOOS + 0x474e553)
//#define PT_AARCH64_MEMTAG_MTE (PT_LOPROC + 0x2)


//节类型定义
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_GNUHASH 0x6FFFFFF6
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_NUM 12
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff
#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_RELA_LIVEPATCH 0x00100000
#define SHF_RO_AFTER_INIT 0x00200000
#define SHF_MASKPROC 0xf0000000
#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_HIPROC 0xff1f
#define SHN_LIVEPATCH 0xff20
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_HIRESERVE 0xffff

//符号表定义

#define ELF_ST_BIND(x) ((x) >> 4)
#define ELF_ST_TYPE(x) (((unsigned int) x) & 0xf)
#define ELF32_ST_BIND(x) ELF_ST_BIND(x)
#define ELF32_ST_TYPE(x) ELF_ST_TYPE(x)
#define ELF64_ST_BIND(x) ELF_ST_BIND(x)
#define ELF64_ST_TYPE(x) ELF_ST_TYPE(x)
//BIND
#define STB_LOPROC 13
#define STB_HIPROC 15
#define STB_LOOS 10
#define STB_HIOS 12
#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
//TYPE
#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_COMMON 5
#define STT_TLS 6
#define STT_LOOS 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_HIPROC 15

//hash函数
uint32_t xh_elf_hash(const char* name)
{
	uint32_t h = 0, g;

	while (*name) {
		h = (h << 4) + *name++;
		g = h & 0xf0000000;
		h ^= g;
		h ^= g >> 24;
	}

	return h;
}

//重定位表
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)
#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i) & 0xffffffff)