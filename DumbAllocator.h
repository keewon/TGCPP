#ifndef DUMB_ALLOCATOR_H
#define DUMB_ALLOCATOR_H

#include "DAS.h"

template <typename T> class dumb_allocator;

template <> class dumb_allocator<void>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    // reference to void members are impossible.
    typedef void value_type;
    template <class U> 
        struct rebind { typedef dumb_allocator<U> other; };
};    

namespace dumb_alloc{
    inline void destruct(char *){}
    inline void destruct(wchar_t*){}
    template <typename T> 
        inline void destruct(T *t){t->~T();}
} // namespace

    
template <typename T>
class dumb_allocator
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> 
        struct rebind { typedef dumb_allocator<U> other; };
    dumb_allocator(){}
    pointer address(reference x) const {return &x;}
    const_pointer address(const_reference x) const {return &x;}
    pointer allocate(size_type size, dumb_allocator<void>::const_pointer hint = 0)
    {
        return static_cast<pointer>(
                das_alloc( size*sizeof(T) )
                );
    }
    //for Dinkumware:
    char *_Charalloc(size_type n){return static_cast<char*>(
            das_alloc( n )
            );
    }
    // end Dinkumware

    template <class U> dumb_allocator(const dumb_allocator<U>&){}
    void deallocate(pointer p, size_type n)
    {
        das_free(p, n);
    }
    void deallocate(void *p, size_type n)
    {
        das_free(p, n);
    }
    size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
    void construct(pointer p, const T& val)
    {
        new(static_cast<void*>(p)) T(val);
    }
    void construct(pointer p)
    {
        new(static_cast<void*>(p)) T();
    }
    void destroy(pointer p){dumb_alloc::destruct(p);}
    
};

template <typename T, typename U>
inline bool operator==(const dumb_allocator<T>&, const dumb_allocator<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const dumb_allocator<T>&, const dumb_allocator<U>){return false;}


// For VC6/STLPort 4-5-3 see /stl/_alloc.h, line 464
// "If custom allocators are being used without member template classes support :
// user (on purpose) is forced to define rebind/get operations !!!"
#ifdef _WIN32
#define DUMB_ALLOC_CDECL __cdecl
#else
#define DUMB_ALLOC_CDECL
#endif

namespace std{
template <class _Tp1, class _Tp2>
inline dumb_allocator<_Tp2>& DUMB_ALLOC_CDECL
__stl_alloc_rebind(dumb_allocator<_Tp1>& __a, const _Tp2*) 
{  
    return (dumb_allocator<_Tp2>&)(__a); 
}


template <class _Tp1, class _Tp2>
inline dumb_allocator<_Tp2> DUMB_ALLOC_CDECL
__stl_alloc_create(const dumb_allocator<_Tp1>&, const _Tp2*) 
{ 
    return dumb_allocator<_Tp2>(); 
}

} // namespace std
// end STLPort

#endif


