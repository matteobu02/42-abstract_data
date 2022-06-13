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
	std::vector<int>	myvector;
	std::string			tmp[5] = {"","", "", "", ""};

	std::cout << tmp[-1] << std::endl;
	for (size_t i=1;i<10;i++) myvector.push_back(i);
	std::cout << &myvector << std::endl;
	std::cout << myvector.at(-100) << std::endl;

	return (0);
}
