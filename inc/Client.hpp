#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ostream>
#include <string>

class Authstate
{
	public:
		Authstate();
		Authstate(bool statePassword);
		Authstate(const Authstate& other);
		bool	getPasswordReceived();
		bool	getNickReceived();
		bool	getFullyRegistered();
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
	
	Authstate				_authstate;
	std::string				_username;
	std::string				_hostname;
	std::string				_realname;
	std::string				_servername;
	std::string				_nickname;
	std::string				_password; // take off
	std::string				_buf; // output buffer
	std::string				_inputBuffer;
	int						_fd;
	struct epoll_event*		_ev;

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
	Authstate		  &getAuthstate();
	std::string		  &getBuf();
	std::string		  &getClientInputBuffer();
	int					getFd() const;
	struct epoll_event*	getEvent() const;
	bool				isfullyRegistered();
	// SETTERS
	void setFd(int fd);
	void setUsername(std::string const &username);
	void setNickname(std::string const &nickname);
	void setPassword(std::string const &password);
	void setAuthState(const std::string state);
	void setRealname(std::string const &realname);
	void setHostname(std::string const &hostname);

	void addToBuffer(std::string data);

};
std::ostream &operator<<(std::ostream &o, const Client &obj);

#endif
