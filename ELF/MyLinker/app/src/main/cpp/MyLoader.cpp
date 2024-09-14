//
// Created by 69182 on 2024-09-12.
//

#include "MyLoader.h"
#include "soinfo.h"

int g_myneed[20];
uint32_t g_needed_count = 0;

void MyLoader::run(const char *path) {

    int fd;
    struct stat sb;

    fd = open(path,O_RDONLY );
    if (fd == -1){
        LOGE("打开文件失败  %s",path);
        return;
    }
    fstat(fd,&sb);
    start_addr_ = mmap(NULL,sb.st_size,PROT_READ | PROT_WRITE ,MAP_PRIVATE,fd ,0);
    if (NULL == start_addr_){

        LOGE("内存申请失败！");
        return;
    }
    //1. 读取so文件
    if (!Read(path,fd,0,sb.st_size)){
        LOGD("Read so Failed");
        munmap(start_addr_,sb.st_size);
        close(fd);
        return;

    }


    //2. 载入so
    if (!Load()){
        LOGD("Load so Failed");
        munmap(start_addr_,sb.st_size);
        close(fd);
        return;
    }
    //3. 使被加载的so有执行权限，否则再调用.init_array时会报错
    //mprotect((void*)load_bias_,sb.st_size, PROT_READ | PROT_WRITE | PROT_EXEC);


    //4. 预链接，主要处理.dynamic节
     si_->prelink_image();
    //5. 正式链接，在这里处理重定位信息
     si_->link_image();
    //6. 调用.init和.init_array
     si_->call_constructors();

    if (fd != -1){
        close(fd);
    }
}
bool Utils::__bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                              ElfW(Addr) load_bias, TlsSegment* out) {
    for (size_t i = 0; i < phdr_count; ++i) {
        const ElfW(Phdr)& phdr = phdr_table[i];
        if (phdr.p_type == PT_TLS) {
            *out = TlsSegment {
                    phdr.p_memsz,
                    phdr.p_align,
                    reinterpret_cast<void*>(load_bias + phdr.p_vaddr),
                    phdr.p_filesz,
            };
            return true;
        }
    }
    return false;
}
// Return true if the alignment of a TLS segment is a valid power-of-two. Also
// cap the alignment if it's too high.
bool Utils::__bionic_check_tls_alignment(size_t* alignment) {
    // N.B. The size does not need to be a multiple of the alignment. With
    // ld.bfd (or after using binutils' strip), the TLS segment's size isn't
    // rounded up.
    if (*alignment == 0 || !powerof2(*alignment)) {
        return false;
    }
    // Bionic only respects TLS alignment up to one page.
    *alignment = MIN(*alignment, PAGE_SIZE);
    return true;
}

//预链接，保存.dynamic节的信息保存在修正的so文件中
bool soinfo::prelink_image() {
    /* Extract dynamic section */
    ElfW(Word) dynamic_flags = 0;
    Utils::phdr_table_get_dynamic_section(phdr, phnum, load_bias, &dynamic, &dynamic_flags);

    /* We can't log anything until the linker is relocated */
    bool relocating_linker = (flags_ & FLAG_LINKER) != 0;
    if (!relocating_linker) {
        LOGE("[prelink_image ] [ Linking  ]" );
        //return false;
    }

    if (dynamic == nullptr) {
        if (!relocating_linker) {
            LOGE("[prelink_image] missing PT_DYNAMIC in ");
        }
        return false;
    } else {
        if (!relocating_linker) {
            LOGE("[prelink_image] dynamic = %p", dynamic);
        }
    }


    TlsSegment tls_segment;
    if (Utils::__bionic_get_tls_segment(phdr, phnum, load_bias, &tls_segment)) {
        if (!Utils::__bionic_check_tls_alignment(&tls_segment.alignment)) {
            if (!relocating_linker) {
                LOGE("[prelink_image ] TLS segment alignment in  2: %zu",
                        tls_segment.alignment);
            }
            //return false;
        }
        tls_ = std::make_unique<soinfo_tls>();
        tls_->segment = tls_segment;
    }

    // Extract useful information from dynamic section.
    // Note that: "Except for the DT_NULL element at the end of the array,
    // and the relative order of DT_NEEDED elements, entries may appear in any order."
    //
    // source: http://www.sco.com/developers/gabi/1998-04-29/ch5.dynamic.html
    uint32_t needed_count = 0;
    for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
        LOGE("d = %p, d[0](tag) = %p d[1](val) = %p",
              d, reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
        switch (d->d_tag) {
            case DT_SONAME:
                // this is parsed after we have strtab initialized (see below).
                break;

            case DT_HASH:
                nbucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[0];
                nchain_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[1];
                bucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr + 8);
                chain_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr + 8 + nbucket_ * 4);
                break;

            case DT_GNU_HASH:
                gnu_nbucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[0];
                // skip symndx
                gnu_maskwords_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[2];
                gnu_shift2_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[3];

                gnu_bloom_filter_ = reinterpret_cast<ElfW(Addr)*>(load_bias + d->d_un.d_ptr + 16);
                gnu_bucket_ = reinterpret_cast<uint32_t*>(gnu_bloom_filter_ + gnu_maskwords_);
                // amend chain for symndx = header[1]
                gnu_chain_ = gnu_bucket_ + gnu_nbucket_ -
                             reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[1];

                if (!powerof2(gnu_maskwords_)) {
                    LOGE("invalid maskwords for gnu_hash = 0x%x, in \"%s\" expecting power to two",
                           gnu_maskwords_, get_realpath());
                    return false;
                }
                --gnu_maskwords_;

                flags_ |= FLAG_GNU_HASH;
                break;

            case DT_STRTAB:
                strtab_ = reinterpret_cast<const char*>(load_bias + d->d_un.d_ptr);
                break;

            case DT_STRSZ:
                strtab_size_ = d->d_un.d_val;
                break;

            case DT_SYMTAB:
                symtab_ = reinterpret_cast<ElfW(Sym)*>(load_bias + d->d_un.d_ptr);
                break;

            case DT_SYMENT:
                if (d->d_un.d_val != sizeof(ElfW(Sym))) {
                    LOGE("invalid DT_SYMENT: %zd in \"%s\"",
                           static_cast<size_t>(d->d_un.d_val), get_realpath());
                    return false;
                }
                break;

            case DT_PLTREL:
#if defined(USE_RELA)
                if (d->d_un.d_val != DT_RELA) {
          LOGE("unsupported DT_PLTREL in \"%s\"; expected DT_RELA", get_realpath());
          return false;
        }
#else
                if (d->d_un.d_val != DT_REL) {
                    LOGE("unsupported DT_PLTREL in \"%s\"; expected DT_REL", get_realpath());
                    return false;
                }
#endif
                break;

            case DT_JMPREL:
#if defined(USE_RELA)
                plt_rela_ = reinterpret_cast<ElfW(Rela)*>(load_bias + d->d_un.d_ptr);
#else
                plt_rel_ = reinterpret_cast<ElfW(Rel)*>(load_bias + d->d_un.d_ptr);
#endif
                break;

            case DT_PLTRELSZ:
#if defined(USE_RELA)
                plt_rela_count_ = d->d_un.d_val / sizeof(ElfW(Rela));
#else
                plt_rel_count_ = d->d_un.d_val / sizeof(ElfW(Rel));
#endif
                break;

            case DT_PLTGOT:

                // Ignore for other platforms... (because RTLD_LAZY is not supported)
                break;

            case DT_DEBUG:

                break;
#if defined(USE_RELA)
                case DT_RELA:
        rela_ = reinterpret_cast<ElfW(Rela)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_RELASZ:
        rela_count_ = d->d_un.d_val / sizeof(ElfW(Rela));
        break;

      case DT_ANDROID_RELA:
        android_relocs_ = reinterpret_cast<uint8_t*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_ANDROID_RELASZ:
        android_relocs_size_ = d->d_un.d_val;
        break;

      case DT_ANDROID_REL:
        LOGE("unsupported DT_ANDROID_REL in \"%s\"", get_realpath());
        return false;

      case DT_ANDROID_RELSZ:
        LOGE("unsupported DT_ANDROID_RELSZ in \"%s\"", get_realpath());
        return false;

      case DT_RELAENT:
        if (d->d_un.d_val != sizeof(ElfW(Rela))) {
          LOGE("invalid DT_RELAENT: %zd", static_cast<size_t>(d->d_un.d_val));
          return false;
        }
        break;

      // Ignored (see DT_RELCOUNT comments for details).
      case DT_RELACOUNT:
        break;

      case DT_REL:
        LOGE("unsupported DT_REL in \"%s\"", get_realpath());
        return false;

      case DT_RELSZ:
        LOGE("unsupported DT_RELSZ in \"%s\"", get_realpath());
        return false;

#else
            case DT_REL:
                rel_ = reinterpret_cast<ElfW(Rel)*>(load_bias + d->d_un.d_ptr);
                break;

            case DT_RELSZ:
                rel_count_ = d->d_un.d_val / sizeof(ElfW(Rel));
                break;

            case DT_RELENT:
                if (d->d_un.d_val != sizeof(ElfW(Rel))) {
                    LOGE("invalid DT_RELENT: %zd", static_cast<size_t>(d->d_un.d_val));
                    return false;
                }
                break;

            case DT_ANDROID_REL:
                android_relocs_ = reinterpret_cast<uint8_t*>(load_bias + d->d_un.d_ptr);
                break;

            case DT_ANDROID_RELSZ:
                android_relocs_size_ = d->d_un.d_val;
                break;

            case DT_ANDROID_RELA:
                LOGE("unsupported DT_ANDROID_RELA in \"%s\"", get_realpath());
                return false;

            case DT_ANDROID_RELASZ:
                LOGE("unsupported DT_ANDROID_RELASZ in \"%s\"", get_realpath());
                return false;

                // "Indicates that all RELATIVE relocations have been concatenated together,
                // and specifies the RELATIVE relocation count."
                //
                // TODO: Spec also mentions that this can be used to optimize relocation process;
                // Not currently used by bionic linker - ignored.
            case DT_RELCOUNT:
                break;

            case DT_RELA:
                LOGE("unsupported DT_RELA in \"%s\"", get_realpath());
                return false;

            case DT_RELASZ:
                LOGE("unsupported DT_RELASZ in \"%s\"", get_realpath());
                return false;

#endif
            case DT_RELR:
                relr_ = reinterpret_cast<ElfW(Relr)*>(load_bias + d->d_un.d_ptr);
                break;

            case DT_RELRSZ:
                relr_count_ = d->d_un.d_val / sizeof(ElfW(Relr));
                break;

            case DT_RELRENT:
                if (d->d_un.d_val != sizeof(ElfW(Relr))) {
                    LOGE("invalid DT_RELRENT: %zd", static_cast<size_t>(d->d_un.d_val));
                    return false;
                }
                break;

            case DT_INIT:
                init_func_ = reinterpret_cast<linker_ctor_function_t>(load_bias + d->d_un.d_ptr);
                LOGE("%s constructors (DT_INIT) found at %p", get_realpath(), init_func_);
                break;

            case DT_FINI:
                fini_func_ = reinterpret_cast<linker_dtor_function_t>(load_bias + d->d_un.d_ptr);
                LOGE("%s destructors (DT_FINI) found at %p", get_realpath(), fini_func_);
                break;

            case DT_INIT_ARRAY:
                init_array_ = reinterpret_cast<linker_ctor_function_t*>(load_bias + d->d_un.d_ptr);
                LOGE("%s constructors (DT_INIT_ARRAY) found at %p", get_realpath(), init_array_);
                break;

            case DT_INIT_ARRAYSZ:
                init_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
                break;

            case DT_FINI_ARRAY:
                fini_array_ = reinterpret_cast<linker_dtor_function_t*>(load_bias + d->d_un.d_ptr);
                LOGE("%s destructors (DT_FINI_ARRAY) found at %p", get_realpath(), fini_array_);
                break;

            case DT_FINI_ARRAYSZ:
                fini_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
                break;

            case DT_PREINIT_ARRAY:
                preinit_array_ = reinterpret_cast<linker_ctor_function_t*>(load_bias + d->d_un.d_ptr);
                LOGE("%s constructors (DT_PREINIT_ARRAY) found at %p", get_realpath(), preinit_array_);
                break;

            case DT_PREINIT_ARRAYSZ:
                preinit_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
                break;

            case DT_TEXTREL:
#if defined(__LP64__)
                LOGE("\"%s\" has text relocations", get_realpath());
                return false;
#else
                has_text_relocations = true;
                break;
#endif

            case DT_SYMBOLIC:
                has_DT_SYMBOLIC = true;
                break;

            case DT_NEEDED:
                g_myneed[g_needed_count] = d->d_un.d_val;
                g_needed_count++;
                ++needed_count;
                break;

            case DT_FLAGS:
                if (d->d_un.d_val & DF_TEXTREL) {
#if defined(__LP64__)
                    LOGE("has text relocations");
          return false;
#else
                    has_text_relocations = true;
#endif
                }
                if (d->d_un.d_val & DF_SYMBOLIC) {
                    has_DT_SYMBOLIC = true;
                }
                break;

            case DT_FLAGS_1:
                //set_dt_flags_1(d->d_un.d_val);

                if ((d->d_un.d_val & ~SUPPORTED_DT_FLAGS_1) != 0) {
                    LOGE("Warning: \"%s\" has unsupported flags DT_FLAGS_1=%p "
                            "(ignoring unsupported flags)",
                            get_realpath(), reinterpret_cast<void*>(d->d_un.d_val));
                }
                break;

                // Ignored: "Its use has been superseded by the DF_BIND_NOW flag"
            case DT_BIND_NOW:
                break;

            case DT_VERSYM:
                versym_ = reinterpret_cast<ElfW(Versym)*>(load_bias + d->d_un.d_ptr);
                break;

            case DT_VERDEF:
                verdef_ptr_ = load_bias + d->d_un.d_ptr;
                break;
            case DT_VERDEFNUM:
                verdef_cnt_ = d->d_un.d_val;
                break;

            case DT_VERNEED:
                verneed_ptr_ = load_bias + d->d_un.d_ptr;
                break;

            case DT_VERNEEDNUM:
                verneed_cnt_ = d->d_un.d_val;
                break;

            case DT_RUNPATH:
                // this is parsed after we have strtab initialized (see below).
                break;

            case DT_TLSDESC_GOT:
            case DT_TLSDESC_PLT:
                // These DT entries are used for lazy TLSDESC relocations. Bionic
                // resolves everything eagerly, so these can be ignored.
                break;

            default:
                if (!relocating_linker) {
                    /**/

                }

                break;
        }
    }


    LOGD("si->base = %p, si->strtab = %p, si->symtab = %p",
          reinterpret_cast<void*>(base), strtab_, symtab_);

    // Sanity checks.
    if (relocating_linker && needed_count != 0) {
        LOGE("linker cannot have DT_NEEDED dependencies on other libraries");
        //return false;
    }
    if (nbucket_ == 0 && gnu_nbucket_ == 0) {
        LOGE("empty/missing DT_HASH/DT_GNU_HASH in \"%s\" "
               "(new hash type from the future?)", get_realpath());
        return false;
    }
    if (strtab_ == nullptr) {
        LOGE("empty/missing DT_STRTAB in \"%s\"", get_realpath());
        return false;
    }
    if (symtab_ == nullptr) {
        LOGE("empty/missing DT_SYMTAB in \"%s\"", get_realpath());
        return false;
    }

    // second pass - parse entries relying on strtab
    for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
        switch (d->d_tag) {
            case DT_SONAME:
                //set_soname(get_string(d->d_un.d_val));
                soname_ = get_string(d->d_un.d_val);
                LOGD("set soname = %s", soname_);
                break;
            case DT_RUNPATH:
                //set_dt_runpath(get_string(d->d_un.d_val));
               string strset_dt_runpath =  get_string(d->d_un.d_val);
                LOGD("set_dt_runpath(%s)", strset_dt_runpath.c_str());
                break;

        }
    }


    return true;
}

const char *soinfo::get_realpath() const {

    return "un implmentation get_realpath()";
}
const char *soinfo::get_string(ElfW(Word) index) const {
    return strtab_ + index;
}
//修复重定位节
bool soinfo::link_image() {

    local_group_root_ = this;
    if (android_relocs_ != nullptr){
        LOGD("android_relocs_ 不用处理");
    } else{
        LOGD("bad android relication header..");
    }


#if defined(USE_RELA)
    if (rela_ != nullptr) {
        LOGD("[ relocating %s ]", get_realpath());
        if (!relocate(plain_reloc_iterator(rela_, rela_count_))) {
          return false;
        }
    }
    if (plt_rela_ != nullptr) {
        LOGD("[ relocating %s plt ]", get_realpath());
        if (!relocate(plain_reloc_iterator(plt_rela_, plt_rela_count_))) {
            return false;
        }

    }
#else
    LOGE("TODO: !defined(USE_RELA) ");
#endif

    LOGD("[ finished linking %s ]", get_realpath());


    // We can also turn on GNU RELRO protection if we're not linking the dynamic linker
    // itself --- it can't make system calls yet, and will have to call protect_relro later.
    if (!((flags_ & FLAG_LINKER) != 0) && !protect_relro()) {
        return false;
    }

    return true;
}

template<typename ElfRelIteratorT>
bool soinfo::relocate(ElfRelIteratorT &&rel_iterator) {

    for (size_t idx = 0; rel_iterator.has_next(); ++idx) {
        const auto rel = rel_iterator.next();
        if (rel == nullptr) {
            return false;
        }

        ElfW(Word) type = ELFW(R_TYPE)(rel->r_info);
        ElfW(Word) sym = ELFW(R_SYM)(rel->r_info);

        ElfW(Addr) reloc = static_cast<ElfW(Addr)>(rel->r_offset + load_bias);
        ElfW(Addr) sym_addr = 0;
        const char* sym_name = nullptr;
        ElfW(Addr) addend = Utils::get_addend(rel, reloc);

        LOGD("Processing \"%s\" relocation at index %zd", get_realpath(), idx);
        if (type == R_GENERIC_NONE) {
            continue;
        }

        const ElfW(Sym)* s = nullptr;
        soinfo* lsi = nullptr;

        if (sym != 0) {

            sym_name = get_string(symtab_[sym].st_name);
//            LOGD("sym = %lx   sym_name: %s   st_value: %lx", sym, sym_name, symtab_[sym].st_value);


            for(int s = 0; s < g_needed_count; s++) {
                void* handle = dlopen(get_string(g_myneed[s]),RTLD_NOW);
                sym_addr = reinterpret_cast<Elf64_Addr>(dlsym(handle, sym_name));
                if(sym_addr) break;

            }

            if(!sym_addr) {
                if(symtab_[sym].st_value != 0) {
                    sym_addr = load_bias + symtab_[sym].st_value;
                }else {
                    LOGE("%s find addr fail", sym_name);
                }

            }else {
//                LOGD("%s find addr success : %lx", sym_name, sym_addr);
            }

        }

        switch (type) {
            case R_GENERIC_JUMP_SLOT:
                *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + addend);
                break;
            case R_GENERIC_GLOB_DAT:
                *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + addend);
                break;
            case R_GENERIC_RELATIVE:
                *reinterpret_cast<ElfW(Addr)*>(reloc) = (load_bias + addend);
                break;
            case R_GENERIC_IRELATIVE:
            {
                ElfW(Addr) ifunc_addr = Utils::call_ifunc_resolver(load_bias + addend);
                *reinterpret_cast<ElfW(Addr)*>(reloc) = ifunc_addr;
                break;
            }

#if defined(__aarch64__)
                case R_AARCH64_ABS64:
                *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend;
                 break;
                  case R_AARCH64_ABS32:
                    {
                      const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT32_MIN);
                      const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT32_MAX);
                      if ((min_value <= (sym_addr + addend)) &&
                          ((sym_addr + addend) <= max_value)) {
                        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend;
                      } else {
                        LOGE("0x%016llx out of range 0x%016llx to 0x%016llx",
                               sym_addr + addend, min_value, max_value);
                        return false;
                      }
                    }
                    break;
                  case R_AARCH64_ABS16:
                    {
                      const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT16_MIN);
                      const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT16_MAX);
                      if ((min_value <= (sym_addr + addend)) &&
                          ((sym_addr + addend) <= max_value)) {
                        *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + addend);
                      } else {
                        LOGE("0x%016llx out of range 0x%016llx to 0x%016llx",
                               sym_addr + addend, min_value, max_value);
                        return false;
                      }
                    }
                    break;
                  case R_AARCH64_PREL64:
                    *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - rel->r_offset;
                    break;
                  case R_AARCH64_PREL32:
                    {
                      const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT32_MIN);
                      const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT32_MAX);
                      if ((min_value <= (sym_addr + addend - rel->r_offset)) &&
                          ((sym_addr + addend - rel->r_offset) <= max_value)) {
                        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - rel->r_offset;
                      } else {
                        LOGE("0x%016llx out of range 0x%016llx to 0x%016llx",
                               sym_addr + addend - rel->r_offset, min_value, max_value);
                        return false;
                      }
                    }
                    break;
                  case R_AARCH64_PREL16:
                    {
                      const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT16_MIN);
                      const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT16_MAX);
                      if ((min_value <= (sym_addr + addend - rel->r_offset)) &&
                          ((sym_addr + addend - rel->r_offset) <= max_value)) {
                        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - rel->r_offset;
                      } else {
                        LOGE("0x%016llx out of range 0x%016llx to 0x%016llx",
                               sym_addr + addend - rel->r_offset, min_value, max_value);
                        return false;
                      }
                    }
                    break;

                  case R_AARCH64_COPY:
                    LOGE("%s R_AARCH64_COPY relocations are not supported", get_realpath());
                    return false;
                  case R_AARCH64_TLS_TPREL64:
                    LOGD("RELO TLS_TPREL64 *** %16llx <- %16llx - %16llx\n",
                               reloc, (sym_addr + addend), rel->r_offset);
                    break;
                  case R_AARCH64_TLS_DTPREL32:
                      LOGD("RELO TLS_DTPREL32 *** %16llx <- %16llx - %16llx\n",
                               reloc, (sym_addr + addend), rel->r_offset);
                    break;
#endif
                    default:
                        LOGE("unknown reloc type %d @ %p (%zu)  sym_name: %s", type, rel, idx, sym_name);
                        return false;


        }
    }

    return true;
}

bool soinfo::protect_relro() {

    if (Utils::phdr_table_protect_gnu_relro(phdr, phnum, load_bias,PROT_READ) < 0) {
        LOGE("can't enable GNU RELRO protection for \"%s\": %s",
               get_realpath(), strerror(errno));
        return false;
    }
    return true;
}
int g_argc = 0;
char** g_argv = nullptr;
char** g_envp = nullptr;

//有参构造
static void call_function(const char* function_name __unused,
                          linker_ctor_function_t function,
                          const char* realpath __unused) {
    if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
        return;
    }

    LOGD("[ Calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
    function(g_argc, g_argv, g_envp);
    LOGD("[ Done calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
}
//无参构造
static void call_function(const char* function_name __unused,
                          linker_dtor_function_t function,
                          const char* realpath __unused) {
    if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
        return;
    }

    LOGD("[ Calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
    function();
    LOGD("[ Done calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

template <typename F>
static void call_array(const char* array_name __unused,
                       F* functions,
                       size_t count,
                       bool reverse,
                       const char* realpath) {
    if (functions == nullptr) {
        return;
    }

    LOGD("[ Calling %s (size %zd) @ %p for '%s' ]", array_name, count, functions, realpath);

    int begin = reverse ? (count - 1) : 0;
    int end = reverse ? -1 : count;
    int step = reverse ? -1 : 1;

    for (int i = begin; i != end; i += step) {
        LOGD("[ %s[%d] == %p ]", array_name, i, functions[i]);
        call_function("function", functions[i], realpath);
    }

    LOGD("[ Done calling %s for '%s' ]", array_name, realpath);
}


void soinfo::call_constructors() {

// 我们在实际调用构造函数之前设置了constructors_called，否则它无法
// 防止递归构造函数调用。构造函数递归的一个简单示例是 libc debug malloc，它在 libc_malloc_debug_leak.so 中实现：
// 1. 程序依赖于 libc，因此这里调用了 libc 的构造函数。
// 2. libc 构造函数调用 dlopen() 来加载 libc_malloc_debug_leak.so。
// 3. dlopen() 调用新创建的 libc_malloc_debug_leak.so 的 soinfo 上的构造函数。
// 4. 调试 .so 依赖于 libc，因此 CallConstructors 会使用 libc soinfo 再次调用。如果它没有触发上面的 early-
// out，则会再次调用 libc 构造函数（递归！）。
    //constructors_called = true;

    LOGD("[call_constructors] constructors_called value = %d" , constructors_called);

    //对于so文件来说，由于没有_start函数，因此init_func和init_array_都无法传参，只能是默认值
    // DT_INIT should be called before DT_INIT_ARRAY if both are present.
    call_function("DT_INIT", init_func_, get_realpath());
    call_array("DT_INIT_ARRAY", init_array_, init_array_count_, false, get_realpath());



}

const char *MyLoader::get_string(ElfW(Word) index) const {
    return strtab_ + index;
}

bool MyLoader::CheckPhdr(ElfW(Addr) loaded) {
    const ElfW(Phdr)* phdr_limit = phdr_table_ + phdr_num_;
    ElfW(Addr) loaded_end = loaded + (phdr_num_ * sizeof(ElfW(Phdr)));
    for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
        if (phdr->p_type != PT_LOAD) {
            continue;
        }
        ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
        ElfW(Addr) seg_end = phdr->p_filesz + seg_start;
        if (seg_start <= loaded && loaded_end <= seg_end) {
            loaded_phdr_ = reinterpret_cast<const ElfW(Phdr)*>(loaded);
            return true;
        }
    }
    LOGE("\"%s\" loaded phdr %p not in loadable segment",
           name_.c_str(), reinterpret_cast<void*>(loaded));
    return false;

}
//将pdhr头的偏移修正，load_bias_
bool MyLoader::FindPhdr() {
    const ElfW(Phdr)* phdr_limit = phdr_table_ + phdr_num_;

    // If there is a PT_PHDR, use it directly.
    for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
        if (phdr->p_type == PT_PHDR) {
            return CheckPhdr(load_bias_ + phdr->p_vaddr);
        }
    }

    // Otherwise, check the first loadable segment. If its file offset
    // is 0, it starts with the ELF header, and we can trivially find the
    // loaded program header from it.
    for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
        if (phdr->p_type == PT_LOAD) {
            if (phdr->p_offset == 0) {
                ElfW(Addr)  elf_addr = load_bias_ + phdr->p_vaddr;
                const ElfW(Ehdr)* ehdr = reinterpret_cast<const ElfW(Ehdr)*>(elf_addr);
                ElfW(Addr)  offset = ehdr->e_phoff;
                return CheckPhdr(reinterpret_cast<ElfW(Addr)>(ehdr) + offset);
            }
            break;
        }
    }

    LOGE("can't find loaded phdr for \"%s\"", name_.c_str());
    return false;

}

bool MyLoader::Read(const char *name, int fd, off64_t file_offset, off64_t file_size) {
    bool  ret = false;
    name_ = name;
    fd_ = fd;
    file_offset_ = file_offset;
    file_size_ = file_size;
    if (ReadElfHeader() && ReadProgramHeader()){
        ret = true;
    }


    return ret;
}

bool MyLoader::ReadElfHeader() {
    bool ret = true;
    memcpy(&header_,start_addr_,sizeof (header_));
    //增加验证文件头
    //验证32位还是64位
    int elf_class = header_.e_ident[EI_CLASS];
    if (elf_class != ELFCLASS64 && elf_class != ELFCLASS32){
        ret = false;
    }
    //验证字节序
    if (header_.e_ident[EI_DATA] != ELFDATA2LSB) {
        return false;
    }
    //验证是不是动态库
    if (header_.e_type != ET_DYN) {
        return false;
    }
    //验证版本
    if (header_.e_version != EV_CURRENT) {
        return false;
    }
    //验证架构
    if (header_.e_machine != EM_386 && header_.e_machine != EM_AARCH64 && header_.e_machine != EM_ARM && header_.e_machine != EM_X86_64 ) {

        return false;
    }
    //验证节名称表
    if (header_.e_shstrndx == 0) {

        return false;
    }
    return ret;
}

bool MyLoader::ReadProgramHeader() {
    bool ret = true;
    phdr_num_ = header_.e_phnum;
    size_t  size = phdr_num_ * sizeof(ElfW(Phdr));

    void* data = Utils::getMapData(fd_,file_offset_,header_.e_phoff,size);
    if (data == NULL){
        LOGE("programHeader mmap Failed!");
        ret = false;
    }
    phdr_table_ = (ElfW(Phdr)*)data;
    if (phdr_num_ < 1  || phdr_num_ > 65535){
        ret = false;
    }
    return ret;
}

bool MyLoader::Load() {
    bool ret = false;
    if (ReserveAddressSpace() && LoadSegments() && FindPhdr()){
        ret = true;
    }

    //获取当前so，（加载器的so）
    si_ = Utils::get_soinfo("libmylinker.so");
    if (!si_){
        LOGE("si_ return nullptr");
        return false;
    }
    LOGD("si_ -> base : %lx " , si_->base);

    //使si_可以被修改
    bool bPort = mprotect((void*) PAGE_START(reinterpret_cast<ElfW(Addr)>(si_)), 0x1000, PROT_READ | PROT_WRITE);
    if (!bPort){
        LOGD("[Load ] 修改si_ 结构体内存属性失败 ");
    }

    //修正so
    si_->base = load_start();
    si_->size = load_size();
    si_->load_bias = load_bias();
    si_->phnum = phdr_count();
    si_->phdr = loaded_phdr();

    return ret;
}

bool MyLoader::ReserveAddressSpace() {
    ElfW(Addr) min_vaddr;

    load_size_ = phdr_table_get_load_size(phdr_table_, phdr_num_, &min_vaddr);
    if (load_size_ == 0) {
        return false;
    }

    uint8_t* addr = reinterpret_cast<uint8_t*>(min_vaddr);
    void* start;

    //Assume position independent executable by default.
    void* mmap_hint = nullptr;
    start = mmap(mmap_hint,load_size_,PROT_NONE ,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);

    //没太理解为什么start-addr就是load_bias了
    load_start_ = start;
    //addr是load段的最低地址，通常是0。start是mmap的首地址。
    //load_bias代表的是待加载的这片内存
    load_bias_ = reinterpret_cast<uint8_t*>(start) - addr;
    return true;
}
//返回可加载段的大小，0x1000向上取整
size_t MyLoader::phdr_table_get_load_size(const ElfW(Phdr) *phdr_table, size_t phdr_count,
                                          ElfW(Addr) *out_min_vaddr) {
    ElfW(Addr) min_vaddr = UINTPTR_MAX;
    ElfW(Addr) max_vaddr = 0;

    bool found_pt_load = false;
    for (size_t i = 0; i < phdr_count; ++i) {
        const ElfW(Phdr)* phdr = &phdr_table[i];

        if (phdr->p_type != PT_LOAD) {
            continue;
        }
        found_pt_load = true;

        if (phdr->p_vaddr < min_vaddr) {
            min_vaddr = phdr->p_vaddr;
        }

        if (phdr->p_vaddr + phdr->p_memsz > max_vaddr) {
            max_vaddr = phdr->p_vaddr + phdr->p_memsz;
        }
    }
    if (!found_pt_load) {
        min_vaddr = 0;
    }

    min_vaddr = PAGE_START(min_vaddr);
    max_vaddr = PAGE_END(max_vaddr);

    if (out_min_vaddr != nullptr) {
        *out_min_vaddr = min_vaddr;
    }

    return max_vaddr - min_vaddr;

}

bool MyLoader::LoadSegments() {

    bool ret = true;
    for (size_t i = 0; i < phdr_num_; ++i) {
        const ElfW(Phdr)* phdr = &phdr_table_[i];

        if (phdr->p_type != PT_LOAD) {
            continue;
        }

        // Segment addresses in memory.
        ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
        ElfW(Addr) seg_end   = seg_start + phdr->p_memsz;

        ElfW(Addr) seg_page_start = PAGE_START(seg_start);
        ElfW(Addr) seg_page_end   = PAGE_END(seg_end);

        ElfW(Addr) seg_file_end   = seg_start + phdr->p_filesz;

        // File offsets.
        ElfW(Addr) file_start = phdr->p_offset;
        ElfW(Addr) file_end   = file_start + phdr->p_filesz;

        ElfW(Addr) file_page_start = PAGE_START(file_start);
        ElfW(Addr) file_length = file_end - file_page_start;

        if (file_size_ <= 0) {

            return false;
        }

        if (file_end > static_cast<size_t>(file_size_)) {

            return false;
        }


        if (file_length != 0) {
            //段属性读写执行
            int prot = PFLAGS_TO_PROT(phdr->p_flags);
            if ((prot & (PROT_EXEC | PROT_WRITE)) == (PROT_EXEC | PROT_WRITE)) {
                // W + E PT_LOAD segments are not allowed in O.

                    return false;
            }

            /*  */
            //seg_page_start是内存首地址
            void* seg_addr = mmap64(reinterpret_cast<void*>(seg_page_start),
                                    file_length,
                                    prot,
                                    MAP_FIXED|MAP_PRIVATE,
                                    fd_,
                                    file_offset_ + file_page_start);
            if (seg_addr == MAP_FAILED) {
                LOGD("mmap error ： %d-%s.",errno,strerror(errno));
                return false;
            }
            //bool mprotectRet = mprotect((void*)seg_page_start,file_length,prot);
            //if (!mprotectRet){
               // LOGE("[function :LoadSegments] mmap64 修改内存属性失败! %s", strerror(errno));
               // return false;
           // }

        }

        // if the segment is writable, and does not end on a page boundary,
        // zero-fill it until the page limit.
        //如果内存可写，将分页对齐 - 文件尾部的空间清0
        if ((phdr->p_flags & PF_W) != 0 && PAGE_OFFSET(seg_file_end) > 0) {
            memset(reinterpret_cast<void*>(seg_file_end), 0, PAGE_SIZE - PAGE_OFFSET(seg_file_end));
        }

        seg_file_end = PAGE_END(seg_file_end);

        // seg_file_end is now the first page address after the file
        // content. If seg_end is larger, we need to zero anything
        // between them. This is done by using a private anonymous
        // map for all extra pages.
        if (seg_page_end > seg_file_end) {
            size_t zeromap_size = seg_page_end - seg_file_end;
            void* zeromap = mmap(reinterpret_cast<void*>(seg_file_end),
                                 zeromap_size,
                                 PFLAGS_TO_PROT(phdr->p_flags),
                                 MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE,
                                 -1,
                                 0);
            if (zeromap == MAP_FAILED) {

                LOGE("[function :LoadSegments] mmap .bss段失败!");
                return false;
            }

            prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, zeromap, zeromap_size, ".bss");
        }
    }

    return ret;
}

bool MyLoader::ReadSectionHeaders() {
    bool ret = true;
    shdr_num_ = header_.e_shnum;

    if (shdr_num_ == 0) {

        return false;
    }

    size_t size = shdr_num_ * sizeof(ElfW(Shdr));

    void* data = Utils::getMapData(fd_,file_offset_,header_.e_shoff,size);
    if (data == NULL){
        LOGE("Read SectionHeader failed");
        return false;
    }
    shdr_table_ = static_cast<const ElfW(Shdr)*>(data);

    return ret;
}

bool MyLoader::ReadDynamicSection() {
    bool ret = true;

    // 1. Find .dynamic section (in section headers)
    const ElfW(Shdr)* dynamic_shdr = nullptr;
    for (size_t i = 0; i < shdr_num_; ++i) {
        if (shdr_table_[i].sh_type == SHT_DYNAMIC) {
            dynamic_shdr = &shdr_table_ [i];
            break;
        }
    }

    if (dynamic_shdr == nullptr) {
        return false;
    }

    // Make sure dynamic_shdr offset and size matches PT_DYNAMIC phdr
    size_t pt_dynamic_offset = 0;
    size_t pt_dynamic_filesz = 0;
    for (size_t i = 0; i < phdr_num_; ++i) {
        const ElfW(Phdr)* phdr = &phdr_table_[i];
        if (phdr->p_type == PT_DYNAMIC) {
            pt_dynamic_offset = phdr->p_offset;
            pt_dynamic_filesz = phdr->p_filesz;
        }
    }
    //android8.0及以后程序头的动态节和节头的动态节需保持一致，否则加载失败
    if (pt_dynamic_offset != dynamic_shdr->sh_offset) {

            return false;
    }

    if (pt_dynamic_filesz != dynamic_shdr->sh_size) {
            return false;
    }

    //动态节sh_link依赖字符串表索引，如果大于节头数量，则动态节是无效的
    if (dynamic_shdr->sh_link >= shdr_num_) {

        return false;
    }

    const ElfW(Shdr)* strtab_shdr = &shdr_table_[dynamic_shdr->sh_link];

    //如果sh_link不是字符串表一样无效
    if (strtab_shdr->sh_type != SHT_STRTAB) {

        return false;
    }


    void* data = Utils::getMapData(fd_,file_offset_,dynamic_shdr->sh_offset,dynamic_shdr->sh_size);

    if (data == NULL)
    {
        LOGD("Read Dynamic Failed");
        return false;
    }

    dynamic_ = static_cast<const ElfW(Dyn)*>(data);


    void* data2 = Utils::getMapData(fd_,file_offset_,strtab_shdr->sh_offset,strtab_shdr->sh_size);
    if (data2 == NULL){
        LOGD("Read StrTab Failed");
        return false;
    }

    strtab_ = static_cast<const char*>(data2);
    strtab_size_ = strtab_shdr->sh_size;

    return ret;
}

int Utils::phdr_table_protect_gnu_relro(const ElfW(Phdr) *phdr_table, size_t phdr_count,
                                                    ElfW(Addr) load_bias, int prot_flags) {

    const ElfW(Phdr)* phdr = phdr_table;
    const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

    for (phdr = phdr_table; phdr < phdr_limit; phdr++) {
        if (phdr->p_type != PT_GNU_RELRO) {
            continue;
        }

        // Tricky: what happens when the relro segment does not start
        // or end at page boundaries? We're going to be over-protective
        // here and put every page touched by the segment as read-only.

        // This seems to match Ian Lance Taylor's description of the
        // feature at http://www.airs.com/blog/archives/189.

        //    Extract:
        //       Note that the current dynamic linker code will only work
        //       correctly if the PT_GNU_RELRO segment starts on a page
        //       boundary. This is because the dynamic linker rounds the
        //       p_vaddr field down to the previous page boundary. If
        //       there is anything on the page which should not be read-only,
        //       the program is likely to fail at runtime. So in effect the
        //       linker must only emit a PT_GNU_RELRO segment if it ensures
        //       that it starts on a page boundary.
        ElfW(Addr) seg_page_start = PAGE_START(phdr->p_vaddr) + load_bias;
        ElfW(Addr) seg_page_end   = PAGE_END(phdr->p_vaddr + phdr->p_memsz) + load_bias;

        int ret = mprotect(reinterpret_cast<void*>(seg_page_start),
                           seg_page_end - seg_page_start,
                           prot_flags);
        if (ret < 0) {
            return -1;
        }
    }
    return 0;
}

ElfW(Addr) Utils::get_export_func(char *path, char *func_name) {

    struct stat sb;
    int fd = open(path,O_RDONLY);
    fstat(fd,&sb);
    void* base = mmap(NULL,sb.st_size,PROT_READ | PROT_WRITE, MAP_PRIVATE , fd,0);
    //读取elf header
    ElfW(Ehdr) header;
    memcpy((void*)&header,base,sizeof(header));

    //读取section header table

    size_t  size = header.e_shnum * sizeof(ElfW(Shdr));

    void* tmp = mmap(NULL,size , PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    if (tmp == NULL){
        LOGE("[get_export_func] mmap error %s", strerror(errno));
        return 0;
    }
    ElfW(Shdr)* shdr_table;

    memcpy(tmp,(void*)((ElfW(Off))base + header.e_shoff),size);
    shdr_table = (ElfW(Shdr)*)tmp;

    char* shstrtab = (char*)(shdr_table[header.e_shstrndx].sh_offset + (ElfW(Off))base);
    void* symtab = nullptr;
    char* strtab = nullptr;
    uint32_t  symtab_size = 0;
    //遍历获取.symtab和.strtab节
    for (size_t i = 0; i < header.e_shnum; ++i) {
        const ElfW(Shdr)* shdr = &shdr_table[i];
        char* section_name = shstrtab + shdr->sh_name;
        if(!strcmp(section_name, ".symtab")) {
//            LOGD("[test] %d: shdr->sh_name = %s", i, (shstrtab + shdr->sh_name));
            symtab = reinterpret_cast<void*>(shdr->sh_offset + (ElfW(Off))base);
            symtab_size = shdr->sh_size;
        }
        if(!strcmp(section_name, ".strtab")) {
//            LOGD("[test] %d: shdr->sh_name = %s", i, (shstrtab + shdr->sh_name));
            strtab = reinterpret_cast<char*>(shdr->sh_offset + (ElfW(Off))base);
        }

        if(strtab && symtab)break;
    }

    // 读取 Symbol table
    ElfW(Sym)* sym_table;
    tmp = mmap(nullptr, symtab_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memcpy(tmp, symtab, symtab_size);
    sym_table = static_cast<ElfW(Sym)*>(tmp);

    int sym_num = symtab_size / sizeof(ElfW(Sym));

    // 遍历 Symbol table
    for(int i = 0; i < sym_num; i++) {
        const ElfW(Sym) *sym = &sym_table[i];
        char* sym_name = strtab + sym->st_name;
        if(strstr(sym_name, func_name)) {
            return sym->st_value;
        }


    }
    return 0;
}



ElfW(Addr) Utils::get_addend(ElfW(Rela) *rela, ElfW(Addr) reloc_addr) {

        return rela->r_addend;

}

ElfW(Addr) Utils::call_ifunc_resolver(ElfW(Addr) resolver_addr) {

    typedef ElfW(Addr) (*ifunc_resolver_t)(void);
    ifunc_resolver_t ifunc_resolver = reinterpret_cast<ifunc_resolver_t>(resolver_addr);
    ElfW(Addr) ifunc_addr = ifunc_resolver();

    return ifunc_addr;
}

void Utils::phdr_table_get_dynamic_section(const ElfW(Phdr) *phdr_table, size_t phdr_count,
                                     ElfW(Addr) load_bias, ElfW(Dyn) **dynamic,ElfW(Word) *dynamic_flags) {
    *dynamic = nullptr;
    for (size_t i = 0; i<phdr_count; ++i) {
        const ElfW(Phdr)& phdr = phdr_table[i];
        if (phdr.p_type == PT_DYNAMIC) {
            *dynamic = reinterpret_cast<ElfW(Dyn)*>(load_bias + phdr.p_vaddr);
            if (dynamic_flags) {
                *dynamic_flags = phdr.p_flags;
            }
            return;
        }
    }
    return;
}


void *Utils::getMapData(int fd, off64_t base_offset, size_t elf_offset, size_t size) {
    off64_t offset;
    offset = base_offset +  elf_offset;

    //page_min是offset第三位清0后的值
    off64_t page_min = page_start(offset);
    off64_t end_offset;

    safe_add(&end_offset, offset, size);
    safe_add(&end_offset, end_offset, page_offset(offset));


    size_t map_size = static_cast<size_t>(end_offset - page_min);


    uint8_t* map_start = static_cast<uint8_t*>(
            mmap64(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, page_min));

    if (map_start == MAP_FAILED) {
        return nullptr;
    }




    return map_start + page_offset(offset);
}
//低三位清0
off64_t Utils::page_start(off64_t offset) {
    return offset & kPageMask;
}

//只保留第三位
size_t Utils::page_offset(off64_t offset) {
    return static_cast<size_t>(offset & (PAGE_SIZE-1));
}

//arg【0】 = arg【1】 + arg【2】
bool Utils::safe_add(off64_t *out, off64_t a, size_t b) {

    if (static_cast<uint64_t>(INT64_MAX - a) < b) {
        return false;
    }

    *out = a + b;
    return true;

}

soinfo *Utils::get_soinfo(const char *so_name) {

    typedef  soinfo* (*FunctionPtr)(ElfW(Addr));
    char line[1024];
    ElfW(Addr) linker_base = 0;
    ElfW(Addr) so_addr = 0;
    FILE * fp = fopen("/proc/self/maps","r");
    while(fgets(line,1024,fp)){
        if (strstr(line,"linker64") && !linker_base){
            //sccanf(line,"%x",&so_addr);
            char* addr = strtok(line,"-");
            linker_base = strtoull(addr,NULL,16);

        }else if (strstr(line,so_name) && !so_addr){
            char* addr = strtok(line,"-");
            so_addr = strtoull(addr,NULL,16);
        }

        if (linker_base && so_addr){
            break;
        }
    }
    //so_addr = ( ElfW(Addr))dlopen("libmylinker.so",RTLD_NOW );
    //int pid = getpid();  ///apex/com.android.runtime/bin/linker64
    ElfW(Addr) func_offset = Utils::get_export_func("/apex/com.android.runtime/bin/linker64","find_containing_library");
    if (!func_offset){
        LOGE("func_offset == 0? check it --> get_soinfo");
        return nullptr;
    }
    ElfW(Addr) find_containing_library_addr = linker_base + func_offset;
    FunctionPtr  find_containing_library = (FunctionPtr)find_containing_library_addr;
    return find_containing_library(so_addr);

}

































