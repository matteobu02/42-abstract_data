/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 13:51:58 by mbucci            #+#    #+#             */
/*   Updated: 2023/03/31 23:09:45 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "functionnal.hpp"
#include "iterators.hpp"

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	struct Node
	{
		public:
			typedef T		value_type;
			typedef T*		pointer;
			typedef Alloc	allocator_type;
			typedef Node*	node_ptr;


			pointer		data;
			bool		is_black;
			node_ptr	parent;
			node_ptr	left;
			node_ptr	right;


			Node() : data(NULL), is_black(false), parent(NULL), left(NULL), right(NULL)
			{}

			Node(const value_type& val, bool is_black=true, node_ptr parent=NULL, node_ptr left=NULL, node_ptr right=NULL)
				: data(NULL), is_black(is_black), parent(parent), left(left), right(right)
			{
				construct_node(val);
			}

			Node(const Node& x) : data(NULL)
			{
				construct_node(*(x.data));
				is_black = x.is_black;
				parent = x.parent;
				left = x.left;
				right = x.right;
			}

			virtual ~Node()
			{
				clear_node();
			}

			Node operator= (const Node& other)
			{
				if (&other != this)
				{
					allocator_type alloc;
					alloc.destroy(data);
					alloc.construct(data, *(other.data));

					is_black = other.is_black;
					parent = other.parent;
					left = other.left;
					right = other.right;
				}
				return *this;
			}

			void clear_node()
			{
				if (data)
				{
					allocator_type alloc;

					alloc.destroy(data);
					alloc.deallocate(data, 1);
					data = NULL;
				}
			}

			void construct_node(const value_type& val)
			{
				if (!data)
				{
					allocator_type alloc;

					data = alloc.allocate(1);
					alloc.construct(data, val);
				}
			}
	};

	template <class T>
	class Iterator : public ft::iterator<ft::bidirectional_iterator_tag, Node<T> >
	{
		public:
			typedef T																					value_type;
			typedef T*																					pointer;
			typedef const T*																			const_pointer;
			typedef T&																					reference;
			typedef const T&																			const_reference;
			typedef Node<T>*																			node_ptr;
			typedef Node<const T>*																		const_node_ptr;
			typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<T> >::difference_type	difference_type;
			typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<T> >::iterator_category	iterator_category;

			// Constructors
			Iterator() : _m_node(NULL)							{}
			Iterator(node_ptr ptr) : _m_node(ptr)				{}
			Iterator(const Iterator& x) : _m_node(x._m_node)	{}

			// Destructor
			virtual ~Iterator() {}

			// Member Functions Overloads
			Iterator& operator= (const Iterator& x)
			{
				if (&x != this)
					_m_node = x._m_node;
				return *this;
			}

			operator Iterator<const value_type> () const		{ return Iterator<const value_type>(reinterpret_cast<const_node_ptr>(_m_node)); }

			node_ptr base() const								{ return _m_node; }
			pointer			operator-> ()						{ return _m_node->data; }
			const_pointer	operator-> ()	const				{ return _m_node->data; }
			reference 		operator* ()						{ return &(operator->()); }
			const_reference operator* ()	const				{ return &(operator->()); }

			Iterator &operator++ ()
			{
				if (_m_node)
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
				}
				return *this;
			}

			Iterator operator++ (int)
			{
				Iterator ret = *this;
				++(*this);
				return ret;
			}

			Iterator &operator-- ()
			{
				if (_m_node)
				{
					if (_m_node->left)
					{
						while (_m_node->left)
							_m_node = _m_node->left;
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
				}
				return *this;
			}

			Iterator operator-- (int)
			{
				Iterator ret = *this;
				--(*this);
				return ret;
			}

			bool operator== (const Iterator& rhs) const
			{ return (base() == rhs.base()); }

			bool operator!= (const Iterator& rhs) const
			{ return (base() != rhs.base()); }

		protected:
			node_ptr	_m_node;
	};


	template <class T, class Comp = std::less<T>, class Alloc = std::allocator<T> >
	class RBT
	{
		/*
		 * RBT CONDITIONS:
		 * 	- Each node is either RED or BLACK.
		 * 	- The ROOT and LEAVES are ALWAYS BLACK.
		 * 	- IF a node is RED, then its children are BLACK.
		 * 	- All paths from a node to its NIL descendents
		 * 		contain the same number of BLACK nodes.
		 *
		 * RBT MAIN OPERATIONS:
		 * 	- SEARCH
		 * 	- INSERT
		 * 	- REMOVE
		 *
		 * 	SOURCE: https://www.geeksforgeeks.org/introduction-to-red-black-tree/
		 */

		public:
			typedef	T															value_type;
			typedef Comp														value_compare;
			typedef Alloc														allocator_type;
			typedef size_t														size_type;
			typedef Node<value_type, allocator_type>*							node_ptr;
			typedef Node<const value_type, allocator_type>*						const_node_ptr;
			typedef Iterator<value_type>										iterator;
			typedef Iterator<const value_type>									const_iterator;
			typedef typename ft::reverse_iterator<iterator>						reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>				const_reverse_iterator;
			typedef typename Alloc::template rebind<Node<value_type, allocator_type> >::other	node_allocator_type;

			//////////////////////
			// MEMBER FUNCTIONS //
			//////////////////////

			RBT() : _m_root(NULL), _m_size(0)
			{
				_m_comp = value_compare();
			}

			RBT(const value_compare& comp)
				: _m_root(NULL), _m_comp(comp), _m_size(0) {}

			RBT(const RBT& tree)
				: _m_root(NULL), _m_comp(tree._m_comp), _m_size(tree._m_size)
			{ _m_root = _m_clone_tree(tree._m_root, NULL);}

			template <class InputIterator>
			RBT(InputIterator first, InputIterator last, const value_compare& comp = value_compare())
				: _m_root(NULL), _m_comp(comp), _m_size(0)
			{
				insert(*(first++));
				for (iterator it = begin(); first != last; ++first)
					insert(*first, it++);
			}

			virtual ~RBT()	{ clear(_m_root); }

			RBT operator= (const RBT& tree)
			{
				if (&tree != this)
				{
					clear(_m_root);
					_m_comp = tree._m_comp;
					_m_root = _m_clone_tree(tree._m_root, NULL);
					_m_size = tree._m_size;
				}
				return *this;
			}

			/////////////////////
			// MAIN OPERATIONS //
			/////////////////////

			iterator search(const value_type& val)
			{
				node_ptr current = _m_root;

				while (current)
				{
					if (_m_comp(val, *(current->data)))
						current = current->left;
					else if (_m_comp(*(current->data), val))
						current = current->right;
					else
						return iterator(current);
				}
				return end();
			}

			iterator insert(const value_type& val)
			{
				return insert(val, end());
			}

			iterator insert(const value_type& val, iterator previous)
			{
				node_ptr current = _m_root;

				if (!current)
				{
					_m_root = _m_create_node(val, true);
					return iterator(_m_root);
				}

				if (previous != end())
					current = previous.base();

				node_ptr parent;
				while (current)
				{
					parent = current;
					if (_m_comp(val, *(current->data)))
						current = current->left;
					else if (_m_comp(*(current->data), val))
						current = current->right;
					else
						return iterator(current);
				}

				node_ptr new_node = _m_create_node(val, false, parent);
				_m_link_gp_to_child(new_node, new_node);
				_m_rbt_insert_check(new_node);

				return iterator(new_node);
			}

			bool remove(node_ptr target)
			{
				if (target)
				{
					if (target->right && target->left)
					{
						allocator_type alloc;

						node_ptr substitute = _m_find_smallest_node(target->right);
						alloc.destroy(target->data);
						alloc.construct(target->data, *(substitute->data));
						target = substitute;
					}
					_m_rbt_remove_node(target);

					return true;
				}
				return false;
			}

			bool remove(const value_type& val)
			{
				node_ptr target = search(val).base();
				return remove(target);
			}

			///////////
			// OTHER //
			///////////

			void clear(node_ptr root)
			{
				if (root)
				{
					if (root->left)
						clear(root->left);
					if (root->right)
						clear(root->right);
					_m_destroy_node(root);
				}
			}

			size_type	get_tree_size()	const	{ return _m_size; }
			size_type	max_size()		const	{ return _m_node_alloc.max_size(); }

			iterator				begin()				{ return iterator(_m_find_smallest_node(_m_root)); }
			const_iterator			begin()		const	{ return iterator(_m_find_smallest_node(_m_root)); }
			iterator			 	end()				{ return iterator((_m_root ? _m_find_greatest_node(_m_root)->right : _m_root)); }
			const_iterator 			end()		const	{ return iterator((_m_root ? _m_find_greatest_node(_m_root)->right : _m_root)); }
			reverse_iterator		rbegin()			{ return reverse_iterator(end()); }
			const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(end()); }
			reverse_iterator		rend()				{ return reverse_iterator(begin()); }
			const_reverse_iterator	rend()		const	{ return const_reverse_iterator(begin()); }


		private:
			node_ptr			_m_root;
			value_compare		_m_comp;
			node_allocator_type	_m_node_alloc;
			size_type			_m_size;

			node_ptr _m_create_node(const value_type& val, bool b=false, node_ptr p=NULL, node_ptr l=NULL, node_ptr r=NULL)
			{
				node_ptr new_node = _m_node_alloc.allocate(1);
				_m_node_alloc.construct(new_node, val);
				new_node->is_black = b;
				new_node->parent = p;
				new_node->left = l;
				new_node->right = r;
				++_m_size;

				return new_node;
			}

			void _m_destroy_node(node_ptr target)
			{
				allocator_type alloc;

				_m_node_alloc.destroy(target);
				_m_node_alloc.deallocate(target, 1);
				target = NULL;
				--_m_size;
			}

			node_ptr _m_clone_tree(const node_ptr root, const node_ptr parent) const
			{
				if (!root)
					return NULL;

				node_ptr ret = _m_create_node(*(root->data), root->is_black, parent);
				ret->left = _m_clone_tree(root->left, root);
				ret->right = _m_clone_tree(root->right, root);

				return ret;
			}

			node_ptr _m_find_smallest_node(node_ptr target) const
			{
				if (target && target->left)
					return _m_find_smallest_node(target->left);
				return target;
			}

			node_ptr _m_find_greatest_node(node_ptr target) const
			{
				if (target && target->right)
					return _m_find_greatest_node(target->right);
				return target;
			}

			void _m_link_gp_to_child(node_ptr target, node_ptr child)
			{
				if (target->parent)
				{
					if (_m_comp(*(target->data), *(target->parent->data)))
						target->parent->left = child;
					else
						target->parent->right = child;
					if (child)
						child->parent = target->parent;
				}
				if (target == _m_root)
				{
					_m_root = child;
					if (_m_root)
					{
						_m_root->is_black = true;
						_m_root->parent = NULL;
					}
				}
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
					if (_m_comp(*(GFATHER->data), *(GFATHER->parent->data)))
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
					if (_m_comp(*(GFATHER->data), *(GFATHER->parent->data)))
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

			void _m_rbt_insert_check(node_ptr x)
			{
				if (x == _m_root)
					x->is_black = true;

				if (FATHER && GFATHER && !FATHER->is_black)
				{
					node_ptr uncle = _m_comp(*(FATHER->data), *(GFATHER->data)) ? GFATHER->right : GFATHER->left;
					if (uncle && !uncle->is_black)
					{
						FATHER->is_black = true;
						uncle->is_black = true;
						GFATHER->is_black = false;
						_m_rbt_insert_check(GFATHER);
					}
					else
					{
						if (_m_comp(*(FATHER->data), *(GFATHER->data)))
						{
							if (_m_comp(*(x->data), *(FATHER->data)))
								_m_rbt_left_left_rotate(x);
							else
								_m_rbt_left_right_rotate(x);
							ft::swap(FATHER->is_black, FATHER->right->is_black);
						}
						else
						{
							if (_m_comp(*(x->data), *(FATHER->data)))
								_m_rbt_right_left_rotate(x);
							else
								_m_rbt_right_right_rotate(x);
							ft::swap(FATHER->is_black, FATHER->left->is_black);
						}
					}
				}
			}

			void _m_rbt_remove_node(node_ptr node)
			{
				node_ptr child = (node->right) ? node->right : node->left;
				if (node->is_black)
				{
					if (!child || (child && child->is_black))
						_m_rbt_remove_check(node);
					if (child && !child->is_black)
						child->is_black = true;
				}
				_m_link_gp_to_child(node, child);
				_m_destroy_node(node);
			}

			void _m_rbt_remove_check(node_ptr target)
			{
				// When a black node is removed, it leaves a double-
				// node. It's a NIL node but still colored black.
				// When a node is double-black, one of its darkness's
				// is moved to its parent. The sibling becomes red. 
				// If the parent was red, it becomes black.
				// And if it was already black, it becomes double-balck.
				// This function would then be recalled on the parent.

				if (target == _m_root)
					return ;
				
				node_ptr s = (target->parent->left == target) ? target->parent->right : target->parent->left;
				node_ptr sr = s ? s->right : NULL;
				node_ptr sl = s ? s->left : NULL;
				node_ptr x = (target->parent->left == target) ? (sr ? sr : sl) : (sl ? sl : sr);

				// if sibling is red:
				if (s && !s->is_black)
				{
					if (_m_comp(*(x->data), *(FATHER->data)))
					{
						_m_rbt_left_left_rotate(sl);
						ft::swap(FATHER->is_black, FATHER->right->is_black);
					}
					else
					{
						_m_rbt_right_right_rotate(sr);
						ft::swap(FATHER->is_black, FATHER->left->is_black);
					}
					_m_rbt_remove_check(target);
				}

				else // sibling is black (or is NIL):
				{
					// if at least one of the sibling's children is red:
					if ((sr && !sr->is_black) || (sl && !sl->is_black))
					{
						if (_m_comp(*(FATHER->data), *(GFATHER->data)))
						{
							if (_m_comp(*(x->data), *(FATHER->data)))
								_m_rbt_left_left_rotate(x);
							else
								_m_rbt_left_right_rotate(x);
						}
						else
						{
							if (_m_comp(*(x->data), *(FATHER->data)))
								_m_rbt_right_left_rotate(x);
							else
								_m_rbt_right_right_rotate(x);
						}

						x->is_black = true;
						if (!target->parent->is_black)
						{
							if (x == _m_root)
								target->parent->is_black = true;
							else
								ft::swap(x->is_black, target->parent->is_black);
						}

					}
					else // if sibling's children are both black:
					{
						if (s)
							s->is_black = false;
						if (target->parent->is_black)
							_m_rbt_remove_check(target->parent);
						else
							target->parent->is_black = true;
					}
				}
			}

#undef FATHER
#undef GFATHER

	};
}
