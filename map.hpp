/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 15:11:25 by mbucci            #+#    #+#             */
/*   Updated: 2023/03/31 22:50:14 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include "algorithm.hpp"
#include "functionnal.hpp"
#include "utility.hpp"
#include "rbt.hpp"

namespace ft
{
	template < class Key, class T, class Compare = std::less<Key>,
		 class Alloc = std::allocator< ft::pair<const Key,T> > >
	class map
	{
		public:
			typedef Key										key_type;
			typedef T										mapped_type;
			typedef ft::pair<const key_type, mapped_type>	value_type;
			typedef Compare									key_compare;
			typedef Alloc									allocator_type;
			typedef ptrdiff_t								difference_type;
			typedef size_t									size_type;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
			typedef typename allocator_type::pointer		pointer;
			typedef typename allocator_type::const_pointer	const_pointer;

			////////////////////
			// MEMBER CLASSES //
			////////////////////

			class value_compare : public ft::binary_function<value_type, value_type, bool>
			{
				friend class map;

				protected:
					Compare comp;
					value_compare (Compare c) : comp(c) {}

				public:
					typedef bool		result_type;
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;

					bool operator() (const value_type& lhs, const value_type& rhs) const
					{ return comp(lhs.first, rhs.first); }
			};

		private:
			typedef ft::RBT<value_type, value_compare, allocator_type>	redblacktree;

		public:
			typedef typename redblacktree::iterator						iterator;
			typedef typename redblacktree::const_iterator				const_iterator;
			typedef typename ft::reverse_iterator<iterator>				reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>		const_reverse_iterator;

			//////////////////////
			// MEMBER FUNCTIONS //
			//////////////////////

			explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
				: _m_alloc(alloc), _m_comp(comp), _m_tree(comp) {}

			template <class InputIt>
			map (InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
				: _m_alloc(alloc), _m_comp(comp), _m_tree(comp)
			{
				insert(first, last);
			}

			map (const map& other) : _m_alloc(other._m_alloc), _m_comp(other._m_comp), _m_tree(other._m_tree)
			{}

			virtual ~map() {}

			map& operator= (const map& other)
			{
				if (&other != this)
				{
					_m_alloc = other._m_alloc;
					_m_comp = other._m_comp;
					_m_tree = other._m_tree;
				}
				return *this;
			}

			allocator_type	get_allocator() const { return _m_alloc; }

			/////////////////////
			// ELEMENT ACCESS  //
			/////////////////////

			mapped_type& at(const key_type& key)
			{
				iterator target = _m_tree.search(ft::make_pair(key, mapped_type()));
				if (target != end())
					return target->second;
				throw std::out_of_range("map");
			}

			const mapped_type& at(const key_type& key) const
			{
				iterator target = _m_tree.search(ft::make_pair(key, mapped_type()));
				if (target != end())
					return target->second;
				throw std::out_of_range("map");
			}

			mapped_type& operator[] (const key_type& key)
			{ return insert(ft::make_pair(key, mapped_type())).first->second; }

			///////////////
			// ITERATORS //
			///////////////

			iterator				begin()				{ return _m_tree.begin(); }
			const_iterator			begin()		const	{ return _m_tree.begin(); }
			iterator				end()				{ return _m_tree.end(); }
			const_iterator			end()		const	{ return _m_tree.end(); }
			reverse_iterator		rbegin()			{ return _m_tree.rbegin(); }
			const_reverse_iterator	rbegin()	const	{ return _m_tree.rbegin(); }
			reverse_iterator		rend()				{ return _m_tree.rend(); }
			const_reverse_iterator	rend()		const	{ return _m_tree.rend(); }

			//////////////
			// CAPACITY //
			//////////////

			bool 		empty() 	const	{ return !(_m_tree.get_tree_size()); }
			size_type	size()		const	{ return _m_tree.get_tree_size(); }
			size_type	max_size()	const	{ return _m_tree.max_size(); }

			///////////////
			// MODIFIERS //
			///////////////

			void clear() { _m_tree.clear(_m_tree.get_root()); }

			ft::pair<iterator, bool> insert(const value_type& value)
			{
				size_type old_size = size();

				return ft::make_pair(_m_tree.insert(value), old_size != size());
			}

			iterator insert(iterator pos, const value_type& value)
			{ return _m_tree.insert(value, pos); }

			template<class InputIt>
			void insert(InputIt first, InputIt last)
			{
				for (; first != last; ++first)
					insert(*first);
			}

			iterator erase(iterator pos)
			{
				if (pos != end())
					_m_tree.remove((pos++).base());
				return pos;
			}

			iterator erase(iterator first, iterator last)
			{
				for (; first != last; ++first)
					erase(first);
				return last;
			}

			size_type erase(const key_type& key)
			{ return _m_tree.remove(ft::make_pair(key, mapped_type())); }

			void swap(map& other)
			{
				ft::swap(_m_tree, other._m_tree);
				ft::swap(_m_alloc, other._m_alloc);
				ft::swap(_m_comp, other._m_comp);
			}

			////////////
			// LOOKUP //
			////////////

			size_type count(const key_type& key) const
			{ return ((_m_tree.search(ft::make_pair(key, mapped_type())) != end()) ? 1 : 0); }

			iterator		find(const key_type& key)			{ return _m_tree.search(ft::make_pair(key, mapped_type())); }
			const_iterator	find(const key_type& key) const		{ return _m_tree.search(ft::make_pair(key, mapped_type())); }

			ft::pair<iterator, iterator> 				equal_range(const key_type& key);
			ft::pair<const_iterator, const_iterator>	equal_range(const key_type& key) const;

			iterator		lower_boud(const key_type& key);
			const_iterator	lower_boud(const key_type& key) const;

			iterator		upper_boud(const key_type& key);
			const_iterator	upper_boud(const key_type& key) const;

			///////////////
			// OBSERVERS //
			///////////////
			
			key_compare key_comp() const;

		private:
			allocator_type	_m_alloc;
			key_compare		_m_comp;
			redblacktree	_m_tree;
	};

	//////////////////////////
	// NON-MEMBER FUNCTIONS //
	//////////////////////////

	template <class Key, class T, class Compare, class Alloc>
	bool operator== (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		if (lhs.size() == rhs.size())
		{
			for (size_t i = 0; i < lhs.size(); i++)
				if (lhs[i] != rhs[i])
					return false;
			return true;
		}
		return false;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!= (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator< (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<= (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator> (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>= (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	void swap (const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}
