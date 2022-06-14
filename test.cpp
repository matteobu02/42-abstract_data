#include <vector>
#include <map>
#include <stack>
#include <string>
#include "vector.hpp"

#include <iostream>

void	print_vect(std::vector<int> const &vect)
{
	for (size_t i = 0; i < vect.size(); i++)
		std::cout << ' ' << vect[i];
	std::cout << '\n';
}

int	main()
{
	std::vector<int>	myvector(150);

	std::cout << myvector.capacity() << std::endl;
	std::cout << myvector.size() << std::endl;
	myvector.resize(4000);
	std::cout << myvector.capacity() << std::endl;
	std::cout << myvector.size() << std::endl;
	return (0);
}
