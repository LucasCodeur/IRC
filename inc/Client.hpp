#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ostream>
#include <string>

class Client
{
	public:
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

	// GETTERS
	std::string const &getUsername() const;
	std::string const &getNickname() const;
	std::string const &getPassword() const;
	std::string const &getBuf() const;
	int getFd() const;
	authState getAuthState() const;

	// SETTERS
	void setUsername(std::string const &username);
	void setNickname(std::string const &nickname);
	void setPassword(std::string const &password);
	void setAuthState(authState state);
};
std::ostream &operator<<(std::ostream &o, const Client &obj);

#endif // !CLIENT_HPP
