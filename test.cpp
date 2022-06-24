#include <vector>
#include <map>
#include <stack>
#include <string>
#include "vector.hpp"

#include <iostream>
#define X 5

void	print_vect(std::vector<int> const &vect)
{
	for (size_t i = 0; i < vect.size(); i++)
		std::cout << vect[i] << ' ';
	std::cout << '\n';
}

int	main()
{
	std::vector<int>	mv;
	//ft::vector<int>		mv(X);

	std::cout << mv.capacity() << std::endl;
	std::cout << mv.size() << std::endl;
	mv.pop_back();
	std::cout << mv.capacity() << std::endl;
	std::cout << mv.size() << std::endl;
	//print_vect(mv);
	return (0);
}
