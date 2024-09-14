//
// Created by 69182 on 2024-09-12.
//

#pragma once

#include "soinfo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <unistd.h>
#include <linux/prctl.h>   /* PR_*常量的定义*/
#include <sys/prctl.h>
using namespace std;


#define PR_SET_VMA   0x53564d41
#define PR_SET_VMA_ANON_NAME    0
#define FLAG_LINKER           0x00000010 // The linker itself
#define FLAG_GNU_HASH         0x00000040 // uses gnu hash
#define SUPPORTED_DT_FLAGS_1 (DF_1_NOW | DF_1_GLOBAL | DF_1_NODELETE | DF_1_PIE)
#define R_GENERIC_NONE 0
#define R_GENERIC_JUMP_SLOT R_AARCH64_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_AARCH64_GLOB_DAT
#define R_GENERIC_RELATIVE  R_AARCH64_RELATIVE
#define R_GENERIC_IRELATIVE R_AARCH64_IRELATIVE
#define R_AARCH64_TLS_TPREL64           1030
#define R_AARCH64_TLS_DTPREL32          1031


/** Returns the lesser of its two arguments. */
#define MIN(a,b) (((a)<(b))?(a):(b))
/** Returns the greater of its two arguments. */
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PAGE_START(x) ((x) & PAGE_MASK)
#define PAGE_END(x) PAGE_START((x) + (PAGE_SIZE-1))
#define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)
#define MAYBE_MAP_FLAG(x, from, to)  (((x) & (from)) ? (to) : 0)
#define PFLAGS_TO_PROT(x)            (MAYBE_MAP_FLAG((x), PF_X, PROT_EXEC) | \
                                      MAYBE_MAP_FLAG((x), PF_R, PROT_READ) | \
                                      MAYBE_MAP_FLAG((x), PF_W, PROT_WRITE))
#define powerof2(x) ((((x)-1)&(x))==0)



#if defined(__LP64__)
#define ELFW(what) ELF64_ ## what
#else
#define ELFW(what) ELF32_ ## what
#endif



class soinfo;
constexpr off64_t  kPageMask = ~static_cast<off64_t>(PAGE_SIZE -1);

typedef void(*linker_ctor_function_t)(int , char** , char**);
typedef void(*linker_dtor_function_t)();

class Utils{
public:
    static size_t page_offset(off64_t offset);
    static off64_t page_start(off64_t offset);
    static bool safe_add(off64_t* out , off64_t a, size_t b );
    static  void* getMapData(int fd, off64_t base_offset, size_t elf_offset, size_t size);
    static void phdr_table_get_dynamic_section(const ElfW(Phdr)* phdr_table,size_t phdr_count,ElfW(Addr) load_bias , ElfW(Dyn)** dynamic,
                                                ElfW(Word)* dynamic_flags);

    static soinfo* get_soinfo(const char* so_name);
    static ElfW(Addr) call_ifunc_resolver(ElfW(Addr) resolver_addr);
    static ElfW(Addr) get_addend(ElfW(Rela)* rela, ElfW(Addr) reloc_addr __unused);
    static ElfW(Addr) get_export_func(char* path,char* func_name);

    static int phdr_table_protect_gnu_relro(const ElfW(Phdr)* phdr_table, size_t phdr_count, ElfW(Addr) load_bias,int prot_flags);
    static bool __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                  ElfW(Addr) load_bias, TlsSegment* out);
    static bool __bionic_check_tls_alignment(size_t* alignment);

};




class plain_reloc_iterator {
#if defined(USE_RELA)
    typedef ElfW(Rela) rel_t;
#else
    typedef ElfW(Rel) rel_t;
#endif
public:
    plain_reloc_iterator(rel_t* rel_array, size_t count)
            : begin_(rel_array), end_(begin_ + count), current_(begin_) {}

    bool has_next() {
        return current_ < end_;
    }

    rel_t* next() {
        return current_++;
    }
private:
    rel_t* const begin_;
    rel_t* const end_;
    rel_t* current_;

    DISALLOW_COPY_AND_ASSIGN(plain_reloc_iterator);
};


//myloader定义
class MyLoader{
private:
    int fd_;
    off64_t file_offset_;
    off64_t file_size_;
    ElfW(Ehdr) header_;
    size_t phdr_num_;
    const ElfW(Phdr)* phdr_table_;
    size_t shdr_num_;
    const ElfW(Shdr)* shdr_table_;
    const ElfW(Dyn)* dynamic_;
    const char* strtab_;
    size_t strtab_size_;
    string name_;
    void* load_start_;
    size_t load_size_;
    ElfW(Addr) load_bias_;
    void* start_addr_;
    const ElfW(Phdr)* loaded_phdr_;
    soinfo* si_;

public:
    MyLoader():fd_(-1),file_offset_(0),file_size_(0),phdr_num_(0),phdr_table_(nullptr),shdr_table_(nullptr),shdr_num_(0),dynamic_(nullptr),
                strtab_(nullptr),strtab_size_(0),load_start_(nullptr),load_size_(0){

    }
    size_t phdr_count() const { return phdr_num_;}

    ElfW(Addr) load_start() const { return load_size_; }

    size_t load_size() const {return load_size_; }

    ElfW(Addr) load_bias() const { return load_bias_; }

    const ElfW(Phdr)* loaded_phdr() const { return loaded_phdr_; }
    //off64_t  == long long
public:
    bool Read(const char* name , int fd , off64_t file_offset , off64_t file_size);

    bool  ReadElfHeader();

    bool ReadProgramHeader();
    bool ReadSectionHeaders();
    bool ReadDynamicSection();

    bool Load();

    bool ReserveAddressSpace();

    size_t phdr_table_get_load_size(const ElfW(Phdr)* phdr_table, size_t phdr_count, ElfW(Addr)* out_min_vaddr);

    bool LoadSegments();

    bool FindPhdr();

    bool CheckPhdr(ElfW(Addr) loaded);

    const char* get_string(ElfW(Word) index) const;


    void run(const char* path);

};