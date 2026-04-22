#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ostream>
#include <string>

class Client
{
	enum authState
	{
		EMPTY,
		PASSWORD_RECEIVED,
		NICK_RECEIVED,
		FULLY_REGISTERED
	};

  private:
	std::string		_username;
	std::string		_nickname;
	std::string		_password;
	std::string		_buf;
	int				_fd;
	authState		_authState;


  public:
	// CONSTRUCTOR
	Client();
	~Client();
	Client(Client const &original);

	// OPERATOR
	Client &operator=(Client const &other);


};
std::ostream &operator<<(std::ostream &o, const Client &obj);

#endif // !CLIENT_HPP