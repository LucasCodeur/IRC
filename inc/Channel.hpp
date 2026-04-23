#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <ostream>
#include <string>
#include <vector>

class Channel
{
	private:
	std::string			_name;
	std::string			_topic;
	std::string			_password;
	std::vector<int>	_users;
	std::vector<int>	_operators;
	std::vector<int>	_invited;
	int					_mode;

	public:
	// CONSTRUCTOR
	Channel();
	~Channel();
	Channel(Channel const &original);

	// OPERATOR
	Channel &operator=(Channel const &other);

	// GETTERS
	std::string const &getName() const;
	std::string const &getTopic() const;
	std::string const &getPassword() const;
	std::vector<int> const &getUsers() const;
	std::vector<int> const &getOperators() const;
	std::vector<int> const &getInvited() const;
	int getMode() const;

	// SETTERS
	void setTopic(std::string const &topic);
	void setPassword(std::string const &password);
	void setMode(int mode);

	// // METHODS
	// void addUser(int fd);
	// void removeUser(int fd);
	// void addOperator(int fd);
	// void removeOperator(int fd);
	// void addInvited(int fd);
};
std::ostream &operator<<(std::ostream &o, const Channel &obj);

#endif // !CHANNEL_HPP
