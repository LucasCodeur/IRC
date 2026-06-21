#ifndef UTILS_HPP
# define UTILS_HPP

# include <ostream>

namespace utils_server
{
	bool		receiveData(int socket, std::string& buf);
	void		check_password(std::string& password);
	bool		convertPort(std::string port, int& portToSet);
	std::string extractCommand(std::string& buffer, bool security);
	void		setNonBlocking(int sock);
}

#endif
