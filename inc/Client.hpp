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
	std::string		_hostname;
	std::string		_realname;
	std::string		_servername;
	std::string		_nickname;
	std::string		_password; // take off
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
	std::string const &getHostname() const;
	std::string const &getNickname() const;
	std::string const &getPassword() const;
	std::string const &getRealname() const;
	std::string const &getServername() const;
	std::string const &getBuf() const;
	int getFd() const;
	authState getAuthState() const;

	// SETTERS
	void setFd(int fd);
	void setUsername(std::string const &username);
	void setNickname(std::string const &nickname);
	void setPassword(std::string const &password);
	void setAuthState(authState state);
	void setRealname(std::string const &realname);

	// HELPERS
	static const char *authStateToString(authState state);
};
std::ostream &operator<<(std::ostream &o, const Client &obj);

#endif // !CLIENT_HPP
