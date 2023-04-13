/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:58:10 by mbucci            #+#    #+#             */
/*   Updated: 2023/04/13 17:18:18 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <memory>
#include "algorithm.hpp"
#include "functionnal.hpp"
#include "utility.hpp"
#include "rbt.hpp"

namespace ft
{
	template <class Key,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<Key> >
	class set
	{
		public:
			typedef Key										key_type;
			typedef Key										value_type;
			typedef size_t									size_type;
			typedef ptrdiff_t								difference_type;
			typedef Compare									key_compare;
			typedef Compare									value_compare;
			typedef Allocator								allocator_type;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
			typedef typename allocator_type::pointer		pointer;
			typedef typename allocator_type::const_pointer	const_pointer;

		private:
			typedef ft::RBT<value_type, value_compare, allocator_type>	redblacktree;

		public:
			typedef typename redblacktree::iterator					iterator;
			typedef typename redblacktree::const_iterator			const_iterator;
			typedef typename ft::reverse_iterator<iterator>			reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>	const_reverse_iterator;

			//////////////////////
			// MEMBER FUNCTIONS //
			//////////////////////

			explicit set (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
				: _m_alloc(alloc), _m_comp(comp), _m_tree(alloc, comp) {}

			template <class InputIt>
			set (InputIt first, InputIt last, const key_compare& comp = Compare(), const allocator_type& alloc = allocator_type())
				: _m_alloc(alloc), _m_comp(comp), _m_tree(first, last, alloc, comp) {}

			set (const set& other) : _m_alloc(other._m_alloc), _m_comp(other._m_comp), _m_tree(other._m_tree)
			{}

			virtual ~set() {}

			set operator= (const set& other)
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

			///////////////
			// ITERATORS //
			///////////////

			iterator begin() { return _m_tree.begin(); }
			const_iterator begin() const { return _m_tree.begin(); }
			iterator end() { return _m_tree.end(); }
			const_iterator end() const { return _m_tree.end(); }
			reverse_iterator rbegin() { return reverse_iterator(begin()); }
			const_reverse_iterator rbegin() const { return const_reverse_iterator(begin()); }
			reverse_iterator rend() { return reverse_iterator(end()); }
			const_reverse_iterator rend() const { return const_reverse_iterator(end()); }

			//////////////
			// CAPACITY //
			//////////////

			bool empty() const { return !size(); }
			size_type size() const { return _m_tree.size(); }
			size_type max_size() const { return _m_tree.max_size(); }

			///////////////
			// MODIFIERS //
			///////////////

			void clear() { _m_tree.clear(); }

			ft::pair<iterator, bool> insert(const value_type& value)
			{
				size_type old_size = size();
				iterator ret = _m_tree.insert(value);

				return ft::make_pair(ret, old_size != size());
			}

			iterator insert(iterator pos, const value_type& value)
			{
				if (pos != end() && _m_comp(value, *pos))
					return _m_tree.insert(value, pos.base());
				return insert(value).first;
			}

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
				_m_tree.remove(first, last);
				return last;
			}

			size_type erase(const key_type& key)
			{ return _m_tree.remove(key); }

			void swap(set& other)
			{
				ft::swap(_m_tree, other._m_tree);
				ft::swap(_m_alloc, other._m_alloc);
				ft::swap(_m_comp, other._m_comp);
			}

			////////////
			// LOOKUP //
			////////////

			size_type count(const key_type& key) const
			{ return (find(key) != end()); }

			iterator find(const Key& key) { return _m_tree.search(key); }
			const_iterator find(const Key& key) const { return _m_tree.search(key); }

			ft::pair<iterator, iterator> equal_range(const Key& key) { return ft::make_pair(lower_bound(key), upper_bound(key)); }
			ft::pair<const_iterator, const_iterator> equal_range(const Key& key) const { return ft::make_pair(lower_bound(key), upper_bound(key)); }

			iterator lower_bound(const Key& key) { return _m_tree.lower_bound(key); }
			const_iterator lower_bound(const Key& key) const { return _m_tree.lower_bound(key); }

			iterator upper_bound(const Key& key) { return _m_tree.upper_bound(key); }
			const_iterator upper_bound(const Key& key) const { return _m_tree.upper_bound(key); }

			///////////////
			// OBSERVERS //
			///////////////

			key_compare key_comp() const { return _m_comp; }
			value_compare value_comp() const { return value_compare(_m_comp); }

		private:
			allocator_type	_m_alloc;
			key_compare		_m_comp;
			redblacktree	_m_tree;
	};

	template <class Key, class Compare, class Alloc>
	bool operator== (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		if (lhs.size() == rhs.size())
		{
			typename set<Key, Compare, Alloc>::const_iterator itl, itr;
			itl = lhs.begin();
			itr = rhs.begin();

			for (; itl != lhs.end(); itl++, itr++)
				if (*itl != *itr)
					return false;
			return true;
		}
		return false;
	}

	template <class Key, class Compare, class Alloc>
	bool operator!= (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class Compare, class Alloc>
	bool operator< (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class Compare, class Alloc>
	bool operator<= (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Key, class Compare, class Alloc>
	bool operator> (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class Key, class Compare, class Alloc>
	bool operator>= (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class Key, class Compare, class Alloc>
	void swap (set<Key, Compare, Alloc>& lhs, set<Key, Compare, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}
