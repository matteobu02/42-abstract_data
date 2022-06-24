/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 15:11:25 by mbucci            #+#    #+#             */
/*   Updated: 2022/06/24 16:00:27 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template < class Key,
			 class T,
			 class Compare = less<Key>,
			 class Alloc = allocator<pair<const Key,T> >
			> class map
			{
			};
}
