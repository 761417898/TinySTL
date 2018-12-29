#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "alloc.h"
#include "constructor.h"
#include <iostream>

namespace tinystl {
    /*
     *以变量数量为单位分配
    */
    template<class T>
    class allocator {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    public:
        static T* allocate();
        static T* allocate(size_t n);
        static void deallocate(T *ptr);
        static void deallocate(T *ptr, size_t n);
    };

    template<class T>
    T *allocator<T>::allocate() {
        return static_cast<T*>(alloc::allocate(sizeof(T)));
    }

    template<class T>
    T *allocator<T>::allocate(size_t n) { 
        return static_cast<T*>(alloc::allocate(sizeof(T) * n));
    }

    template<class T>
    void allocator<T>::deallocate(T *ptr) {
        alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
    }

    template<class T>
    void allocator<T>::deallocate(T *ptr, size_t n) {
        alloc::deallocate(static_cast<void*>(ptr), sizeof(T) * n);
    }
}

#endif // ALLOCATOR_H

