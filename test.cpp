#include <vector>
#include <map>
#include <stack>
#include <string>
//#include "vector.hpp"

#include <iostream>

void	print_vect(std::vector<int> const &vect)
{
	for (size_t i=0;i<vect.size();i++)
		std::cout << ' ' << vect[i];
	std::cout << '\n';
}

int	main()
{
	std::vector<int>	myvector(-1);

	std::cout << myvector.capacity() << std::endl;
	return (0);
}
