#define STD	0
#define FT 1
#define TESTED_TYPE int
#define TESTED_NAMESPACE FT

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "map.hpp"

#define T_SIZE_TYPE typename TESTED_NAMESPACE::vector<T>::size_type

//template <typename T>
//void	printSize(TESTED_NAMESPACE::vector<T> const &vct, bool print_content = true)
//{
//	const T_SIZE_TYPE size = vct.size();
//	const T_SIZE_TYPE capacity = vct.capacity();
//	const std::string isCapacityOk = (capacity >= size) ? "OK" : "KO";
//	// Cannot limit capacity's max value because it's implementation dependent
//
//	std::cout << "size: " << size << std::endl;
//	std::cout << "capacity: " << isCapacityOk << std::endl;
//	std::cout << "max_size: " << vct.max_size() << std::endl;
//	if (print_content)
//	{
//		typename TESTED_NAMESPACE::vector<T>::const_iterator it = vct.begin();
//		typename TESTED_NAMESPACE::vector<T>::const_iterator ite = vct.end();
//		std::cout << std::endl << "Content is:" << std::endl;
//		for (; it != ite; ++it)
//			std::cout << "- " << *it << std::endl;
//	}
//	std::cout << "###############################################" << std::endl;
//}

int		main(void)
{

#if TESTED_NAMESPACE
#undef TESTED_NAMESPACE
#define TESTED_NAMESPACE ft

	ft::map<std::string, int>	m;
	std::cout << m.size() << std::endl;
	
#else
#undef TESTED_NAMESPACE
#define TESTED_NAMESPACE std

	std::vector<int>	v(5, 5);
	std::vector<int>::iterator it = v.begin();

	it++;
	it++;
	it++;
	it++;
	it++;
	it++;
	it++;
	std::cout << it << std::endl;
#endif
	
	return (0);
}
