/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbucci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 13:49:27 by mbucci            #+#    #+#             */
/*   Updated: 2022/07/12 13:11:16 by mbucci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdint>

namespace ft
{
	template<typename T, T v>
	struct integral_constant
	{
		static const T					value = v;
		typedef T						value_type;
		typedef integral_constant<T,v>	type;
	};

	template<typename T, T v>
	const T integral_constant<T, v>::value;

	typedef integral_constant<bool, true>	true_type;
	typedef integral_constant<bool, false>	false_type;

	template<typename>
	struct is_integral_helper : public false_type {};

	template<>
	struct is_integral_helper<bool> : public true_type {};

	template<>
	struct is_integral_helper<char> : public true_type {};

	template<>
	struct is_integral_helper<signed char> : public true_type {};

	template<>
	struct is_integral_helper<unsigned char> : public true_type {};

	template<>
	struct is_integral_helper<wchar_t> : public true_type {};

	template<>
	struct is_integral_helper<char16_t> : public true_type {};

	template<>
	struct is_integral_helper<char32_t> : public true_type {};

	template<>
	struct is_integral_helper<short> : public true_type {};

	template<>
	struct is_integral_helper<unsigned short> : public true_type {};

	template<>
	struct is_integral_helper<int> : public true_type {};

	template<>
	struct is_integral_helper<unsigned int> : public true_type {};

	template<>
	struct is_integral_helper<long> : public true_type {};

	template<>
	struct is_integral_helper<unsigned long> : public true_type {};

	template<>
	struct is_integral_helper<long long> : public true_type {};

	template<>
	struct is_integral_helper<unsigned long long> : public true_type {};

	template<typename T>
	struct is_integral : public is_integral_helper<T> {};


	template<bool, typename T = void>
	struct enable_if {};

	template<typename T>
	struct enable_if<true, T> { typedef T type; };
}
