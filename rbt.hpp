/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 13:51:58 by mbucci            #+#    #+#             */
/*   Updated: 2023/04/12 16:56:51 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "functionnal.hpp"
#include "iterators.hpp"

namespace ft
{
    template <class T>
    struct Node
    {
        public:
            typedef T        value_type;
            typedef Node*    node_ptr;


            value_type  data;
            bool        is_black;
            node_ptr    parent;
            node_ptr    left;
            node_ptr    right;


            Node() : is_black(false), parent(NULL), left(NULL), right(NULL) {}

            Node(const value_type& val, bool is_black=true, node_ptr parent=NULL, node_ptr left=NULL, node_ptr right=NULL)
                : data(val), is_black(is_black), parent(parent), left(left), right(right) {}

            Node(const Node& x)
            {
                data = x.data;
                is_black = x.is_black;
                parent = x.parent;
                left = x.left;
                right = x.right;
            }

            virtual ~Node() {}

            Node operator= (const Node& other)
            {
                if (&other != this)
                {
                    data = other.data;
                    is_black = other.is_black;
                    parent = other.parent;
                    left = other.left;
                    right = other.right;
                }
                return *this;
            }
    };

    template <class T>
    class Iterator : public ft::iterator<ft::bidirectional_iterator_tag, Node<T> >
    {
        public:
            typedef T                                                                                   value_type;
            typedef T*                                                                                  pointer;
            typedef const T*                                                                            const_pointer;
            typedef T&                                                                                  reference;
            typedef const T&                                                                            const_reference;
            typedef Node<T>*                                                                            node_ptr;
            typedef Node<const T>*                                                                      const_node_ptr;
            typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<T> >::difference_type    difference_type;
            typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<T> >::iterator_category  iterator_category;

            // Constructors
            Iterator() : _m_node(NULL)                          {}
            Iterator(node_ptr ptr) : _m_node(ptr)               {}
            Iterator(const Iterator& x) : _m_node(x._m_node)    {}

            // Destructor
            virtual ~Iterator() {}

            // Member Functions Overloads
            Iterator& operator= (const Iterator& x)
            {
                if (&x != this)
                    _m_node = x._m_node;
                return *this;
            }

            operator Iterator<const value_type> () const    { return Iterator<const value_type>(reinterpret_cast<const_node_ptr>(_m_node)); }

            node_ptr base() const                    { return _m_node; }
            reference        operator* ()            { return _m_node->data; }
            const_reference  operator* ()    const   { return _m_node->data; }
            pointer          operator-> ()           { return &(operator* ()); }
            const_pointer    operator-> ()   const   { return &(operator* ()); }

            Iterator &operator++ ()
            {
                if (_m_node->right)
                {
                    _m_node = _m_node->right;
                    while (_m_node->left)
                        _m_node = _m_node->left;
                }
                else
                {
                    node_ptr p = _m_node->parent;
                    while (p && _m_node == p->right)
                    {
                        _m_node = p;
                        p = p->parent;
                    }
                    _m_node = p;
                }
                return *this;
            }

            Iterator operator++ (int)
            {
                Iterator ret(*this);
                ++(*this);
                return ret;
            }

            Iterator &operator-- ()
            {
                if (_m_node->left)
                {
                    _m_node = _m_node->left;
                    while (_m_node->right)
                        _m_node = _m_node->right;
                }
                else
                {
                    node_ptr p = _m_node->parent;
                    while (p && _m_node == p->left)
                    {
                        _m_node = p;
                        p = p->parent;
                    }
                    _m_node = p;
                }
                return *this;
            }

            Iterator operator-- (int)
            {
                Iterator ret(*this);
                --(*this);
                return ret;
            }

            bool operator== (const Iterator& rhs) const
            { return (base() == rhs.base()); }

            bool operator!= (const Iterator& rhs) const
            { return (base() != rhs.base()); }

        protected:
            node_ptr _m_node;
    };


    template <class T, class Comp = std::less<T>, class Alloc = std::allocator<T> >
    class RBT
    {
        /*
         * RBT CONDITIONS:
         *     - Each node is either RED or BLACK.
         *     - The ROOT and LEAVES are ALWAYS BLACK.
         *     - IF a node is RED, then its children are BLACK.
         *     - All paths from a node to its NIL descendents
         *         contain the same number of BLACK nodes.
         *
         * RBT MAIN OPERATIONS:
         *     - SEARCH
         *     - INSERT
         *     - REMOVE
         *
         *     SOURCE: https://www.geeksforgeeks.org/introduction-to-red-black-tree/
         */

        public:
            typedef T                                                            value_type;
            typedef Comp                                                         value_compare;
            typedef Alloc                                                        allocator_type;
            typedef size_t                                                       size_type;
            typedef Node<value_type>*                                            node_ptr;
            typedef Node<const value_type>*                                      const_node_ptr;
            typedef Iterator<value_type>                                         iterator;
            typedef Iterator<const value_type>                                   const_iterator;
            typedef typename ft::reverse_iterator<iterator>                      reverse_iterator;
            typedef typename ft::reverse_iterator<const_iterator>                const_reverse_iterator;
            typedef typename Alloc::template rebind<Node<value_type> >::other    node_allocator_type;

            //////////////////////
            // MEMBER FUNCTIONS //
            //////////////////////

            RBT() : _m_nil(_m_create_nil_node()), _m_size(0)
            {
                _m_comp = value_compare();
                _m_alloc = allocator_type();
                _m_root = _m_nil;
                _m_max_node = _m_nil;
            }

            RBT(const allocator_type& alloc, const value_compare& comp)
                : _m_nil(_m_create_nil_node()), _m_alloc(alloc), _m_comp(comp), _m_size(0)
            {
                _m_root = _m_nil;
                _m_max_node = _m_nil;
            }

            RBT(const RBT& tree) : _m_alloc(tree._m_alloc), _m_comp(tree._m_comp), _m_size(0)
            {
                _m_nil = _m_create_nil_node();
                _m_root = _m_nil;
                _m_max_node = _m_nil;
                _m_clone_tree(tree.begin(), tree.end());
            }

            template <class InputIterator>
            RBT(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
                    const value_compare& comp = value_compare())
                : _m_nil(_m_create_nil_node()), _m_alloc(alloc), _m_comp(comp), _m_size(0)
            {
                _m_root = _m_nil;
                _m_max_node = _m_nil;

                for (; first != last; ++first)
                    insert(*first);

                _m_update_max_node();
            }

            virtual ~RBT()
            {
                clear();
                _m_destroy_nil_node();
            }

            RBT operator= (const RBT& tree)
            {
                if (&tree != this)
                {
                    clear();
                    _m_alloc = tree._m_alloc;
                    _m_comp = tree._m_comp;
                    _m_clone_tree(tree.begin(), tree.end());
                }
                return *this;
            }

            /////////////////////
            // MAIN OPERATIONS //
            /////////////////////

            iterator search(const value_type& val) const
            {
                node_ptr current = _m_root;

                while (!_m_isnil(current))
                {
                    if (_m_comp(val, current->data))
                        current = current->left;
                    else if (_m_comp(current->data, val))
                        current = current->right;
                    else
                        return iterator(current);
                }
                return iterator(_m_nil);
            }

            iterator insert(const value_type& val)
            {
                return insert(val, _m_nil);
            }

            iterator insert(const value_type& val, node_ptr previous)
            {
                node_ptr current = _m_root;

                if (_m_isnil(_m_root))
                {
                    _m_root = _m_add_node(val, true, NULL, _m_nil);
                    _m_update_max_node(_m_root);
                    return iterator(_m_root);
                }

                if (!_m_isnil(previous))
                    current = previous;

                node_ptr parent;
                while (!_m_isnil(current))
                {
                    parent = current;
                    if (_m_comp(val, current->data))
                        current = current->left;
                    else if (_m_comp(current->data, val))
                        current = current->right;
                    else
                        return iterator(current);
                }

                node_ptr new_node = _m_add_node(val, false, parent);
                if (current == _m_nil)
                {
                    new_node->right = _m_nil;
                    _m_update_max_node(new_node);
                }

                _m_link_gp_to_child(new_node, new_node);
                _m_rbt_insert_fix(new_node);

                return iterator(new_node);
            }

            bool remove(node_ptr target)
            {
                if (!_m_isnil(target))
                {
                    if (!_m_isnil(target->right) && !_m_isnil(target->left))
                    {
                        node_ptr substitute = _m_find_smallest_node(target->right);
                        _m_swap_nodes(target, substitute);
                    }
                    _m_rbt_remove_check(target);

                    return true;
                }
                return false;
            }

            void remove(iterator first, iterator last)
            {
                while (first != last)
                    remove((first++).base());
            }

            bool remove(const value_type& val)
            {
                return remove(search(val).base());
            }

            ///////////
            // OTHER //
            ///////////

            void clear()
            {
                _m_clear_tree(_m_root);
                _m_size = 0;
                _m_root = _m_nil;
            }

            void swap(RBT& other)
            {
                ft::swap(_m_root, other._m_root);
                ft::swap(_m_nil, other._m_nil);
                ft::swap(_m_alloc, other._m_alloc);
                ft::swap(_m_comp, other._m_comp);
                ft::swap(_m_node_alloc, other._m_node_alloc);
                ft::swap(_m_size, other._m_size);
            }

            size_type size()     const { return _m_size; }
            size_type max_size() const { return (_m_alloc.max_size()); }

            iterator               begin()        { return iterator(_m_find_smallest_node(_m_root)); }
            const_iterator         begin()  const { return iterator(_m_find_smallest_node(_m_root)); }
            iterator               end()          { return iterator(_m_nil); }
            const_iterator         end()    const { return iterator(_m_nil); }
            reverse_iterator       rbegin()       { return reverse_iterator(end()); }
            const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
            reverse_iterator       rend()         { return reverse_iterator(begin()); }
            const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

            iterator lower_bound(const value_type& val) const
            {
                node_ptr current = _m_root;
                node_ptr prev = _m_nil;

                while (!_m_isnil(current))
                {
                    if (_m_comp(current->data, val))
                        current = current->right;
                    else
                    {
                        prev = current;
                        current = current->left;
                    }
                }
                return iterator(prev);
            }

            iterator upper_bound(const value_type& val) const
            {
                node_ptr current = _m_root;
                node_ptr prev = _m_nil;

                while (!_m_isnil(current))
                {
                    if (_m_comp(val, current->data))
                    {
                        prev = current;
                        current = current->left;
                    }
                    else
                        current = current->right;
                }
                return iterator(prev);
            }

        private:
            node_ptr            _m_root;
            node_ptr            _m_nil;
            allocator_type      _m_alloc;
            value_compare       _m_comp;
            size_type           _m_size;
            node_allocator_type _m_node_alloc;
            node_ptr            _m_max_node;


            bool _m_isnil(node_ptr node) const
            {
                return (!node || (node == _m_nil));
            }

            node_ptr _m_create_node(const value_type& val, bool b=false, node_ptr p=NULL, node_ptr r=NULL)
            {
                node_ptr new_node = _m_node_alloc.allocate(1);
                _m_alloc.construct(&(new_node->data), val);
                new_node->is_black = b;
                new_node->parent = p;
                new_node->left = NULL;
                new_node->right = r;

                return new_node;
            }

            node_ptr _m_add_node(const value_type& val, bool b=false, node_ptr p=NULL, node_ptr r=NULL)
            {
                node_ptr ret = _m_create_node(val, b, p, r);
                ++_m_size;

                return ret;
            }

            node_ptr _m_create_nil_node()
            {
                node_ptr nil_node = _m_node_alloc.allocate(1);
                nil_node->is_black = true;
                nil_node->parent = NULL;
                nil_node->left = NULL;
                nil_node->right = NULL;

                return nil_node;
            }

            void _m_destroy_node(node_ptr target)
            {
                _m_alloc.destroy(&(target->data));
                _m_node_alloc.deallocate(target, 1);
            }

            void _m_remove_node(node_ptr target)
            {
                _m_destroy_node(target);
                --_m_size;
            }

            void _m_destroy_nil_node()
            {
                _m_nil->parent = NULL;
                _m_node_alloc.deallocate(_m_nil, 1);
            }

            void _m_clear_tree(node_ptr root)
            {
                if (!_m_isnil(root))
                {
                    if (!_m_isnil(root->left))
                        _m_clear_tree(root->left);
                    if (!_m_isnil(root->right))
                        _m_clear_tree(root->right);
                    _m_remove_node(root);
                }
            }

            void _m_clone_tree(const_iterator first, const_iterator last)
            {
                insert(*(first++));
                for (; first != last; ++first)
                    insert(*first, _m_max_node);
            }

            void _m_update_max_node(node_ptr node=NULL)
            {
                if (node)
                    _m_max_node = node;
                else
                    _m_max_node = _m_find_greatest_node(_m_root);
                _m_nil->parent = _m_max_node;
            }

            node_ptr _m_find_smallest_node(node_ptr target) const
            {
                if (!_m_isnil(target) && !_m_isnil(target->left))
                    return _m_find_smallest_node(target->left);
                return target;
            }

            node_ptr _m_find_greatest_node(node_ptr target) const
            {
                if (!_m_isnil(target) && !_m_isnil(target->right))
                    return _m_find_greatest_node(target->right);
                return target;
            }

            void _m_link_gp_to_child(node_ptr target, node_ptr child)
            {
                if (target->parent)
                {
                    if (_m_comp(target->data, target->parent->data))
                        target->parent->left = child;
                    else
                        target->parent->right = child;
                }
                else
                    _m_root = child;
                if (!_m_isnil(child))
                    child->parent = target->parent;
            }

            void _m_swap_nodes(node_ptr n1, node_ptr n2)
            {
                ft::swap(n1->is_black, n2->is_black);
                if (n1->parent)
                {
                    if (_m_comp(n1->data, n1->parent->data))
                        n1->parent->left = n2;
                    else
                        n1->parent->right = n2;
                }
                else
                    _m_root = n2;

                if (n2 == n1->right)
                {
                    n1->right = n2->right;
                    if (n2->right)
                        n2->right->parent = n1;
                    n2->right = n1;
                    n2->parent = n1->parent;
                    n1->parent = n2;
                }
                else
                {
                    n2->parent->left = n1;
                    ft::swap(n1->parent, n2->parent);
                    ft::swap(n1->right, n2->right);

                    n2->right->parent = n2;
                    if (n1->right)
                        n1->right->parent = n1;
                }

                if (_m_root == n2)
                    n2->parent = NULL;

                // n2 should never have a left child
                ft::swap(n1->left, n2->left);
                n2->left->parent = n2;
            }

#define FATHER x->parent
#define GFATHER x->parent->parent

            void _m_rbt_left_left_rotate(node_ptr x)
            {
                // gf takes as left node whatever the father had as his right node.
                GFATHER->left = FATHER->right;
                if (FATHER->right)
                    FATHER->right->parent = GFATHER;
                // father takes gf as his right node.
                FATHER->right = GFATHER;
                // ggf takes father as his child.
                if (GFATHER->parent)
                {
                    if (_m_comp(GFATHER->data, GFATHER->parent->data))
                        GFATHER->parent->left = FATHER;
                    else
                        GFATHER->parent->right = FATHER;
                }
                else // no parent means root
                    _m_root = FATHER;
                // gf is now gf's father.
                GFATHER = GFATHER->parent;
                // gf new parent is father.
                FATHER->right->parent = FATHER;
                // swap gf's and father's colors.
            }

            void _m_rbt_left_right_rotate(node_ptr x)
            {
                GFATHER->left = x;
                FATHER->right = x->left;
                x->left = FATHER;
                FATHER = GFATHER;
                x->left->parent = x;
                _m_rbt_left_left_rotate(x->left);
            }

            void _m_rbt_right_right_rotate(node_ptr x)
            {
                GFATHER->right = FATHER->left;
                if (FATHER->left)
                    FATHER->left->parent = GFATHER;
                FATHER->left = GFATHER;
                if (GFATHER->parent)
                {
                    if (_m_comp(GFATHER->data, GFATHER->parent->data))
                        GFATHER->parent->left = FATHER;
                    else
                        GFATHER->parent->right = FATHER;
                }
                else
                    _m_root = FATHER;
                GFATHER = GFATHER->parent;
                FATHER->left->parent = FATHER;
            }

            void _m_rbt_right_left_rotate(node_ptr x)
            {
                GFATHER->right = x;
                FATHER->left = x->right;
                x->right = FATHER;
                FATHER = GFATHER;
                x->right->parent = x;
                _m_rbt_right_right_rotate(x->right);
            }

            void _m_rbt_insert_fix(node_ptr x)
            {
                if (x == _m_root)
                    x->is_black = true;

                if (FATHER && GFATHER && !FATHER->is_black)
                {
                    node_ptr uncle = _m_comp(FATHER->data, GFATHER->data) ? GFATHER->right : GFATHER->left;
                    if (!_m_isnil(uncle) && !uncle->is_black)
                    {
                        FATHER->is_black = true;
                        uncle->is_black = true;
                        GFATHER->is_black = false;
                        _m_rbt_insert_fix(GFATHER);
                    }

                    else
                    {
                        if (_m_comp(FATHER->data, GFATHER->data))
                        {
                            if (_m_comp(x->data, FATHER->data))
                                _m_rbt_left_left_rotate(x);
                            else
                                _m_rbt_left_right_rotate(x);
                            ft::swap(FATHER->is_black, FATHER->right->is_black);
                        }
                        else
                        {
                            if (_m_comp(x->data, FATHER->data))
                                _m_rbt_right_left_rotate(x);
                            else
                                _m_rbt_right_right_rotate(x);
                            ft::swap(FATHER->is_black, FATHER->left->is_black);
                        }
                    }
                }
            }

            void _m_rbt_remove_check(node_ptr node)
            {
                node_ptr child = (!_m_isnil(node->right)) ? node->right : node->left;

                if (node->is_black)
                {
                    if (_m_isnil(child) || (!_m_isnil(child) && child->is_black))
                        _m_rbt_remove_fix(node);
                    if (!_m_isnil(child) && !child->is_black)
                        child->is_black = true;
                }
                if (node->parent && (node == node->parent->right) && _m_comp(node->data, node->parent->data))
                {
                    if (node->parent == _m_max_node)
                        child = _m_nil;
                    node->parent->right = child;
                    if (child)
                        child->parent = node->parent;
                }
                else
                    _m_link_gp_to_child(node, child);

                if (node == _m_max_node)
                {
                    if (node->parent)
                    {
                        node->parent->right = _m_nil;
                        _m_update_max_node(node->parent);
                    }
                    else
                    {
                        _m_nil->parent = _m_root;
                        if (_m_root)
                            _m_root->right = _m_nil;
                        else
                            _m_root = _m_nil;
                    }
                }
                _m_remove_node(node);
            }

            void _m_rbt_remove_fix(node_ptr target)
            {
                // When a black node is removed, it leaves a double-
                // balck node. It's a NIL node but still colored black.
                // When a node is double-black, one of its darkness's
                // is moved to its parent. The sibling then becomes red.
                // If the parent was red, it becomes black.
                // And if it was already black, it becomes double-balck.
                // This function would then be recalled on the parent.

                if (target == _m_root)
                    return;
                
                node_ptr s = (target->parent->left == target) ? target->parent->right : target->parent->left;
                node_ptr sr = s ? s->right : NULL;
                node_ptr sl = s ? s->left : NULL;
                node_ptr x = (target->parent->left == target) ? (sr ? sr : sl) : (sl ? sl : sr);

                // if sibling is red:
                if (!_m_isnil(s) && !s->is_black)
                {
                    if (_m_comp(x->data, FATHER->data))
                    {
                        _m_rbt_left_left_rotate(sl);
                        ft::swap(FATHER->is_black, FATHER->right->is_black);
                    }
                    else
                    {
                        _m_rbt_right_right_rotate(sr);
                        ft::swap(FATHER->is_black, FATHER->left->is_black);
                    }
                    _m_rbt_remove_fix(target);
                }

                else // sibling is black (or is NIL):
                {
                    // if at least one of the sibling's children is red:
                    if ((!_m_isnil(sr) && !sr->is_black) || (!_m_isnil(sl) && !sl->is_black))
                    {
                        if (_m_comp(FATHER->data, GFATHER->data))
                        {
                            if (_m_comp(x->data, FATHER->data))
                                _m_rbt_left_left_rotate(x);
                            else
                                _m_rbt_left_right_rotate(x);
                        }
                        else
                        {
                            if (_m_comp(x->data, FATHER->data))
                                _m_rbt_right_left_rotate(x);
                            else
                                _m_rbt_right_right_rotate(x);
                        }
                        x->is_black = true;
                    }
                    else // if both sibling's children are black:
                    {
                        if (!_m_isnil(s))
                            s->is_black = false;
                        if (target->parent->is_black)
                            _m_rbt_remove_fix(target->parent);
                        else
                            target->parent->is_black = true;
                    }
                }
            }

#undef FATHER
#undef GFATHER

    };

    template <class T, class Comp, class Alloc>
    void swap(RBT<T, Comp, Alloc>& lhs, RBT<T, Comp, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}
