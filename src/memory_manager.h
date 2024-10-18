#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemoryBlock
{
    void *memory;
    size_t size;
    bool is_avalaible;
    MemoryBlock *next;
};


