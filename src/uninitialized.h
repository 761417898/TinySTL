#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "iterator.h"
#include "__type_traits.h"
#include <iostream>

namespace tinystl {
/******************* uninitialized_fill_n *************************/
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
        Size n, const T& x, __true_type) {
    //fix, 应该调用algorithm中的fill_n 
    ForwardIterator cur = first;
    while (n--) {
        *cur = x;
        ++cur;
    }
    return cur;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
        Size n, const T& x, __false_type) {
    ForwardIterator cur = first;
    while (n--) {
        construct(&*cur, x);
        ++cur;
    }
    return cur;
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
        Size n, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD_type;
    return __uninitialized_fill_n_aux(first, n, x, is_POD_type());
}
    
template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
        Size n, const T& x) {
    return __uninitialized_fill_n(first, n, x, value_type(first));
}

/******************* uninitialized_copy ***************************/
template<class InputIterator, class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first,
        InputIterator last, ForwardIterator result, __true_type) {
    //return copy(first, last, result);
    ForwardIterator cur = result;
    for (; first != last; ++first, ++cur) {
        *cur = *first;
    }
    return cur;
}

template<class InputIterator, class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first,
        InputIterator last, ForwardIterator result, __false_type) {
    ForwardIterator cur = result;
    for (; first != last; ++first, ++cur) {
        construct(&*cur, *first);
    }
    return cur;
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first,
        InputIterator last, ForwardIterator result, T*) {
    typedef typename __type_traits<T>::is_POD_type is_POD_type;
    return __uninitialized_copy_aux(first, last, result, is_POD_type());
}
    
template<class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,
        InputIterator last, ForwardIterator result) {
    return __uninitialized_copy(first, last, result, value_type(result));
}

/******************* uninitialized_fill ***************************/
template<class ForwardIterator, class T>
inline ForwardIterator __uninitialized_fill_aux(ForwardIterator first,
        ForwardIterator last, const T& x, __true_type) {
    //fill(first, last, x)
    ForwardIterator cur = first;
    while (cur != last) {
        *cur = x;
        ++cur;
    }
}

template<class ForwardIterator, class T>
inline ForwardIterator __uninitialized_fill_aux(ForwardIterator first,
        ForwardIterator last, const T& x, __false_type) {
    ForwardIterator cur = first;
    while (cur != last) {
        construct(&*cur, x);
        ++cur;
    }
}

template<class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first,
        ForwardIterator last, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD_type;
    __uninitialized_fill_aux(first, last, x, is_POD_type());
} 


template<class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first,
        ForwardIterator last, const T& x) {
    __uninitialized_fill(first, last, x, value_type(first));
}

}
#endif // UNINITIALED_H

