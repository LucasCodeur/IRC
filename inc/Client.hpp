#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ostream>
#include <string>

class AuthState
{
	public:
		AuthState();
		AuthState(const AuthState& other);
		bool	getPasswordReceived() const;
		bool	getNickReceived() const;
		bool	getFullyRegistered() const;
		void	setPasswordReceived(bool state);
		void	setNickReceived(bool state);
		void	setFullyRegistered(bool state);
	private:
		bool	passwordReceived;
		bool	nickReceived;
		bool	fullyRegistered;
};

class Client
{
	private:
	
	AuthState		_authState;
	std::string		_username;
	std::string		_hostname;
	std::string		_realname;
	std::string		_servername;
	std::string		_nickname;
	std::string		_password; // take off
	std::string		_buf;
	int				_fd;

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
	AuthState	&getAuthState();
	std::string &getBuf();
	int getFd() const;

	// SETTERS
	void setFd(int fd);
	void setUsername(std::string const &username);
	void setNickname(std::string const &nickname);
	void setPassword(std::string const &password);
	void setAuthState(const std::string state);
	void setRealname(std::string const &realname);

};
std::ostream &operator<<(std::ostream &o, const Client &obj);

#endif // !CLIENT_HPP
