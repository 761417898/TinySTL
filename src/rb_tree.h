#ifndef RB-TREE_H
#define RB-TREE_H

#include "allocator.h"
#include "constructor.h"
#include "iterator.h"
#include "pair.h"

namespace tinystl {
    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red = false;
    const __rb_tree_color_type __rb_tree_black = true;

    struct __rb_tree_node_base {
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base* base_ptr;
        
        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr x) {
            while (x->left) {
                x = x->left;
            }
            return x;
        }

        static base_ptr maximum(base_ptr x) {
            while (x->right) {
                x = x->right;
            }
            return x;
        }
    };

    template <class Value>
    struct __rb_tree_node : public __rb_tree_node_base {
        typedef __rb_tree_node<Value>* link_type;
        Value value_field;
    };

    struct __rb_tree_base_iterator {
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node;

        void increment() {
            if (node->right) {
                node = node->right;
                while (node->left) {
                    node = node->left;
                }
            } else {
                base_ptr y = node->parent;
                while (node == y->right) {
                    node = y;
                    y = y->parent;
                }
                if (node->right != y) {
                    node = y;
                }
            }
        }

        void decrement() {
            if (node->color == __rb_tree_red &&
               node->parent->parent == node);
        }
    };


    template <class Value, class Ref, class Ptr>
    struct __rb_tree_iterator : public __rb_tree_base_iterator {
        typedef Value value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
        typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
        typedef __rb_tree_iterator<Value, Ref, Ptr> self;
        typedef __rb_tree_node<Value>* link_type;

        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) : node(x) {}
        __rb_tree_iterator(const_iterator it) : node(it.node) {}

        reference operator*() const {
            return node->value_field;
        }

        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {
            increment();
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            increment();
            return tmp;
        }

        self& operator--() {
            decrement();
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            decrement();
            return tmp;
        }
    };

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    class rb_tree {
    protected:
        typedef void *void_pointer;
        typedef __rb_tree_node_base* base_ptr;
        typedef __rb_tree_node<Value> rb_tree_node;
        typedef allocator<rb_tree_node> rb_tree_node_allocator;
        typedef __rb_tree_color_type color_type;
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef rb_tree_node* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    protected:
        link_type get_node() {
            return rb_tree_node_allocator::allocator();
        }

        void put_node(link_type p) {
            rb_tree_node_allocator::deallocator(p);
        }

        link_type create_node(const value_type& x) {
            link_type tmp = get_node();
            construct(&(tmp->value_field), x);
            return tmp;
        }

        link_type clone_node(link_type x) {
            link_type tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = nullptr;
            tmp->right = nullptr;
            return tmp;
        }

        void destroy_node(link_type p) {
            destory(&(p->value_field));
            put_node(p);
        }
    protected:
        size_type node_count; //记录树的大小（节点数）
        /*
         * 关于header节点
         *
         *
         *
        */
        link_type header;
        Compare key_compare; //节点间大小比较准则
        link_type& root() const  {
            return (link_type&)header->parent;
        }
        link_type& leftmost() const {
            return (link_type&)header->left;
        }
        link_type& rightmost() const {
            return (link_type&)header->right;
        }

        static link_type& left(link_type x) {
            return x->left;
        }
        static link_type& right(link_type x) {
            return x->right;
        }
        static link_type& parent(link_type x) {
            return x->parent;
        }
        static reference value(link_type x) {
            return x->value_field;
        }
        static const Key& key(link_type x) {
            return KeyOfValue()(value(x));
        }
        static color_type& color(link_type x) {
            return x->color;
        }

        static link_type& left(base_ptr x) {
            return x->left;
        }
        static link_type& right(base_ptr x) {
            return x->right;
        }
        static link_type& parent(base_ptr x) {
            return x->parent;
        }
        static reference value(base_ptr x) {
            return (static_cast<link_type>(x))->value_field;
        }
        static const Key& key(base_ptr x) {
            return KeyOfValue()(value(x));
        }
        static color_type& color(base_ptr x) {
            return x->color;
        }


        static link_type minimum(link_type x) {
            return __rb_tree_node_base::minimum(x);
        }
        static link_type maximum(link_type x) {
            return __rb_tree_node_base::maximum(x);
        }

    public:
        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;

    private:
        iterator __insert(base_ptr x, base_ptr y, const value_type& v);
        link_type __copy(link_type x, link_type p);
        void __erase(link_type x);
        void init() {
            header = get_node();
            //header为红色节点，与根节点不同
            color(header) = __rb_tree_red;
            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
        }

    public:
        rb_tree(const Compare& comp = Compare()) : node_count(0),
                key_compare(comp) {
            init();
        }
        ~rb_tree() {
            clear();
            put_node(header);
        }
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
                operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> x);

    public:
        Compare key_comp() const {
            return key_compare;
        }
        iterator begin() {
            return leftmost();
        }
        iterator end() {
            return header;
        }
        bool empty() const {
            return node_count == 0;
        }
        size_type size() const {
            return node_count;
        }
        size_type max_size() const {
            static_cast<size_type>(-1);
        }
        
    public:
        pair<iterator, bool> insert_unique(const value_type& x);
        iterator insert_equal(const value_type& x);
        iterator __insert(base_ptr x_, base_ptr y_, const Value& v);

        inline void __rb_tree_reblance(__rb_tree_node_base* x, __rb_tree_node_base*& root);
        inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root);
        inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root);

        iterator find(const Key& k);
    }

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& x) {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            y = x;
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, v);
    }

    /*
     * x是节点要插入的位置，y的x的父亲节点
     * 这里的KeyOfValue为仿函数，即重载了()运算符的类
     */ 
    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    pair<rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& x) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        iterator j = itarator(y);
        if (comp) {
            if (j == begin()) {
                return static_cast<pair<iterator, bool> >(__insert(x, y, v), true);
            }
            else {
                --j;
            }
        }
        if (key_compare(key(j.node), KeyOfValue()(v)))
            return static_cast<pair<iterator, bool> >(__insert(x, y, v), true);
        return pair<iterator, bool>(j, false);
    }

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value& x) {
        link_type x = static_cast<link_type>(x_);
        link_type y = static_cast<link_type>(y_);
        link_type z;
        if (y == header || x == nullptr || key_compare(KeyOfValue()(v), Key(y))) {
            z = create_node(v);
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            } else if (y == leftmost()) {
                leftmost() = z;
            }
        } else {
            z = create_node(v);
            right(y) = z;
            if (y == rightmost())
                rightmost() = z;
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __rb_tree_reblance(z, header->parent);
        ++node_count;
        return static_cast<iterator>(z);
    }

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_reblance(__rb_tree_node_base* x, 
            __rb_tree_node_base*& root) {
        x->color = __rb_tree_red;
        while (x != root && x->parent->color == __rb_tree_red) {
            if (x->parent == x->parent->parent->left) {
                __rb_tree_node_base* y = x->parent->parent->right; //叔叔节点
                if (y && y->color == __rb_tree_red) { //叔叔节点是红色节点，分层修改颜色并上溯
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                } else { //叔叔节点是黑色，分两种情况，外侧插入有右单旋，内侧插入左单旋、右单旋
                    if (x == x->parent->right) {
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            } else {  //右边的处理方式同左边
                __rb_tree_node_base* y = x->parent->parent->left; //叔叔节点
                if (y && y->color == __rb_tree_red) { //叔叔节点是红色节点，分层修改颜色并上溯
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                } else { //叔叔节点是黑色，分两种情况，外侧插入有右单旋，内侧插入左单旋、右单旋
                    if (x == x->parent->left) {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rotate_left(__rb_tree_node_base* x, 
            __rb_tree_node_base*& root) {
        __rb_tree_node_base* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x == root) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rotate_left(__rb_tree_node_base* x, 
            __rb_tree_node_base*& root) {
        __rb_tree_node_base* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x == root) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare,
            class Alloc = alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (!key_compare(key(x), k)) {
                y = x, x = left(x);
            } else {
                x = right(x);
            }
        }
        iterator j = iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }
}

#endif
