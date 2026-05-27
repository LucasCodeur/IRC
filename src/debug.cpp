#include <vector>
#include <iostream>
#include <string>
#include <Client.hpp>
#include "debug.hpp"

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

void print_info_client(const Client& toPrint)
{
	PRINT("Fd: ", GREEN, "");
	PRINT(toPrint.getFd(), WHITE, "\n");
	PRINT("Username: ", GREEN, "");
	PRINT(toPrint.getUsername(), WHITE, "\n");
	PRINT("Hostname: ", GREEN, "");
	PRINT(toPrint.getHostname(), WHITE, "\n");
	PRINT("Realname: ", GREEN, "");
	PRINT(toPrint.getRealname(), WHITE, "\n");
	PRINT("Servername: ", GREEN, "");
	PRINT(toPrint.getServername(), WHITE, "\n");
	PRINT("Nickname: ", GREEN, "");
	PRINT(toPrint.getNickname(), WHITE, "\n");
	PRINT("Password: ", GREEN, "");
	PRINT(toPrint.getPassword(), WHITE, "\n");
}
