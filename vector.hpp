/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 13:40:02 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/08 15:59:27 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T			value_type;
			typedef Alloc		allocator_type;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef size_t		size_type;

			// Constructors
			explicit vector (const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _size(0), _cap(256)
			{
				_arr = _alloc.allocate(256, 0);
				return ;
			}

			explicit vector (size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(n), _cap(1)
			{
				if (!n)
				{
					_arr = _alloc.allocate(1);
					_arr[0] = 0;
				}
				else
				{
					while (_cap < n)
						_cap *= 2;
					_arr = _alloc.allocate(_cap);
					pointer tmp = &_arr[0];
					for (size_type j = 0; j < _size; j++, tmp++)
						_alloc.construct(tmp, val);
				}
			}
			
			template <class InputIterator>
			vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());

			vector (const vector& x);

			// Destructor
			~vector ();

			// Copy assignment operator
			vector&	operator= (const vector& x);

		protected:
			allocator_type	_alloc;
			pointer			_arr;
			size_type		_size;
			size_type		_cap;
	};
}
