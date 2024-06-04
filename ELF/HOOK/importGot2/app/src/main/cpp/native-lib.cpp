#include <jni.h>
#include <string>
#include <android/log.h>
#include <sys/mman.h>
#include "elf.h"
#include "unistd.h"
#include <dlfcn.h>

//copy for src:https://github.com/guoshan-yang/GotHook.git
#define LOGI(...) __android_log_print(4,"zxa",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"zxa",__VA_ARGS__)

// from bionic hash表的哈希函数
static unsigned elfhash(const char *_name) {
    const unsigned char *name = (const unsigned char *) _name;
    unsigned h = 0, g;

    while (*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}
//GNU hash func   gunhash的哈希函数
static uint32_t elf_gnu_hash(const char *name)
{
    uint32_t h = 5381;

    while(*name != 0)
    {
        h += (h << 5) + *name++;   //等价于 h = h * 33 + c;
    }
    return h;
}

#define LIB_NAME "libimportgot2.so"
//定义函数指针
typedef void* (*MALLOC)(size_t __byte_count);
MALLOC  malloc1 = malloc;

void* (*old_malloc)(size_t __byte_count);

void* new_malloc(size_t __byte_count){
    LOGI("new_malloc.");
    return old_malloc(__byte_count * 2);
}
void* new_malloc2(size_t __byte_count){
    LOGI("new_malloc2.\n");
    return old_malloc(__byte_count);
}
void* new_malloc3(size_t __byte_count){
    LOGI("new_malloc3.\n");
    return old_malloc(__byte_count);
}
//函数向前声明
void hooklibc();
//获取模块基址
long getModuleBase(pid_t pid , const char * name ){
    FILE* fp;
    unsigned long addr = 0;
    char* pch;
    char filename[128] = {0};
    char line[1024] = {0};

    if (pid < 0){

        snprintf(filename,sizeof(filename),"/proc/self/maps");

    }else{
        snprintf(filename,sizeof(filename),"/proc/%d/maps",pid);
    }

    fp = fopen(filename,"r");
    if (fp == NULL){
        LOGE("fopen failed  err = %s\n", strerror(errno));
        return addr;
    }
    while(fgets(line,sizeof(line),fp)){

        if (strstr(line,name)){
            LOGI("line:%s",line);
            pch = strtok(line,"-");
            addr = strtoul(pch,NULL,16);
            if (addr == 0x8000){
                addr = 0;
            }
            break;
        }
    }

    fclose(fp);
    return addr;
}
//开始hook
int hook_malloc(){
    int result = -1;
    LOGI("##############正在解析%s##############",LIB_NAME);
    long addr_base = getModuleBase(getpid(), LIB_NAME);
    if (addr_base == 0){
        LOGE("getModuleBase failed. err = %s\n", strerror(errno));
        return result;
    }
    LOGI("addr_base = %p\n", addr_base);
    old_malloc = malloc;
    //解析elf文件，拿到从程序头中获取动态节区表，解析动态节区表获取需要的节区地址
    //从动态节去表获取节区和从节表头中直接获取节区的区别是，动态节区表在程序头中，程序头不可修改，以保证拿到的节区是正确的
    Elf32_Ehdr * ehdr = (Elf32_Ehdr *)addr_base;

    Elf32_Phdr * phdr = (Elf32_Phdr * )(addr_base + ehdr->e_phoff);
    LOGI("phdr = %p\n", phdr);
    int pdhr_count = ehdr->e_phnum;

    //动态节信息
    long dynamicAddr = 0;
    int dynamicSize = 0;
    for (int i = 0; i < pdhr_count; ++i) {

        if (phdr[i].p_type == PT_DYNAMIC){

            dynamicAddr = addr_base + phdr[i].p_vaddr;
            dynamicSize = phdr->p_memsz;
            break;
        }

    }
    if (dynamicAddr == 0){
        LOGE("not found dynamicAddr. err = %s\n", strerror(errno));
    }
    LOGI("dynamicAddr = %p  dynamicSize = %d\n", dynamicAddr,dynamicSize);

    //遍历动态节区表，解析动态节区表
    Elf32_Dyn * dynTable = (Elf32_Dyn * )dynamicAddr;
    uint32_t jmpRelOff = 0;
    uint32_t strTableOff = 0;
    uint32_t pltRelsz = 0;
    uint32_t symTabOff = 0;
    uint32_t relTableOffset = 0;
    uint32_t relTableSize = 0;
    uint32_t relaTableOffset = 0;
    uint32_t relaTableSize = 0;
    uint32_t relEnt = 0;
    for (int i = 0; i < dynamicSize/ 8; ++i) {
        uint32_t value  = dynTable[i].d_un.d_val;
        switch (dynTable[i].d_tag) {
            //重定位表
            case DT_JMPREL:{
                jmpRelOff = value;
                LOGI("jmpRelOff = %p\n",jmpRelOff);
                break;
            }
            //字符串表
            case DT_STRTAB:{
                strTableOff = value;
                LOGI("strTableOff = %p\n",strTableOff);
                break;
            }
            //重定位表数量
            case DT_PLTRELSZ:{
                pltRelsz = value;
                LOGI("pltRelsz = %p\n",pltRelsz);
                break;
            }
            //符号表
            case DT_SYMTAB:{
                symTabOff = value;
                LOGI("symTabOff = %p\n",symTabOff);
                break;
            }

            case DT_REL:{
                relTableOffset = value;
                LOGI("relTableOffset = %p\n",relTableOffset);
                break;
            }

            case DT_RELSZ:{
                relTableSize = value;
                LOGI("relTableSize = %lx\n",relTableSize);
                break;
            }
            case DT_RELENT:{
                relEnt = value;
                LOGI("relEnt = %lx\n",relEnt);
                break;
            }
            case DT_RELA:{
                relaTableOffset = value;
                LOGI("relaTableOffset = %p\n",relaTableOffset);
                break;
            }
            case DT_RELASZ:{
                relaTableSize = value;
                LOGI("relaTableSize = %lx\n",relaTableSize);
                break;
            }
                //hash表
            case DT_HASH:{
                uint32_t nbucket = ((unsigned *)(addr_base +  value))[0];
                uint32_t nchain =  ((unsigned *)(addr_base + value))[1];
                char* bucket =  (char*)(addr_base + value + 8);
                char* chain = (char*)(addr_base + value + 8 + nbucket * 4);
                LOGI("nbucket = %lx  nchain = %lx bucket = %p chain = %p",nbucket, nchain, bucket, chain);
                break;
            }

        }

    }
    LOGI("##############正在解析重定位表1##############");
    //遍历重定位表 DT_JMPREL
    Elf32_Rel * rel_table = (Elf32_Rel *)(addr_base + jmpRelOff);
    for (int i = 0; i < pltRelsz / 8; ++i) {
        int number = ELF32_R_SYM(rel_table[i].r_info);
        int symtype= ELF32_R_TYPE(rel_table[i].r_info);

        //符号地址
        Elf32_Sym * symTableIndex = (Elf32_Sym *)(addr_base + symTabOff + number * 16);
        //LOGI("symtype = %d symTableIndex = %d",symtype,symTableIndex);
        //获取符号对应字符串
        char* strfunc = (char*)(addr_base+ strTableOff + symTableIndex->st_name );

        //判断是否hook函数
        if(strcmp(strfunc,"malloc") == 0){
            int pagesize = getpagesize();

            //获取分页起始地址
            uint32_t pageStart = ((rel_table[i].r_offset + addr_base) & (~(pagesize - 1)));
            LOGI("pageStart = %lx\n",pageStart);
            int n = mprotect((void*)pageStart,pagesize,PROT_READ | PROT_WRITE );

            //修改hook函数
            void**  gotaddr = (void**)(rel_table[i].r_offset + addr_base);
            LOGI("gotaddr = %p  n = %d",*gotaddr,n);
            *gotaddr = (void*)&new_malloc;
            LOGI("new gotaddr = %p",*gotaddr);
            mprotect((void*)pageStart,pagesize,PROT_READ );
            LOGI("n = %d",n);
            //break;
        }
    }
    LOGI("##############正在解析重定位表2##############");
    //遍历重定位表2 REL
    Elf32_Rel * rel_table2 = (Elf32_Rel *)(addr_base + relTableOffset);
    for (int i = 0; i < relTableSize / 8; ++i) {
        int number = ELF32_R_SYM(rel_table2[i].r_info);
        int symtype= ELF32_R_TYPE(rel_table2[i].r_info);

        //符号地址
        Elf32_Sym * symTableIndex = (Elf32_Sym *)(addr_base + symTabOff + number * 16);
        //LOGI("symtype = %d symTableIndex = %d",symtype,symTableIndex);
        //获取符号对应字符串
        char* strfunc = (char*)(addr_base+ strTableOff + symTableIndex->st_name );

        //判断是否hook函数
        if(strcmp(strfunc,"malloc") == 0){
            int pagesize = getpagesize();

            //获取分页起始地址
            uint32_t pageStart = ((rel_table2[i].r_offset + addr_base) & (~(pagesize - 1)));
            LOGI("plt pageStart = %lx\n",pageStart);
            int n = mprotect((void*)pageStart,pagesize,PROT_READ | PROT_WRITE );

            //修改hook函数
            void**  gotaddr = (void**)(rel_table2[i].r_offset + addr_base);
            LOGI("plt gotaddr = %p  n = %d",*gotaddr,n);
            *gotaddr = (void*)&new_malloc2;
            //__builtin___clear_cache((char*)pageStart,(char*)pageStart + 0x1000);
            LOGI("new plt gotaddr = %p",*gotaddr);
            mprotect((void*)pageStart,pagesize,PROT_READ );
            LOGI("n = %d",n);
            //break;
        }
    }

    //这里处理libc中malloc的符号表单独封装了个函数
        hooklibc();



    return result;

}

void hooklibc(){
    //hook libc.so > malloc
    LOGI("##############正在解析libc.so##############");
    uint32_t libcBase = getModuleBase(-1,"/system/lib/libc.so");
    LOGI("libcBase = %lx",libcBase);
    Elf32_Ehdr* libcEhdr = (Elf32_Ehdr*)libcBase;
    Elf32_Phdr * libcPhdr = (Elf32_Phdr * )(libcBase + libcEhdr->e_phoff);
    uint32_t  phdrNum = libcEhdr->e_phnum;
    Elf32_Shdr * libcShdr = (Elf32_Shdr * )(libcBase + libcEhdr->e_shoff);
    uint32_t  shdrNum = libcEhdr->e_shnum;

    //获取动态节
    Elf32_Dyn * symSection = NULL;
    uint32_t symSectionSize = 0;
    for (int i = 0; i < phdrNum; ++i) {
        if(libcPhdr[i].p_type == PT_DYNAMIC){
            symSection = ( Elf32_Dyn *)(libcBase + libcPhdr[i].p_vaddr);
            symSectionSize = libcPhdr->p_memsz;
        }
    }
    if (symSection == NULL){
        LOGE("not found symSection err = %s ", strerror(errno));
        return;
    }
    LOGI("symSection = %p",symSection);
    //解析动态节
    Elf32_Sym * symTab = NULL;
    uint32_t nbucket_hash = 0;
    uint32_t nchain_hash = 0;
    uint32_t * bucket_hash = NULL;
    uint32_t* chain_hash = NULL;
    char* strTab = NULL;
    Elf32_Rel * rel = NULL;
    uint32_t  relSize = 0;
    //gnu.hash表字段定义
    uint32_t nbucket_gnu = 0;
    uint32_t symOffset_gnu = 0;
    uint32_t nbloom_gnu = 0;
    uint32_t bloom_shift_gnu = 0;
    uint32_t * bloom_gnu = NULL;
    uint32_t *bucket_gnu = NULL;
    uint32_t * chain_gnu = NULL;

    uint32_t  is_use_gnu_hash = 0;

    for (int i = 0; i < symSectionSize / 8 ; ++i) {
        uint32_t value = libcBase  +  symSection[i].d_un.d_val;
        switch (symSection[i].d_tag) {
            //符号表
            case DT_SYMTAB:{
                symTab = (Elf32_Sym * )value;
                LOGI("DT_SYMTAB = %p",symTab);
                break;
            }
                //hash表
            case DT_HASH:{
                //.gnu.hash和hash表结构不一样，查询方式也不一样
                nbucket_hash = ((unsigned *)(value))[0];
                nchain_hash =  ((unsigned *)(value))[1];
                bucket_hash =  (uint32_t*)(char*)(value + 8);
                chain_hash = (uint32_t*)(char*)(value + 8 + nbucket_hash * 4);
                LOGI("[DT_HASH ]nbucket = %lx  nchain = %lx bucket = %p chain = %p",nbucket_hash, nchain_hash, bucket_hash, chain_hash);
                break;
            }
            case DT_GNU_HASH:{
                //.gnu.hash和hash表结构不一样，查询方式也不一样
                nbucket_gnu = ((unsigned *)(value))[0];
                symOffset_gnu =  ((unsigned *)(value))[1];
                nbloom_gnu =  ((unsigned *)(value))[2];
                bloom_shift_gnu =  ((unsigned *)(value))[3];
                bloom_gnu = (uint32_t*)(&((unsigned *)(value))[4]);
                bucket_gnu =  (uint32_t*)(&bloom_gnu[nbloom_gnu]);
                chain_gnu = (uint32_t*)(&bucket_gnu[nbucket_gnu]);
                is_use_gnu_hash = 1;
                LOGI("[DT_GNU_HASH] nbucket_gnu = %lx  symOffset_gnu = %lx nbloom_gnu = %lx bloom_shift_gnu = %lx \r\n"
                     "bloom_gnu = %p bucket_gnu = %p chain_gnu = %p "
                ,nbucket_gnu, symOffset_gnu, nbloom_gnu, bloom_shift_gnu,bloom_gnu,bucket_gnu,chain_gnu);

                break;
            }
                //字符串表
            case DT_STRTAB:{
                strTab = (char * )value;
                LOGI("DT_STRTAB = %p",strTab);
                break;
            }

        }
    }

    LOGI("before new_malloc3 addr = %p",new_malloc3);
    int   symIndex = -1;
    //这里处理hash表，如果有gnu_hash表则有先用这种快速hash表查找，为了熟悉gnuhash写的，没做很好的兼容，只实现了gnuhash查询，没做修改
    //这里做了分支处理，用了gnuhash就不能在查hash表了
    //这里gnuhash查询的方式借鉴了xhook的查询方式
    if (is_use_gnu_hash){
        //gnuhash查询

        //gnu_hash_def
        uint32_t hash = elf_gnu_hash("malloc");
        uint32_t elfClass_bias = sizeof(void*) * 8;
        size_t word = bloom_gnu[(hash / elfClass_bias) % nbloom_gnu];
        size_t mask = 0
                      | (size_t)1 << (hash % elfClass_bias)
                      | (size_t)1 << ((hash >> bloom_shift_gnu) % elfClass_bias);
        //
        if ((word & mask) != mask )
        {
            LOGE("gnu_hash lookup err mask");
            //return result;
        }
        //忽视未定义的
        uint32_t i = bucket_gnu[hash % nbucket_gnu];
        if (i < symOffset_gnu){

            LOGE("gnu hash lookup err symTabOff");
            //return result;
        }

        //在chain链中查找
        while(1){

            //非常牛逼的查询效率。一次就找对了
            const char* symname =  strTab + symTab[i].st_name;
            const uint32_t symhash = chain_gnu[i - symOffset_gnu];
            int a1 = (hash |(uint32_t)1);
            int a2 = (symhash | (uint32_t)1);
            int a3 =strcmp(symname,"malloc");
            if ( ( a1 == a2 )  && ( a3 == 0 )){
                symIndex = i;
                LOGI("found symname:%s at symIndex: %d (GNU_HASH DEF)\n", symname, symIndex);
                break;
            }
            if(symhash & (uint32_t)1 ) break;

            i++;
        }

        //gnu_hash_undef
        if (symIndex == -1){


            for(int j = 0;  j < symOffset_gnu;  j++)
            {
                const char *symname = libcBase + strTab + symTab[j].st_name;
                if(0 == strcmp(symname, "malloc"))
                {
                    symIndex = i;
                    LOGI("found symbol %s at symIndex: %d (GNU_HASH UNDEF)\n", symname, symIndex);
                    break;
                }
            }

        }


    }else{
        //hash查询
        uint32_t uhash =  elfhash("malloc");
        int a1 = uhash % nbucket_hash;
        symIndex = bucket_hash[elfhash("malloc") % nbucket_hash];
        LOGI("symindex = %d",symIndex);
        for (int i = symIndex; 0 != i ; i = chain_hash[i]) {

            if (strcmp(strTab + symTab[i].st_name,"malloc") == 0){
                LOGI("found symindex func");
                //修改符号表
                int pagesize = getpagesize();
                uint32_t  pageStart =( (uintptr_t)&symTab[i]  ) & (~(pagesize -1)) ;
                int n =  mprotect((void*)pageStart,pagesize,PROT_WRITE | PROT_READ);
                int off = (uintptr_t)&new_malloc3 - libcBase;
                LOGI("new_malloc3(%p) - libcBase(%lx) = off(%lx)",new_malloc3,libcBase,off);
                symTab[i].st_value =off;
                n =  mprotect((void*)pageStart,pagesize,PROT_WRITE | PROT_READ);
                symIndex = i;
                break;
            }
        }
    }



}

void __attribute__((constructor)) init_func(){
    //测试用的
    //getModuleBase(-1,"libimportgot2.so");
    //getModuleBase(-1,"/system/lib/libc.so");
    //在初始化函数之前开始hook
    hook_malloc();

}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_importgot2_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    LOGI("Java_com_example_importgot2_MainActivity_stringFromJNI");
    //测试hookgot表，输出new_malloc.表示hook成功
    void* addr = malloc(10);
    //测试hookplt表，输出new_malloc2.表示hook成功
    void* addr1 = malloc1(10);
    //格式addr1
    memset(addr,0xcc,10 * 2);
    //打印函数地址，测试用的
    LOGI("new_malloc = %p", new_malloc);
    LOGI("new_malloc2 = %p",new_malloc2);
    //测试hooklibc的函数表，输出new_malloc3.表示hook成功
    void* funcBase  = dlopen("libc.so",RTLD_NOW);
    LOGI("libc.so baseaddr = %p",funcBase);
    MALLOC malloc3 = (MALLOC)dlsym(funcBase,"malloc");
    void * malloc3addr = malloc3(10);
    memset(malloc3addr,0xbb,10);
    LOGI("alter new_malloc3 addr = %p",malloc3);
    return env->NewStringUTF(hello.c_str());
}