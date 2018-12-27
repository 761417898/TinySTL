#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cstring>

#include "iterator.h"
#include "__type_traits.h"

namespace tinystl {
    /*********************** copy  ******************************************/
    /*
     *copy -> __copy_dispatch -> (迭代器)__copy
     *copy -> __copy_dispatch -> __copy_t -> trivial memmove
     *copy -> __copy_dispatch -> __copy_t -> non-trivial __copy_d
    */

    template<typename InputIterator,typename OutputIterator>
    struct __copy_dispatch {
        OutputIterator operator()(InputIterator first,InputIterator last,OutputIterator result) {
            return __copy(first,last,result,iterator_category(first));
        }
    };
    template<typename T>
    struct __copy_dispatch<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial_assignment;
            return __copy_t(first,last,result,has_trivial_assignment());
        }
    };
    template<typename T>
    struct __copy_dispatch<T*, T*> {
        T* operator()(T* first,T* last,T* result) {
            typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial_assignment;
            return __copy_t(first,last,result,has_trivial_assignment());
        }
    };

    template<typename InputIterator, typename OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }

    char *copy(const char *first, const char *last, char *result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    template<typename InputIterator,typename OutputIterator>
    OutputIterator __copy(InputIterator first,InputIterator last,OutputIterator result,input_iterator_tag) {
        while(first != last) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }

    template<typename RandomAccessIterator,typename OutputIterator>
    OutputIterator __copy(RandomAccessIterator first,RandomAccessIterator last, OutputIterator result,random_access_iterator_tag) {
        return __copy_d(first, last, result, iterator_traits<RandomAccessIterator>::difference_type());
    }

    template<typename T>
    T* __copy_t(const T* first,const T* last,T* result,__false_type) {
        return __copy_d(first,last,result,(ptrdiff_t*)0);
    }

    template<typename T>
    T* __copy_t(const T* first,const T* last,T* result,__true_type) {
        //trivial使用更高效率的memmove
        memmove(result,first,(last-first)*sizeof(T));
        return result + (last - first);
    }

    template<typename RandomIterator,typename OutputIterator,typename Distance>
    OutputIterator __copy_d(RandomIterator first,RandomIterator last,OutputIterator result,Distance*) {
        Distance n = last - first;
        while (n--) {
            *result = *first;
            ++result;
            ++first;
        }
        return result;
    }

    /*********************** copy_back  ******************************************/
    template<typename InputIterator,typename OutputIterator,typename Distance>
    OutputIterator __copy_d_backward(InputIterator first,InputIterator last,OutputIterator result,Distance*) {
        Distance n = last - first;
        while (n--) {
            *(result-1) = *(last-1);
            --result;
            --last;
        }
        return result;
    };


    template<typename InputIterator,typename OutputIterator>
    OutputIterator __copy_t_backward(InputIterator first,InputIterator last,OutputIterator result,input_iterator_tag) {
        while(last != first) {
            *(result-1) = *(last - 1);
            --last;
            --result;
        }
        return result;
    };

    template<typename InputIterator,typename OutputIterator>
    OutputIterator __copy_t_backward(InputIterator first,InputIterator last,OutputIterator result,random_access_iterator_tag) {
        return __copy_d_backward(first,last,result,difference_type(first));
    };

    template<typename InputIterator,typename OutputIterator>
    struct __copy_dispatch_backward {
        OutputIterator operator()(InputIterator first,InputIterator last,OutputIterator result) {
            return __copy_t_backward(first,last,result,iterator_category(first));
        }
    };

    template<typename T>
    struct __copy_dispatch_backward<T*,T*> {
        T* operator()(T* first,T* last,T* result) {
            return __copy_d_backward(first,last,result,(ptrdiff_t*)(0));
        }
    };

    template<typename T>
    struct __copy_dispatch_backward<const T*,T*> {
        T* operator()(const T* first,const T* last,T* result) {
            return __copy_d_backward(first,last,result,(ptrdiff_t*)(0));
        }
    };

    template<typename InputIterator,typename OutputIterator>
    OutputIterator copy_backward(InputIterator first,InputIterator last,OutputIterator result) {
        return	__copy_dispatch_backward<InputIterator,OutputIterator>()(first,last,result);
    }

    /******************************* fill ***************************************************/
    template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; ++first)
			*first = value;
	}
	inline void fill(char *first, char *last, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), last - first);
	}
	inline void fill(wchar_t *first, wchar_t *last, const wchar_t& value)
	{
		memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
	}
	/**************************** fill_n **********************************************/
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
	template<class Size>
	char *fill_n(char *first, Size n, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}
	template<class Size>
	wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value)
	{
		memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
		return first + n;
	}

	/************************ max **********************************/
	template<typename T>
    T max(const T& a,const T& b)
    {
        return a > b ? a : b;
    }
}

#endif // ALGORITHM_H

