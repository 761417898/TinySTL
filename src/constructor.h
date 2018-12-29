#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include <new> //placement_new

#include "iterator.h"
#include "__type_traits.h"
#include <iostream>

namespace tinystl {
    template<class T1, class T2>
    inline void construct(T1 *p, const T2 &value) {
        //placement_new
        new(p) T1(value);
    }

    template<class T>
    inline void destory(T *pointer) {
        pointer->~T();
    }

    template<class ForwardIterator>
    void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

    template<class ForwardIterator>
    void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
        for (; first != last; ++first) {
            destory(&*first);
        }
    }

    template<class ForwardIterator, class T>
    void __destory(ForwardIterator first, ForwardIterator last, T*) {
        //type_traits
        typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
        __destory_aux(first, last, trivial_destructor());
    }

    template<class ForwardIterator>
    void destory(ForwardIterator first, ForwardIterator last) {
        //iterator_traits
        __destory(first, last, value_type(first));
    }
}

#endif // CONSTRUCTOR_H

