/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 16:11:06 by mbucci            #+#    #+#             */
/*   Updated: 2023/03/15 17:30:59 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include "iterator_traits.hpp"
#include "algorithm.hpp"

namespace ft
{
	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

	// RANDOM ACCESS ITERATOR
	template <typename T>
	class random_access_iterator : public iterator<ft::random_access_iterator_tag, T>
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
			virtual ~random_access_iterator() {}

			// THIS
			// Member Functions Overloads
			random_access_iterator& operator= (const random_access_iterator& x)
			{
				if (&x != this)
					_ptr = x._ptr;
				return *this;
			}

			pointer base() const								{ return _ptr; }
			reference operator* () const						{ return *_ptr; }
			pointer operator-> () const							{ return &(operator*()); }
			operator random_access_iterator<const T>() const	{ return _ptr; }

			random_access_iterator &operator++ ()
			{
				_ptr++;
				return *this;
			}

			random_access_iterator operator++ (int)
			{
				random_access_iterator	ret = *this;
				_ptr++;
				return ret;
			}
			
			random_access_iterator &operator-- ()
			{
				_ptr--;
				return *this;
			}

			random_access_iterator operator-- (int)
			{
				random_access_iterator	ret = *this;
				_ptr--;
				return ret;
			}

			random_access_iterator operator+ (difference_type n) const
			{ return (random_access_iterator(_ptr + n)); }
			
			random_access_iterator operator- (difference_type n) const
			{ return (random_access_iterator(_ptr - n)); }

			random_access_iterator& operator-= (difference_type n)
			{
				_ptr -= n;
				return *this;
			}

			random_access_iterator& operator+= (difference_type n)
			{
				_ptr += n;
				return *this;
			}

			reference operator[] (difference_type n) const { return _ptr[n]; }

		protected:
			pointer	_ptr;
	};

	template <class I1, class I2>
		bool operator== (const random_access_iterator<I1>& lhs, const random_access_iterator<I2>& rhs)
	{ return (lhs.base() == rhs.base()); }

	template <class I1, class I2>
		bool operator!= (const random_access_iterator<I1>& lhs, const random_access_iterator<I2>& rhs)
	{ return (lhs.base() != rhs.base()); }

	template <class I1, class I2>
		bool operator< (const random_access_iterator<I1>& lhs, const random_access_iterator<I2>& rhs)
	{ return (lhs.base() < rhs.base()); }

	template <class I1, class I2>
		bool operator<= (const random_access_iterator<I1>& lhs, const random_access_iterator<I2>& rhs)
	{ return (lhs.base() <= rhs.base()); }

	template <class I1, class I2>
		bool operator> (const random_access_iterator<I1>& lhs, const random_access_iterator<I2>& rhs)
	{ return (lhs.base() > rhs.base()); }

	template <class I1, class I2>
		bool operator>= (const random_access_iterator<I1>& lhs, const random_access_iterator<I2>& rhs)
	{ return (lhs.base() >= rhs.base()); }

	template <class Iterator>
		random_access_iterator<Iterator> operator+ (const typename random_access_iterator<Iterator>::difference_type lhs,
			const random_access_iterator<Iterator>& rhs)
	{ return (rhs + lhs); }

	template <class I1, class I2>
		typename random_access_iterator<I1>::difference_type operator- (const random_access_iterator<I1>& lhs,
			const random_access_iterator<I2>& rhs)
	{ return (lhs.base() - rhs.base()); }

	// REVERSE ITERATOR
	template <class Iterator> class reverse_iterator
	{
		public:
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;

			reverse_iterator() : _base(NULL) 							{}
			explicit reverse_iterator (iterator_type it) : _base(it)	{}
			template <class Iter>
				reverse_iterator (const reverse_iterator<Iter>& rev_it) : _base(rev_it.base()) {}

			iterator_type base() const								{ return _base; }
			operator reverse_iterator<const Iterator>() const		{ return _base; }

			reference operator* () const
			{
				iterator_type tmp(_base);
				return *--tmp;
			}

			reverse_iterator operator+ (difference_type n) const	{ return (reverse_iterator(_base - n)); }

			reverse_iterator& operator++ ()
			{
				--_base;
				return *this;
			}

			reverse_iterator operator++ (int)
			{
				reverse_iterator tmp(*this);
				--_base;
				return tmp;
			}

			reverse_iterator& operator+= (difference_type n)
			{
				_base -= n;
				return *this;
			}

			reverse_iterator operator- (difference_type n) const
			{ return reverse_iterator(_base + n); }

			reverse_iterator& operator-- ()
			{
				++_base;
				return *this;
			}

			reverse_iterator operator-- (int)
			{
				reverse_iterator tmp(*this);
				++_base;
				return tmp;
			}

			reverse_iterator& operator-= (difference_type n)
			{
				_base += n;
				return *this;
			}

			pointer operator-> () const						{ return &(operator* ()); }
			reference operator[] (difference_type n) const	{ return _base[-n - 1]; }

		private:
			iterator_type _base;
	};

	template <class Iterator1, class Iterator2>
		bool operator== (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (lhs.base() == rhs.base()); }

	template <class Iterator1, class Iterator2>
		bool operator!= (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (lhs.base() != rhs.base()); }

	template <class Iterator1, class Iterator2>
		bool operator< (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (lhs.base() > rhs.base()); }

	template <class Iterator1, class Iterator2>
		bool operator<= (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (lhs.base() >= rhs.base()); }

	template <class Iterator1, class Iterator2>
		bool operator> (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (lhs.base() < rhs.base()); }

	template <class Iterator1, class Iterator2>
		bool operator>= (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (lhs.base() <= rhs.base()); }

	template <class Iterator>
		reverse_iterator<Iterator> operator+ (typename reverse_iterator<Iterator>::difference_type n,
		const reverse_iterator<Iterator>& rev_it)
		{ return (rev_it + n); }

	template <class Iterator1, class Iterator2>
		typename reverse_iterator<Iterator1>::difference_type
		operator- (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
		{ return (rhs.base() - lhs.base()); }
}
