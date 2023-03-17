/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 13:40:02 by mbucci            #+#    #+#             */
/*   Updated: 2023/03/11 22:54:48 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <iostream>
#include "algorithm.hpp"
#include "iterator_traits.hpp"
#include "iterators.hpp"
#include "type_traits.hpp"

namespace ft
{
	template <class T, class Alloc = std::allocator<T> > class vector
	{
		public:
			typedef T															value_type;
			typedef Alloc														allocator_type;
			typedef typename allocator_type::reference							reference;
			typedef typename allocator_type::const_reference					const_reference;
			typedef typename allocator_type::pointer							pointer;
			typedef typename allocator_type::const_pointer						const_pointer;
			typedef typename ft::random_access_iterator<value_type>				iterator;
			typedef typename ft::random_access_iterator<const value_type>		const_iterator;
			typedef typename ft::reverse_iterator<iterator>						reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>				const_reverse_iterator;
			typedef typename ft::iterator_traits<iterator>::difference_type		difference_type;
			typedef size_t														size_type;

			explicit vector (const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _size(0), _cap(0) {}

			explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _size(n), _cap(n)
			{
				_arr = _alloc.allocate(n);
				_construct_range(0, n, val);
			}
			
			template <class InputIterator>
			vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL) : _alloc(alloc)
			{
				size_type	range = ft::distance(first, last);

				if (!range)
					throw (std::length_error("vector"));
				else if (range > max_size())
					throw (std::length_error("cannot create ft::vector larger than max_size()"));
				_arr = _alloc.allocate(range);
				_size = _cap = range;
				for (size_type i = 0; first != last; first++, i++)
					_alloc.construct(_arr + i, *first);
			}

			vector (const vector& x) : _alloc(x._alloc), _size(x._size), _cap(x._size)
			{
				_arr = _alloc.allocate(_cap);
				for (size_type i = 0; i < _size; i++)
					_alloc.construct(_arr + i, x[i]);
			}

			virtual ~vector()
			{
				clear();
				if (_cap)
					_alloc.deallocate(_arr, _cap);
			}

			vector&	operator= (const vector& x)
			{
				if (&x != this)
				{
					if (x._size > _cap)
						_reallocate(x._size);
					clear();
					_size = x._size;
					for (size_type i = 0; i < _size; i++)
						_alloc.construct(_arr + i, x[i]);
				}
				return *this;
			}

			// ITERATORS
			iterator				begin()				{ return _arr; }
			const_iterator 			begin()		const	{ return _arr; }
			iterator 				end()				{ return (_arr + _size); }
			const_iterator			end() 		const	{ return (_arr + _size); }
			reverse_iterator		rbegin()			{ return reverse_iterator(end()); }
			const_reverse_iterator	rbegin() 	const	{ return const_reverse_iterator(end()); }
			reverse_iterator		rend()				{ return reverse_iterator(begin()); }
			const_reverse_iterator	rend() 		const	{ return const_reverse_iterator(begin()); }

			// CAPACITY
			size_type	size() 		const 	{ return _size; }
			size_type	max_size()	const 	{ return _alloc.max_size(); }
			size_type	capacity()	const	{ return _cap; }
			bool		empty()		const	{ return !_size; }

			void resize (size_type n, value_type val = value_type())
			{
				if (n == _size)
					return ;
				else if (n > _size)
				{
					if (n > _cap)
					{
						size_type	Ncap = (2 * _cap);
						if (Ncap < n)
							Ncap = n;
						_reallocate(Ncap);
					}
					_construct_range(_size, n, val);
				}
				else
					_destroy_range(n, _size);
				_size = n;
			}

			void reserve (size_type n)
			{
				if (n > _cap)
					_reallocate(n);
			}

			// ELEMENT ACCESS
			reference		operator[] (size_type n)		{ return _arr[n]; }
			const_reference	operator[] (size_type n) const	{ return _arr[n]; }

			reference		at (size_type n)
			{
				if (n >= _size)
					throw (std::out_of_range("vector"));
				return _arr[n];
			}

			const_reference	at (size_type n) const
			{
				if (n >= _size)
					throw (std::out_of_range("vector"));
				return _arr[n];
			}

			reference 		front()			{ return _arr[0]; }
			const_reference	front()	const	{ return _arr[0]; }
			reference 		back()			{ return _arr[_size - 1]; }
			const_reference	back()	const	{ return _arr[_size - 1]; }

			// MODIFIERS
			template <class InputIterator>
			void assign (InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			{
				size_type	i;

				if (!empty())
					clear();
				reserve(ft::distance(first, last));
				i = 0;
				while (first != last)
				{
					_alloc.construct(_arr + i++, *first);
					++first;
				}
				_size = i;
			}

			void assign (size_type n, const value_type& val)
			{
				if (!empty())
					clear();
				reserve(n);
				_size = n;
				_construct_range(0, _size, val);
			}
			
			void push_back (const value_type& val)
			{
				if (!_cap)
					_reallocate(1);
				else if (_cap == _size)
					_reallocate(_cap * 2);
				_alloc.construct(_arr + _size++, val);
			}

			void pop_back()
			{
				if (!empty())
					_alloc.destroy(_arr + --_size);
			}

			iterator insert (iterator position, const value_type& val)
			{
				size_type dist = ft::distance(begin(), position);
				insert(position, 1, val);
				return (begin() + dist);
			}

			void insert (iterator position, size_type n, const value_type& val)
			{
				iterator	it;
				size_type	range = ft::distance(position, position + n);

				if (_cap < _size + range)
				{
					pointer		newAlloc = _alloc.allocate(_size + range);
					iterator	itn(newAlloc);

					it = begin();
					for (; it != position; itn++, it++)
						_alloc.construct(itn.base(), *it);
					for (size_type i = 0; i < n; i++, itn++)
						_alloc.construct(itn.base(), val);
					for (; it != end(); itn++, it++)
						_alloc.construct(itn.base(), *it);
					if (!empty())
					{
						_destroy_range(0, _size);
						_alloc.deallocate(_arr, _cap);
					}
					_cap = _size + range;
					_arr = newAlloc;
				}
				else
				{
					it = end() + n;
					for (size_type i = 0; i < n; i++, --it)
						_alloc.construct(it.base(), *(it - n));
					for (size_type i = 0; i < n; i++, --it)
						*it = val;
				}
				_size += range;
			}

			template <class InputIterator>
				void insert (iterator position, InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			{
				iterator	it;
				size_type	range = ft::distance(first, last);

				if (_cap < _size + range)
				{
					pointer		newAlloc = _alloc.allocate(_size + range);
					iterator	itn(newAlloc);

					it = begin();
					for (; it != position; itn++, it++)
						_alloc.construct(itn.base(), *it);
					for (; first != last; itn++, first++)
						_alloc.construct(itn.base(), *first);
					for (; it != end(); itn++, it++)
						_alloc.construct(itn.base(), *it);

					if (!empty())
					{
						_destroy_range(0, _size);
						_alloc.deallocate(_arr, _cap);
					}
					_cap = _size + range;
					_arr = newAlloc;
				}
				else
				{
					it = end() + range;
					for (size_type i = 0; i < range; i++, --it)
						_alloc.construct(it.base(), *(it - range));
					for (; last != first; --last, --it)
						*it = *last;
				}
				_size += range;
			}

			iterator erase (iterator position)
			{
				iterator tmp = position;
				for (iterator it = tmp + 1; it < end(); it++, tmp++)
					*tmp = *it;
				_alloc.destroy(_arr + --_size);
				return position;
			}

			iterator erase (iterator first, iterator last)
			{
				if (first == last)
					return erase(first);
				iterator tmp = first;
				for (iterator it = last; it < end(); ++it, tmp++)
				{
					*tmp = *it;
					_alloc.destroy(it.base());
				}
				_size -= ft::distance(first, last);
				return first;
			}

			void swap (vector& v)
			{
				if (&v != this)
				{
					ft::swap(_arr, v._arr);
					ft::swap(_size, v._size);
					ft::swap(_cap, v._cap);
				}
			}

			void clear()
			{
				if (!empty())
				{
					_destroy_range(0, _size);
					_size = 0;
				}
			}

			// ALLOCATOR
			allocator_type get_allocator() const { return _alloc; }

		private:
			allocator_type	_alloc;
			pointer			_arr;
			size_type		_size;
			size_type		_cap;

			void _reallocate(size_type n)
			{
				pointer newAlloc = _alloc.allocate(n);
				for (size_type i = 0; i < _size; i++)
				{
					_alloc.construct(newAlloc + i, _arr[i]);
					_alloc.destroy(_arr + i);
				}
				if (!empty())
					_alloc.deallocate(_arr, _cap);
				_cap = n;
				_arr = newAlloc;
			}

			void _construct_range(size_type start, size_type end, const value_type& val)
			{
				for (size_type i = start; i < end; i++)
					_alloc.construct(_arr + i, val);
			}

			void _destroy_range(size_type start, size_type end)
			{
				for (size_type i = start; i < end; i++)
					_alloc.destroy(_arr + i);
			}
	};

	// Non-member function overloads
	template <class T, class Alloc>
		bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
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

	template <class T, class Alloc>
		bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return !(lhs == rhs);
		}

	template <class T, class Alloc>
		bool operator< (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

	template <class T, class Alloc>
		bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return !(rhs < lhs);
		}

	template <class T, class Alloc>
		bool operator> (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (rhs < lhs);
		}

	template <class T, class Alloc>
		bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return !(lhs < rhs);
		}

	template <class T, class Alloc>
		void swap (vector<T,Alloc>& lhs, vector<T,Alloc>& rhs)
		{
			lhs.swap(rhs);
		}
}
