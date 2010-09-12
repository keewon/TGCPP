// Dumb Allocation System

//#define DEBUG_DAS

#include "DumbAllocator.h"
#ifdef DEBUG_DAS
#include <stdio.h>
#endif

#include <stdlib.h>

static DumbAllocationSystem *gDAS = NULL;

void das_set_global(DumbAllocationSystem *a)
{
	gDAS = a;
}

void* das_alloc(size_t size)
{
	return gDAS->alloc(size);
}

void das_free(void* p, size_t size)
{
	return gDAS->free(p, size);
}


bool DumbAllocationSystem::initialize(size_t pool_size, size_t pool_additional_size)
{
    if (pool_size == 0)
        return false;

    size_t s = (pool_size + sizeof(int*)-1) / sizeof(int*);

    buf = (int*)malloc( s * sizeof(int*) );
    if (buf)
    {
        next = buf;
        end = buf + s;
        return true;
    }

    total_free = 0;
    next = buf;
    return false;
}

void* DumbAllocationSystem::alloc(size_t size)
{
    size_t s = (size + sizeof(int*)-1) / sizeof(int*);

    if (s==0)
        return NULL;

    if (end >= next + s)
    {
        int* result = next;
        next += s;
        return (void*)result;
    }
    return NULL;
}

void DumbAllocationSystem::free(void* p, size_t size)
{
    total_free+=size;
}

void DumbAllocationSystem::reset(void)
{
#ifdef DEBUG_DAS
    printf("total alloc: %u\n", ((unsigned int)next-(unsigned int)buf));
    printf("total free: %u\n", total_free);
#endif
    next = buf;
	total_free = 0;
}

void DumbAllocationSystem::finalize(void)
{
    if (buf)
    {
        ::free(buf);
    }
    buf = NULL;
    end = NULL;
    next = NULL;
	total_free = 0;
}
