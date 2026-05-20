#include <vector>
#include <iostream>
#include <string>

void print_vec(const std::vector<std::vector<std::string> > params)
{
	int size = params.size();
	for (int i = 0; i < size; i++)
	{
		int sizeParams = params[0].size();
		for (int j = 0; j < sizeParams; j++)
			std::cout << "Vector: " << i << " Params: " << j << " content: " << params[i][j] << std::endl; 
	}
}
