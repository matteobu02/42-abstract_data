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
	//std::vector<int>	mv(10);
	ft::vector<int>		mv(10);
	
	std::vector<int>::iterator	it = mv.begin();
	ft::vector<int>	n(it + 10, it - 5);
	
	return (0);
}
