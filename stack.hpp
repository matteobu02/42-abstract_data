/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:23:35 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/09 16:00:56 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class T, class Container = deque<T> >
	class stack
	{
		public:
			typedef T			value_type;
			typedef Container	container_type;
			typedef size_t		size_type;

			explicit stack (const container_type& ctnr = container_type())
			{

			}

		private:
	};
}
