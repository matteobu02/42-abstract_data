/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 16:11:06 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/28 17:11:53 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectionnal_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectionnal_iterator_tag {};

	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		public:
			typedef T			value_type;
			typedef Distance	difference_type;
			typedef Pointer		pointer;
			typedef Reference	reference;
			typedef Category	iterator_category;
	};

	template <class Iterator> class iterator_traits
	{
		public:
			typedef typename Iterator::difference_type		difference_type;
			typedef typename Iterator::value_type			value_type;
			typedef typename Iterator::pointer				pointer;
			typedef typename Iterator::reference			reference;
			typedef typename Iterator::iterator_category	iterator_category;
	};

	template <class T> class iterator_traits<T*>
	{
		public:
			typedef ptrdiff_t					difference_type;
			typedef T							value_type;
			typedef T*							pointer;
			typedef T&							reference;
			typedef random_access_iterator_tag	iterator_category;
	};

	template <class T> class iterator_traits<const T*>
	{
		public:
			typedef ptrdiff_t							difference_type;
			typedef T									value_type;
			typedef const T*							pointer;
			typedef const T&							reference;
			typedef random_access_iterator_tag			iterator_category;
	};

	template <typename T>
	class random_access_iterator : public iterator<random_access_iterator_tag, T>
	{
		public:
			typedef typename iterator<random_access_iterator_tag, T>::difference_type	difference_type;
			typedef typename iterator<random_access_iterator_tag, T>::value_type		value_type;
			typedef typename iterator<random_access_iterator_tag, T>::pointer			pointer;
			typedef typename iterator<random_access_iterator_tag, T>::reference			reference;
			typedef typename iterator<random_access_iterator_tag, T>::iterator_category	iterator_category;

			// Constructors
			random_access_iterator() : _ptr(NULL)									{}
			random_access_iterator(pointer ptr) : _ptr(ptr)							{}
			random_access_iterator(const random_access_iterator& x) : _ptr(x._ptr)	{}

			// Destructor
			~random_access_iterator()												{}
			
			// Member Function
			random_access_iterator& operator= (const random_access_iterator& x)
			{
				if (&x != this)
					this->_ptr = x._ptr;
				return (*this);
			}

			random_access_iterator& operator* () const { return (*this->_ptr); }
			
			const pointer&	getPtr() const { return (this->_ptr); }


		protected:
			pointer	_ptr;
	};

	template <class Iterator1, class Iterator2>
	bool operator== (const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.getPtr() == rhs.getPtr());
	}

	template <class Iterator1, class Iterator2>
	bool operator!= (const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.getPtr() != rhs.getPtr());
	}

	template <class Iterator1, class Iterator2>
	bool operator< (const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.getPtr() < rhs.getPtr());
	}

	template <class Iterator1, class Iterator2>
	bool operator<= (const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.getPtr() <= rhs.getPtr());
	}

	template <class Iterator1, class Iterator2>
	bool operator> (const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.getPtr() > rhs.getPtr());
	}

	template <class Iterator1, class Iterator2>
	bool operator>= (const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs)
	{
		return (lhs.getPtr() >= rhs.getPtr());
	}
}
