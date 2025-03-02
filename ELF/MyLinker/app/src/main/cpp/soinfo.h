//
// Created by 69182 on 2024-09-12.
//


#pragma once

#include <string>
#include <vector>
#include <android/dlext.h>
#include <link.h>

#include "include/linked_list.h"
#include "include/macros.h"
#include "include/linker_sleb128.h"
#include <stdio.h>
#include <dlfcn.h>

#define FLAG_NEW_SOINFO       0x40000000 // new soinfo format

#define SOINFO_VERSION 5

#if defined(__aarch64__) || defined(__x86_64__)
#define USE_RELA 1
#endif



struct soinfo;

class SoinfoListAllocator {
public:
    static LinkedListEntry<soinfo>* alloc();
    static void free(LinkedListEntry<soinfo>* entry);

private:
    // unconstructable
    DISALLOW_IMPLICIT_CONSTRUCTORS(SoinfoListAllocator);
};

class NamespaceListAllocator {
public:
    static LinkedListEntry<android_namespace_t>* alloc();
    static void free(LinkedListEntry<android_namespace_t>* entry);

private:
    // unconstructable
    DISALLOW_IMPLICIT_CONSTRUCTORS(NamespaceListAllocator);
};

typedef LinkedList<soinfo, SoinfoListAllocator> soinfo_list_t;
typedef LinkedList<android_namespace_t, NamespaceListAllocator> android_namespace_list_t;



typedef void (*linker_dtor_function_t)();
typedef void (*linker_ctor_function_t)(int, char**, char**);

class SymbolName {
public:
    explicit SymbolName(const char* name)
            : name_(name), has_elf_hash_(false), has_gnu_hash_(false),
              elf_hash_(0), gnu_hash_(0) { }

    const char* get_name() {
        return name_;
    }

    uint32_t elf_hash();
    uint32_t gnu_hash();

private:
    const char* name_;
    bool has_elf_hash_;
    bool has_gnu_hash_;
    uint32_t elf_hash_;
    uint32_t gnu_hash_;

    DISALLOW_IMPLICIT_CONSTRUCTORS(SymbolName);
};



struct version_info {
    constexpr version_info() : elf_hash(0), name(nullptr), target_si(nullptr) {}

    uint32_t elf_hash;
    const char* name;
    const soinfo* target_si;
};
struct TlsIndex {
    size_t module_id;
    size_t offset;
};

struct TlsDynamicResolverArg {
    size_t generation;
    TlsIndex index;
};


class VersionTracker;

struct TlsSegment {
    size_t size = 0;
    size_t alignment = 1;
    const void* init_ptr = "";    // Field is non-null even when init_size is 0.
    size_t init_size = 0;
};

static constexpr size_t kTlsUninitializedModuleId = 0;

struct soinfo_tls {
    TlsSegment segment;
    size_t module_id = kTlsUninitializedModuleId;
};

#if defined(__work_around_b_24465209__)
#define SOINFO_NAME_LEN 128
#endif


struct soinfo {
#if defined(__work_around_b_24465209__)
    private:
  char old_name_[SOINFO_NAME_LEN];
#endif
public:
    const ElfW(Phdr)* phdr;
    size_t phnum;
#if defined(__work_around_b_24465209__)
    ElfW(Addr) unused0; // DO NOT USE, maintained for compatibility.
#endif
    ElfW(Addr) base;
    size_t size;

#if defined(__work_around_b_24465209__)
    uint32_t unused1;  // DO NOT USE, maintained for compatibility.
#endif

    ElfW(Dyn)* dynamic;

#if defined(__work_around_b_24465209__)
    uint32_t unused2; // DO NOT USE, maintained for compatibility
  uint32_t unused3; // DO NOT USE, maintained for compatibility
#endif

    soinfo* next;
private:
    uint32_t flags_;

    const char* strtab_;
    ElfW(Sym)* symtab_;

    size_t nbucket_;
    size_t nchain_;
    uint32_t* bucket_;
    uint32_t* chain_;

#if defined(__mips__) || !defined(__LP64__)
    // This is only used by mips and mips64, but needs to be here for
    // all 32-bit architectures to preserve binary compatibility.
    ElfW(Addr)** plt_got_;
#endif

#if defined(USE_RELA)
    ElfW(Rela)* plt_rela_;
  size_t plt_rela_count_;

  ElfW(Rela)* rela_;
  size_t rela_count_;
#else
    ElfW(Rel)* plt_rel_;
    size_t plt_rel_count_;

    ElfW(Rel)* rel_;
    size_t rel_count_;
#endif

    linker_ctor_function_t* preinit_array_;
    size_t preinit_array_count_;

    linker_ctor_function_t* init_array_;
    size_t init_array_count_;
    linker_dtor_function_t* fini_array_;
    size_t fini_array_count_;

    linker_ctor_function_t init_func_;
    linker_dtor_function_t fini_func_;

#if defined(__arm__)
    public:
  // ARM EABI section used for stack unwinding.
  uint32_t* ARM_exidx;
  size_t ARM_exidx_count;
 private:
#elif defined(__mips__)
    uint32_t mips_symtabno_;
  uint32_t mips_local_gotno_;
  uint32_t mips_gotsym_;
  bool mips_relocate_got(const VersionTracker& version_tracker,
                         const soinfo_list_t& global_group,
                         const soinfo_list_t& local_group);
#if !defined(__LP64__)
  bool mips_check_and_adjust_fp_modes();
#endif
#endif
    size_t ref_count_;
public:
    link_map link_map_head;

    bool constructors_called;

    // When you read a virtual address from the ELF file, add this
    // value to get the corresponding address in the process' address space.
    ElfW(Addr) load_bias;

#if !defined(__LP64__)
    bool has_text_relocations;
#endif
    bool has_DT_SYMBOLIC;

public:
    soinfo(android_namespace_t* ns, const char* name, const struct stat* file_stat,
           off64_t file_offset, int rtld_flags);
    ~soinfo();

    void call_constructors();
    void call_destructors();
    void call_pre_init_constructors();
    bool prelink_image();
    bool link_image(const soinfo_list_t& global_group, const soinfo_list_t& local_group,
                    const android_dlextinfo* extinfo, size_t* relro_fd_offset);
    bool link_image();
    bool protect_relro();

    void add_child(soinfo* child);
    void remove_all_links();

    ino_t get_st_ino() const;
    dev_t get_st_dev() const;
    off64_t get_file_offset() const;

    uint32_t get_rtld_flags() const;
    uint32_t get_dt_flags_1() const;
    void set_dt_flags_1(uint32_t dt_flags_1);

    soinfo_list_t& get_children();
    const soinfo_list_t& get_children() const;

    soinfo_list_t& get_parents();

    bool find_symbol_by_name(SymbolName& symbol_name,
                             const version_info* vi,
                             const ElfW(Sym)** symbol) const;

    ElfW(Sym)* find_symbol_by_address(const void* addr);
    ElfW(Addr) resolve_symbol_address(const ElfW(Sym)* s) const;

    const char* get_string(ElfW(Word) index) const;
    bool can_unload() const;
    bool is_gnu_hash() const;

    bool inline has_min_version(uint32_t min_version __unused) const {
#if defined(__work_around_b_24465209__)
        return (flags_ & FLAG_NEW_SOINFO) != 0 && version_ >= min_version;
#else
        return true;
#endif
    }

    bool is_linked() const;
    bool is_linker() const;
    bool is_main_executable() const;

    void set_linked();
    void set_linker_flag();
    void set_main_executable();
    void set_nodelete();

    size_t increment_ref_count();
    size_t decrement_ref_count();
    size_t get_ref_count() const;

    soinfo* get_local_group_root() const;

    void set_soname(const char* soname);
    const char* get_soname() const;
    const char* get_realpath() const;
    const ElfW(Versym)* get_versym(size_t n) const;
    ElfW(Addr) get_verneed_ptr() const;
    size_t get_verneed_cnt() const;
    ElfW(Addr) get_verdef_ptr() const;
    size_t get_verdef_cnt() const;

    int get_target_sdk_version() const;

    void set_dt_runpath(const char *);
    const std::vector<std::string>& get_dt_runpath() const;
    android_namespace_t* get_primary_namespace();
    void add_secondary_namespace(android_namespace_t* secondary_ns);
    android_namespace_list_t& get_secondary_namespaces();

    soinfo_tls* get_tls() const;

    void set_mapped_by_caller(bool reserved_map);
    bool is_mapped_by_caller() const;

    uintptr_t get_handle() const;
    void generate_handle();
    void* to_handle();

private:
    bool is_image_linked() const;
    void set_image_linked();

    bool elf_lookup(SymbolName& symbol_name, const version_info* vi, uint32_t* symbol_index) const;
    ElfW(Sym)* elf_addr_lookup(const void* addr);
    bool gnu_lookup(SymbolName& symbol_name, const version_info* vi, uint32_t* symbol_index) const;
    ElfW(Sym)* gnu_addr_lookup(const void* addr);

    bool lookup_version_info(const VersionTracker& version_tracker, ElfW(Word) sym,
                             const char* sym_name, const version_info** vi);

    template<typename ElfRelIteratorT>
    bool relocate(const VersionTracker& version_tracker, ElfRelIteratorT&& rel_iterator,
                  const soinfo_list_t& global_group, const soinfo_list_t& local_group);

    template<typename ElfRelIteratorT>
    bool relocate( ElfRelIteratorT&& rel_iterator);

    bool relocate_relr();
    void apply_relr_reloc(ElfW(Addr) offset);

private:
    // This part of the structure is only available
    // when FLAG_NEW_SOINFO is set in this->flags.
    uint32_t version_;

    // version >= 0
    dev_t st_dev_;
    ino_t st_ino_;

    // dependency graph
    soinfo_list_t children_;
    soinfo_list_t parents_;

    // version >= 1
    off64_t file_offset_;
    uint32_t rtld_flags_;
    uint32_t dt_flags_1_;
    size_t strtab_size_;

    // version >= 2

    size_t gnu_nbucket_;
    uint32_t* gnu_bucket_;
    uint32_t* gnu_chain_;
    uint32_t gnu_maskwords_;
    uint32_t gnu_shift2_;
    ElfW(Addr)* gnu_bloom_filter_;

    soinfo* local_group_root_;

    uint8_t* android_relocs_;
    size_t android_relocs_size_;

    const char* soname_;
    std::string realpath_;

    const ElfW(Versym)* versym_;

    ElfW(Addr) verdef_ptr_;
    size_t verdef_cnt_;

    ElfW(Addr) verneed_ptr_;
    size_t verneed_cnt_;

    int target_sdk_version_;

    // version >= 3
    std::vector<std::string> dt_runpath_;
    android_namespace_t* primary_namespace_;
    android_namespace_list_t secondary_namespaces_;
    uintptr_t handle_;

    friend soinfo* get_libdl_info(const char* linker_path, const soinfo& linker_si);

    // version >= 4
    ElfW(Relr)* relr_;
    size_t relr_count_;

    // version >= 5
    std::unique_ptr<soinfo_tls> tls_;
    std::vector<TlsDynamicResolverArg> tlsdesc_args_;
};



// This function is used by dlvsym() to calculate hash of sym_ver
uint32_t calculate_elf_hash(const char* name);

const char* fix_dt_needed(const char* dt_needed, const char* sopath);

template<typename F>
void for_each_dt_needed(const soinfo* si, F action) {
    for (const ElfW(Dyn)* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
        if (d->d_tag == DT_NEEDED) {
            action(fix_dt_needed(si->get_string(d->d_un.d_val), si->get_realpath()));
        }
    }
}



