/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 13:40:02 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/13 14:55:32 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T											value_type;
			typedef Alloc										allocator_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef size_t										size_type;

			// Constructors
			explicit vector (const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _size(0), _cap(256)
			{
				this->_arr = this->_alloc.allocate(this->_cap);
				return ;
			}

			explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(n)
			{
				if (n < 1)
					throw (std::length_error("vector"));
				else
				{

					while (this->_cap < n)
						this->_cap *= 2;
					this->_arr = this->_alloc.allocate(_cap);
					pointer tmp = &this->_arr[0];
					for (size_type j = 0; j < this->_size; j++, tmp++)
						this->_alloc.construct(tmp, val);
				}
			}
			
			/*template <class InputIterator>
			vector (InputIterator first, InputIterator last,
				const allocator_type& alloc = allocator_type()) : _alloc(alloc)
			{
				//for ()
			}*/

			vector (const vector& x);

			// Destructor
			~vector ()
			{
				for (pointer ptr = &this->_arr[0]; this->_size > 0; --this->_size, ptr++)
					this->_alloc.destroy(ptr);
				this->_alloc.deallocate(this->_arr, this->_cap);
				return ;
			}

			// Copy assignment operator
			vector&	operator= (const vector& x)
			{
				if (&x == this)
					return (*this);
				for (pointer ptr = &this->_arr[0]; this->_size > 0; --this->_size, ptr++)
					this->_alloc.destroy(ptr);
				this->_alloc.deallocate(this->_arr, this->_cap);
				this->_arr = NULL;
				this->_size = x._size;
				this->_cap = x._cap;
				//copy
			}

			// CAPACITY
			size_type	size() const 		{ return (this->_size); }
			size_type	max_size() const 	{ return (this->_alloc.max_size()); }
			size_type	capacity() const	{ return (this->_cap); }
			bool		empty() const		{ return (!this->_size); }

			/*void	resize (size_type n, value_type val = value_type())
			{
				if (n == this->_size)
					return ;
				else if (n < this->_size)
				{
					size_type	i;

					
				}
			}*/

			/*void	reserve (size_type n)
			{
				if (n > this->_cap)
					
			}*/

			// ELEMENT ACCESS
			reference		operator[] (size_type n)		{ return (this->_arr[n]); }
			const_reference	operator[] (size_type n) const	{ return (this->_arr[n]); }

			reference		at (size_type n)
			{
				if (n >= this->_size || n < 0)
					throw (std::out_of_range("vector"));
				return (this->_arr[n]);
			}

			const_reference	at (size_type n)
			{
				if (n >= this->_size || n < 0)
					throw (std::out_of_range("vector"));
				return (this->_arr[n]);
			}

			reference		front()	{ return (this->_arr[0]); }
			const_reference	front()	{ return (this->_arr[0]); }
			reference		back()	{ return (this->_arr[this->_size - 1]); }
			const_reference	back()	{ return (this->_arr[this->_size - 1]); }

			// MODIFIERS
			template <class InputIterator>
				void	assign (InputIterator first, InputIterator last);

			void	assign (size_type n, const value_type& val);

			void	push_back (const value_type& val)
			{
				if (this->_size + 1 > this->_cap)
					// reallocate
			}

		protected:
			allocator_type	_alloc;
			pointer			_arr;
			size_type		_size;
			size_type		_cap;

			void	reallocate(size_type n)
			{
				size_type	NewCap;
				pointer		NewAlloc;
				pointer		tmp;

				NewCap = this->_cap;
				tmp = this->_arr;
				while (NewCap < n)
					NewCap *= 2;
				NewAlloc = _alloc.allocate(NewCap);
				for (size_type i = 0; i < this->_size; i++)
				{
					this->_alloc.construct(NewAlloc++, tmp[i]);
					this->_alloc.destroy(tmp[i])
				}
				this->_alloc.deallocate(this->_arr, this->_cap);
				this->_cap = NewCap;
				this->_arr = NewAlloc - this->_size;
			}
	};
}
