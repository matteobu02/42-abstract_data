/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 15:11:39 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/24 16:02:23 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class Iterator> class iterator_traits
	{
	};

	template <class T> class iterator_traits<T*>
	{
	};

	template <class T> class iterator_traits<const T*>
	{
	};
}
