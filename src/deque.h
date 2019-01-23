#ifndef DEQUE_H
#define DEQUE_H

#include "algorithm.h"
#include "allocator.h"
#include "constructor.h"

namespace tinystl {
    template<class T, class Alloc = allocator<T>, size_t BufSize = 0>
    class deque {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef size_t size_type;
    protected:
        typedef pointer* map_pointer;
        map_pointer map;
        size_type map_size;
    };

    inline size_t __deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    struct __deque_iterator {
        typedef __deque_iterator<T, T&, T*, BufSize> iterator;
        typedef __deque_iterator<T, const T&, const T*, BufSize> const_iterator;
        static size_t buffer_size() {
            return __deque_buf_size(BufSize, sizeof(T));
        }

        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr Pointer;
        typedef Ref reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T **map_pointer;

        typedef __deque_iterator<T, Ref, Ptr, BufSize> self;
        T *cur;
        T *first;
        T *last;
        map_pointer node;

        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        reference operator*() const {
            return *cur;
        }

        pointer operator->() const {
            return &(operator*());
        }

        difference_type operator-(const self& x) const {
            return difference_type(buffer_size()) * (node - x.node - 1) +
                (cur - first) + (x.last - x.cur);
        }

        self& operator++() {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        //后置++
        self& operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self& operator--() {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        //后置--
        self& operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
        
        self operator+=(difference_type n) {
            difference_type offset = n + (cur - first);
            if (offset >= 0 && offset < difference_type(buffer_size()))
                cur += n;
            else {
                difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
                    : -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type((buffer_size())));
            }
            return *this;
        }

        self operator+(difference_type n) const {
            self tmp = *this;
            return tmp += n;
        }

        self& operator-=(difference_type n) {
            return *this += -n;
        }

        self operator-(difference_type n) const {
            self tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }

        bool operator==(const self& x) const {
            return cur == x.cur;
        }

        bool operator!=(const self& x) const {
            return !(*this == x);
        }

        bool operator<(const self& x) const {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
    };begin() == list_int.end());
        assert(list_int.size == 

    template<class T, class Alloc = alloc, size_t BufSize = 0>
    class deque {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef size_t size_type;
    public:
        typedef __deque_iterator<T, T&, T*, BufSize> iterator;
    protected:
        typedef pointer* map_pointer;
    protected:
        iterator start;
        iterator finish;
        map_pointer map;
        size_type map_size;
    public:
        iterator begin() {
            return start;
        }

        iterator end() {
            return finish;
        }

        reference operator[](size_type n) {
            return start[difference_type(n)];
        }

        reference front() {
            return *start;
        }

        reference back() {
            iterator tmp = finish;
            --tmp;
            return *tmp;
        }

        size_type size() const {
            return finish - start;
        }

        size_type max_size const {
            return size_type(-1);
        }

        bool empty() const {
            return finish == start;
        }
    protected:
        typedef Alloc data_allocator;
        typedef allocator<pointer> map_pointer;
        void fill_initialize(size_type n, const value_type& value);
        void create_map_and_nodes(size_type num_elements);
        void push_back_aux(const value_type& t);
        void push_front_aux(const value_type& t);
        void reserve_map_at_back(size_type nodes_to_add = 1) {
            if (nodes_to_add + 1 > map_size - (finish.node - map))
                reallocate_map(nodes_to_add, false);
        }
        void reserve_map_at_front(size_type nodes_to_add = 1) {
            if (nodes_to_add > start.node - map)
                reallocate_map(nodes_to_add, true);
        }
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        void pop_back_aux();
        void pop_front_aux();
        iterator insert_aux(iterator pos, const value_type& x);
    public:
        deque(int n, const value_type& value) : start(), finish(), map(nullptr), map_size(0){
            fill_initialize(n, value);
        }
        void push_back(const value_type& t) {
            if (finish.cur != finish.last - 1) {
                construct(finish.cur, t);
                ++finish.cur;
            } else {
                push_back_aux(t);
            }
        }
        void push_front(const value_type& t) {
            if (start.cur != start.first) {
                constructe(start.cur - 1, t);
                --start.cur;
            } else
                push_front_aux(t);
        }
        void pop_back() {
            if (finish.cur != finish.first) {
                --finish.cur;
                destory(finish.cur);
            } else {
                pop_back_aux();
            }
        }
        void pop_front() {
            if (start.cur != start.last - 1) {
                destory(start.cur);
                ++start.cur;
            } else {
                pop_front_aux();
            }
        }
        void clear();
        iterator erase(iterator pos) {
            iterator next = pos;
            ++next;
            difference_type index = pos - start;
            if (index < (size() >> 1)) {
            //如果清除点之前的元素比较少，就移动清除点之前的元素
                copy_back(start, pos, next);
                pop_front();
            } else {
            //反之，移动清除点之后的元素
                copy(next, finish, pos);
                pop_back();
            }
            return start + index;
        }
        iterator erase(iterator first, iterator last);
        iterator insert(iterator position, const value_type& x) {
            if (position.cur == start.cur) {
                push_front(x);
                return start;
            } else if (position.cur = finish.cur) {
                push_back(x);
                iterator tmp = finish;
                --tmp;
                return tmp;
            } else {
                return insert_aux(position, x);
            }
        }
    };

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
        create_map_and_nodes(n);
        map_pointer cur;
        for (cur = start.node; cur < finish.node; ++cur)
            uninitialized_fill(*cur, *cur + buffer_size(); value);    
        uninitialized_fill(finish.first, finish.cur, value);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
        size_type num_nodes = num_elements / buffer_size() + 1;
        map_size = max(initial_map_size(), num_nodes + 2);
        map = map_pointer::allocator(map_size);
        //另nstart和nfinish指向所有nodes节点的最中间区段，使得两头的扩充量一样大
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;
        map_pointer cur;
        for (cur = nstart; cur <= nfinish; ++cur) {
            *cur = allocate_node();
        }
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.cur + num_elements % buffer_size();
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_back();//符合条件重新换一个map
        *(finish.node + 1) = allocate_node();
        constructe(finish.cur, t_copy);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_front();
        *(start.node - 1) = allocate_node();
        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        construct(start.cur, t_copy);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;
        map_pointer new_nstart;
        if (map_size > 2 * new_num_nodes) {
            new_nstart = map + (map_size - new_num_nodes) / 2
                    + (add_at_front ? nodes_to_add : 0);
            if (new_nstart < start.node)
                copy(start.node, finish.node + 1, new_nstart);
            else
                copy_back(start.node, finish.node + 1, new_nstart + old_num_nodes);
        } else {
            size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            copy(start.node, finish.node + 1, new_nstart);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num_nodes - 1);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, ALloc, BufSize>::pop_back_aux() {
        deallocate_node(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destory(finish.cur);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, ALloc, BufSize>::pop_front_aux() {
        destory(start.cur);
        deallocate_node(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::clear() {
        for (map_pointer node = start.node + 1; node < finish.node; ++node) {
            destory(*node, *node + buffer_size());
            data_allocator::deallocate(*node, buffer_size());
        }
        if (start.node != finish.node) {
            //只留下头缓冲区
            destory(start.cur, start, last);
            destory(finish.first, finish.cur);
            data_allocator::deallocate(finish.first, buffer_size());
        } else {
            destory(start,cur, finish.cur);
        }
        finish = start;
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::erase(iterator first, iterator last) {
        if (first == start && last == finish) {
            clear();
            return finish;
        } else {
            difference_type n = last - first;
            difference_type elems_before = first - start;
            if (elems_before < (size() - n) / 2) {
                //前面的元素较少
                copy_backward(start, first, last);
                iterator new_start = start + n;
                destory(start, new_start);
                for (map_pointer cur = start.node; cur < new_start.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size());
                start = new_start;
            } else {
                copy(last, finish, first);
                iterator new_finish = finish - n;
                destory(new_finish, finish);
                for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size());
                finish = new_finish;
            }
            return start + elems_before;
        }
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& x) {
        difference_type index = pos - start;
        value_type x_copy = x;
        if (index < size() / 2) {
            push_front(front());
            iterator front1 = start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            pos = start + index;
            iterator pos1 = pos;
            ++pos1;
            copy(front2, pos1, front1);
        } else {
            push_back(back());
            iterator back1 = finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = start + index;
            copy_backward(pos, back2, back1);
        }
        *pos = x_copy;
        return pos;
    }
}

#endif
