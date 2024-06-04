
//xhook对于hash节的查询
static int xh_elf_hash_lookup(xh_elf_t *self, const char *symbol, uint32_t *symidx)
{
    uint32_t    hash = xh_elf_hash((uint8_t *)symbol);
    const char *symbol_cur;
    uint32_t    i;

    for(i = self->bucket[hash % self->bucket_cnt]; 0 != i; i = self->chain[i])
    {
        symbol_cur = self->strtab + self->symtab[i].st_name;

        if(0 == strcmp(symbol, symbol_cur))
        {
            *symidx = i;
            XH_LOG_INFO("found %s at symidx: %u (ELF_HASH)\n", symbol, *symidx);
            return 0;
        }
    }

    return XH_ERRNO_NOTFND;
}

//xhook对于gnu.hash节的查询
static int xh_elf_find_symidx_by_name(xh_elf_t *self, const char *symbol, uint32_t *symidx)
{
    if(self->is_use_gnu_hash)
        return xh_elf_gnu_hash_lookup(self, symbol, symidx);
    else
        return xh_elf_hash_lookup(self, symbol, symidx);
}

static int xh_elf_gnu_hash_lookup(xh_elf_t *self, const char *symbol, uint32_t *symidx)
{
    if(0 == xh_elf_gnu_hash_lookup_def(self, symbol, symidx)) return 0;
    if(0 == xh_elf_gnu_hash_lookup_undef(self, symbol, symidx)) return 0;
    return XH_ERRNO_NOTFND;
}


static int xh_elf_gnu_hash_lookup_def(xh_elf_t *self, const char *symbol, uint32_t *symidx)
{
    uint32_t hash = xh_elf_gnu_hash((uint8_t *)symbol);

    static uint32_t elfclass_bits = sizeof(ElfW(Addr)) * 8;
    size_t word = self->bloom[(hash / elfclass_bits) % self->bloom_sz];
    size_t mask = 0
                  | (size_t)1 << (hash % elfclass_bits)
                  | (size_t)1 << ((hash >> self->bloom_shift) % elfclass_bits);

    //if at least one bit is not set, this symbol is surely missing
    if((word & mask) != mask) return XH_ERRNO_NOTFND;

    //ignore STN_UNDEF
    uint32_t i = self->bucket[hash % self->bucket_cnt];
    if(i < self->symoffset) return XH_ERRNO_NOTFND;

    //loop through the chain
    while(1)
    {
        const char     *symname = self->strtab + self->symtab[i].st_name;
        const uint32_t  symhash = self->chain[i - self->symoffset];

        if((hash | (uint32_t)1) == (symhash | (uint32_t)1) && 0 == strcmp(symbol, symname))
        {
            *symidx = i;
            XH_LOG_INFO("found %s at symidx: %u (GNU_HASH DEF)\n", symbol, *symidx);
            return 0;
        }

        //chain ends with an element with the lowest bit set to 1
        if(symhash & (uint32_t)1) break;

        i++;
    }

    return XH_ERRNO_NOTFND;
}

static int xh_elf_gnu_hash_lookup_undef(xh_elf_t *self, const char *symbol, uint32_t *symidx)
{
    uint32_t i;

    for(i = 0; i < self->symoffset; i++)
    {
        const char *symname = self->strtab + self->symtab[i].st_name;
        if(0 == strcmp(symname, symbol))
        {
            *symidx = i;
            XH_LOG_INFO("found %s at symidx: %u (GNU_HASH UNDEF)\n", symbol, *symidx);
            return 0;
        }
    }
    return XH_ERRNO_NOTFND;
}

case DT_HASH:
{
//ignore DT_HASH when ELF contains DT_GNU_HASH hash table
if(1 == self->is_use_gnu_hash) continue;

raw = (uint32_t *)(self->bias_addr + dyn->d_un.d_ptr);
if((ElfW(Addr))raw < self->base_addr) return XH_ERRNO_FORMAT;
self->bucket_cnt  = raw[0];
self->chain_cnt   = raw[1];
self->bucket      = &raw[2];
self->chain       = &(self->bucket[self->bucket_cnt]);
break;
}
case DT_GNU_HASH:
{
raw = (uint32_t *)(self->bias_addr + dyn->d_un.d_ptr);
if((ElfW(Addr))raw < self->base_addr) return XH_ERRNO_FORMAT;
self->bucket_cnt  = raw[0];
self->symoffset   = raw[1];
self->bloom_sz    = raw[2];
self->bloom_shift = raw[3];
self->bloom       = (ElfW(Addr) *)(&raw[4]);
self->bucket      = (uint32_t *)(&(self->bloom[self->bloom_sz]));
self->chain       = (uint32_t *)(&(self->bucket[self->bucket_cnt]));
self->is_use_gnu_hash = 1;
break;
}


