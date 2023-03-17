/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 23:19:47 by mbucci            #+#    #+#             */
/*   Updated: 2023/02/19 11:57:05 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "iterator_traits.hpp"

namespace ft
{
	template <class InputIterator1, class InputIterator2>
		bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2)
		{
			while (first1 != last1)
			{
				if (first2 == last2 || *first2 < *first1)
					return false;
				else if (*first1 < *first2)
					return true;
				first1++;
				first2++;
			}
			return (first2 != last2);
		}

	template <class InputIterator1, class InputIterator2, class Compare>
		bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2, Compare comp)
		{
			while (first1 != last1)
			{
				if (first2 == last2 || comp(*first2 < *first1))
					return false;
				else if (comp(*first1, *first2))
					return true;
				first1++;
				first2++;
			}
			return (first2 != last2);
		}

	template <class T>
		void swap(T& a, T& b)
		{
			T tmp = a;
			a = b;
			b = tmp;
		}

	template <class InputIterator>
		typename iterator_traits<InputIterator>::difference_type
		__distance(InputIterator _first, InputIterator _last, input_iterator_tag)
		{
			typename iterator_traits<InputIterator>::difference_type n = 0;
			while (_first != _last)
			{
				_first++;
				n++;
			}
			return n;
		}

	template <class RandomAccessIterator>
		typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator _first, RandomAccessIterator _last, random_access_iterator_tag)
		{
			return (_last - _first);
		}

	template <class InputIterator>
		typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
		{
			return __distance(first, last, typename ft::iterator_traits<InputIterator>::iterator_category());
		}
}
