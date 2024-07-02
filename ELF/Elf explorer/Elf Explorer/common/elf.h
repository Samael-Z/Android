
#pragma once


/* This is the info that is needed to parse the dynamic section of the file */
#define DT_NULL	0
#define DT_NEEDED	1
#define DT_PLTRELSZ	2
#define DT_PLTGOT	3
#define DT_HASH		4
#define DT_STRTAB	5
#define DT_SYMTAB	6
#define DT_RELA		7
#define DT_RELASZ	8
#define DT_RELAENT	9
#define DT_STRSZ	10
#define DT_SYMENT	11
#define DT_INIT		25
#define DT_FINI		26
#define DT_SONAME	14
#define DT_RPATH 	15
#define DT_SYMBOLIC	16
#define DT_REL	    17
#define DT_RELSZ	18
#define DT_RELENT	19
#define DT_PLTREL	20
#define DT_DEBUG	21
#define DT_TEXTREL	22
#define DT_JMPREL	23
#define DT_LOPROC	0x70000000
#define DT_HIPROC	0x7fffffff
#define DT_MIPS_RLD_VERSION	0x70000001
#define DT_MIPS_TIME_STAMP	0x70000002
#define DT_MIPS_ICHECKSUM	0x70000003
#define DT_MIPS_IVERSION	0x70000004
#define DT_MIPS_FLAGS	0x70000005
#define RHF_NONE	  0
#define RHF_HARDWAY	  1
#define RHF_NOTPOT	  2
#define DT_MIPS_BASE_ADDRESS	0x70000006
#define DT_MIPS_CONFLICT	0x70000008
#define DT_MIPS_LIBLIST	0x70000009
#define DT_MIPS_LOCAL_GOTNO	0x7000000a
#define DT_MIPS_CONFLICTNO	0x7000000b
#define DT_MIPS_LIBLISTNO	0x70000010
#define DT_MIPS_SYMTABNO	0x70000011
#define DT_MIPS_UNREFEXTNO	0x70000012
#define DT_MIPS_GOTSYM	0x70000013
#define DT_MIPS_HIPAGENO	0x70000014
#define DT_MIPS_RLD_MAP	0x70000016


//	Name           Size Alignment   Purpose
//	====           ==== =========   =======
//	Elf32_Addr      4       4       Unsigned program address
//	Elf32_Half      2       2       Unsigned medium integer
//	Elf32_Off       4       4       Unsigned file offset
//	Elf32_Sword     4       4       Signed large integer
//	Elf32_Word      4       4       Unsigned large integer
//	unsigned char   1       1       Unsigned small integer
typedef	DWORD	Elf32_Addr; //无符号程序地址
typedef	WORD	Elf32_Half; //无符号中整数
typedef	DWORD	Elf32_Off;  //无符号文件偏移
typedef	DWORD	Elf32_Sword;//带符号整数
typedef	DWORD	Elf32_Word; //无符号整数

typedef LONGLONG Elf64_Addr;  //无符号程序地址
typedef WORD     Elf64_Half;  //无符号中整数
typedef LONGLONG Elf64_Off;   //无符号文件偏移
typedef DWORD	 Elf64_Sword; //带符号整数
typedef DWORD	 Elf64_Word;  //无符号整数
typedef LONGLONG Elf64_Xword;
typedef LONGLONG Elf64_Sxword;

#define EI_NIDENT       16

// ELF header
typedef struct{
	unsigned char       e_ident[EI_NIDENT];
	Elf32_Half          e_type;
	//	e_type
	//
	//	该成员确定该object的类型。
	//
	//	Name        Value  Meaning
	//	====        =====  =======
	//	ET_NONE         0  No file type
	//	ET_REL          1  Relocatable file
	//	ET_EXEC         2  Executable file
	//	ET_DYN          3  Shared object file
	//	ET_CORE         4  Core file
	//	ET_LOPROC  0xff00  Processor-specific
	//	ET_HIPROC  0xffff  Processor-specific

	Elf32_Half          e_machine;
	//	e_machine
	//			
	//	该成员变量指出了运行该程序需要的体系结构。
	//
	//	Name      Value  Meaning
	//	====      =====  =======
	//	EM_NONE       0  No machine
	//	EM_M32        1  AT&T WE 32100
	//	EM_SPARC      2  SPARC
	//	EM_386        3  Intel 80386
	//	EM_68K        4  Motorola 68000
	//	EM_88K        5  Motorola 88000
	//	EM_860        7  Intel 80860
	//	EM_MIPS       8  MIPS RS3000

	Elf32_Word          e_version;
	//	e_version
	//
	//	这个成员确定object文件的版本。
	//
	//	Name         Value  Meaning
	//	====         =====  =======
	//	EV_NONE          0  Invalid version
	//	EV_CURRENT       1  Current version

	Elf32_Addr          e_entry;
	//	该成员是系统第一个传输控制的虚拟地址，在那启动进程。
	//	假如文件没有如何关联的入口点，该成员就保持为 0。

	Elf32_Off           e_phoff;
	//	该成员保持着程序头表（program header table）在文件中的偏移量(以字节计数)。
	//	假如该文件没有程序头表的的话，该成员就保持为 0。

	Elf32_Off           e_shoff;
	//	该成员保持着section头表（section header table）在文件中的偏移量(以字节计数)。
	//	假如该文件没有section头表的的话，该成员就保持为0。

	Elf32_Word          e_flags;
	//	该成员保存着相关文件的特定处理器标志。
	//	flag的名字来自于EF_<machine>_<flag>。看下机器信息“Machine Information”部分的flag的定义。

	Elf32_Half          e_ehsize;
	//	该成员保存着ELF头大小(以字节计数)。

	Elf32_Half          e_phentsize;
	//	该成员保存着在文件的程序头表（program header table）中一个入口的大小
	//	(以字节计数)。所有的入口都是同样的大小。

	Elf32_Half          e_phnum;
	//	该成员保存着在程序头表中入口的个数。
	//	因此，e_phentsize和e_phnum的乘机就是表的大小(以字节计数).
	//	假如没有程序头表（program header table），e_phnum变量为0。

	Elf32_Half          e_shentsize;
	//	该成员保存着section头的大小(以字节计数)。
	//	一个section头是在section头表(section header table)的一个入口；
	//	所有的入口都是同样的大小。

	Elf32_Half          e_shnum;
	//	该成员保存着在section header table中的入口数目。
	//	因此，e_shentsize和e_shnum的乘积就是section头表的大小(以字节计数)。
	//	假如文件没有section头表，e_shnum值为0。

	Elf32_Half          e_shstrndx;
	//	该成员保存着跟section名字字符表相关入口的section头表(section header table)索引。
	//	假如文件中没有section名字字符表，该变量值为SHN_UNDEF。
	//	更详细的信息 看下面“Sections”和字符串表(“String Table”) 。

} Elf32_Ehdr;

typedef struct {
	unsigned char   e_ident[EI_NIDENT]; 
	Elf64_Half      e_type;
	Elf64_Half      e_machine;
	Elf64_Word      e_version;
	Elf64_Addr      e_entry;
	Elf64_Off       e_phoff;
	Elf64_Off       e_shoff;
	Elf64_Word      e_flags;
	Elf64_Half      e_ehsize;
	Elf64_Half      e_phentsize;
	Elf64_Half      e_phnum;
	Elf64_Half      e_shentsize;
	Elf64_Half      e_shnum;
	Elf64_Half      e_shstrndx;
} Elf64_Ehdr;



//e_ident[]
enum{
	EI_MAG0,       //0x7f
	EI_MAG1,       //'E'
	EI_MAG2,       //'L'
	EI_MAG3,       //'F'
	EI_CLASS,      //文件类
	EI_DATA,       //数据编码
	EI_VERSION,    //文件版本  必须为EV_CURRENT
	EI_OSABI,	   //操作系统/ABI标识
	EI_ABIVERSION, //ABI版本
	EI_PAD,        //填充字节的开头 e_ident中未使用字节的起始位置
	EI_NIDENT1 = 16  //e_ident[] 的大小
};
//e_ident[EI_CLASS] 用于标识文件的类或容量
enum{
	ELFCLASSNONE,  //无效类
	ELFCLASS32,    //32位目标文件
	ELFCLASS64     //64位目标文件
};
//e_ident[EI_DATA]用于指定目标文件中特定于处理器的数据的数据编码
enum{
	ELFDATANONE,
	ELFDATA2LSB, //编码用于指定2的补码，其中最低有效字节占用最低地址
	ELFDATA2MSB  //编码用于指定2的补码，其中最高有效字节占用最低地址
};
//e_type
enum{
	ET_NONE,//无文件类型
	ET_REL, //可重定位文件
	ET_EXEC,//可执行文件
	ET_DYN, //共享库文件
	ET_CORE,//核心转储文件
	ET_LOOS  =0xfe00,
	ET_HIOS  =0xfeff,
	ET_LOPROC=0xff00, //特定于处理器
	ET_HIPROC=0xffff, //特定于处理器
};

//e_machine
enum {    // list has to to be completed
	EM_NONE         = 0,    //No machine
	EM_M32          = 1,    //AT&T WE 32100
	EM_SPARC        = 2,    //SPARC
	EM_386          = 3,    //Intel 80386
	EM_68K          = 4,    //Motorola 68000
	EM_88K          = 5,    //Motorola 88000
	reserved6       = 6,    //Reserved for future use (was EM_486)
	EM_860          = 7,    //Intel 80860
	EM_MIPS         = 8,    //MIPS I Architecture
	EM_S370         = 9,    //IBM System/370 Processor
	EM_MIPS_RS3_LE  = 10,    //MIPS RS3000 Little-endian
	reserved11      = 11,    //Reserved for future use
	reserved12      = 12,    //Reserved for future use
	reserved13      = 13,    //Reserved for future use
	reserved14      = 14,    //Reserved for future use
	EM_PARISC       = 15,    //Hewlett-Packard PA-RISC
	reserved16      = 16,    //Reserved for future use
	EM_VPP500       = 17,    //Fujitsu VPP500
	EM_SPARC32PLUS  = 18,    //Enhanced instruction set SPARC
	EM_960          = 19,    //Intel 80960
	EM_PPC          = 20,    //PowerPC
	EM_PPC64        = 21,    //64-bit PowerPC
	EM_S390         = 22,    //IBM System/390 Processor
	reserved23      = 23,    //Reserved for future use
	reserved24      = 24,    //Reserved for future use
	reserved25      = 25,    //Reserved for future use
	reserved26      = 26,    //Reserved for future use
	reserved27      = 27,    //Reserved for future use
	reserved28      = 28,    //Reserved for future use
	reserved29      = 29,    //Reserved for future use
	reserved30      = 30,    //Reserved for future use
	reserved31      = 31,    //Reserved for future use
	reserved32      = 32,    //Reserved for future use
	reserved33      = 33,    //Reserved for future use
	reserved34      = 34,    //Reserved for future use
	reserved35      = 35,    //Reserved for future use
	EM_V800         = 36,    //NEC V800
	EM_FR20         = 37,    //Fujitsu FR20
	EM_RH32         = 38,    //TRW RH-32
	EM_RCE          = 39,    //Motorola RCE
	EM_ARM          = 40,    //Advanced RISC Machines ARM
	EM_ALPHA        = 41,    //Digital Alpha
	EM_SH           = 42,    //Hitachi SH
	EM_SPARCV9      = 43,    //SPARC Version 9
	EM_TRICORE      = 44,    //Siemens TriCore embedded processor
	EM_ARC          = 45,    //Argonaut RISC Core, Argonaut Technologies Inc.
	EM_H8_300       = 46,    //Hitachi H8/300
	EM_H8_300H      = 47,    //Hitachi H8/300H
	EM_H8S          = 48,    //Hitachi H8S
	EM_H8_500       = 49,    //Hitachi H8/500
	EM_IA_64        = 50,    //Intel IA-64 processor architecture
	EM_MIPS_X       = 51,    //Stanford MIPS-X
	EM_COLDFIRE     = 52,    //Motorola ColdFire
	EM_68HC12       = 53,    //Motorola M68HC12
	EM_MMA          = 54,    //Fujitsu MMA Multimedia Accelerator
	EM_PCP          = 55,    //Siemens PCP
	EM_NCPU         = 56,    //Sony nCPU embedded RISC processor
	EM_NDR1         = 57,    //Denso NDR1 microprocessor
	EM_STARCORE     = 58,    //Motorola Star*Core processor
	EM_ME16         = 59,    //Toyota ME16 processor
	EM_ST100        = 60,    //STMicroelectronics ST100 processor
	EM_TINYJ        = 61,    //Advanced Logic Corp. TinyJ embedded processor family
	EM_X86_64       = 62,    //AMD x86-64 architecture
	EM_PDSP         = 63,    //Sony DSP Processor
	EM_PDP10        = 64,    //Digital Equipment Corp. PDP-10
	EM_PDP11        = 65,    //Digital Equipment Corp. PDP-11
	EM_FX66         = 66,    //Siemens FX66 microcontroller
	EM_ST9PLUS      = 67,    //STMicroelectronics ST9+ 8/16 bit microcontroller
	EM_ST7          = 68,    //STMicroelectronics ST7 8-bit microcontroller
	EM_68HC16       = 69,    //Motorola MC68HC16 Microcontroller
	EM_68HC11       = 70,    //Motorola MC68HC11 Microcontroller
	EM_68HC08       = 71,    //Motorola MC68HC08 Microcontroller
	EM_68HC05       = 72,    //Motorola MC68HC05 Microcontroller
	EM_SVX          = 73,    //Silicon Graphics SVx
	EM_ST19         = 75,    //Digital VAX
	EM_CRIS         = 76,    //Axis Communications 32-bit embedded processor
	EM_JAVELIN      = 77,    //Infineon Technologies 32-bit embedded processor
	EM_FIREPATH     = 78,    //Element 14 64-bit DSP Processor
	EM_ZSP          = 79,    //LSI Logic 16-bit DSP Processor
	EM_MMIX         = 80,    //Donald Knuth's educational 64-bit processor
	EM_HUANY        = 81,    //Harvard University machine-independent object files
	EM_PRISM        = 82,    //SiTera Prism
	EM_AVR          = 83,    //Atmel AVR 8-bit microcontroller
	EM_FR30         = 84,    //Fujitsu FR30
	EM_D10V         = 85,    //Mitsubishi D10V
	EM_D30V         = 86,    //Mitsubishi D30V
	EM_V850         = 87,    //NEC v850
	EM_M32R         = 88,    //Mitsubishi M32R
	EM_MN10300      = 89,    //Matsushita MN10300
	EM_MN10200      = 90,    //Matsushita MN10200
	EM_PJ           = 91,    //picoJava
	EM_OPENRISC     = 92,    //OpenRISC 32-bit embedded processor
	EM_ARC_A5       = 93,    //ARC Cores Tangent-A5
	EM_XTENSA       = 94,    //Tensilica Xtensa Architecture
	EM_VIDEOCORE    = 95,    //Alphamosaic VideoCore processor
	EM_TMM_GPP      = 96,    //Thompson Multimedia General Purpose Processor
	EM_NS32K        = 97,    //National Semiconductor 32000 series
	EM_TPC          = 98,    //Tenor Network TPC processor
	EM_SNP1K        = 99,    //Trebia SNP 1000 processor
	EM_ST200        = 100,    //STMicroelectronics (www.st.com) ST200 microcontroller
	EM_IP2K         = 101,    //Ubicom IP2xxx microcontroller family
	EM_MAX          = 102,    //MAX Processor
	EM_CR           = 103,    //National Semiconductor CompactRISC microprocessor
	EM_F2MC16       = 104,    //Fujitsu F2MC16
	EM_MSP430       = 105,    //Texas Instruments embedded microcontroller msp430
	EM_BLACKFIN     = 106,    //Analog Devices Blackfin (DSP) processor
	EM_SE_C33       = 107,    //S1C33 Family of Seiko Epson processors
	EM_SEP          = 108,    //Sharp embedded microprocessor
	EM_ARCA         = 109,    //Arca RISC Microprocessor
	EM_UNICORE      = 110    //Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University
};

//version
enum{
	EV_NONE, //无效版本
	EV_CURRENT //当前版本  >=0
};


// Program Header		
typedef struct {
	Elf32_Word p_type;
	//	Name             Value
	//	====             =====
	//	PT_NULL              0
	//	PT_LOAD              1
	//	PT_DYNAMIC           2
	//	PT_INTERP            3
	//	PT_NOTE              4
	//	PT_SHLIB             5
	//	PT_PHDR              6
	//	PT_LOPROC   0x70000000
	//	PT_HIPROC   0x7fffffff

	Elf32_Off  p_offset;
	//	该成员给出了该段的驻留位置相对于文件开始处的偏移。

	Elf32_Addr p_vaddr;
	//	该成员给出了该段在内存中的首字节地址。

	Elf32_Addr p_paddr;

	Elf32_Word p_filesz;
	//	文件映像中该段的字节数；它可能是 0 。

	Elf32_Word p_memsz;
	//	内存映像中该段的字节数；它可能是 0 。

	Elf32_Word p_flags;

	Elf32_Word p_align;
	//	该成员给出了该段在内存和文件中排列值。
	//	0 和 1 表示不需要排列。否则， p_align 必须为正的 2 的幂，
	//	并且 p_vaddr 应当等于 p_offset 模 p_align 。


}Elf32_Phdr;

typedef struct {
	Elf64_Word      p_type;
	Elf64_Word      p_flags;
	Elf64_Off       p_offset;
	Elf64_Addr      p_vaddr;
	Elf64_Addr      p_paddr;
	Elf64_Xword     p_filesz;
	Elf64_Xword     p_memsz;
	Elf64_Xword     p_align;
} Elf64_Phdr;


// Section Header
typedef struct {
	Elf32_Word sh_name;
	//	该成员指定了这个section的名字。
	//	它的值是section报头字符表section的索引。[看以下的“String Table”], 以NULL空字符结束。

	Elf32_Word sh_type;
	//	Section Types, sh_type
	//	---------------------------
	//	Name                 Value    Description
	//	====                 =====    ===========
	//	SHT_NULL				 0    该值表明该section头是无效的；它没有相关的section。
	//	SHT_PROGBITS			 1    该section保存被程序定义了的一些信息，它的格式和意义取决于程序本身。
	//	SHT_SYMTAB				 2    该sections保存着一个符号表（symbol table）。
	//	SHT_STRTAB				 3    该section保存着一个字符串表。
	//	SHT_RELA				 4    该section保存着具有明确加数的重定位入口。
	//	SHT_HASH				 5    该标号保存着一个标号的哈希(hash)表。
	//	SHT_DYNAMIC				 6    该section保存着动态连接的信息。
	//	SHT_NOTE				 7    该section保存着其他的一些标志文件的信息。
	//	SHT_NOBITS				 8    该类型的section在文件中不占空间，但是类似SHT_PROGBITS。
	//	SHT_REL					 9    该section保存着重定位的入口。
	//	SHT_SHLIB				10    该section类型保留但语意没有指明。包含这个类型的section的程序是不符合ABI的。
	//	SHT_DYNSYM				11    该sections保存着一个符号表（symbol table）。
	//	SHT_LOPROC		0x70000000    在这范围之间的值为特定处理器语意保留的。
	//	SHT_HIPROC		0x7fffffff    在这范围之间的值为特定处理器语意保留的。
	//	SHT_LOUSER		0x80000000    该变量为应用程序保留的索引范围的最小边界。
	//	SHT_HIUSER		0xffffffff    该变量为应用程序保留的索引范围的最大边界。


	Elf32_Word sh_flags;
	//	Section Attribute Flags, sh_flags
	//	-----------------------------------		
	//	Name                Value    Description
	//	====                =====    ===========
	//	SHF_WRITE             0x1    该section包含了在进程执行过程中可被写的数据。
	//	SHF_ALLOC             0x2    该section在进程执行过程中占据着内存。
	//	SHF_EXECINSTR         0x4    该section包含了可执行的机器指令。
	//	SHF_MASKPROC   0xf0000000    所有的包括在这掩码中的位为特定处理语意保留的。

	Elf32_Addr sh_addr;
	//	假如该section将出现在进程的内存映象空间里，该成员给出了一个该section在内存中的位置。否则，该变量为0。

	Elf32_Off  sh_offset;
	//	该成员变量给出了该section的字节偏移量(从文件开始计数)。

	Elf32_Word sh_size;
	//	该成员给你了section的字节大小。

	Elf32_Word sh_link;
	//	该成员保存了一个section报头表的索引连接，它的解释依靠该section的类型。
	//	更多信息参见表"sh_link and sh_info Interpretation"

	Elf32_Word sh_info;
	//	该成员保存着额外的信息，它的解释依靠该section的类型。

	//	sh_link and sh_info Interpretation

	//	-------------------------------------------------------------------------------
	//	sh_type        sh_link                          sh_info
	//	=======        =======                          =======
	//	SHT_DYNAMIC    The section header index of      0
	//	               the string table used by
	//	               entries in the section.
	//	-------------------------------------------------------------------------------
	//	SHT_HASH       The section header index of      0
	//	               the symbol table to which the
	//	               hash table applies.
	//	-------------------------------------------------------------------------------
	//	SHT_REL        The section header index of      The section header index of
	//	SHT_RELA       the associated symbol table.     the section to which the
	//	                                                relocation applies.
	//	-------------------------------------------------------------------------------
	//	SHT_SYMTAB     The section header index of      One greater than the symbol
	//	-------------------------------------------------------------------------------
	//	SHT_DYNSYM     the associated string table.     table index of the last local
	//	               symbol (binding STB_LOCAL).
	//	-------------------------------------------------------------------------------
	//	other          SHN_UNDEF                        0
	//	-------------------------------------------------------------------------------


	Elf32_Word sh_addralign;
	//	一些sections有地址对齐的约束。

	Elf32_Word sh_entsize;
	//	一些sections保存着一张固定大小入口的表，就象符号表。
	//	对于这样一个section来说，该成员给出了每个入口的字节大小。
	//	如果该section没有保存着一张固定大小入口的表，该成员就为0。
} Elf32_Shdr;

typedef struct {
	Elf64_Word      sh_name;
	Elf64_Word      sh_type;
	Elf64_Xword     sh_flags;
	Elf64_Addr      sh_addr;
	Elf64_Off       sh_offset;
	Elf64_Xword     sh_size;
	Elf64_Word      sh_link;
	Elf64_Word      sh_info;
	Elf64_Xword     sh_addralign;
	Elf64_Xword     sh_entsize;
} Elf64_Shdr;



typedef struct {
	Elf32_Word      st_name; //字符串表索引
	Elf32_Addr      st_value;//关联符号的值
	Elf32_Word      st_size; //关联大小
	unsigned char   st_info; //符号的类型和绑定属性
	unsigned char   st_other;//符号的可见性
	Elf32_Half      st_shndx;//相关节头表索引
} Elf32_Sym;


typedef struct {
	Elf64_Word      st_name;
	unsigned char   st_info;
	unsigned char   st_other;
	Elf64_Half      st_shndx;
	Elf64_Addr      st_value;
	Elf64_Xword     st_size;
} Elf64_Sym;



enum st_shndx{
	SHN_UNDEF,//未定义、缺少、无关或无意义的节引用。例如，已定义的与节数 SHN_UNDEF 有关的符号即是未定义符号。
	SHN_LORESERVE = 0xff00,//所保留索引的范围的下边界。
	SHN_LOPROC = 0xff00,//-SHN_HIPROC  此范围内包含的值保留用于特定于处理器的语义。
	SHN_BEFORE = 0xff00,//与 SHF_LINK_ORDER 和 SHF_ORDERED 节标志一起用于初始和最终节排序
	SHN_AFTER = 0xff01,//与 SHF_LINK_ORDER 和 SHF_ORDERED 节标志一起用于初始和最终节排序
	SHN_AMD64_LCOMMON = 0xff02,//特定于 x64 的通用块标签。此标签与 SHN_COMMON 类似，但用于标识较大的通用块。
	SHN_HIPROC = 0xff1f,
	SHN_LOOS = 0xff20,//-SHN_HIOS  此范围内包含的值保留用于特定于操作系统的语义。
	SHN_LOSUNW = 0xff3f,  //-SHN_HISUNW 此范围内包含的值保留用于特定于 Sun 的语义。
	SHN_SUNW_IGNORE = 0xff3f,//此节索引用于在可重定位目标文件中提供临时符号定义。保留供 dtrace(1M) 内部使用。
	SHN_HISUNW = 0xff3f,
	SHN_HIOS = 0xff3f,
	SHN_ABS = 0xfff1,//对应引用的绝对值。例如，已定义的与节数 SHN_ABS 相关的符号具有绝对值，并且不受重定位影响。
	SHN_COMMON = 0xfff2,//已定义的与此节相关的符号为通用符号，如 FORTRAN COMMON 或未分配的 C 外部变量。这些符号有时称为暂定符号。
	SHN_XINDEX = 0xffff,//转义值，用于表示实际节头索引过大，以致无法放入包含字段。节头索引可在特定于显示节索引的结构的其他位置中找到。
	SHN_HIRESERVE = 0xffff//所保留索引的范围的上边界。系统保留了 SHN_LORESERVE 和 SHN_HIRESERVE 之间的索引（包括这两个值）。这些值不会引用节头表。节头表不包含对应于所保留索引的项。
};


typedef struct {
	Elf32_Addr      r_offset;//对于可重定位文件，该值表示节偏移。
							 //对于可执行文件或共享库，该值表示受重定位影响的存储单元的虚拟地址。
							
	Elf32_Word      r_info;  //此成员指定必须对其进行重定位的符号表索引以及要应用的重定位类型
} Elf32_Rel;



typedef struct {
	Elf32_Addr      r_offset;
	Elf32_Word      r_info;
	Elf32_Sword     r_addend;//此成员指定常量加数，用于计算将存储在可重定位字段中的值
} Elf32_Rela;


typedef struct {
	Elf64_Addr      r_offset;
	Elf64_Xword     r_info;
} Elf64_Rel;


typedef struct {
	Elf64_Addr      r_offset;
	Elf64_Xword     r_info;
	Elf64_Sxword    r_addend;
} Elf64_Rela;


//32 位 SPARC 仅使用 Elf32_Rela 重定位项。 
//64 位 SPARC 和 64 位 x86 仅使用 Elf64_Rela 重定位项。
//x86 仅使用 Elf32_Rel 重定位项
typedef struct {

	Elf32_Sword d_tag;

	union {

		Elf32_Word      d_val;

		Elf32_Addr      d_ptr;

		Elf32_Off       d_off;

	} d_un;

} Elf32_Dyn;

typedef struct {

	Elf64_Xword d_tag;
	union {
		Elf64_Xword     d_val;
		Elf64_Addr      d_ptr;
	} d_un;
} Elf64_Dyn;