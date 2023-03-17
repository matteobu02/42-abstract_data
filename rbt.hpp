/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 13:51:58 by mbucci            #+#    #+#             */
/*   Updated: 2023/03/17 02:32:05 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define DEBUG 1

#include <iostream>
#include "iterators.hpp"

namespace ft
{
	template <class T, class Comp = std::less<T>, class Alloc = std::allocator<T> >
	class RBT
	{
		public:

			typedef	T		value_type;
			typedef Comp	value_compare;
			typedef Alloc	allocator_type;
			typedef size_t	size_type;

			template <class N>
			struct Node
			{
				public:

					typedef N		value_type;
					typedef Node*	node_ptr;

					Node() : is_black(true) {}

					Node(const value_type& val, bool is_black=true, node_ptr parent=NULL, node_ptr left=NULL, node_ptr right=NULL)
						: data(val), is_black(is_black), parent(parent), left(left), right(right) {}

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

				public:
					value_type	data;
					bool		is_black;
					node_ptr	parent;
					node_ptr	left;
					node_ptr	right;
			};

			typedef Node<value_type>*											node_ptr;
			typedef typename Alloc::template rebind<Node<value_type> >::other	node_allocator_type;

			template <class N>
			class Iterator : public ft::iterator<ft::bidirectional_iterator_tag, Node<N> >
			{
				public:
					typedef T*																					pointer;
					typedef T&																					reference;
					typedef Node<N>*																			node_ptr;
					typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<N> >::difference_type	difference_type;
					typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<N> >::value_type			value_type;
					typedef typename ft::iterator<ft::bidirectional_iterator_tag, Node<N> >::iterator_category	iterator_category;

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

					node_ptr base() const						{ return _m_node; }
					reference operator* ()						{ return _m_node->data; }
					pointer operator-> ()						{ return &(operator*()); }
					operator Iterator<const Node<N> >() const	{ return _m_node->data; }

					Iterator &operator++ ()
					{
						if (_m_node->right)
							_m_node = _m_find_smallest_node(_m_node->right);
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
						Iterator ret = *this;
						++(*this);
						return ret;
					}

					Iterator &operator-- ()
					{
						if (_m_node->left)
							_m_node = _m_find_smallest_node(_m_node->left);
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
						Iterator ret = *this;
						--(*this);
						return ret;
					}

					Iterator operator+ (difference_type n) const
					{
						Iterator ret = *this;
						return (ret += n);
					}

					Iterator operator- (difference_type n) const
					{
						Iterator ret = *this;
						return (ret -= n);
					}

					Iterator& operator-= (difference_type n)
					{
						difference_type i = 0;

						while (i++ < n)
							--(*this);
						return *this;
					}

					Iterator& operator+= (difference_type n)
					{
						difference_type i = 0;

						while (i++ < n)
							++(*this);
						return *this;
					}


					bool operator== (const Iterator& other) const
					{ return (base() == other.base()); }

					bool operator!= (const Iterator& other) const
					{ return (base() != other.base()); }

				protected:
					node_ptr	_m_node;
			};

			typedef Iterator<value_type>							iterator;
			typedef Iterator<const value_type>						const_iterator;
			typedef typename ft::reverse_iterator<iterator>			reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>	const_reverse_iterator;

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
			 */

			//////////////////////
			// MEMBER FUNCTIONS //
			//////////////////////

			RBT(const allocator_type& alloc = allocator_type(), const value_compare& comp = value_compare())
				: _m_root(NULL), _m_comp(comp), _m_alloc(alloc), _m_size(0) {}

			RBT(const RBT& tree)
				: _m_root(NULL), _m_comp(tree._m_comp), _m_alloc(tree._m_alloc), _m_size(tree._m_size)
			{ _m_root = _m_clone_tree(tree._m_root, NULL);}

			virtual ~RBT()	{ clear(_m_root); }

			RBT operator= (const RBT& tree)
			{
				if (&tree != this)
				{
					clear(_m_root);
					_m_comp = tree._m_comp;
					_m_alloc = tree._m_comp;
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
					if (_m_comp(val, current->data))
						current = current->left;
					else if (_m_comp(current->data, val))
						current = current->right;
					else
						return iterator(current);
				}
				return end();
			}

			void insert(const value_type& val)
			{
				node_ptr current = _m_root;

				if (!current)
				{
					_m_root = _m_create_node(val, true);
					return ;
				}

				node_ptr parent;
				while (current)
				{
					parent = current;
					if (_m_comp(val, current->data))
						current = current->left;
					else if (_m_comp(current->data, val))
						current = current->right;
					else
						return ; // TODO: find a way to update pair.
				}

				node_ptr new_node = _m_create_node(val, false, parent);
				_m_link_gp_to_child(new_node, new_node);

				_m_rbt_insert_check(new_node);
			}

			void remove(const value_type& val)
			{
				node_ptr target = search(val).base();
				if (!target)
					return ;

				if (target->right && target->left)
				{
					node_ptr substitute = _m_find_smallest_node(target->right);
					target->data = substitute->data;
					_m_rbt_remove_node(substitute);
				}
				else
					_m_rbt_remove_node(target);
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

			node_ptr get_root()			const	{ return _m_root; }
			size_type get_tree_size()	const	{ return _m_size; }

			iterator				begin()				{ return _m_find_smallest_node(_m_root); }
			const_iterator			begin()		const	{ return _m_find_smallest_node(_m_root); }
			iterator			 	end()				{ return _m_find_greatest_node(_m_root); }
			const_iterator 			end()		const	{ return _m_find_greatest_node(_m_root); }
			reverse_iterator		rbegin()			{ return reverse_iterator(end()); }
			const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(end()); }
			reverse_iterator		rend()				{ return reverse_iterator(begin()); }
			const_reverse_iterator	rend()		const	{ return const_reverse_iterator(begin()); }

			///////////
			// DEBUG //
			///////////

			struct Trunk
			{
				Trunk *prev;
				std::string str;

				Trunk(Trunk *prev, std::string str)
				{
					this->prev = prev;
					this->str = str;
				}
			};

			void showTrunks(Trunk *p)
			{
				if (p == NULL) {
					return;
				}

				showTrunks(p->prev);
				std::cout << p->str;
			}

			void printTree(node_ptr root, Trunk *prev, bool isLeft, bool type)
			{
				if (root == NULL) {
					return;
				}

				std::string prev_str = "    ";
				Trunk *trunk = new Trunk(prev, prev_str);

				printTree(root->right, trunk, true, type);

				if (!prev) {
					trunk->str = "———";
				}
				else if (isLeft)
				{
					trunk->str = "/———";
					prev_str = "   |";
				}
				else {
					trunk->str = "\\———";
					prev->str = prev_str;
				}

				showTrunks(trunk);
				std::cout << " " << root->data << "(" << (root->is_black ? "B" : "R") << ")" << std::endl;
				if (prev) {
					prev->str = prev_str;
				}
				trunk->str = "   |";

				printTree(root->left, trunk, false, type);
				delete trunk;
			}

		private:
			node_ptr			_m_root;
			value_compare		_m_comp;
			allocator_type		_m_alloc;
			node_allocator_type	_m_node_alloc;
			size_type			_m_size;

			node_ptr _m_create_node(const value_type& val, bool is_black=false, node_ptr parent=NULL, node_ptr left=NULL, node_ptr right=NULL)
			{
				node_ptr new_node = _m_node_alloc.allocate(1);
				_m_alloc.construct(&(new_node->data), val);
				new_node->is_black = is_black;
				new_node->parent = parent;
				new_node->left = left;
				new_node->right = right;
				++_m_size;

				return new_node;
			}

			void _m_destroy_node(node_ptr target)
			{
				--_m_size;
				_m_alloc.destroy(&(target->data));
				_m_node_alloc.deallocate(target, 1);
			}

			node_ptr _m_clone_tree(node_ptr root, node_ptr parent)
			{
				if (!root)
					return NULL;

				node_ptr ret = _m_create_node(root->data, root->is_black, parent);
				ret->left = _m_clone_tree(root->left, root);
				ret->right = _m_clone_tree(root->right, root);

				return ret;
			}

			void _m_link_gp_to_child(node_ptr target, node_ptr child)
			{
				if (target->parent)
				{
					if (_m_comp(target->data, target->parent->data))
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

			static node_ptr _m_find_smallest_node(node_ptr target)
			{
				if (target && target->left)
					return _m_find_smallest_node(target->left);
				return target;
			}

			static node_ptr _m_find_greatest_node(node_ptr target)
			{
				if (target && target->right)
					return _m_find_greatest_node(target->right);
				return target;
			}

	// The algorithm I used:
	// https://www.geeksforgeeks.org/insertion-in-red-black-tree/
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

			void _m_rbt_insert_check(node_ptr x)
			{
				if (x == _m_root)
					x->is_black = true;

				if (FATHER && GFATHER && !FATHER->is_black)
				{
					node_ptr uncle = _m_comp(FATHER->data, GFATHER->data) ? GFATHER->right : GFATHER->left;
					if (uncle && !uncle->is_black)
					{
						FATHER->is_black = true;
						uncle->is_black = true;
						GFATHER->is_black = false;
						_m_rbt_insert_check(GFATHER);
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
					_m_rbt_remove_check(target);
				}

				else // sibling is black (or is NIL):
				{
					// if at least one of the sibling's children is red:
					if ((sr && !sr->is_black) || (sl && !sl->is_black))
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
					else if (s) // if sibling's children are both black:
					{
						s->is_black = false;
						if (s->parent->is_black)
							_m_rbt_remove_check(s->parent);
						else
							s->parent->is_black = true;
					}
				}
			}

#undef FATHER
#undef GFATHER

	};
}
