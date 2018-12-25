#ifndef VECTOR_H
#define VECTOR_H

#include "../src/allocator.h"
#include "../src/constructor.h"
#include "../src/uninitialized.h"

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
            uninitialized_fill_n(start, n, x);
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
        size_type size() const {
            return size_type(end() - begin());
        }
        size_type capacity() const {
            return size_type(end_of_storage() - begin());
        }
        bool empty() const {
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
            if (finish != end_of_storage()) {
                constructe(finish, x);
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
}

#endif
