//
// Created by zhangxinan on 2024/6/6.
//
#include <stdint.h>
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
