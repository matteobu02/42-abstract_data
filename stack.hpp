/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:23:35 by mbucci            #+#    #+#             */
/*   Updated: 2023/02/19 12:10:30 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.hpp"

namespace ft
{
	template <class T, class Container = ft::vector<T> > class stack
	{
		public:
			typedef T			value_type;
			typedef Container	container_type;
			typedef size_t		size_type;

			explicit stack (const container_type& ctnr = container_type()) : _c(ctnr)
			{
			}

			bool empty() const		{ return _c.empty(); }
			size_type size() const	{ return _c.size(); }

			value_type& top()				{ return _c.back(); }
			const value_type& top() const	{ return _c.back(); }

			void push (const value_type& val)
			{ _c.push_back(val); }

			void pop()
			{ _c.pop_back(); }
			
			template <class T1, class Container1>
				friend bool operator== (const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs)
				{ return (lhs._c == rhs._c); }

			template <class T1, class Container1>
				friend bool operator< (const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs)
				{ return (lhs._c < rhs._c); }

		protected:
			container_type _c;
	};

	template <class T, class Container>
		bool operator!= (const stack<T, Container>& lhs, const stack<T, Container>& rhs)
		{ return !(lhs == rhs); }

	template <class T, class Container>
		bool operator<= (const stack<T, Container>& lhs, const stack<T, Container>& rhs)
		{ return !(rhs < lhs); }

	template <class T, class Container>
		bool operator> (const stack<T, Container>& lhs, const stack<T, Container>& rhs)
		{ return (rhs < lhs); }

	template <class T, class Container>
		bool operator>= (const stack<T, Container>& lhs, const stack<T, Container>& rhs)
		{ return !(lhs < rhs); }
}
