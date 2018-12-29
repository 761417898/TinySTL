#ifndef VECTOR_H
#define VECTOR_H

#include "algorithm.h"
#include "allocator.h"
#include "constructor.h"
#include "iterator.h"
#include "uninitialized.h"

namespace tinystl {
    template<class T, class Alloc = allocator<T> >
    class vector {
    public:
        //内嵌型别
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    protected:
        typedef Alloc data_allocator;
        iterator start;
        iterator finish;
        iterator end_of_storage;

        void insert_aux(iterator position, const T&x);
        void deallocate() { 
            if (start) {
                data_allocator::deallocate(start, end_of_storage - start);
            }
        }
        void fill_initialize(size_type n, const T& value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }
        iterator allocate_and_fill(size_type n, const T&x) {
            iterator result = data_allocator::allocate(n);
            uninitialized_fill_n(result, n, x); 
            return result;
        }
    public:
        //访问
        iterator begin() {
            return start;
        }
        iterator end() {
            return finish;
        }
        size_type size() {
            return static_cast<size_type>(end() - begin());
        }
        size_type capacity() {
            return static_cast<size_type>(end_of_storage() - begin());
        }
        bool empty() {
            return begin() == end();
        }
        reference operator[](size_type n) {
            return *(begin() + n);
        }
        //构造
        vector() : start(0), finish(0), end_of_storage(0) {}
        vector(size_type n, const T& value) {
            fill_initialize(n, value);
        }
        vector(int n, const T& value) {
            fill_initialize(n, value);
        }
        vector(long n, const T& value) {
            fill_initialize(n, value);
        }
        explicit vector(size_type n) {
            fill_initialize(n, T());
        }

        ~vector() {
            destory(start, finish);
            deallocate();
        }
        //element
        reference front() {
            return *begin();
        }
        reference back() {
            return *(end() - 1);
        }
        void push_back(const T& x) {
            if (finish != end_of_storage) {
                construct(finish, x);
                ++finish;
            }
            insert_aux(end(), x);
        }
        void pop_back() {
            --finish;
            destory(finish);
        }
        iterator erase(iterator position) {
            //fix，应该使用Algorithm中的copy
            if (position != end()) 
                uninitialized_copy(position + 1, end(), position);
            --finish;
            destory(finish);
            return position;
        }
        iterator erase(iterator first, iterator last) {
            //fix
            iterator i = uninitialized_copy(last, finish, first);
            destory(i, finish);
            finish = finish - (last - first);
            return first;
        }

        //insert
        iterator insert (iterator position, const T& val);
	    iterator insert (iterator position, size_type n, const T& val);

        void resize(size_type new_size, const T& x) {
            if (new_size < size()) {
                erase(begin() + new_size, end());
            } else {
                insert(end(), new_size - size(), x);
            }
        }
        void resize(size_type new_size) {
            resize(new_size, T());
        }
        void clear() {
            erase(begin(), end());
        }
    };

    template<typename T,typename Alloc>
    void vector<T,Alloc>::insert_aux(iterator position,const T& value)
    {
	    if(finish!=end_of_storage)
	    {
		    construct(finish,*(finish - 1));
		    ++finish;
		    copy_backward(position,finish-2,finish-1);
		    *position = value;
	    }
	    else
	    {
		    const size_type old_size = size();
		    const size_type new_size = (old_size == 0 ? 1 : 2 * old_size);

		    iterator new_start = data_allocator::allocate(new_size);
		    iterator new_finish =uninitialized_copy(start,position,new_start);

		    construct(new_finish,value);
		    ++new_finish;
		    uninitialized_copy(position,finish,new_finish);
            
            destory(begin(), end());
		    deallocate();
		    start = new_start;
		    finish = new_finish;
		    end_of_storage = start+new_size;
	    }
    }

    template<typename T,typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::insert (iterator position, const T& val) {
        return vector<T, Alloc>::insert_aux(position, val);
    }

    template<typename T,typename Alloc>
    typename vector<T, Alloc>::iterator vector<T,Alloc>::insert(iterator position,  size_type n, const T& val)
    {
	    if(end_of_storage - finish>=n) {
		    size_type m = finish - position;
		    iterator old_finish = finish;
		    if(m >= n ) {
			    uninitialized_copy(finish-n,finish,finish);
			    finish += n;
			    copy_backward(position,old_finish-n,old_finish);
		    	fill(position,position+n,val);
			
		    } else {
		    	fill(position,old_finish,val);
		    	finish = uninitialized_fill_n(finish,n-m,val);
		    	finish = uninitialized_copy(position,old_finish,finish);
		    }
		    return position;
	    } else {
		    const size_type old_size = size();
		    const size_type new_size = old_size + max(n,old_size);
		    const difference_type offset = position - begin();
		    iterator new_start = data_allocator::allocate(new_size);
		    iterator new_finish =uninitialized_copy(start,position,new_start);

		    new_finish = uninitialized_fill_n(new_finish,n,val);
		    new_finish = uninitialized_copy(position,finish,new_finish);

		    destroy(begin(),end());
		    deallocate();

		    start = new_start;
		    finish = new_finish;
		    end_of_storage = start+new_size;
		    return start+offset;

	    }
    }
}

#endif
