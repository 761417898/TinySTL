#ifndef LIST_H
#define LIST_H

#include "allocator.h"
#include "constructor.h"
#include "iterator.h"
#include <iostream>

namespace tinystl {
    template<class T>
    struct __list_node {
        __list_node<T> *prev;
        __list_node<T> *next;
        T data;
        __list_node(const T& val = T()) : prev(nullptr), next(nullptr), data(val) {}

        bool operator==(const __list_node<T> &node) {
            return prev == node.prev && next == node.next && data == node.data;
        }

        bool operator!=(const __list_node<T> &node) {
            return prev != node.prev || next != node.next || data != node.data;
        }
    };

    template<class T>
    struct __list_iterator {
        typedef bidirectional_iterator_tag iterator_catrgory;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef __list_node<T>* link_type;
        typedef __list_iterator<T> self;

        link_type node;
        //constructor
        __list_iterator(const link_type& p = nullptr) : node(p) {}
        
        bool operator==(const __list_iterator<T> &iter) {
            return node == iter.node;
        }
        bool operator!=(const __list_iterator<T> &iter) {
            return node != iter.node;
        }
        reference operator*() {
            return (*node).data;
        }
        pointer operator->() {
            return &((*node).data);
        }
        self operator++() {
            node = node->next;
            return *this;
        }
        self operator++(int) {
            auto temp = node;
            node = node->next;
            return temp;
        }
        self operator--() {
            node = node->prev;
            return *this;
        }
        self operator--(int) {
            auto temp = node;
            node = node->prev;
            return temp;
        }
    };

    
    template<class T, class Alloc = allocator<__list_node<T> > >
    class list {
    public:
        typedef T value_type;
        typedef size_t size_type;
        typedef T& reference;
        typedef __list_node<T> *link_type;
        typedef __list_iterator<T> iterator;
        typedef __list_iterator<const T> const_iterator;
        typedef Alloc list_node_allocator;

    private:
        link_type node;
        
        link_type new_node(const T& val = T()) {
            link_type p = list_node_allocator::allocate();
            construct(p, __list_node<T>(val));
            return p;
        }
        void delete_node(link_type ptr) {
            destory(ptr);
            list_node_allocator::deallocate(ptr);
        }

    public:
        //iterator
        iterator begin() {
            return (*node).next;
        }
        iterator end() {
            return node;
        }
        //construct
        list() {
            node = new_node();
            node->prev = node->next = node;
        }
        ~list(){
            clear();
            delete_node(node);
        }

        bool empty() {
            return node->next == node;
        }

        size_type size() {
            size_type result = 0;
            iterator i = this->begin();
            iterator j = this->end();
            while (i != j) {
                ++i;
                ++result;
            }
            return result;
        }

        reference front() {
            return *begin();
        }
        reference back() {
            return *(--end());
        }

        void push_front(const T& val) {
            insert(begin(), val);
        }
        void push_back(const T& val) {
            insert(end(), val);
        }
        void pop_front() {
            erase(begin());
        }
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }

    public:
        iterator insert(iterator position, const T& val) {
            link_type tmp = new_node(val);
            tmp->next = position.node;
            tmp->prev = position.node->prev;
            (position.node->prev)->next = tmp;
            position.node->prev = tmp;
            //using namespace std;
            //cout << node->data << "   " << node->data << endl;
            return tmp;
        }
        //erase
        iterator erase(iterator position) {
            link_type next_node = position.node->next;
            link_type prev_node = position.node->prev;
            prev_node->next = next_node;
            next_node->prev = prev_node;
            delete_node(position.node);
            return iterator(next_node);
        }
        
        void clear() {
            link_type cur = node->next;
            while (cur != node) {
                link_type tmp = cur;
                cur = cur->next;
                delete_node(tmp);
            }
            node->next = node;
            node->prev = node;
        }

        void remove(const T& val) {
            iterator cur = begin();
            iterator last = end();
            while (cur != last) {
                if (cur.node->data == val) {
                    erase(cur);
                    return;
                }
                ++cur;
            }
        }

        void unique() {
            iterator first = begin();
            iterator last = end();
            if (first == last) {
                return;
            }
            iterator next = first;
            while (++next != last) {
                if (*first == *next) {
                    erase(next);
                } else {
                    first = next;
                }
                next = first;
            }
        }
        //将first-last之间的所有元素移动到position之前
        /*
         * 1 2 3 4 5 6 假设first 4, last 6, position 2,第一句是把5的后指针指向2，第二句把3的后指针指向6，第三句把1的后指针指向4，第四句暂存节点1
         * 第五句把2的前指针指向5，第六句把6的前指针指向3，第七句把4的前指针指向暂存的节点1
         */
        void transfer(iterator position, iterator first, iterator last) {
            if (position != last) {
                ((last.node)->prev)->next = position.node;
                ((first.node)->prev)->next = last.node;
                ((position.node)->prev)->next = first.node;
                link_type tmp = (position.node)->prev;
                position.node->prev = last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp;
            }
        }
        //拼接
        void splice(iterator position, list<T, Alloc> &new_list) {
            if (!new_list.empty()) {
                transfer(position, new_list.begin(), new_list.end());
            }
        }
        void splice(iterator position, list<T, Alloc>&, iterator i) {
            iterator j = i;
            ++j;
            if (position == i || position == j) {
                return;
            }
            transfer(position, i, j);
        }
        void splice(iterator position, list<T, Alloc>&, 
                iterator first, iterator last) {
            if (first != last) {
                transfer(position, first, last);
            }
        }
        //将new_list合并到自身，要求两个list已sort
        void merge(list<T, Alloc>& new_list) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = new_list.begin();
            iterator last2 = new_list.end();
            while (first1 != last1 && first2 != last2) {
                if (*first2 < *first1) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    first2 = next;
                } else {
                    ++first1;
                }
            }
            if (first2 != last2) {
                transfer(last1, first2, last2);
            }
        }
        //将*this的内容逆向重置
        void revese() {
            if (node->next == node || node->next->next == node) {
                return;
            }
            iterator first = begin();
            ++first;
            while (first != end()) {
                iterator old = first;
                ++first;
                transfer(begin(), old, first);
            }
        }

        void swap(list<T, Alloc> &new_list) {
            link_type tmp = node;
            node = new_list->node;
            new_list->node = tmp;
        }
        //STL sort参数必须是RandomAcessIterator
        //有点merge sort的意思又不是，
        void sort() {
            if (node->next == node || node->next->next == node) {
                return;
            }
            list<T, Alloc> carry;
            list<T, Alloc> counter[64];
            //count[i]暂存2*i个节点，故64足够了
            int fill = 0;
            while (!empty()) {
                carry.splice(carry.begin(), *this, begin());
                int i = 0;
                while(i < fill && !counter[i].empty()) {//这里的!counter[i].empty()，是用来对第i列最多2^i个元素控制的
                counter[i].merge(carry);                //清空carry，将它合并到counter[i]中
                carry.swap(counter[i++]);//因为可能还会向下传递，要将内容在此放到carry中，清空counter[i]，因为此时carry内容为空
                }
                carry.swap(counter[i]);//carry不为空的话，那么就将所有内容放到后面一个counter中
                if (i == fill) ++fill;//说明前面的所有counter都进行了更新，那么之前第fill中存在的元素个数为2^（fill+1)，所以上面一句话中开一个新的counter来储存，这里来更新循环次数
            } 
 
            for (int i = 1; i < fill; ++i) counter[i].merge(counter[i-1]);     //最后合并所有的counter
            swap(counter[fill-1]);
        }
    };
}

#endif
