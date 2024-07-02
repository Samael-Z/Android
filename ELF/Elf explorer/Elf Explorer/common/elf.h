
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
typedef	DWORD	Elf32_Addr; //�޷��ų����ַ
typedef	WORD	Elf32_Half; //�޷���������
typedef	DWORD	Elf32_Off;  //�޷����ļ�ƫ��
typedef	DWORD	Elf32_Sword;//����������
typedef	DWORD	Elf32_Word; //�޷�������

typedef LONGLONG Elf64_Addr;  //�޷��ų����ַ
typedef WORD     Elf64_Half;  //�޷���������
typedef LONGLONG Elf64_Off;   //�޷����ļ�ƫ��
typedef DWORD	 Elf64_Sword; //����������
typedef DWORD	 Elf64_Word;  //�޷�������
typedef LONGLONG Elf64_Xword;
typedef LONGLONG Elf64_Sxword;

#define EI_NIDENT       16

// ELF header
typedef struct{
	unsigned char       e_ident[EI_NIDENT];
	Elf32_Half          e_type;
	//	e_type
	//
	//	�ó�Աȷ����object�����͡�
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
	//	�ó�Ա����ָ�������иó�����Ҫ����ϵ�ṹ��
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
	//	�����Աȷ��object�ļ��İ汾��
	//
	//	Name         Value  Meaning
	//	====         =====  =======
	//	EV_NONE          0  Invalid version
	//	EV_CURRENT       1  Current version

	Elf32_Addr          e_entry;
	//	�ó�Ա��ϵͳ��һ��������Ƶ������ַ�������������̡�
	//	�����ļ�û����ι�������ڵ㣬�ó�Ա�ͱ���Ϊ 0��

	Elf32_Off           e_phoff;
	//	�ó�Ա�����ų���ͷ��program header table�����ļ��е�ƫ����(���ֽڼ���)��
	//	������ļ�û�г���ͷ��ĵĻ����ó�Ա�ͱ���Ϊ 0��

	Elf32_Off           e_shoff;
	//	�ó�Ա������sectionͷ��section header table�����ļ��е�ƫ����(���ֽڼ���)��
	//	������ļ�û��sectionͷ��ĵĻ����ó�Ա�ͱ���Ϊ0��

	Elf32_Word          e_flags;
	//	�ó�Ա����������ļ����ض���������־��
	//	flag������������EF_<machine>_<flag>�����»�����Ϣ��Machine Information�����ֵ�flag�Ķ��塣

	Elf32_Half          e_ehsize;
	//	�ó�Ա������ELFͷ��С(���ֽڼ���)��

	Elf32_Half          e_phentsize;
	//	�ó�Ա���������ļ��ĳ���ͷ��program header table����һ����ڵĴ�С
	//	(���ֽڼ���)�����е���ڶ���ͬ���Ĵ�С��

	Elf32_Half          e_phnum;
	//	�ó�Ա�������ڳ���ͷ������ڵĸ�����
	//	��ˣ�e_phentsize��e_phnum�ĳ˻����Ǳ�Ĵ�С(���ֽڼ���).
	//	����û�г���ͷ��program header table����e_phnum����Ϊ0��

	Elf32_Half          e_shentsize;
	//	�ó�Ա������sectionͷ�Ĵ�С(���ֽڼ���)��
	//	һ��sectionͷ����sectionͷ��(section header table)��һ����ڣ�
	//	���е���ڶ���ͬ���Ĵ�С��

	Elf32_Half          e_shnum;
	//	�ó�Ա��������section header table�е������Ŀ��
	//	��ˣ�e_shentsize��e_shnum�ĳ˻�����sectionͷ��Ĵ�С(���ֽڼ���)��
	//	�����ļ�û��sectionͷ��e_shnumֵΪ0��

	Elf32_Half          e_shstrndx;
	//	�ó�Ա�����Ÿ�section�����ַ��������ڵ�sectionͷ��(section header table)������
	//	�����ļ���û��section�����ַ����ñ���ֵΪSHN_UNDEF��
	//	����ϸ����Ϣ �����桰Sections�����ַ�����(��String Table��) ��

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
	EI_CLASS,      //�ļ���
	EI_DATA,       //���ݱ���
	EI_VERSION,    //�ļ��汾  ����ΪEV_CURRENT
	EI_OSABI,	   //����ϵͳ/ABI��ʶ
	EI_ABIVERSION, //ABI�汾
	EI_PAD,        //����ֽڵĿ�ͷ e_ident��δʹ���ֽڵ���ʼλ��
	EI_NIDENT1 = 16  //e_ident[] �Ĵ�С
};
//e_ident[EI_CLASS] ���ڱ�ʶ�ļ����������
enum{
	ELFCLASSNONE,  //��Ч��
	ELFCLASS32,    //32λĿ���ļ�
	ELFCLASS64     //64λĿ���ļ�
};
//e_ident[EI_DATA]����ָ��Ŀ���ļ����ض��ڴ����������ݵ����ݱ���
enum{
	ELFDATANONE,
	ELFDATA2LSB, //��������ָ��2�Ĳ��룬���������Ч�ֽ�ռ����͵�ַ
	ELFDATA2MSB  //��������ָ��2�Ĳ��룬���������Ч�ֽ�ռ����͵�ַ
};
//e_type
enum{
	ET_NONE,//���ļ�����
	ET_REL, //���ض�λ�ļ�
	ET_EXEC,//��ִ���ļ�
	ET_DYN, //������ļ�
	ET_CORE,//����ת���ļ�
	ET_LOOS  =0xfe00,
	ET_HIOS  =0xfeff,
	ET_LOPROC=0xff00, //�ض��ڴ�����
	ET_HIPROC=0xffff, //�ض��ڴ�����
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
	EV_NONE, //��Ч�汾
	EV_CURRENT //��ǰ�汾  >=0
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
	//	�ó�Ա�����˸öε�פ��λ��������ļ���ʼ����ƫ�ơ�

	Elf32_Addr p_vaddr;
	//	�ó�Ա�����˸ö����ڴ��е����ֽڵ�ַ��

	Elf32_Addr p_paddr;

	Elf32_Word p_filesz;
	//	�ļ�ӳ���иöε��ֽ������������� 0 ��

	Elf32_Word p_memsz;
	//	�ڴ�ӳ���иöε��ֽ������������� 0 ��

	Elf32_Word p_flags;

	Elf32_Word p_align;
	//	�ó�Ա�����˸ö����ڴ���ļ�������ֵ��
	//	0 �� 1 ��ʾ����Ҫ���С����� p_align ����Ϊ���� 2 ���ݣ�
	//	���� p_vaddr Ӧ������ p_offset ģ p_align ��


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
	//	�ó�Աָ�������section�����֡�
	//	����ֵ��section��ͷ�ַ���section��������[�����µġ�String Table��], ��NULL���ַ�������

	Elf32_Word sh_type;
	//	Section Types, sh_type
	//	---------------------------
	//	Name                 Value    Description
	//	====                 =====    ===========
	//	SHT_NULL				 0    ��ֵ������sectionͷ����Ч�ģ���û����ص�section��
	//	SHT_PROGBITS			 1    ��section���汻�������˵�һЩ��Ϣ�����ĸ�ʽ������ȡ���ڳ�����
	//	SHT_SYMTAB				 2    ��sections������һ�����ű�symbol table����
	//	SHT_STRTAB				 3    ��section������һ���ַ�����
	//	SHT_RELA				 4    ��section�����ž�����ȷ�������ض�λ��ڡ�
	//	SHT_HASH				 5    �ñ�ű�����һ����ŵĹ�ϣ(hash)��
	//	SHT_DYNAMIC				 6    ��section�����Ŷ�̬���ӵ���Ϣ��
	//	SHT_NOTE				 7    ��section������������һЩ��־�ļ�����Ϣ��
	//	SHT_NOBITS				 8    �����͵�section���ļ��в�ռ�ռ䣬��������SHT_PROGBITS��
	//	SHT_REL					 9    ��section�������ض�λ����ڡ�
	//	SHT_SHLIB				10    ��section���ͱ���������û��ָ��������������͵�section�ĳ����ǲ�����ABI�ġ�
	//	SHT_DYNSYM				11    ��sections������һ�����ű�symbol table����
	//	SHT_LOPROC		0x70000000    ���ⷶΧ֮���ֵΪ�ض����������Ᵽ���ġ�
	//	SHT_HIPROC		0x7fffffff    ���ⷶΧ֮���ֵΪ�ض����������Ᵽ���ġ�
	//	SHT_LOUSER		0x80000000    �ñ���ΪӦ�ó�������������Χ����С�߽硣
	//	SHT_HIUSER		0xffffffff    �ñ���ΪӦ�ó�������������Χ�����߽硣


	Elf32_Word sh_flags;
	//	Section Attribute Flags, sh_flags
	//	-----------------------------------		
	//	Name                Value    Description
	//	====                =====    ===========
	//	SHF_WRITE             0x1    ��section�������ڽ���ִ�й����пɱ�д�����ݡ�
	//	SHF_ALLOC             0x2    ��section�ڽ���ִ�й�����ռ�����ڴ档
	//	SHF_EXECINSTR         0x4    ��section�����˿�ִ�еĻ���ָ�
	//	SHF_MASKPROC   0xf0000000    ���еİ������������е�λΪ�ض��������Ᵽ���ġ�

	Elf32_Addr sh_addr;
	//	�����section�������ڽ��̵��ڴ�ӳ��ռ���ó�Ա������һ����section���ڴ��е�λ�á����򣬸ñ���Ϊ0��

	Elf32_Off  sh_offset;
	//	�ó�Ա���������˸�section���ֽ�ƫ����(���ļ���ʼ����)��

	Elf32_Word sh_size;
	//	�ó�Ա������section���ֽڴ�С��

	Elf32_Word sh_link;
	//	�ó�Ա������һ��section��ͷ����������ӣ����Ľ���������section�����͡�
	//	������Ϣ�μ���"sh_link and sh_info Interpretation"

	Elf32_Word sh_info;
	//	�ó�Ա�����Ŷ������Ϣ�����Ľ���������section�����͡�

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
	//	һЩsections�е�ַ�����Լ����

	Elf32_Word sh_entsize;
	//	һЩsections������һ�Ź̶���С��ڵı�������ű�
	//	��������һ��section��˵���ó�Ա������ÿ����ڵ��ֽڴ�С��
	//	�����sectionû�б�����һ�Ź̶���С��ڵı��ó�Ա��Ϊ0��
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
	Elf32_Word      st_name; //�ַ���������
	Elf32_Addr      st_value;//�������ŵ�ֵ
	Elf32_Word      st_size; //������С
	unsigned char   st_info; //���ŵ����ͺͰ�����
	unsigned char   st_other;//���ŵĿɼ���
	Elf32_Half      st_shndx;//��ؽ�ͷ������
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
	SHN_UNDEF,//δ���塢ȱ�١��޹ػ�������Ľ����á����磬�Ѷ��������� SHN_UNDEF �йصķ��ż���δ������š�
	SHN_LORESERVE = 0xff00,//�����������ķ�Χ���±߽硣
	SHN_LOPROC = 0xff00,//-SHN_HIPROC  �˷�Χ�ڰ�����ֵ���������ض��ڴ����������塣
	SHN_BEFORE = 0xff00,//�� SHF_LINK_ORDER �� SHF_ORDERED �ڱ�־һ�����ڳ�ʼ�����ս�����
	SHN_AFTER = 0xff01,//�� SHF_LINK_ORDER �� SHF_ORDERED �ڱ�־һ�����ڳ�ʼ�����ս�����
	SHN_AMD64_LCOMMON = 0xff02,//�ض��� x64 ��ͨ�ÿ��ǩ���˱�ǩ�� SHN_COMMON ���ƣ������ڱ�ʶ�ϴ��ͨ�ÿ顣
	SHN_HIPROC = 0xff1f,
	SHN_LOOS = 0xff20,//-SHN_HIOS  �˷�Χ�ڰ�����ֵ���������ض��ڲ���ϵͳ�����塣
	SHN_LOSUNW = 0xff3f,  //-SHN_HISUNW �˷�Χ�ڰ�����ֵ���������ض��� Sun �����塣
	SHN_SUNW_IGNORE = 0xff3f,//�˽����������ڿ��ض�λĿ���ļ����ṩ��ʱ���Ŷ��塣������ dtrace(1M) �ڲ�ʹ�á�
	SHN_HISUNW = 0xff3f,
	SHN_HIOS = 0xff3f,
	SHN_ABS = 0xfff1,//��Ӧ���õľ���ֵ�����磬�Ѷ��������� SHN_ABS ��صķ��ž��о���ֵ�����Ҳ����ض�λӰ�졣
	SHN_COMMON = 0xfff2,//�Ѷ������˽���صķ���Ϊͨ�÷��ţ��� FORTRAN COMMON ��δ����� C �ⲿ��������Щ������ʱ��Ϊ�ݶ����š�
	SHN_XINDEX = 0xffff,//ת��ֵ�����ڱ�ʾʵ�ʽ�ͷ�������������޷���������ֶΡ���ͷ���������ض�����ʾ�������Ľṹ������λ�����ҵ���
	SHN_HIRESERVE = 0xffff//�����������ķ�Χ���ϱ߽硣ϵͳ������ SHN_LORESERVE �� SHN_HIRESERVE ֮�������������������ֵ������Щֵ�������ý�ͷ����ͷ��������Ӧ���������������
};


typedef struct {
	Elf32_Addr      r_offset;//���ڿ��ض�λ�ļ�����ֵ��ʾ��ƫ�ơ�
							 //���ڿ�ִ���ļ�����⣬��ֵ��ʾ���ض�λӰ��Ĵ洢��Ԫ�������ַ��
							
	Elf32_Word      r_info;  //�˳�Աָ�������������ض�λ�ķ��ű������Լ�ҪӦ�õ��ض�λ����
} Elf32_Rel;



typedef struct {
	Elf32_Addr      r_offset;
	Elf32_Word      r_info;
	Elf32_Sword     r_addend;//�˳�Աָ���������������ڼ��㽫�洢�ڿ��ض�λ�ֶ��е�ֵ
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


//32 λ SPARC ��ʹ�� Elf32_Rela �ض�λ� 
//64 λ SPARC �� 64 λ x86 ��ʹ�� Elf64_Rela �ض�λ�
//x86 ��ʹ�� Elf32_Rel �ض�λ��
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