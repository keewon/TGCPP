
#ifndef DUMB_ALLOCATION_SYSTEM_H
#define DUMB_ALLOCATION_SYSTEM_H

#include <new>

class DumbAllocationSystem
	{
	protected:
		int* buf;
		int* next;
		int* end;
		unsigned int total_free;
	public:
		DumbAllocationSystem() : buf(NULL), next(NULL), end(NULL), total_free(0) {}
		virtual ~DumbAllocationSystem() { finalize(); }
		void* alloc(size_t size);
		void free(void* p, size_t size);
		bool initialize(size_t pool_init_size, size_t pool_additional_size);
		void reset(void);
		void finalize(void);
	};

void* das_alloc(size_t size);
void das_free(void* p, size_t size);
void das_set_global(DumbAllocationSystem *a);

#endif
