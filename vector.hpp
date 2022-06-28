/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 13:40:02 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/28 17:12:43 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <iostream>
#include "iterators.hpp"
#include "lexicographical_compare.hpp"

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
			//typedef typename ft::reverse_iterator<iterator>						reverse_iterator;
			//typedef typename ft::reverse_iterator<const_iterator>				const_reverse_iterator;
			typedef size_t														size_type;

			explicit vector (const allocator_type& alloc = allocator_type()) : _alloc(alloc)
			{
			}

			explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(n), _cap(n)
			{
				pointer	tmp;

				this->_arr = this->_alloc.allocate(n);
				tmp = this->_arr;
				for (size_type i = 0; i < n; i++)
					this->_alloc.construct(tmp++, val);
				return ;
			}
			
			template <class InputIterator>
			vector (InputIterator first, InputIterator last,
				const allocator_type& alloc = allocator_type()) : _alloc(alloc)
			{
				int	range = last - first;

				if (!range)
					throw (std::length_error("vactor"));
				this->_arr = this->_alloc.allocate(range);
				this->_size = this->_cap = range;
				for (size_type i = 0; first != last; first++)
					this->_alloc.construct(this->_arr + i, *first);
				return ;
			}


			vector (const vector& x) : _alloc(x._alloc), _size(x._size), _cap(x._cap)
			{
				pointer	tmp;

				this->_arr = this->_alloc.allocate(this->_cap);
				tmp = this->_arr;
				for (size_type i = 0; i < this->_size; i++)
					this->_alloc.construct(tmp++, x[i]);
				return ;
			}

			~vector()
			{
				for (pointer ptr = this->_arr; this->_size > 0; this->_size--)
					this->_alloc.destroy(ptr++);
				this->_alloc.deallocate(this->_arr, this->_cap);
				return ;
			}

			vector&	operator= (const vector& x)
			{
				if (&x != this)
				{
					pointer	tmp;

					if (x._size > this->_size)
						reallocate(x._size);
					tmp = this->_arr;
					for (size_type i = 0; i < this->_size; i++)
						this->_alloc.destroy(tmp++);
					this->_size = x._size;
					this->_cap = x._cap;
					tmp = this->_arr;
					for (size_type i = 0; i < this->_size; i++)
						this->_alloc.construct(tmp++, x[i]);
				}
				return (*this);
			}

			// ITERATORS
			iterator begin()				{ return (this->_arr); }
			const_iterator begin() const	{ return (this->_arr); }

			// CAPACITY
			size_type size() const 		{ return (this->_size); }
			size_type max_size() const 	{ return (this->_alloc.max_size()); }
			size_type capacity() const	{ return (this->_cap); }
			bool empty() const			{ return (!this->_size); }

			void resize (size_type n, value_type val = value_type())
			{
				if (n == this->_size)
					return ;
				else if (n > this->_size)
				{
					if (n > this->_cap)
					{
						size_type	Ncap = (2 * this->_cap);
						if (Ncap < n)
							Ncap = n;
						reallocate(Ncap);
					}
					pointer	tmp = this->_arr + this->_size;
					for (size_type i = this->_size; i < n; i++)
						this->_alloc.construct(tmp++, val);
				}
				else
				{
					for (size_type i = n; i < this->_size; i++)
						this->_alloc.destroy(this->_arr + i);
				}
				this->_size = n;
				return ;
			}

			void reserve (size_type n)
			{
				if (n > this->_cap)
				{
					reallocate(n);
					this->_cap = n;
				}
				return ;
			}

			// ELEMENT ACCESS
			reference		operator[] (size_type n)		{ return (this->_arr[n]); }
			const_reference	operator[] (size_type n) const	{ return (this->_arr[n]); }

			reference		at (size_type n)
			{
				if (n >= this->_size)
					throw (std::out_of_range("vector"));
				return (this->_arr[n]);
			}

			const_reference	at (size_type n) const
			{
				if (n >= this->_size)
					throw (std::out_of_range("vector"));
				return (this->_arr[n]);
			}

			reference front()				{ return (this->_arr[0]); }
			const_reference front() const	{ return (this->_arr[0]); }
			reference back()				{ return (this->_arr[this->_size - 1]); }
			const_reference back() const	{ return (this->_arr[this->_size - 1]); }

			// MODIFIERS
			template <class InputIterator>
			void assign (InputIterator first, InputIterator last);
			void assign (size_type n, const value_type& val);
			
			void push_back (const value_type& val)
			{
				if (this->_cap == this->_size)
				{
					reallocate(this->_cap * 2);
				}
				this->_alloc.construct(this->_arr + this->_size, val);
				this->_size++;
				return ;
			}

			void pop_back()
			{
				if (!this->empty())
					this->_alloc.destroy(this->_arr + this->_size--);
				return ;
			}

			/*iterator	insert (iterator position, const value_type& val);
			void		insert (iterator position, size_type n, const value_type& val);
			template <class InputIterator>
			void		insert (iterator position, InputIterator first, InputIterator last);

			iterator	erase (iterator position);
			iterator	erase (iterator first, iterator last);*/

			void	swap (vector& x);

			void	clear()
			{
				if (!this->empty())
				{
					for (size_type i = 0; i < this->_size; i++)
						this->_alloc.destroy(this->_arr + i);
					this->_size = 0;
				}
				return ;
			}

			// ALLOCATOR
			allocator_type get_allocator() const { return (this->_alloc); }

		private:
			allocator_type	_alloc;
			pointer			_arr;
			size_type		_size;
			size_type		_cap;

			void reallocate(size_type n)
			{
				pointer newAlloc = this->_alloc.allocate(n);
				for (size_type i = 0; i < this->_size; i++)
				{
					this->_alloc.construct(newAlloc++, this->_arr[i]);
					this->_alloc.destroy(this->_arr + i);
				}
				this->_alloc.deallocate(this->_arr, this->_cap);
				this->_cap = n;
				this->_arr = newAlloc - this->_size;

				return ;
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
						return (false);
				return (true);
			}
			return (false);
		}

	template <class T, class Alloc>
		bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (!(lhs == rhs));
		}

	template <class T, class Alloc>
		bool operator< (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		}

	template <class T, class Alloc>
		bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (!(rhs < lhs));
		}

	template <class T, class Alloc>
		bool operator> (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (rhs < lhs);
		}

	template <class T, class Alloc>
		bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (!(lhs < rhs));
		}

	template <class T, class Alloc>
		void swap (vector<T,Alloc>& x, vector<T,Alloc>& y)
		{
			x.swap(y);
			return ;
		}
}
