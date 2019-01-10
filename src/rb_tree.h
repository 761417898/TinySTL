#ifndef RB-TREE_H
#define RB-TREE_H

#include "allocator.h"
#include "constructor.h"
#include "itearator.h"

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
        size_type node_count;
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
    }
}

#endif
