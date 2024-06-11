#include <jni.h>
#include <string>
#include <inttypes.h>
#include "xh_log.h"
#include <unistd.h>
#include <sys/mman.h>
#include "xh_elf.h"

#if defined(__arm__)
#define XH_ELF_R_GENERIC_JUMP_SLOT R_ARM_JUMP_SLOT      //.rel.plt
#define XH_ELF_R_GENERIC_GLOB_DAT  R_ARM_GLOB_DAT       //.rel.dyn
#define XH_ELF_R_GENERIC_ABS       R_ARM_ABS32          //.rel.dyn
#elif defined(__aarch64__)
#define XH_ELF_R_GENERIC_JUMP_SLOT R_AARCH64_JUMP_SLOT
#define XH_ELF_R_GENERIC_GLOB_DAT  R_AARCH64_GLOB_DAT
#define XH_ELF_R_GENERIC_ABS       R_AARCH64_ABS64
#elif defined(__i386__)
#define XH_ELF_R_GENERIC_JUMP_SLOT R_386_JMP_SLOT
#define XH_ELF_R_GENERIC_GLOB_DAT  R_386_GLOB_DAT
#define XH_ELF_R_GENERIC_ABS       R_386_32
#elif defined(__x86_64__)
#define XH_ELF_R_GENERIC_JUMP_SLOT R_X86_64_JUMP_SLOT
#define XH_ELF_R_GENERIC_GLOB_DAT  R_X86_64_GLOB_DAT
#define XH_ELF_R_GENERIC_ABS       R_X86_64_64
#endif
#if defined(__LP64__)
#define XH_ELF_R_SYM(info)  ELF64_R_SYM(info)
#define XH_ELF_R_TYPE(info) ELF64_R_TYPE(info)
#else
#define XH_ELF_R_SYM(info)  ELF32_R_SYM(info)
#define XH_ELF_R_TYPE(info) ELF32_R_TYPE(info)
#endif

//ELF hash func
uint32_t xh_elf_hash(const char *name)
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

//GNU hash func
uint32_t xh_elf_gnu_hash(const char *name)
{
    uint32_t h = 5381;

    while(*name != 0)
    {
        h += (h << 5) + *name++;
    }
    return h;
}

static int xh_elf_check(xh_elf_t *self)
{
    if(0 == self->base_addr)
    {
        XH_LOG_ERROR("base_addr == 0\n");
        return 1;
    }
    if(0 == self->bias_addr)
    {
        XH_LOG_ERROR("bias_addr == 0\n");
        return 1;
    }
    if(NULL == self->ehdr)
    {
        XH_LOG_ERROR("ehdr == NULL\n");
        return 1;
    }
    if(NULL == self->phdr)
    {
        XH_LOG_ERROR("phdr == NULL\n");
        return 1;
    }
    if(NULL == self->strtab)
    {
        XH_LOG_ERROR("strtab == NULL\n");
        return 1;
    }
    if(NULL == self->symtab)
    {
        XH_LOG_ERROR("symtab == NULL\n");
        return 1;
    }
    if(NULL == self->bucket)
    {
        XH_LOG_ERROR("bucket == NULL\n");
        return 1;
    }
    if(NULL == self->chain)
    {
        XH_LOG_ERROR("chain == NULL\n");
        return 1;
    }
    if(1 == self->is_use_gnu_hash && NULL == self->bloom)
    {
        XH_LOG_ERROR("bloom == NULL\n");
        return 1;
    }

    return 0;
}

static ElfW(Phdr) *xh_elf_get_first_segment_by_type(xh_elf_t *self, ElfW(Word) type)
{
    ElfW(Phdr) *phdr;

    for(phdr = self->phdr; phdr < self->phdr + self->ehdr->e_phnum; phdr++)
    {
        if(phdr->p_type == type)
        {
            return phdr;
        }
    }
    return NULL;
}

static ElfW(Phdr) *xh_elf_get_first_segment_by_type_offset(xh_elf_t *self, ElfW(Word) type, ElfW(Off) offset)
{
    ElfW(Phdr) *phdr;

    for(phdr = self->phdr; phdr < self->phdr + self->ehdr->e_phnum; phdr++)
    {
        if(phdr->p_type == type && phdr->p_offset == offset)
        {
            return phdr;
        }
    }
    return NULL;
}


int xh_elf_init(xh_elf_t *self, uint64_t base_addr, const char *pathname)
{
    if(0 == base_addr || NULL == pathname) return -1;

    //always reset
    memset(self, 0, sizeof(xh_elf_t));

    self->pathname = pathname;
    self->base_addr = (ElfW(Addr))base_addr;
    self->ehdr = (ElfW(Ehdr) *)base_addr;
    self->phdr = (ElfW(Phdr) *)(base_addr + self->ehdr->e_phoff); //segmentation fault sometimes

    //find the first load-segment with offset 0
    ElfW(Phdr) *phdr0 = xh_elf_get_first_segment_by_type_offset(self, PT_LOAD, 0);
    if(NULL == phdr0)
    {
        XH_LOG_ERROR("Can NOT found the first load segment. %s", pathname);
        return -1;
    }

#if XH_ELF_DEBUG
    if(0 != phdr0->p_vaddr)
        XH_LOG_DEBUG("first load-segment vaddr NOT 0 (vaddr: %p). %s",
                     (void *)(phdr0->p_vaddr), pathname);
#endif

    //save load bias addr
    if(self->base_addr < phdr0->p_vaddr) return -1;
    self->bias_addr = self->base_addr - phdr0->p_vaddr;

    //find dynamic-segment
    ElfW(Phdr) *dhdr = xh_elf_get_first_segment_by_type(self, PT_DYNAMIC);
    if(NULL == dhdr)
    {
        XH_LOG_ERROR("Can NOT found dynamic segment. %s", pathname);
        return -1;
    }

    //parse dynamic-segment
    self->dyn          = (ElfW(Dyn) *)(self->bias_addr + dhdr->p_vaddr);
    self->dyn_sz       = dhdr->p_memsz;
    ElfW(Dyn) *dyn     = self->dyn;
    ElfW(Dyn) *dyn_end = self->dyn + (self->dyn_sz / sizeof(ElfW(Dyn)));
    uint32_t  *raw;
    for(; dyn < dyn_end; dyn++)
    {
        switch(dyn->d_tag) //segmentation fault sometimes
        {
            case DT_NULL:
                //the end of the dynamic-section
                dyn = dyn_end;
                break;
            case DT_STRTAB:
            {
                self->strtab = (const char *)(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))(self->strtab) < self->base_addr) return -1;
                break;
            }
            case DT_SYMTAB:
            {
                self->symtab = (ElfW(Sym) *)(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))(self->symtab) < self->base_addr) return -1;
                break;
            }
            case DT_PLTREL:
                //use rel or rela?
                self->is_use_rela = (dyn->d_un.d_val == DT_RELA ? 1 : 0);
                break;
            case DT_JMPREL:
            {
                self->relplt = (ElfW(Addr))(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))(self->relplt) < self->base_addr) return -1;
                break;
            }
            case DT_PLTRELSZ:
                self->relplt_sz = dyn->d_un.d_val;
                break;
            case DT_REL:
            case DT_RELA:
            {
                self->reldyn = (ElfW(Addr))(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))(self->reldyn) < self->base_addr) return -1;
                break;
            }
            case DT_RELSZ:
            case DT_RELASZ:
                self->reldyn_sz = dyn->d_un.d_val;
                break;
            case DT_ANDROID_REL:
            case DT_ANDROID_RELA:
            {
                self->relandroid = (ElfW(Addr))(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))(self->relandroid) < self->base_addr) return -1;
                break;
            }
            case DT_ANDROID_RELSZ:
            case DT_ANDROID_RELASZ:
                self->relandroid_sz = dyn->d_un.d_val;
                break;
            case DT_HASH:
            {
                //ignore DT_HASH when ELF contains DT_GNU_HASH hash table
                if(1 == self->is_use_gnu_hash) continue;

                raw = (uint32_t *)(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))raw < self->base_addr) return -1;
                self->bucket_cnt  = raw[0];
                self->chain_cnt   = raw[1];
                self->bucket      = &raw[2];
                self->chain       = &(self->bucket[self->bucket_cnt]);
                break;
            }
/*            case DT_GNU_HASH:
            {
                raw = (uint32_t *)(self->bias_addr + dyn->d_un.d_ptr);
                if((ElfW(Addr))raw < self->base_addr) return -1;
                self->bucket_cnt  = raw[0];
                self->symoffset   = raw[1];
                self->bloom_sz    = raw[2];
                self->bloom_shift = raw[3];
                self->bloom       = (ElfW(Addr) *)(&raw[4]);
                self->bucket      = (uint32_t *)(&(self->bloom[self->bloom_sz]));
                self->chain       = (uint32_t *)(&(self->bucket[self->bucket_cnt]));
                self->is_use_gnu_hash = 1;
                break;
            }*/
        }
    }

    //check android rel/rela
    if(0 != self->relandroid)
    {
        const char *rel = (const char *)self->relandroid;
        if(self->relandroid_sz < 4 ||
           rel[0] != 'A' ||
           rel[1] != 'P' ||
           rel[2] != 'S' ||
           rel[3] != '2')
        {
            XH_LOG_ERROR("android rel/rela format error\n");
            return -1;
        }

        self->relandroid += 4;
        self->relandroid_sz -= 4;
    }

    //check elf info
    if(0 != xh_elf_check(self))
    {
        XH_LOG_ERROR("elf init check failed. %s", pathname);
        return -1;
    }

#if XH_ELF_DEBUG
        xh_elf_dump(self);
#endif

    XH_LOG_INFO("init OK: %s (%s %s PLT:%u DYN:%u ANDROID:%u)\n", self->pathname,
                self->is_use_rela ? "RELA" : "REL",
                self->is_use_gnu_hash ? "GNU_HASH" : "ELF_HASH",
                self->relplt_sz, self->reldyn_sz, self->relandroid_sz);

    return 0;
}
int gnu_hash_lookup(xh_elf_t *self,const char* funcName){

    //xh_elf_gnu_hash_lookup_def
   int symIndex = 0;
   uint32_t gnu_hash =  xh_elf_gnu_hash(funcName);
   uint32_t elfclass_bits = sizeof(ElfW(Addr)) * 8;
   size_t word = self->bloom[ (gnu_hash / elfclass_bits) % self->bloom_sz];
   size_t  mask = 0 | (size_t)1 << (gnu_hash % elfclass_bits) | (size_t)1 << ((gnu_hash >> self->bloom_shift) % elfclass_bits);
    //if at least one bit is not set, this symbol is surely missing
   if((word & mask) != mask) return -1;

    //ignore STN_UNDEF
    uint32_t i = self->bucket[gnu_hash % self->bucket_cnt];
    if(i < self->symoffset) return -1;
    //loop through the chain

    while(1){
        const char* symName = self->strtab + self->symtab[i].st_name;
        const uint32_t symHash = self->chain[i - self->symoffset];

        if((gnu_hash | (uint32_t)1) == (symHash | (uint32_t)1) && 0 == strcmp(symName, funcName))
        {
            symIndex = i;
            XH_LOG_INFO("found %s at symidx: %d (GNU_HASH DEF)\n", funcName, symIndex);
            return symIndex;
        }
        //chain ends with an element with the lowest bit set to 1
        if(symHash & (uint32_t)1) break;
        i++;
    }

    //xh_elf_gnu_hash_lookup_undef
    for(i = 0; i < self->symoffset; i++)
    {
        const char *symname = self->strtab + self->symtab[i].st_name;
        if(0 == strcmp(symname, funcName))
        {
            symIndex = i;
            XH_LOG_INFO("found %s at symidx: %d (GNU_HASH UNDEF)\n", funcName, symIndex);
            return symIndex;
        }
    }

}
int hash_lookup(xh_elf_t *self,const char* funcName){


    int symIndex = 0;
    uint32_t hash = xh_elf_hash(funcName);

    uint32_t i = self->bucket[hash % self->bucket_cnt];
    for (; i != 0 ; i = self->chain[i]) {

        const char* symName = self->strtab + self->symtab[i].st_name;
        //const uint32_t symHash = self->chain[i - self->symoffset];
        if (strcmp(symName,funcName) == 0){
            symIndex = i;
            XH_LOG_INFO("found %s at symidx: %d (hash_lookup)\n", funcName, symIndex);
            return symIndex;
        }

    }
    return symIndex;
}
int xh_elf_find_and_replace_func_by_str(xh_elf_t *self,void *new_func, const char* funcName){
    return 0;
}
static int xh_elf_find_and_replace_func(xh_elf_t *self,void *new_func, const char* funcName)
{
    ElfW(Rela)     *relDyn;
    ElfW(Rela)     *rela;
    int            r = 0;

    int symIndex = 0;
    if (self->is_use_gnu_hash){
        //快速hash查找

        symIndex =  gnu_hash_lookup(self,funcName);

    } else{
        //普通hash查找

        symIndex =  hash_lookup(self,funcName);

    }
    if (symIndex == NULL){

        //字符串查找
        xh_elf_find_and_replace_func_by_str(self,new_func,funcName);
    }


    //开始遍历重定位表.rela.dyn
    relDyn= (ElfW(Rela) *)self->reldyn;
    for (int i = 0; i < self->reldyn_sz / sizeof(ElfW(Rela)); ++i) {

        if (XH_ELF_R_SYM(relDyn[i].r_info) == symIndex){

            XH_LOG_INFO("found sym in .rel.dyn table");

            int pagesize = getpagesize();
            ElfW(Addr) pageStart = (self->bias_addr +  relDyn[i].r_offset) & (~(pagesize -1)) ;
            int n =  mprotect((void*)pageStart,pagesize,PROT_READ | PROT_WRITE);
            XH_LOG_INFO("修改内存属性 n = %d",n);
            //do replace
            void** got = (void**)(self->bias_addr +  relDyn[i].r_offset);
            *got = new_func;
            n =  mprotect((void*)pageStart,pagesize,PROT_READ);
            r++;
        }
        
    }
    //开始遍历重定位表.rela.plt
    rela= (ElfW(Rela) *)self->relplt;
    for (int i = 0; i < self->relplt_sz / sizeof(ElfW(Rela)); ++i) {

        if (XH_ELF_R_SYM(rela[i].r_info) == symIndex){

            XH_LOG_INFO("found sym in .rel.plt table");

            int pagesize = getpagesize();
            ElfW(Addr) pageStart = (self->bias_addr +  rela[i].r_offset) & (~(pagesize -1)) ;
            int n =  mprotect((void*)pageStart,pagesize,PROT_READ | PROT_WRITE);
            XH_LOG_INFO("修改内存属性 n = %d",n);
            //do replace
            void** got = (void**)(self->bias_addr +  rela[i].r_offset);
            *got = new_func;
            n =  mprotect((void*)pageStart,pagesize,PROT_READ);
            r++;
        }

    }

    return r;
}


//ELF header checker
int xh_elf_check_elfheader(uintptr_t base_addr)
{
    ElfW(Ehdr) *ehdr = (ElfW(Ehdr) *)base_addr;

    //check magic
    if(0 != memcmp(ehdr->e_ident, ELFMAG, SELFMAG)) return -1;

    //check class (64/32)
#if defined(__LP64__)
    if(ELFCLASS64 != ehdr->e_ident[EI_CLASS]) return -1;
#else
    if(ELFCLASS32 != ehdr->e_ident[EI_CLASS]) return -1;
#endif

    //check endian (little/big)
    if(ELFDATA2LSB != ehdr->e_ident[EI_DATA]) return -1;

    //check version
    if(EV_CURRENT != ehdr->e_ident[EI_VERSION]) return -1;

    //check type
    if(ET_EXEC != ehdr->e_type && ET_DYN != ehdr->e_type) return -1;

    //check machine
#if defined(__arm__)
    if(EM_ARM != ehdr->e_machine) return -1;
#elif defined(__aarch64__)
    if(EM_AARCH64 != ehdr->e_machine) return -1;
#elif defined(__i386__)
    if(EM_386 != ehdr->e_machine) return -1;
#elif defined(__x86_64__)
    if(EM_X86_64 != ehdr->e_machine) return -1;
#else
    return -1;
#endif

    //check version   //校验版本号，安卓上目前只能是EV_CURRENT
    if(EV_CURRENT != ehdr->e_version) return -1;
    return 0;
}

//libgothook64.so
uint64_t getModuleBase(const char *pathname)
{
    FILE      *fp;
    char       line[512];
    uint64_t start  = 0;
    uint64_t  end  = 0;
    char       perm[5];
    if(NULL == (fp = fopen("/proc/self/maps", "r"))) return 0;

    while(fgets(line, sizeof(line), fp))
    {
        if(NULL != pathname)
            if(NULL == strstr(line, pathname)) continue;

        if(strstr(line, pathname)) XH_LOG_INFO("%s\n",line);
        if(sscanf(line, "%llX-%llX %4s ", &start, &end, perm) != 3) continue;

        //if(sscanf(line, "%"PRIxPTR"-%"PRIxPTR" %4s ", &start, &end, perm) != 3) continue;


        if((perm[0] == 'r') && (perm[1] == '-') &&  (perm[2] == 'x') && (perm[3] == 'p') ){
            break;
        }
    }

    fclose(fp);
    return start;
}
size_t new_strlen(const char* const s )  {

    XH_LOG_INFO("new_strlen is exec");
    return strlen(s);
}

void hook(const char* modName,const char* funcName){

    int nResult = -1;
    uint64_t modBase =  getModuleBase(modName);
    if (modName == NULL) {XH_LOG_ERROR("获取模块基址失败 %s",modName);}
    XH_LOG_INFO("%s modBase = %p",modName,modBase);
    //检查解析elf文件(magic,架构，版本等是否正确)
    nResult =  xh_elf_check_elfheader(modBase);
    if (nResult == -1) XH_LOG_ERROR("ELF文件格式有误 %p",modBase);

    //开始解析elf文件
    xh_elf_t   elfso;
    nResult = xh_elf_init(&elfso,modBase,modName);
    if (nResult == -1) XH_LOG_ERROR("获取elf文件格式字段失败");
    void*   old_strlen = ( void*)strlen;
    nResult = xh_elf_find_and_replace_func(&elfso, (void *)new_strlen,"strlen");
    if (nResult == -1) XH_LOG_ERROR("替换函数失败");
    XH_LOG_INFO("成功替换函数地址");

}



void __attribute__((constructor)) init_hook(){
    XH_LOG_INFO("init_hook begin...");

    hook("libgothook64.so","strlen");

    XH_LOG_INFO("init_hook end...");
}




extern "C" JNIEXPORT jstring JNICALL
Java_com_example_gothook64_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    XH_LOG_INFO("hello len = %d ",strlen("hello"));
    return env->NewStringUTF(hello.c_str());
}